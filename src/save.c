/*
 * SPDX-License-Identifier: ISC
 *
 * Copyright (C) 2018 Michael Drake <tlsa@netsurf-browser.org>
 */

/**
 * \file
 * \brief Save client's data structure to YAML, using schema.
 *
 * This uses `libyaml` to emit YAML documents, it uses the client-provided
 * schema to access the client data, and validates it before emitting the YAML.
 */

#include <inttypes.h>
#include <stdbool.h>
#include <strings.h>
#include <assert.h>
#include <limits.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

#include <yaml.h>

#include "mem.h"
#include "data.h"
#include "util.h"

/**
 * A CYAML load state machine stack entry.
 */
typedef struct cyaml_state {
	/** Current load state machine state. */
	enum cyaml_state_e state;
	/** Schema for the expected value in this state. */
	const cyaml_schema_value_t *schema;
	/** Anonymous union for schema type specific state. */
	union {
		/** Additional state for \ref CYAML_STATE_IN_STREAM state. */
		struct {
			/** Number of documents read in stream. */
			uint32_t doc_count;
		} stream;
		/**
		 * Additional state for \ref CYAML_STATE_IN_MAP_KEY and
		 *  \ref CYAML_STATE_IN_MAP_VALUE states.
		 */
		struct {
			const cyaml_schema_field_t *field;
		} mapping;
		/**  Additional state for \ref CYAML_STATE_IN_SEQUENCE state. */
		struct {
			uint32_t entry;
			uint32_t count;
		} sequence;
	};
	const uint8_t *data; /**< Start of client value data for this state. */
	bool done; /**< Whether the state has been handled. */
} cyaml_state_t;

/**
 * Internal YAML loading context.
 */
typedef struct cyaml_ctx {
	const cyaml_config_t *config; /**< Settings provided by client. */
	cyaml_state_t *state;   /**< Current entry in state stack, or NULL. */
	cyaml_state_t *stack;   /**< State stack */
	uint32_t stack_idx;     /**< Next (empty) state stack slot */
	uint32_t stack_max;     /**< Current stack allocation limit. */
	unsigned seq_count;     /**< Top-level sequence count. */
	yaml_emitter_t *emitter;  /**< Internal libyaml parser object. */
} cyaml_ctx_t;

/**
 * Ensure that the CYAML load context has space for a new stack entry.
 *
 * \param[in]  ctx     The CYAML saving context.
 * \return \ref CYAML_OK on success, or appropriate error code otherwise.
 */
static cyaml_err_t cyaml__stack_ensure(
		cyaml_ctx_t *ctx)
{
	cyaml_state_t *temp;
	uint32_t max = ctx->stack_max + 16;

	if (ctx->stack_idx < ctx->stack_max) {
		return CYAML_OK;
	}

	temp = cyaml__realloc(ctx->config, ctx->stack, 0,
			sizeof(*ctx->stack) * max, false);
	if (temp == NULL) {
		return CYAML_ERR_OOM;
	}

	ctx->stack = temp;
	ctx->stack_max = max;
	ctx->state = ctx->stack + ctx->stack_idx - 1;

	return CYAML_OK;
}

/**
 * Helper to simplify emitting libyaml events.
 *
 * This is a slightly peculiar function, but it is intended to reduce the
 * boilerplate required to emit events.
 *
 * Intended usage is something like:
 *
 * ```
 * cyaml_err_t err;
 * yaml_event_t event;
 * int ret = yaml_mapping_end_event_initialize(&event);
 * err = cyaml__emit_event_helper(ctx, ret, &event);
 * ```
 *
 * \param[in]  ctx    The CYAML saving context.
 * \param[in]  valid  Whether the event pointer is valid.  Typically this
 *                    will be the value returned by the libyaml call that
 *                    initialised the event.  As such, if `valid` is non-zero,
 *                    the helper will try to emit the event, otherwise, it
 *                    will return \ref CYAML_ERR_LIBYAML_EVENT_INIT.
 * \param[in]  event  The event to try to emit, if valid.
 * \return \ref CYAML_OK on success, or appropriate error code otherwise.
 */
static cyaml_err_t cyaml__emit_event_helper(
		const cyaml_ctx_t *ctx,
		int valid,
		yaml_event_t *event)
{
	if (valid == 0) {
		cyaml__log(ctx->config, CYAML_LOG_ERROR,
				"LibYAML: Failed to initialise event\n");
		return CYAML_ERR_LIBYAML_EVENT_INIT;
	}

	/* Emit event and update save state stack. */
	valid = yaml_emitter_emit(ctx->emitter, event);
	if (valid == 0) {
		cyaml__log(ctx->config, CYAML_LOG_ERROR,
				"LibYAML: Failed to emit event %s\n",
				ctx->emitter->problem);
		return CYAML_ERR_LIBYAML_EMITTER;
	}

	return CYAML_OK;
}

/**
 * Emit a YAML start event for the state being pushed to the stack.
 *
 * \param[in]  ctx     The CYAML saving context.
 * \param[in]  state   The CYAML load state we're pushing a stack entry for.
 * \return \ref CYAML_OK on success, or appropriate error code otherwise.
 */
