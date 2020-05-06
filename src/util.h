/*
 * SPDX-License-Identifier: ISC
 *
 * Copyright (C) 2017-2020 Michael Drake <tlsa@netsurf-browser.org>
 */

/**
 * \file
 * \brief CYAML common utility functions.
 */

#ifndef CYAML_UTIL_H
#define CYAML_UTIL_H

#include "cyaml/cyaml.h"
#include "utf8.h"

/** Compile time assertion macro. */
#define cyaml_static_assert(e) \
{ \
	enum { \
		cyaml_static_assert_check = 1 / (!!(e)) \
	}; \
}

/** Macro to squash unused variable compiler warnings. */
#define CYAML_UNUSED(_x) ((void)(_x))

/**
 * Check whether the host is little endian.
 *
 * Checks whether least significant bit is in the first byte of a `uint16_t`.
 *
 * \return true if host is little endian.
 */
static inline bool cyaml__host_is_little_endian(void)
{
	static const uint16_t test = 1;

	return ((uint8_t *) &test)[0] == 1;
}

/**
 * Check whether the host is big endian.
 *
 * \return true if host is big endian.
 */
static inline bool cyaml__host_is_big_endian(void)
{
	return !cyaml__host_is_little_endian();
}

/** CYAML bitfield type. */
typedef uint32_t cyaml_bitfield_t;

/** Number of bits in \ref cyaml_bitfield_t. */
#define CYAML_BITFIELD_BITS (sizeof(cyaml_bitfield_t) * CHAR_BIT)

/** CYAML state machine states. */
enum cyaml_state_e {
	CYAML_STATE_START,        /**< Initial state. */
	CYAML_STATE_IN_STREAM,    /**< In a stream. */
	CYAML_STATE_IN_DOC,       /**< In a document. */
	CYAML_STATE_IN_MAP_KEY,   /**< In a mapping. */
	CYAML_STATE_IN_MAP_VALUE, /**< In a mapping. */
	CYAML_STATE_IN_SEQUENCE,  /**< In a sequence. */
	CYAML_STATE__COUNT,       /**< Count of states, **not a valid
	                               state itself**. */
};

/**
 * Convert a CYAML state into a human readable string.
 *
 * \param[in]  state  The state to convert.
 * \return String representing state.
 */
static inline const char * cyaml__state_to_str(enum cyaml_state_e state)
{
	static const char * const strings[CYAML_STATE__COUNT] = {
		[CYAML_STATE_START]        = "start",
		[CYAML_STATE_IN_STREAM]    = "in stream",
		[CYAML_STATE_IN_DOC]       = "in doc",
		[CYAML_STATE_IN_MAP_KEY]   = "in mapping (key)",
		[CYAML_STATE_IN_MAP_VALUE] = "in mapping (value)",
		[CYAML_STATE_IN_SEQUENCE]  = "in sequence",
	};
	if ((unsigned)state >= CYAML_STATE__COUNT) {
		return "<invalid>";
	}
	return strings[state];
}

/**
 * Convert a CYAML type into a human readable string.
 *
 * \param[in]  type  The state to convert.
 * \return String representing state.
 */
static inline const char * cyaml__type_to_str(cyaml_type_e type)
{
	static const char * const strings[CYAML__TYPE_COUNT] = {
		[CYAML_INT]            = "INT",
		[CYAML_UINT]           = "UINT",
		[CYAML_BOOL]           = "BOOL",
		[CYAML_ENUM]           = "ENUM",
		[CYAML_FLAGS]          = "FLAGS",
		[CYAML_FLOAT]          = "FLOAT",
		[CYAML_UNION]          = "UNION",
		[CYAML_STRING]         = "STRING",
		[CYAML_MAPPING]        = "MAPPING",
		[CYAML_BITFIELD]       = "BITFIELD",
		[CYAML_SEQUENCE]       = "SEQUENCE",
		[CYAML_SEQUENCE_FIXED] = "SEQUENCE_FIXED",
		[CYAML_IGNORE]         = "IGNORE",
	};
	if ((unsigned)type >= CYAML__TYPE_COUNT) {
		return "<invalid>";
	}
	return strings[type];
}

