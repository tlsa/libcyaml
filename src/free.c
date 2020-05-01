/*
 * SPDX-License-Identifier: ISC
 *
 * Copyright (C) 2017-2020 Michael Drake <tlsa@netsurf-browser.org>
 */

/**
 * \file
 * \brief Free data structures created by the CYAML load functions.
 *
 * As described in the public API for \ref cyaml_free(), it is preferable for
 * clients to write their own free routines, tailored for their data structure.
 *
 * Recursion and stack usage
 * -------------------------
 *
 * This generic CYAML free routine is implemented using recursion, rather
 * than iteration with a heap-allocated stack.  This is because recursion
 * seems less bad than allocating within the free code, and the stack-cost
 * of these functions isn't huge.  The maximum recursion depth is of course
 * bound by the schema, however schemas for recursively nesting data structures
 * are unbound, e.g. for a data tree structure.
 */

#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "data.h"
#include "util.h"
#include "mem.h"

/**
 * Free stack entry.
 *
 * This allows inspection back up the call stack; the parent pointers form
 * a linked list.
 */
typedef struct cyaml_free_stack {
	/** Schema for value to be freed. */
	const cyaml_schema_value_t * const schema;
	/** Parent free stack entry pointer. */
	const struct cyaml_free_stack * const parent;
	/** Data for value to be freed. */
	uint8_t *data;
} cyaml_free_stack_t;

/**
 * Internal function for freeing a CYAML-parsed data structure.
 *
 * \param[in]  cfg     The client's CYAML library config.
 * \param[in]  parent  Parent entry on the free stack.
 * \param[in]  count   If data is of type \ref CYAML_SEQUENCE, this is the
 *                     number of entries in the sequence.
 */
static void cyaml__free_value(
		const cyaml_config_t *cfg,
		cyaml_free_stack_t *parent,
		uint64_t count);

/**
 * Internal function for freeing a CYAML-parsed sequence.
 *
 * \param[in]  cfg     The client's CYAML library config.
 * \param[in]  parent  Parent entry on the free stack.
 * \param[in]  count   The sequence's entry count.
 */
static void cyaml__free_sequence(
		const cyaml_config_t *cfg,
		const cyaml_free_stack_t *parent,
		uint64_t count)
{
	uint32_t data_size = parent->schema->sequence.entry->data_size;

	cyaml__log(cfg, CYAML_LOG_DEBUG,
			"Free: Freeing sequence with count: %u\n", count);

	if (parent->schema->sequence.entry->flags & CYAML_FLAG_POINTER) {
		data_size = sizeof(parent->data);
	}

	for (unsigned i = 0; i < count; i++) {
		cyaml_free_stack_t stack = {
			.schema = parent->schema->sequence.entry,
			.data = parent->data + data_size * i,
			.parent = parent,
		};
		cyaml__log(cfg, CYAML_LOG_DEBUG,
				"Free: Freeing sequence entry: %u\n", i);
		cyaml__free_value(cfg, &stack, 0);
	}
}

/**
 * Internal function for freeing a CYAML-parsed mapping.
 *
 * \param[in]  cfg     The client's CYAML library config.
 * \param[in]  parent  Parent entry on the free stack.
 */
static void cyaml__free_mapping(
		const cyaml_config_t *cfg,
		const cyaml_free_stack_t *parent)
{
	const cyaml_schema_field_t *schema = parent->schema->mapping.fields;

	while (schema->key != NULL) {
		cyaml_free_stack_t stack = {
			.data = parent->data + schema->data_offset,
			.schema = &schema->value,
			.parent = parent,
		};
		uint64_t count = 0;
		cyaml__log(cfg, CYAML_LOG_DEBUG,
				"Free: Freeing key: %s (at offset: %u)\n",
				schema->key, (unsigned)schema->data_offset);
		if (schema->value.type == CYAML_SEQUENCE) {
			cyaml_err_t err;
			count = cyaml_data_read(schema->count_size,
					parent->data + schema->count_offset,
					&err);
			if (err != CYAML_OK) {
				return;
			}
		}
		cyaml__free_value(cfg, &stack, count);
		schema++;
	}
}

/**
 * Attempt to read union discriminant from client data using search schema.
 *
 * \param[in]  cfg            The client's CYAML library config.
 * \param[in]  union_schema   The schema for the union.
 * \param[in]  search_schema  A schema to search for union discriminant.
 * \param[in]  data           Pointer to client data for value at schema.
 * \param[out] idx_out        Returns union discriminant value on success.
 * \return CYAML_OK on success, CYAML_ERR_UNION_DISC_NOT_FOUND if the given
 *         schema entry did not contain a discriminant, or appropriate error
 *         otherwise.
 */
