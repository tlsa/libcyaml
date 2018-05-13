/*
 * SPDX-License-Identifier: ISC
 *
 * Copyright (C) 2018 Michael Drake <tlsa@netsurf-browser.org>
 */

#include <stdbool.h>
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include <cyaml.h>

#include "../../src/data.h"
#include "ttest.h"

/** Macro to squash unused variable compiler warnings. */
#define UNUSED(_x) ((void)(_x))

/** Helper macro to count bytes of YAML input data. */
#define YAML_LEN(_y) (sizeof(_y) - 1)

/**
 * Unit test context data.
 */
typedef struct test_data {
	char **buffer;
	const struct cyaml_config *config;
} test_data_t;

/**
 * Common clean up function to free data alloacted by tests.
 *
 * \param[in]  data  The unit test context data.
 */
static void cyaml_cleanup(void *data)
{
	struct test_data *td = data;

	td->config->mem_fn(*(td->buffer), 0);
}

/**
 * Test saving a positive signed integer.
 *
 * \param[in]  report  The test report context.
 * \param[in]  config  The CYAML config to use for the test.
 * \return true if test passes, false otherwise.
 */
static bool test_save_mapping_entry_int_pos(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	static const unsigned char ref[] =
		"---\n"
		"test_int: 90\n"
		"...\n";
	static const struct target_struct {
		int test_int;
	} data = {
		.test_int = 90,
	};
	static const struct cyaml_schema_field mapping_schema[] = {
		CYAML_FIELD_INT("test_int", CYAML_FLAG_DEFAULT,
				struct target_struct, test_int),
		CYAML_FIELD_END
	};
	static const struct cyaml_schema_value top_schema = {
		CYAML_VALUE_MAPPING(CYAML_FLAG_POINTER,
				struct target_struct, mapping_schema),
	};
	char *buffer;
	size_t len;
	test_data_t td = {
		.buffer = &buffer,
		.config = config,
	};
	cyaml_err_t err;

	ttest_ctx_t tc = ttest_start(report, __func__, cyaml_cleanup, &td);

	err = cyaml_save_data(&buffer, &len, config, &top_schema,
				&data, 0);
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (len != YAML_LEN(ref)) {
		return ttest_fail(&tc, "Bad length: Expected %zu, got: %zu",
				YAML_LEN(ref), len);
	}

	if (memcmp(ref, buffer, len) != 0) {
		return ttest_fail(&tc, "Bad data: "
				"EXPECTED:\n\n%*s\n\nGOT:\n\n%*s\n",
				len, ref, len, buffer);
	}

	return ttest_pass(&tc);
}

/**
 * Run the YAML saving unit tests.
 *
 * \param[in]  rc         The ttest report context.
 * \param[in]  log_level  CYAML log level.
 * \param[in]  log_fn     CYAML logging function, or NULL.
 * \return true iff all unit tests pass, otherwise false.
 */
bool save_tests(
		ttest_report_ctx_t *rc,
		cyaml_log_t log_level,
		cyaml_log_fn_t log_fn)
{
	bool pass = true;
	cyaml_config_t config = {
		.log_fn = log_fn,
		.mem_fn = cyaml_mem,
		.log_level = log_level,
		.flags = CYAML_CFG_DEFAULT,
	};

	ttest_heading(rc, "Save single entry mapping tests: simple types");

	pass &= test_save_mapping_entry_int_pos(rc, &config);

	return pass;
}
