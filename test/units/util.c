/*
 * SPDX-License-Identifier: ISC
 *
 * Copyright (C) 2018 Michael Drake <tlsa@netsurf-browser.org>
 */

#include <stdbool.h>
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include <cyaml.h>

#include "ttest.h"

#define UNUSED(_x) ((void)(_x))

/* Test CYAML_OK error code. */
static bool test_util_err_success_zero(
		ttest_report_ctx_t *report)
{
	ttest_ctx_t tc = ttest_start(report, __func__, NULL, NULL);

	if (CYAML_OK != 0) {
		return ttest_fail(&tc, "CYAML_OK value not zero");
	}

	return ttest_pass(&tc);
}

/* Test valid cyaml_strerror strings. */
static bool test_util_err_strings_valid(
		ttest_report_ctx_t *report)
{
	ttest_ctx_t tc = ttest_start(report, __func__, NULL, NULL);

	if (cyaml_strerror(CYAML_OK) == NULL) {
		return ttest_fail(&tc, "CYAML_OK string is NULL");
	}

	if (strcmp(cyaml_strerror(CYAML_OK), "Success") != 0) {
		return ttest_fail(&tc, "CYAML_OK string not 'Success'");
	}

	for (unsigned i = 1; i <= CYAML_ERR__COUNT; i++) {
		if (cyaml_strerror(i) == NULL) {
			return ttest_fail(&tc, "Error code %u string is NULL",
					i);
		}

		for (unsigned j = 0; j < i; j++) {
			if (strcmp(cyaml_strerror(j), cyaml_strerror(i)) == 0) {
				return ttest_fail(&tc, "Two error codes "
						"have same string (%u and %u)",
						i, j);
			}
		}
	}

	return ttest_pass(&tc);
}

/**
 * Run the CYAML util unit tests.
 *
 * \param[in]  rc         The ttest report context.
 * \param[in]  log_level  CYAML log level.
 * \param[in]  log_fn     CYAML logging function, or NULL.
 * \return true iff all unit tests pass, otherwise false.
 */
bool util_tests(
		ttest_report_ctx_t *rc,
		cyaml_log_t log_level,
		cyaml_log_fn_t log_fn)
{
	bool pass = true;

	UNUSED(log_level);
	UNUSED(log_fn);

	ttest_heading(rc, "Error code tests");

	pass &= test_util_err_success_zero(rc);
	pass &= test_util_err_strings_valid(rc);

	return pass;
}
