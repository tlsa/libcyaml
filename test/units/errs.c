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

/* Test loading with NULL config parameter. */
static bool test_err_load_null_config(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	static const unsigned char yaml[] = "";
	void *data_tgt = NULL;
	test_data_t td = {
		.data = (cyaml_data_t **) &data_tgt,
		.config = config,
		.schema = NULL,
	};
	cyaml_err_t err;

	ttest_ctx_t tc = ttest_start(report, __func__, cyaml_cleanup, &td);

	err = cyaml_load_data(yaml, YAML_LEN(yaml), NULL, NULL,
			(cyaml_data_t **) NULL);
	if (err != CYAML_ERR_BAD_PARAM_NULL_CONFIG) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (data_tgt != NULL) {
		return ttest_fail(&tc, "Data non-NULL on error.");
	}

	return ttest_pass(&tc);
}

/* Test loading with NULL schema. */
static bool test_err_load_null_schema(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	static const unsigned char yaml[] = "";
	void *data_tgt = NULL;
	test_data_t td = {
		.data = (cyaml_data_t **) &data_tgt,
		.config = config,
		.schema = NULL,
	};
	cyaml_err_t err;

	ttest_ctx_t tc = ttest_start(report, __func__, cyaml_cleanup, &td);

	err = cyaml_load_data(yaml, YAML_LEN(yaml), config, NULL,
			(cyaml_data_t **) NULL);
	if (err != CYAML_ERR_BAD_PARAM_NULL_SCHEMA) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (data_tgt != NULL) {
		return ttest_fail(&tc, "Data non-NULL on error.");
	}

	return ttest_pass(&tc);
}

/* Test loading with schema with bad type. */
static bool test_err_schema_bad_type(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	static const unsigned char yaml[] =
		"key:\n";
	struct target_struct {
		int value;
	} *data_tgt = NULL;
	static const struct cyaml_schema_mapping mapping_schema[] = {
		{
			.key = "key",
			.value = {
				.type = 99999,
				.flags = CYAML_FLAG_DEFAULT,
				.data_size = sizeof(data_tgt->value),
			},
			.data_offset = offsetof(struct target_struct, value),
		},
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
			(cyaml_data_t **) &data_tgt);
	if (err != CYAML_ERR_BAD_TYPE_IN_SCHEMA) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (data_tgt != NULL) {
		return ttest_fail(&tc, "Data non-NULL on error.");
	}

	return ttest_pass(&tc);
}

/* Test loading with schema with data size (0). */
static bool test_err_schema_bad_data_size_1(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	static const unsigned char yaml[] =
		"key: 1\n";
	struct target_struct {
		int value;
	} *data_tgt = NULL;
	static const struct cyaml_schema_mapping mapping_schema[] = {
		{
			.key = "key",
			.value = {
				.type = CYAML_INT,
				.flags = CYAML_FLAG_DEFAULT,
				.data_size = 0,
			},
			.data_offset = offsetof(struct target_struct, value),
		},
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
			(cyaml_data_t **) &data_tgt);
	if (err != CYAML_ERR_INVALID_DATA_SIZE) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (data_tgt != NULL) {
		return ttest_fail(&tc, "Data non-NULL on error.");
	}

	return ttest_pass(&tc);
}

/* Test loading with schema with data size (9). */
static bool test_err_schema_bad_data_size_2(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	static const unsigned char yaml[] =
		"key: 1\n";
	struct target_struct {
		int value;
	} *data_tgt = NULL;
	static const struct cyaml_schema_mapping mapping_schema[] = {
		{
			.key = "key",
			.value = {
				.type = CYAML_INT,
				.flags = CYAML_FLAG_DEFAULT,
				.data_size = 9,
			},
			.data_offset = offsetof(struct target_struct, value),
		},
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
			(cyaml_data_t **) &data_tgt);
	if (err != CYAML_ERR_INVALID_DATA_SIZE) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (data_tgt != NULL) {
		return ttest_fail(&tc, "Data non-NULL on error.");
	}

	return ttest_pass(&tc);
}

/* Test loading when schema expects int, but YAML has sequence. */
static bool test_err_schema_expect_int_read_seq(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	static const unsigned char yaml[] =
		"key:\n"
		"  - 90";
	struct target_struct {
		int value;
	} *data_tgt = NULL;
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_INT("key", CYAML_FLAG_DEFAULT,
				struct target_struct, value),
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
			(cyaml_data_t **) &data_tgt);
	if (err != CYAML_ERR_INVALID_VALUE) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (data_tgt != NULL) {
		return ttest_fail(&tc, "Data non-NULL on error.");
	}

	return ttest_pass(&tc);
}

/* Test loading when schema expects int, but YAML ends. */
static bool test_err_schema_expect_int_read_end_1(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	static const unsigned char yaml[] =
		"key:\n";
	struct target_struct {
		int value;
	} *data_tgt = NULL;
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_INT("key", CYAML_FLAG_DEFAULT,
				struct target_struct, value),
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
			(cyaml_data_t **) &data_tgt);
	if (err != CYAML_ERR_INVALID_VALUE) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (data_tgt != NULL) {
		return ttest_fail(&tc, "Data non-NULL on error.");
	}

	return ttest_pass(&tc);
}

/* Test loading when schema expects int, but YAML ends. */
static bool test_err_schema_expect_int_read_end_2(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	static const unsigned char yaml[] =
		"key: |"
		"...";
	struct target_struct {
		int value;
	} *data_tgt = NULL;
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_INT("key", CYAML_FLAG_DEFAULT,
				struct target_struct, value),
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
			(cyaml_data_t **) &data_tgt);
	if (err != CYAML_ERR_LIBYAML_PARSER) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (data_tgt != NULL) {
		return ttest_fail(&tc, "Data non-NULL on error.");
	}

	return ttest_pass(&tc);
}

/* Test loading when schema expects flags, but YAML has scalar. */
static bool test_err_schema_expect_flags_read_scalar(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	static const char * const strings[] = {
		"first",
		"second",
		"third",
		"fourth",
		"fifth",
		"sixth",
	};
	static const unsigned char yaml[] =
		"key: first\n";
	struct target_struct {
		int value;
	} *data_tgt = NULL;
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_FLAGS("key", CYAML_FLAG_DEFAULT,
				struct target_struct, value,
				strings, CYAML_ARRAY_LEN(strings)),
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
			(cyaml_data_t **) &data_tgt);
	if (err != CYAML_ERR_INVALID_VALUE) {
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
	pass &= test_err_load_null_config(rc, &config);
	pass &= test_err_load_null_schema(rc, &config);

	ttest_heading(rc, "Bad schema tests");

	pass &= test_err_schema_bad_type(rc, &config);
	pass &= test_err_schema_bad_data_size_1(rc, &config);
	pass &= test_err_schema_bad_data_size_2(rc, &config);

	ttest_heading(rc, "YAML / schema mismatch tests");

	pass &= test_err_schema_expect_int_read_seq(rc, &config);
	pass &= test_err_schema_expect_int_read_end_1(rc, &config);
	pass &= test_err_schema_expect_int_read_end_2(rc, &config);
	pass &= test_err_schema_expect_flags_read_scalar(rc, &config);

	return pass;
}