static cyaml_err_t cyaml__stack_push_write_event(
		const cyaml_ctx_t *ctx,
		enum cyaml_state_e state)
{
	yaml_event_t event;
	int ret;

	/* Create any appropriate event for the new state. */
	switch (state) {
	case CYAML_STATE_START:
		ret = yaml_stream_start_event_initialize(&event,
				YAML_UTF8_ENCODING);
		break;
	case CYAML_STATE_IN_STREAM:
		ret = yaml_document_start_event_initialize(&event,
				NULL, NULL, NULL, 0);
		break;
	case CYAML_STATE_IN_DOC:
		return CYAML_OK;
	case CYAML_STATE_IN_MAP_KEY:
		ret = yaml_mapping_start_event_initialize(&event, NULL,
				(yaml_char_t *)YAML_MAP_TAG, 1,
				YAML_ANY_MAPPING_STYLE);
		break;
	case CYAML_STATE_IN_SEQUENCE:
		ret = yaml_sequence_start_event_initialize(&event, NULL,
				(yaml_char_t *)YAML_SEQ_TAG, 1,
				YAML_ANY_SEQUENCE_STYLE);
		break;
	default:
		return CYAML_ERR_INTERNAL_ERROR;
	}

	return cyaml__emit_event_helper(ctx, ret, &event);
}

/**
 * Push a new entry onto the CYAML load context's stack.
 *
 * \param[in]  ctx     The CYAML saving context.
 * \param[in]  state   The CYAML load state we're pushing a stack entry for.
 * \param[in]  schema  The CYAML schema for the value expected in state.
 * \param[in]  data    Pointer to where value's data should be read from.
 * \return \ref CYAML_OK on success, or appropriate error code otherwise.
 */
static cyaml_err_t cyaml__stack_push(
		cyaml_ctx_t *ctx,
		enum cyaml_state_e state,
		const cyaml_schema_value_t *schema,
		const cyaml_data_t *data)
{
	cyaml_err_t err;
	cyaml_state_t s = {
		.data = data,
		.state = state,
		.schema = schema,
	};

	err = cyaml__stack_push_write_event(ctx, state);
	if (err != CYAML_OK) {
		return err;
	}

	err = cyaml__stack_ensure(ctx);
	if (err != CYAML_OK) {
		return err;
	}

	switch (state) {
	case CYAML_STATE_IN_MAP_KEY:
		assert(schema->type == CYAML_MAPPING);
		s.mapping.field = schema->mapping.schema;
		break;
	default:
		break;
	}

	cyaml__log(ctx->config, CYAML_LOG_DEBUG,
			"PUSH[%u]: %s\n", ctx->stack_idx,
			cyaml__state_to_str(state));

	ctx->stack[ctx->stack_idx] = s;
	ctx->state = ctx->stack + ctx->stack_idx;
	ctx->stack_idx++;

	return CYAML_OK;
}

/**
 * Emit a YAML end event for the state being popped from the stack.
 *
 * This frees any resources owned by the stack entry.
 *
 * \param[in]  ctx     The CYAML saving context.
 * \param[in]  state   The CYAML load state we're popping from the stack.
 */
static cyaml_err_t cyaml__stack_pop_write_event(
		const cyaml_ctx_t *ctx,
		enum cyaml_state_e state)
{
	yaml_event_t event;
	int ret;

	/* Create any appropriate event for the new state. */
	switch (state) {
	case CYAML_STATE_START:
		return CYAML_OK;
	case CYAML_STATE_IN_STREAM:
		ret = yaml_stream_end_event_initialize(&event);
		break;
	case CYAML_STATE_IN_DOC:
		ret = yaml_document_end_event_initialize(&event, 0);
		break;
	case CYAML_STATE_IN_MAP_KEY:
		ret = yaml_mapping_end_event_initialize(&event);
		break;
	case CYAML_STATE_IN_SEQUENCE:
		ret = yaml_sequence_end_event_initialize(&event);
		break;
	default:
		return CYAML_ERR_INTERNAL_ERROR;
	}

	return cyaml__emit_event_helper(ctx, ret, &event);
}

/**
 * Pop the current entry on the CYAML load context's stack.
 *
 * This frees any resources owned by the stack entry.
 *
 * \param[in]  ctx     The CYAML saving context.
 * \param[in]  emit    Whether end events should be emitted.
 */
static void cyaml__stack_pop(
		cyaml_ctx_t *ctx,
		bool emit)
{
	uint32_t idx = ctx->stack_idx;

	assert(idx != 0);

	if (emit) {
		cyaml__stack_pop_write_event(ctx, ctx->state->state);
	}

	idx--;

	cyaml__log(ctx->config, CYAML_LOG_DEBUG, "POP[%u]: %s\n", idx,
			cyaml__state_to_str(ctx->state->state));

	ctx->state = (idx == 0) ? NULL : &ctx->stack[idx - 1];
	ctx->stack_idx = idx;
}

