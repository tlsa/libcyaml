/*
 * SPDX-License-Identifier: ISC
 *
 * Copyright (C) 2018 Michael Drake <tlsa@netsurf-browser.org>
 */

/**
 * \file
 * \brief CYAML memory allocation handling.
 */

#include <stdbool.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "mem.h"

/* Exported function, documented in include/cyaml/cyaml.h */
void * cyaml_mem(
		void *ptr,
		size_t size)
{
	if (size == 0) {
		free(ptr);
		return NULL;
	}

	return realloc(ptr, size);
}
