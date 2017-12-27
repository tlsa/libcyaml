/*
 * SPDX-License-Identifier: ISC
 *
 * Copyright (C) 2017 Michael Drake <tlsa@netsurf-browser.org>
 */

#include <stdbool.h>
#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <cyaml.h>

#include "ttest.h"

/** In load.c */
extern bool load_tests(
		ttest_report_ctx_t *rc,
		cyaml_log_t log_level,
		cyaml_log_fn_t log_fn);

/** In free.c */
extern bool free_tests(
		ttest_report_ctx_t *rc,
		cyaml_log_t log_level,
		cyaml_log_fn_t log_fn);

/** Print program usage */
void usage(const char *prog_name)
{
	fprintf(stderr, "Usage: %s [-q|-v|-d]\n", prog_name);
}

/** Main entry point */
int main(int argc, char *argv[])
{
	bool pass = true;
	bool quiet = false;
	ttest_report_ctx_t rc;
	cyaml_log_fn_t log_fn = cyaml_log;
	cyaml_log_t log_level = CYAML_LOG_ERROR;
	enum {
		ARG_PROG_NAME,
		ARG_VERBOSE,
		ARG__COUNT,
	};

	if (argc > ARG__COUNT) {
		usage(argv[ARG_PROG_NAME]);
		return EXIT_FAILURE;

	} else if (argc > ARG_VERBOSE) {
		if (strcmp(argv[ARG_VERBOSE], "-q") == 0) {
			quiet = true;
			log_fn = NULL;
		} else if (strcmp(argv[ARG_VERBOSE], "-v") == 0) {
			log_level = CYAML_LOG_INFO;
		} else if (strcmp(argv[ARG_VERBOSE], "-d") == 0) {
			log_level = CYAML_LOG_DEBUG;
		} else {
			usage(argv[ARG_PROG_NAME]);
			return EXIT_FAILURE;
		}
	}

	rc = ttest_init(quiet);

	pass &= free_tests(&rc, log_level, log_fn);
	pass &= load_tests(&rc, log_level, log_fn);

	ttest_report(&rc);

	return (pass) ? EXIT_SUCCESS : EXIT_FAILURE;
}