/**
 * Helper to make allocations for loaded YAML values.
 *
 * If the current state is sequence, this extends any existing allocation
 * for the sequence.
 *
 * The current CYAML loading context's state is updated with new allocation
 * address, where necessary.
 *
 * \param[in]      ctx            The CYAML saving context.
 * \param[in]      schema         The schema for value to get data pointer for.
 * \param[in,out]  value_data_io  Current address of value's data.  Updated to
 *                                new address if value is allocation requiring
 *                                an allocation.
 * \return \ref CYAML_OK on success, or appropriate error code otherwise.
 */
static cyaml_err_t cyaml__data_handle_pointer(
		const cyaml_ctx_t *ctx,
		const cyaml_schema_value_t *schema,
		const uint8_t **value_data_io)
{
	cyaml_err_t err = CYAML_OK;

	if (schema->flags & CYAML_FLAG_POINTER) {
		const uint8_t *data = *value_data_io;

		data = (void *)cyaml_data_read(sizeof(char *), data, &err);
		if (err != CYAML_OK) {
			return err;
		}

		cyaml__log(ctx->config, CYAML_LOG_DEBUG,
				"Handle pointer: %p --> %p\n",
				*value_data_io, data);

		*value_data_io = data;
	}

	return err;
}

/**
 * Dump a backtrace to the log.
 *
 * \param[in]  ctx     The CYAML saving context.
 */
static void cyaml__backtrace(
		const cyaml_ctx_t *ctx)
{
	if (ctx->stack_idx > 1) {
		cyaml__log(ctx->config, CYAML_LOG_ERROR, "Backtrace:\n");
	} else {
		return;
	}

	for (uint32_t idx = ctx->stack_idx - 1; idx != 0; idx--) {
		cyaml_state_t *state = ctx->stack + idx;
		switch (state->state) {
		case CYAML_STATE_IN_MAP_KEY: /* Fall through. */
		case CYAML_STATE_IN_MAP_VALUE:
			assert(state->mapping.field != NULL);
			cyaml__log(ctx->config, CYAML_LOG_ERROR,
					"  in mapping field: %s\n",
					state->mapping.field->key);
			break;
		case CYAML_STATE_IN_SEQUENCE:
			cyaml__log(ctx->config, CYAML_LOG_ERROR,
					"  in sequence entry: %"PRIu32"\n",
					state->sequence.count);
			break;
		default:
			/** \todo \ref CYAML_STATE_IN_DOC handling for multi
			 *        document streams.
			 */
			break;
		}
	}
}

/**
 * Helper to emit YAML scalar events, using libyaml.
 *
 * \param[in]  ctx     The CYAML saving context.
 * \param[in]  schema  The schema for the value to emit.
 * \param[in]  value   The value to emit as a null-terminated C string.
 * \param[in]  tag     YAML tag to use for output.
 * \return \ref CYAML_OK on success, or appropriate error code otherwise.
 */
static cyaml_err_t cyaml__emit_scalar(
		const cyaml_ctx_t *ctx,
		const cyaml_schema_value_t *schema,
		const char *value,
		const char *tag)
{
	int ret;
	yaml_event_t event;

	if (schema == NULL) {
		cyaml__log(ctx->config, CYAML_LOG_INFO, "[%s]\n", value);
	} else {
		cyaml__log(ctx->config, CYAML_LOG_INFO, "  <%s>\n", value);
	}

	ret = yaml_scalar_event_initialize(&event, NULL,
			(yaml_char_t *)tag,
			(yaml_char_t *)value,
			strlen(value),
			1, 0, YAML_PLAIN_SCALAR_STYLE);

	return cyaml__emit_event_helper(ctx, ret, &event);
}

/**
 * Convert signed integer to string.
 *
 * \param[in]  value  The integer to convert.
 * \return String conversion of the value.
 */
static const char * cyaml__get_int(
		int64_t value)
{
	static char string[32];

	sprintf(string, "%"PRIi64, value);

	return string;
}

/**
 * Convert unsigned integer to string.
 *
 * \param[in]  value  The integer to convert.
 * \return String conversion of the value.
 */
static const char * cyaml__get_uint(
		uint64_t value)
{
	static char string[32];

	sprintf(string, "%"PRIu64, value);

	return string;
}

/**
 * Convert double precision floating point value to string.
 *
 * \param[in]  value  The value to convert.
 * \return String conversion of the value.
 */
static const char * cyaml__get_double(
		double value)
{
	static char string[64];

	sprintf(string, "%g", value);

	return string;
}

/**
 * Pad a signed value that's smaller than 64-bit to an int64_t.
 *
 * This sets all the bits in the padded region.
 *
 * \param[in]  raw   Contains a signed value of size bytes.
 * \param[in]  size  Number of bytes used in raw.
 * \return Value padded to 64-bit signed.
 */
