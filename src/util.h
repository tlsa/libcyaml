/*
 * SPDX-License-Identifier: ISC
 *
 * Copyright (C) 2017 Michael Drake <tlsa@netsurf-browser.org>
 */

/**
 * \file
 * \brief CYAML common utility functions.
 */

#ifndef CYAML_UTIL_H
#define CYAML_UTIL_H

#include "cyaml/cyaml.h"

/** Macro to squash unused variable compiler warnings. */
#define CYAML_UNUSED(_x) ((void)(_x))

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
		char *fmt, ...)
{
	if (level >= cfg->log_level && cfg->log_fn != NULL) {
		va_list args;
		va_start(args, fmt);
		cfg->log_fn(level, fmt, args);
		va_end(args);
	}
}

#endif
