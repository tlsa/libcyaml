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

#include "cyaml.h"

/** Macro to squash unused variable compiler warnings. */
#define CYAML_UNUSED(_x) ((void)(_x))

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