static int64_t cyaml_sign_pad(uint64_t raw, size_t size)
{
	uint64_t sign_bit = 1 << (size * CHAR_BIT - 1);
	unsigned padding = (sizeof(raw) - size) * CHAR_BIT;

	if ((sign_bit & raw) && (padding != 0)) {
		raw |= (((uint64_t)1 << padding) - 1) << (size * CHAR_BIT);
	}

	return (int64_t)raw;
}

/**
 * Read a value of type \ref CYAML_INT.
 *
 * \param[in]  ctx     The CYAML saving context.
 * \param[in]  schema  The schema for the value to be read.
 * \param[in]  data    The place to read the value from in the client data.
 * \return \ref CYAML_OK on success, or appropriate error code otherwise.
 */
static cyaml_err_t cyaml__write_int(
		const cyaml_ctx_t *ctx,
		const cyaml_schema_value_t *schema,
		const uint8_t *data)
{
	cyaml_err_t err;
	int64_t number;

	number = cyaml_sign_pad(
			cyaml_data_read(schema->data_size, data, &err),
			schema->data_size);
	if (err == CYAML_OK) {
		const char *string = cyaml__get_int(number);
		err = cyaml__emit_scalar(ctx, schema, string, YAML_INT_TAG);
	}

	return err;
}

/**
 * Read a value of type \ref CYAML_UINT.
 *
 * \param[in]  ctx     The CYAML saving context.
 * \param[in]  schema  The schema for the value to be read.
 * \param[in]  data    The place to read the value from in the client data.
 * \return \ref CYAML_OK on success, or appropriate error code otherwise.
 */
static cyaml_err_t cyaml__write_uint(
		const cyaml_ctx_t *ctx,
		const cyaml_schema_value_t *schema,
		const uint8_t *data)
{
	uint64_t number;
	cyaml_err_t err;

	number = cyaml_data_read(schema->data_size, data, &err);
	if (err == CYAML_OK) {
		const char *string = cyaml__get_uint(number);
		err = cyaml__emit_scalar(ctx, schema, string, YAML_INT_TAG);
	}

	return err;
}

/**
 * Read a value of type \ref CYAML_BOOL.
 *
 * \param[in]  ctx     The CYAML saving context.
 * \param[in]  schema  The schema for the value to be read.
 * \param[in]  data    The place to read the value from in the client data.
 * \return \ref CYAML_OK on success, or appropriate error code otherwise.
 */
static cyaml_err_t cyaml__write_bool(
		const cyaml_ctx_t *ctx,
		const cyaml_schema_value_t *schema,
		const uint8_t *data)
{
	uint64_t number;
	cyaml_err_t err;

	number = cyaml_data_read(schema->data_size, data, &err);
	if (err == CYAML_OK) {
		err = cyaml__emit_scalar(ctx, schema,
			number ? "true" : "false", YAML_BOOL_TAG);
	}

	return err;
}

/**
 * Read a value of type \ref CYAML_ENUM.
 *
 * \param[in]  ctx     The CYAML saving context.
 * \param[in]  schema  The schema for the value to be read.
 * \param[in]  data    The place to read the value from in the client data.
 * \return \ref CYAML_OK on success, or appropriate error code otherwise.
 */
static cyaml_err_t cyaml__write_enum(
		const cyaml_ctx_t *ctx,
		const cyaml_schema_value_t *schema,
		const uint8_t *data)
{
	int64_t number;
	cyaml_err_t err;

	number = cyaml_data_read(schema->data_size, data, &err);
	if (err == CYAML_OK) {
		const char * const *strings = schema->enumeration.strings;
		const char *string = NULL;
		while (strings != NULL && *strings != NULL) {
			if (number == (strings - schema->enumeration.strings)) {
				string = *strings;
				break;
			}
			strings++;
		}
		if (string == NULL) {
			if (schema->flags & CYAML_FLAG_STRICT) {
				return CYAML_ERR_INVALID_VALUE;
			} else {
				return cyaml__write_int(ctx, schema, data);
			}
		}
		err = cyaml__emit_scalar(ctx, schema, string, YAML_STR_TAG);
	}

	return err;
}

/**
 * Read a value of type \ref CYAML_FLOAT.
 *
 * The `data_size` of the schema entry must be the size of a known
 * floating point C type.
 *
 * \note The `long double` type was causing problems, so it isn't currently
 *       supported.
 *
 * \param[in]  ctx     The CYAML saving context.
 * \param[in]  schema  The schema for the value to be read.
 * \param[in]  data    The place to read the value from in the client data.
 * \return \ref CYAML_OK on success, or appropriate error code otherwise.
 */
static cyaml_err_t cyaml__write_float(
		const cyaml_ctx_t *ctx,
		const cyaml_schema_value_t *schema,
		const uint8_t *data)
{
	const char *string = NULL;

	if (schema->data_size == sizeof(float)) {
		float number;
		memcpy(&number, data, schema->data_size);
		string = cyaml__get_double(number);

	} else if (schema->data_size == sizeof(double)) {
		double number;
		memcpy(&number, data, schema->data_size);
		string = cyaml__get_double(number);
	} else {
		return CYAML_ERR_INVALID_DATA_SIZE;
	}

	return cyaml__emit_scalar(ctx, schema, string, YAML_FLOAT_TAG);
}

