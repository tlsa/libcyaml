/*
 * SPDX-License-Identifier: ISC
 *
 * Copyright (C) 2017 Michael Drake <tlsa@netsurf-browser.org>
 */

/**
 * \file
 * \brief Utility functions.
 */

#include <inttypes.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

#include "util.h"

/* Exported function, documented in include/cyaml/cyaml.h */
void cyaml_log(
		cyaml_log_t level,
		const char *fmt,
		va_list args)
{
	static const char * const strings[] = {
		[CYAML_LOG_DEBUG]   = "DEBUG",
		[CYAML_LOG_INFO]    = "INFO",
		[CYAML_LOG_NOTICE]  = "NOTICE",
		[CYAML_LOG_WARNING] = "WARNING",
		[CYAML_LOG_ERROR]   = "ERROR",
	};
	fprintf(stderr, "libcyaml: %s: ", strings[level]);
	vfprintf(stderr, fmt, args);
}