static cyaml_err_t cyaml__get_union_idx(
		const cyaml_config_t *cfg,
		const cyaml_schema_value_t *union_schema,
		const cyaml_schema_value_t *search_schema,
		const uint8_t *data,
		uint16_t *idx_out)
{
	const cyaml_schema_field_t *disc_field;
	int64_t union_discriminant;
	const char *field_str;
	uint16_t field_idx;
	uint16_t disc_idx;
	cyaml_err_t err;

	assert(union_schema->type == CYAML_UNION);

	if (search_schema->type != CYAML_MAPPING &&
			search_schema != union_schema) {
		return CYAML_ERR_UNION_DISC_NOT_FOUND;
	}

	/* Get union discriminant field. */
	disc_idx = cyaml__get_mapping_field_idx(cfg, search_schema,
			union_schema->mapping.union_discriminant);
	disc_field = search_schema->mapping.fields + disc_idx;
	if (disc_idx == CYAML_FIELDS_IDX_NONE ||
			disc_field->value.type != CYAML_ENUM ||
			disc_field->value.flags & CYAML_FLAG_POINTER) {
		return CYAML_ERR_UNION_DISC_NOT_FOUND;
	}

	/* Read in the discriminant from the client data. */
	union_discriminant = cyaml_sign_pad(cyaml_data_read(
			disc_field->value.data_size,
			data + disc_field->data_offset, &err),
			disc_field->value.data_size);
	if (err != CYAML_OK) {
		return err;
	}

	/*Search enum for this value. */
	err = cyaml__schema_get_string_for_value(&disc_field->value,
			union_discriminant, &field_str);
	if (err != CYAML_OK) {
		return err;
	}

	/* Find corresponding mapping field index. */
	field_idx = cyaml__get_mapping_field_idx(cfg, union_schema, field_str);
	if (field_idx == CYAML_FIELDS_IDX_NONE) {
		return CYAML_ERR_INVALID_KEY;
	}

	*idx_out = field_idx;
	return CYAML_OK;
}

/**
 * Internal function for freeing a CYAML-parsed union.
 *
 * \param[in]  cfg     The client's CYAML library config.
 * \param[in]  parent  Parent entry on the free stack.
 */
static void cyaml__free_union(
		const cyaml_config_t *cfg,
		const cyaml_free_stack_t *parent)
{
	const cyaml_schema_field_t *schema = parent->schema->mapping.fields;
	uint64_t count = 0;
	cyaml_err_t err;

	if (parent->schema->mapping.union_discriminant != NULL) {
		err = CYAML_ERR_UNION_DISC_NOT_FOUND;

		for (const cyaml_free_stack_t *p = parent;
				p != NULL; p = p->parent) {
			uint16_t idx;

			err = cyaml__get_union_idx(cfg,
					parent->schema, p->schema,
					parent->data, &idx);
			if (err == CYAML_ERR_UNION_DISC_NOT_FOUND) {
				continue;
			} else if (err != CYAML_OK) {
				break;
			}

			schema += idx;
			break;
		}

		if (err != CYAML_OK) {
			return;
		}
	}

	cyaml__log(cfg, CYAML_LOG_DEBUG,
			"Free: Freeing union of type: %s (at offset: %u)\n",
			schema->key, (unsigned)schema->data_offset);

	if (schema->value.type == CYAML_SEQUENCE) {
		cyaml_err_t err;
		count = cyaml_data_read(schema->count_size,
				parent->data + schema->count_offset,
				&err);
		if (err != CYAML_OK) {
			return;
		}
	}

	{
		cyaml_free_stack_t stack = {
			.data = parent->data + schema->data_offset,
			.schema = &schema->value,
			.parent = parent,
		};
		cyaml__free_value(cfg, &stack, count);
	}
}

/* This function is documented at the forward declaration above. */
static void cyaml__free_value(
		const cyaml_config_t *cfg,
		cyaml_free_stack_t *parent,
		uint64_t count)
{
	if (parent->schema->flags & CYAML_FLAG_POINTER) {
		parent->data = cyaml_data_read_pointer(parent->data);
		if (parent->data == NULL) {
			return;
		}
	}

	if (parent->schema->type == CYAML_MAPPING) {
		cyaml__free_mapping(cfg, parent);

	} else if (parent->schema->type == CYAML_UNION) {
		cyaml__free_union(cfg, parent);

	} else if (parent->schema->type == CYAML_SEQUENCE ||
	           parent->schema->type == CYAML_SEQUENCE_FIXED) {
		if (parent->schema->type == CYAML_SEQUENCE_FIXED) {
			count = parent->schema->sequence.max;
		}
		cyaml__free_sequence(cfg, parent, count);
	}

	if (parent->schema->flags & CYAML_FLAG_POINTER) {
		cyaml__log(cfg, CYAML_LOG_DEBUG, "Free: Freeing: %p\n",
				parent->data);
		cyaml__free(cfg, parent->data);
	}
}

/* Exported function, documented in include/cyaml/cyaml.h */
cyaml_err_t cyaml_free(
		const cyaml_config_t *config,
		const cyaml_schema_value_t *schema,
		cyaml_data_t *data,
		unsigned seq_count)
{
	cyaml_free_stack_t stack = {
		.schema = schema,
		.data = (void *)&data,
	};

	if (config == NULL) {
		return CYAML_ERR_BAD_PARAM_NULL_CONFIG;
	}
	if (config->mem_fn == NULL) {
		return CYAML_ERR_BAD_CONFIG_NULL_MEMFN;
	}
	if (schema == NULL) {
		return CYAML_ERR_BAD_PARAM_NULL_SCHEMA;
	}
	cyaml__log(config, CYAML_LOG_DEBUG, "Free: Top level data: %p\n", data);
	cyaml__free_value(config, &stack, seq_count);
	return CYAML_OK;
}