/**
 * Read a value of type \ref CYAML_STRING.
 *
 * \param[in]  ctx     The CYAML saving context.
 * \param[in]  schema  The schema for the value to be read.
 * \param[in]  data    The place to read the value from in the client data.
 * \return \ref CYAML_OK on success, or appropriate error code otherwise.
 */
static cyaml_err_t cyaml__write_string(
		const cyaml_ctx_t *ctx,
		const cyaml_schema_value_t *schema,
		const uint8_t *data)
{
	return cyaml__emit_scalar(ctx, schema, (const char *)data,
			YAML_STR_TAG);
}

/**
 * Read a scalar value.
 *
 * \param[in]  ctx     The CYAML saving context.
 * \param[in]  schema  The schema for the value to be read.
 * \param[in]  data    The place to read the value from in the client data.
 * \return \ref CYAML_OK on success, or appropriate error code otherwise.
 */
static cyaml_err_t cyaml__write_scalar_value(
		const cyaml_ctx_t *ctx,
		const cyaml_schema_value_t *schema,
		const cyaml_data_t *data)
{
	typedef cyaml_err_t (*cyaml_read_scalar_fn)(
			const cyaml_ctx_t *ctx,
			const cyaml_schema_value_t *schema,
			const uint8_t *data_target);
	static const cyaml_read_scalar_fn fn[CYAML__TYPE_COUNT] = {
		[CYAML_INT]    = cyaml__write_int,
		[CYAML_UINT]   = cyaml__write_uint,
		[CYAML_BOOL]   = cyaml__write_bool,
		[CYAML_ENUM]   = cyaml__write_enum,
		[CYAML_FLOAT]  = cyaml__write_float,
		[CYAML_STRING] = cyaml__write_string,
	};

	assert(fn[schema->type] != NULL);

	return fn[schema->type](ctx, schema, data);
}

/**
 * Emit a sequence of flag values.
 *
 * \param[in]  ctx     The CYAML saving context.
 * \param[in]  schema  The schema for the value to be read.
 * \param[in]  data    The place to read the value from in the client data.
 * \param[in]  number  The value of the flag data.
 * \return \ref CYAML_OK on success, or appropriate error code otherwise.
 */
static cyaml_err_t cyaml__emit_flags_sequence(
		const cyaml_ctx_t *ctx,
		const cyaml_schema_value_t *schema,
		const cyaml_data_t *data,
		int64_t number)
{
	const char * const *strings = schema->enumeration.strings;
	yaml_event_t event;
	cyaml_err_t err;
	int ret;

	ret = yaml_sequence_start_event_initialize(&event, NULL,
			(yaml_char_t *)YAML_SEQ_TAG, 1,
			YAML_ANY_SEQUENCE_STYLE);

	err = cyaml__emit_event_helper(ctx, ret, &event);
	if (err != CYAML_OK) {
		return err;
	}

	while (strings != NULL && *strings != NULL) {
		cyaml_err_t err;
		int64_t flag;

		flag = ((int64_t)1) << (strings - schema->enumeration.strings);
		if (number & flag) {
			err = cyaml__emit_scalar(ctx, schema, *strings,
					YAML_STR_TAG);
			if (err != CYAML_OK) {
				return err;
			}
			number &= ~flag;
		}
		strings++;
	}
	if (number != 0) {
		if (schema->flags & CYAML_FLAG_STRICT) {
			return CYAML_ERR_INVALID_VALUE;
		} else {
			return cyaml__write_int(ctx, schema, data);
		}
	}

	ret = yaml_sequence_end_event_initialize(&event);

	return cyaml__emit_event_helper(ctx, ret, &event);
}

/**
 * Read a value of type \ref CYAML_FLAGS.
 *
 * Since \ref CYAML_FLAGS is a composite value (a sequence of scalars), rather
 * than a simple scaler, this consumes events from the YAML input stream.
 *
 * \param[in]  ctx     The CYAML saving context.
 * \param[in]  schema  The schema for the value to be read.
 * \param[in]  data    The place to read the value from in the client data.
 * \return \ref CYAML_OK on success, or appropriate error code otherwise.
 */
static cyaml_err_t cyaml__write_flags_value(
		const cyaml_ctx_t *ctx,
		const cyaml_schema_value_t *schema,
		const cyaml_data_t *data)
{
	int64_t number;
	cyaml_err_t err;

	number = cyaml_data_read(schema->data_size, data, &err);
	if (err == CYAML_OK) {
		err = cyaml__emit_flags_sequence(ctx, schema, data, number);
	}

	return err;
}

/**
 * Handle a YAML event corresponding to a YAML data value.
 *
 * \param[in]  ctx        The CYAML saving context.
 * \param[in]  schema     CYAML schema for the expected value.
 * \param[in]  data    The place to read the value from in the client data.
 * \param[in]  seq_count  Entry count for sequence values.  Unused for
 *                        non-sequence values.
 * \return \ref CYAML_OK on success, or appropriate error code otherwise.
 */