/**
 * Log to client's logging function, if provided.
 *
 * \param[in] cfg    CYAML client config structure.
 * \param[in] level  Log level of message to log.
 * \param[in] fmt    Format string for message to log.
 * \param[in] ...    Additional arguments used by fmt.
 */
static inline void cyaml__log(
		const cyaml_config_t *cfg,
		cyaml_log_t level,
		const char *fmt, ...)
{
	if (level >= cfg->log_level && cfg->log_fn != NULL) {
		va_list args;
		va_start(args, fmt);
		cfg->log_fn(level, cfg->log_ctx, fmt, args);
		va_end(args);
	}
}

/**
 * Check if comparason should be case sensitive.
 *
 * As described in the API, schema flags take priority over config flags.
 *
 * \param[in]  config  Client's CYAML configuration structure.
 * \param[in]  schema  The CYAML schema for the value to be compared.
 * \return Whether to use case-sensitive comparason.
 */
static inline bool cyaml__is_case_sensitive(
		const cyaml_config_t *config,
		const cyaml_schema_value_t *schema)
{
	if (schema->flags & CYAML_FLAG_CASE_INSENSITIVE) {
		return false;

	} else if (schema->flags & CYAML_FLAG_CASE_SENSITIVE) {
		return true;

	} else if (config->flags & CYAML_CFG_CASE_INSENSITIVE) {
		return false;

	}

	return true;
}

/**
 * Compare two strings.
 *
 * Depending on the client's configuration, and the value's schema,
 * this will do either a case-sensitive or case-insensitive comparason.
 *
 * \param[in]  config  Client's CYAML configuration structure.
 * \param[in]  schema  The CYAML schema for the value to be compared.
 * \param[in]  str1    First string to be compared.
 * \param[in]  str2    Second string to be compared.
 * \return 0 if and only if strings are equal.
 */
static inline int cyaml__strcmp(
		const cyaml_config_t *config,
		const cyaml_schema_value_t *schema,
		const void * const str1,
		const void * const str2)
{
	if (cyaml__is_case_sensitive(config, schema)) {
		return strcmp(str1, str2);
	}

	return cyaml_utf8_casecmp(str1, str2);
}

/** Identifies that no mapping schema entry was found for key. */
#define CYAML_FIELDS_IDX_NONE 0xffff

/**
 * Get the offset to a mapping field by key in a mapping schema array.
 *
 * \param[in]  cfg     The client's CYAML library config.
 * \param[in]  schema  CYAML schema for the mapping value to be loaded.
 * \param[in]  key     Key to search for in mapping schema.
 * \return index the mapping schema's mapping fields array for key, or
 *         \ref CYAML_FIELDS_IDX_NONE if key is not present in schema.
 */
static inline uint16_t cyaml__get_mapping_field_idx(
		const cyaml_config_t *cfg,
		const cyaml_schema_value_t *schema,
		const char *key)
{
	const cyaml_schema_field_t *fields = schema->mapping.fields;
	uint16_t index = 0;

	assert(schema->type == CYAML_UNION ||
	       schema->type == CYAML_MAPPING);

	/* Step through each entry in the schema */
	for (; fields->key != NULL; fields++) {
		if (cyaml__strcmp(cfg, schema, fields->key, key) == 0) {
			return index;
		}
		index++;
	}

	return CYAML_FIELDS_IDX_NONE;
}

/**
 * Count the entries in a mapping field array schema.
 *
 * The mapping schema array must be terminated by an entry with a NULL key.
 *
 * \param[in]  mapping_schema  Array of mapping schema fields.
 * \return Number of entries in mapping_schema array.
 */
static inline uint16_t cyaml__get_mapping_field_count(
		const cyaml_schema_field_t *mapping_schema)
{
	const cyaml_schema_field_t *entry = mapping_schema;

	while (entry->key != NULL) {
		entry++;
	}

	return (uint16_t)(entry - mapping_schema);
}

/**
 * Check of all the bits of a mask are set in a cyaml value flag word.
 *
 * \param[in] flags  The value flags to test.
 * \param[in] mask   Mask of the bits to test for in flags.
 * \return true if all bits of mask are set in flags.
 */
static inline bool cyaml__flag_check_all(
		enum cyaml_flag flags,
		enum cyaml_flag mask)
{
	return ((flags & mask) == mask);
}

#endif
