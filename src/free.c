/*
 * SPDX-License-Identifier: ISC
 *
 * Copyright (C) 2017-2018 Michael Drake <tlsa@netsurf-browser.org>
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

#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "data.h"
#include "util.h"

/**
 * Internal function for freeing a CYAML-parsed data structure.
 *
 * \param[in]  cfg     The client's CYAML library config.
 * \param[in]  schema  The schema describing how to free `data`.
 * \param[in]  data    The data structure to be freed.
 */
static void cyaml__free_value(
		const cyaml_config_t *cfg,
		const cyaml_schema_type_t *schema,
		uint8_t * const data);

/**
 * Internal function for freeing a CYAML-parsed sequence.
 *
 * \param[in]  cfg              The client's CYAML library config.
 * \param[in]  sequence_schema  The schema describing how to free `data`.
 * \param[in]  count            The sequence's entry count.
 * \param[in]  data             The data structure to be freed.
 */
static void cyaml__free_sequence(
		const cyaml_config_t *cfg,
		const cyaml_schema_type_t *sequence_schema,
		uint64_t count,
		uint8_t *data)
{
	const cyaml_schema_type_t *schema = sequence_schema->sequence.schema;
	uint32_t data_size = schema->data_size;

	if (schema->flags & CYAML_FLAG_POINTER) {
		data_size = sizeof(data);
	}

	for (uint64_t i = 0; i < count; i++) {
		cyaml__free_value(cfg, schema, data + data_size * i);
	}
}

/**
 * Internal function for freeing a CYAML-parsed mapping.
 *
 * \param[in]  cfg             The client's CYAML library config.
 * \param[in]  mapping_schema  The schema describing how to free `data`.
 * \param[in]  data            The data structure to be freed.
 */
static void cyaml__free_mapping(
		const cyaml_config_t *cfg,
		const cyaml_schema_type_t *mapping_schema,
		uint8_t * const data)
{
	const cyaml_schema_mapping_t *schema = mapping_schema->mapping.schema;

	while (schema->key != NULL) {
		uint8_t *entry_data = data + schema->data_offset;
		cyaml__free_value(cfg, &schema->value, entry_data);
		schema++;
	}
}

/* This function is documented at the forward declaration above. */
static void cyaml__free_value(
		const cyaml_config_t *cfg,
		const cyaml_schema_type_t *schema,
		uint8_t * const data)
{
	uint8_t *data_target = data;

	if (schema->flags & CYAML_FLAG_POINTER) {
		cyaml_err_t err;
		data_target = (void *)cyaml_data_read(
				sizeof(char *), data, &err);
		if ((err != CYAML_OK) || (data_target == NULL)) {
			return;
		}
	}

	if (schema->type == CYAML_MAPPING) {
		cyaml__free_mapping(cfg, schema, data_target);
	} else if (schema->type == CYAML_SEQUENCE ||
	           schema->type == CYAML_SEQUENCE_FIXED) {
		uint64_t count = schema->sequence.max;
		if (schema->type == CYAML_SEQUENCE) {
			cyaml_err_t err;
			count = cyaml_data_read(schema->sequence.count_size,
					data + schema->sequence.count_offset,
					&err);
			if (err != CYAML_OK) {
				return;
			}
		}
		cyaml__free_sequence(cfg, schema, count, data_target);
	}

	if (schema->flags & CYAML_FLAG_POINTER) {
		cyaml__log(cfg, CYAML_LOG_DEBUG, "Freeing: %p\n", data_target);
		free(data_target);
	}
}

/* Exported function, documented in include/cyaml/cyaml.h */
cyaml_err_t cyaml_free(
		const cyaml_config_t *config,
		const cyaml_schema_type_t *schema,
		cyaml_data_t *data)
{
	if (config == NULL) {
		return CYAML_ERR_BAD_PARAM_NULL_CONFIG;
	}
	if (schema == NULL) {
		return CYAML_ERR_BAD_PARAM_NULL_SCHEMA;
	}
	cyaml__free_value(config, schema, (void *)&data);
	return CYAML_OK;
}