static cyaml_err_t cyaml__write_value(
		cyaml_ctx_t *ctx,
		const cyaml_schema_value_t *schema,
		const uint8_t *data,
		unsigned seq_count)
{
	cyaml_err_t err;

	err = cyaml__data_handle_pointer(ctx, schema, &data);
	if (err != CYAML_OK) {
		return err;
	}

	switch (schema->type) {
	case CYAML_INT:   /* Fall through. */
	case CYAML_UINT:  /* Fall through. */
	case CYAML_BOOL:  /* Fall through. */
	case CYAML_ENUM:  /* Fall through. */
	case CYAML_FLOAT: /* Fall through. */
	case CYAML_STRING:
		err = cyaml__write_scalar_value(ctx, schema, data);
		break;
	case CYAML_FLAGS:
		err = cyaml__write_flags_value(ctx, schema, data);
		break;
	case CYAML_MAPPING:
		err = cyaml__stack_push(ctx, CYAML_STATE_IN_MAP_KEY,
				schema, data);
		break;
	case CYAML_SEQUENCE: /* Fall through. */
	case CYAML_SEQUENCE_FIXED:
		err = cyaml__stack_push(ctx, CYAML_STATE_IN_SEQUENCE,
				schema, data);
		if (err == CYAML_OK) {
			ctx->state->sequence.count = seq_count;
		}
		break;
	case CYAML_IGNORE:
		/* Nothing to output for ignored values; CYAML_IGNORE is an
		 * input filter only. */
		break;
	default:
		err = CYAML_ERR_BAD_TYPE_IN_SCHEMA;
		break;
	}

	return err;
}

/**
 * YAML saving handler for the \ref CYAML_STATE_START state.
 *
 * \param[in]  ctx  The CYAML saving context.
 * \return \ref CYAML_OK on success, or appropriate error code otherwise.
 */
static cyaml_err_t cyaml__write_start(
		cyaml_ctx_t *ctx)
{
	return cyaml__stack_push(ctx, CYAML_STATE_IN_STREAM,
			ctx->state->schema, ctx->state->data);
}

/**
 * YAML loading handler for the \ref CYAML_STATE_IN_STREAM state.
 *
 * \param[in]  ctx  The CYAML saving context.
 * \return \ref CYAML_OK on success, or appropriate error code otherwise.
 */
static cyaml_err_t cyaml__write_stream(
		cyaml_ctx_t *ctx)
{
	cyaml_err_t err;

	if (ctx->state->done) {
		err = CYAML_OK;
		cyaml__stack_pop(ctx, true);
	} else {
		ctx->stack[CYAML_STATE_IN_STREAM].done = true;
		err = cyaml__stack_push(ctx, CYAML_STATE_IN_DOC,
				ctx->state->schema, ctx->state->data);
	}
	return err;
}

/**
 * YAML loading handler for the \ref CYAML_STATE_IN_DOC state.
 *
 * \param[in]  ctx  The CYAML saving context.
 * \return \ref CYAML_OK on success, or appropriate error code otherwise.
 */
static cyaml_err_t cyaml__write_doc(
		cyaml_ctx_t *ctx)
{
	cyaml_err_t err;

	if (ctx->state->done) {
		err = CYAML_OK;
		cyaml__stack_pop(ctx, true);
	} else {
		unsigned seq_count = ctx->seq_count;
		if (ctx->state->schema->type == CYAML_SEQUENCE_FIXED) {
			seq_count = ctx->state->schema->sequence.max;
		}
		ctx->stack[CYAML_STATE_IN_DOC].done = true;
		err = cyaml__write_value(ctx,
				ctx->state->schema,
				ctx->state->data,
				seq_count);
	}
	return err;
}

/**
 * YAML loading handler for the \ref CYAML_STATE_IN_MAP_KEY and \ref
 * CYAML_STATE_IN_MAP_VALUE states.
 *
 * \param[in]  ctx  The CYAML saving context.
 * \return \ref CYAML_OK on success, or appropriate error code otherwise.
 */
static cyaml_err_t cyaml__write_mapping(
		cyaml_ctx_t *ctx)
{
	const cyaml_schema_field_t *field = ctx->state->mapping.field;
	cyaml_err_t err = CYAML_OK;

	if (field != NULL && field->key != NULL) {
		unsigned seq_count = 0;

		err = cyaml__emit_scalar(ctx, NULL, field->key,
				YAML_STR_TAG);
		if (err != CYAML_OK) {
			return err;
		}

		/* Advance the field before writing value, since writing the
		 * value can put a new state entry on the stack. */
		ctx->state->mapping.field++;

		if (field->value.type == CYAML_SEQUENCE) {
			seq_count = cyaml_data_read(field->count_size,
					ctx->state->data + field->count_offset,
					&err);
			if (err != CYAML_OK) {
				return err;
			}
			cyaml__log(ctx->config, CYAML_LOG_INFO,
			"Sequence entry count: %u\n", seq_count);

		} else if (field->value.type == CYAML_SEQUENCE_FIXED) {
			if (field->value.sequence.min !=
			    field->value.sequence.max) {
				return CYAML_ERR_SEQUENCE_FIXED_COUNT;
			}
			seq_count = field->value.sequence.min;
		}

		err = cyaml__write_value(ctx,
				&field->value,
				ctx->state->data + field->data_offset,
				seq_count);
	} else {
		cyaml__stack_pop(ctx, true);
	}

	return err;
}

