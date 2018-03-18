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
#define YAML_LEN(_y) (sizeof(_y) - 1)

typedef struct test_data {
	cyaml_data_t **data;
	const struct cyaml_config *config;
	const struct cyaml_schema_type *schema;
} test_data_t;

/* Common cleanup function to free data loaded by tests. */
static void cyaml_cleanup(void *data)
{
	struct test_data *td = data;

	cyaml_free(td->config, td->schema, *(td->data));
}

/* Test loading with NULL data parameter. */
static bool test_err_load_null_data(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	static const unsigned char yaml[] = "";
	struct target_struct {
		int value;
	} *data_tgt = NULL;
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_END
	};
	static const struct cyaml_schema_type top_schema = {
		CYAML_TYPE_MAPPING(CYAML_FLAG_POINTER,
				struct target_struct, mapping_schema),
	};
	test_data_t td = {
		.data = (cyaml_data_t **) &data_tgt,
		.config = config,
		.schema = &top_schema,
	};
	cyaml_err_t err;

	ttest_ctx_t tc = ttest_start(report, __func__, cyaml_cleanup, &td);

	err = cyaml_load_data(yaml, YAML_LEN(yaml), config, &top_schema,
			(cyaml_data_t **) NULL);
	if (err != CYAML_ERR_BAD_PARAM_NULL_DATA) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (data_tgt != NULL) {
		return ttest_fail(&tc, "Data non-NULL on error.");
	}

	return ttest_pass(&tc);
}

/**
 * Run the CYAML error unit tests.
 *
 * \param[in]  rc         The ttest report context.
 * \param[in]  log_level  CYAML log level.
 * \param[in]  log_fn     CYAML logging function, or NULL.
 * \return true iff all unit tests pass, otherwise false.
 */
bool errs_tests(
		ttest_report_ctx_t *rc,
		cyaml_log_t log_level,
		cyaml_log_fn_t log_fn)
{
	bool pass = true;
	cyaml_config_t config = {
		.log_fn = log_fn,
		.log_level = log_level,
		.flags = CYAML_CFG_DEFAULT,
	};

	/* Since we expect loads of error logging for these tests,
	 * suppress log output if required log level is greater
	 * than \ref CYAML_LOG_INFO.
	 */
	if (log_level > CYAML_LOG_INFO) {
		config.log_fn = NULL;
	}

	ttest_heading(rc, "Bad parameter tests");

	pass &= test_err_load_null_data(rc, &config);

	return pass;
}