/**
 * YAML loading handler for the \ref CYAML_STATE_IN_SEQUENCE state.
 *
 * \param[in]  ctx  The CYAML saving context.
 * \return \ref CYAML_OK on success, or appropriate error code otherwise.
 */
static cyaml_err_t cyaml__write_sequence(
		cyaml_ctx_t *ctx)
{
	cyaml_err_t err = CYAML_OK;

	if (ctx->state->sequence.entry < ctx->state->sequence.count) {
		const cyaml_schema_value_t *schema = ctx->state->schema;
		const cyaml_schema_value_t *value = schema->sequence.schema;
		size_t offset = value->data_size * ctx->state->sequence.entry;
		unsigned seq_count = 0;

		cyaml__log(ctx->config, CYAML_LOG_INFO,
				"Sequence entry %u of %u\n",
				ctx->state->sequence.entry + 1,
				ctx->state->sequence.count);

		/* Advance the entry before writing value, since writing the
		 * value can put a new state entry on the stack. */
		ctx->state->sequence.entry++;

		if (value->type == CYAML_SEQUENCE) {
			return CYAML_ERR_SEQUENCE_IN_SEQUENCE;

		} else if (value->type == CYAML_SEQUENCE_FIXED) {
			if (value->sequence.min != value->sequence.max) {
				return CYAML_ERR_SEQUENCE_FIXED_COUNT;
			}
			seq_count = value->sequence.max;
		}

		err = cyaml__write_value(ctx, value,
				ctx->state->data + offset,
				seq_count);
	} else {
		cyaml__stack_pop(ctx, true);
	}

	return err;
}

/**
 * Check that common save params from client are valid.
 *
 * \param[in] config     The client's CYAML library config.
 * \param[in] schema     The schema describing the content of data.
 * \param[in] data       Points to client's data.
 * \param[in] seq_count  Top level sequence count.
 * \return \ref CYAML_OK on success, or appropriate error code otherwise.
 */
static inline cyaml_err_t cyaml__validate_save_params(
		const cyaml_config_t *config,
		const cyaml_schema_value_t *schema,
		const cyaml_data_t *data,
		unsigned seq_count)
{
	if (config == NULL) {
		return CYAML_ERR_BAD_PARAM_NULL_CONFIG;
	}
	if (config->mem_fn == NULL) {
		return CYAML_ERR_BAD_CONFIG_NULL_MEMFN;
	}
	if (schema == NULL) {
		return CYAML_ERR_BAD_PARAM_NULL_SCHEMA;
	}
	if ((schema->type == CYAML_SEQUENCE) != (seq_count != 0)) {
		return CYAML_ERR_BAD_PARAM_SEQ_COUNT;
	}
	if (!(schema->flags & CYAML_FLAG_POINTER)) {
		return CYAML_ERR_TOP_LEVEL_NON_PTR;
	}
	if (data == NULL) {
		return CYAML_ERR_BAD_PARAM_NULL_DATA;
	}
	return CYAML_OK;
}

/**
 * The main YAML saving function.
 *
 * The public interfaces are wrappers around this.
 *
 * \param[in] config     Client's CYAML configuration structure.
 * \param[in] schema     CYAML schema for the YAML to be saved.
 * \param[in] data       The caller-owned data to be saved.
 * \param[in] seq_count  If top level type is sequence, this should be the
 *                       entry count, otherwise it is ignored.
 * \param[in] emitter    An initialised `libyaml` emitter object
 *                       with its output set.
 * \return \ref CYAML_OK on success, or appropriate error code otherwise.
 */
static cyaml_err_t cyaml__save(
		const cyaml_config_t *config,
		const cyaml_schema_value_t *schema,
		const cyaml_data_t *data,
		unsigned seq_count,
		yaml_emitter_t *emitter)
{
	cyaml_ctx_t ctx = {
		.config = config,
		.emitter = emitter,
		.seq_count = seq_count,
	};
	typedef cyaml_err_t (* const cyaml_write_fn)(
			cyaml_ctx_t *ctx);
	static const cyaml_write_fn fn[CYAML_STATE__COUNT] = {
		[CYAML_STATE_START]        = cyaml__write_start,
		[CYAML_STATE_IN_STREAM]    = cyaml__write_stream,
		[CYAML_STATE_IN_DOC]       = cyaml__write_doc,
		[CYAML_STATE_IN_MAP_KEY]   = cyaml__write_mapping,
		[CYAML_STATE_IN_MAP_VALUE] = cyaml__write_mapping,
		[CYAML_STATE_IN_SEQUENCE]  = cyaml__write_sequence,
	};
	cyaml_err_t err = CYAML_OK;

	err = cyaml__validate_save_params(config, schema, data, seq_count);
	if (err != CYAML_OK) {
		return err;
	}

	err = cyaml__stack_push(&ctx, CYAML_STATE_START, schema, &data);
	if (err != CYAML_OK) {
		goto out;
	}

	do {
		cyaml__log(ctx.config, CYAML_LOG_DEBUG, "Handle state %s\n",
				cyaml__state_to_str(ctx.state->state));
		err = fn[ctx.state->state](&ctx);
		if (err != CYAML_OK) {
			goto out;
		}
	} while (ctx.state->state > CYAML_STATE_START);

	cyaml__stack_pop(&ctx, true);

	assert(ctx.stack_idx == 0);

out:
	if (err != CYAML_OK) {
		cyaml__backtrace(&ctx);
	}
	while (ctx.stack_idx > 0) {
		cyaml__stack_pop(&ctx, false);
	}
	cyaml__free(config, ctx.stack);
	return err;
}

/* Exported function, documented in include/cyaml/cyaml.h */
cyaml_err_t cyaml_save_file(
		const char *path,
		const cyaml_config_t *config,
		const cyaml_schema_value_t *schema,
		const cyaml_data_t *data,
		unsigned seq_count)
{
	FILE *file;
	cyaml_err_t err;
	yaml_emitter_t emitter;

	/* Initialize parser */
	if (!yaml_emitter_initialize(&emitter)) {
		return CYAML_ERR_LIBYAML_EMITTER_INIT;
	}

	/* Open input file. */
	file = fopen(path, "w");
	if (file == NULL) {
		yaml_emitter_delete(&emitter);
		return CYAML_ERR_FILE_OPEN;
	}

	/* Set input file */
	yaml_emitter_set_output_file(&emitter, file);

	/* Parse the input */
	err = cyaml__save(config, schema, data, seq_count, &emitter);
	if (err != CYAML_OK) {
		yaml_emitter_delete(&emitter);
		fclose(file);
		return err;
	}

	/* Cleanup */
	yaml_emitter_delete(&emitter);
	fclose(file);

	return CYAML_OK;
}

/** CYAML save buffer context. */
typedef struct cyaml_buffer_ctx {
	/** Client's CYAML configuration structure. */
	const cyaml_config_t *config;
	size_t len;  /**< Current length of `data` allocation. */
	size_t used; /**< Current number of bytes used in `data`. */
	char *data;  /**< Current allocation for serialised output. */
} cyaml_buffer_ctx_t;

/**
 * Write handler for libyaml.
 *
 * The write handler is called when the emitter needs to flush the accumulated
 * characters to the output.  The handler should write size bytes of the
 * buffer to the output.
 *
 * \todo Could be more efficient about this, but for now, this is fine.
 *
 * \param[in]  data    A pointer to cyaml buffer context struture.
 * \param[in]  buffer  The buffer with bytes to be written.
 * \param[in]  size    The number of bytes to be written.
 * \return 1 on sucess, 0 otherwise.
 */
static int cyaml__buffer_handler(
		void *data,
		unsigned char *buffer,
		size_t size)
{
	cyaml_buffer_ctx_t *buffer_ctx = data;
	enum {
		RETURN_SUCCESS = 1,
		RETURN_FAILURE = 0,
	};

	if (size > (buffer_ctx->len - buffer_ctx->used)) {
		char *temp = cyaml__realloc(
				buffer_ctx->config,
				buffer_ctx->data,
				buffer_ctx->len,
				buffer_ctx->used + size,
				false);
		if (temp == NULL) {
			return RETURN_FAILURE;
		}
		buffer_ctx->data = temp;
		buffer_ctx->len = buffer_ctx->used + size;
	}

	memcpy(buffer_ctx->data + buffer_ctx->used, buffer, size);
	buffer_ctx->used += size;

	return RETURN_SUCCESS;
}

/* Exported function, documented in include/cyaml/cyaml.h */
cyaml_err_t cyaml_save_data(
		char **output,
		size_t *len,
		const cyaml_config_t *config,
		const cyaml_schema_value_t *schema,
		const cyaml_data_t *data,
		unsigned seq_count)
{
	cyaml_err_t err;
	yaml_emitter_t emitter;
	cyaml_buffer_ctx_t buffer_ctx = {
		.config = config,
	};

	/* Initialize parser */
	if (!yaml_emitter_initialize(&emitter)) {
		return CYAML_ERR_LIBYAML_EMITTER_INIT;
	}

	/* Set input file */
	yaml_emitter_set_output(&emitter, cyaml__buffer_handler, &buffer_ctx);

	/* Parse the input */
	err = cyaml__save(config, schema, data, seq_count, &emitter);
	if (err != CYAML_OK) {
		yaml_emitter_delete(&emitter);
		cyaml__free(config, buffer_ctx.data);
		return err;
	}

	/* Cleanup */
	yaml_emitter_delete(&emitter);

	*output = buffer_ctx.data;
	*len = buffer_ctx.used;

	return CYAML_OK;
}
