/*
 * SPDX-License-Identifier: ISC
 *
 * Copyright (C) 2017-2018 Michael Drake <tlsa@netsurf-browser.org>
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

#include "ttest.h"

#define UNUSED(_x) ((void)(_x))
#define YAML_LEN(_y) (sizeof(_y) - 1)

typedef struct test_data {
	cyaml_data_t **data;
	const struct cyaml_config *config;
	const struct cyaml_schema_type *schema;
} test_data_t;

void cyaml_cleanup(void *data)
{
	struct test_data *td = data;

	cyaml_free(td->config, td->schema, *(td->data));
}

static bool test_load_mapping_entry_int_pos(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	int value = 90;
	static const unsigned char yaml[] =
		"test_int: 90\n";
	struct target_struct {
		int test_value_int;
	} *data_tgt = NULL;
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_INT("test_int", CYAML_FLAG_DEFAULT,
				struct target_struct, test_value_int),
		CYAML_MAPPING_END
	};
	static const struct cyaml_schema_type top_schema = {
		CYAML_TYPE_MAPPING(CYAML_FLAG_DEFAULT,
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (data_tgt->test_value_int != value) {
		return ttest_fail(&tc, "Incorrect value");
	}

	return ttest_pass(&tc);
}

static bool test_load_mapping_entry_int_neg(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	int value = -77;
	static const unsigned char yaml[] =
		"test_int: -77\n";
	struct target_struct {
		int test_value_int;
	} *data_tgt = NULL;
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_INT("test_int", CYAML_FLAG_DEFAULT,
				struct target_struct, test_value_int),
		CYAML_MAPPING_END
	};
	static const struct cyaml_schema_type top_schema = {
		CYAML_TYPE_MAPPING(CYAML_FLAG_DEFAULT,
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (data_tgt->test_value_int != value) {
		return ttest_fail(&tc, "Incorrect value");
	}

	return ttest_pass(&tc);
}

static bool test_load_mapping_entry_uint(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	unsigned value = 9999;
	static const unsigned char yaml[] =
		"test_uint: 9999\n";
	struct target_struct {
		unsigned test_value_uint;
	} *data_tgt = NULL;
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_UINT("test_uint", CYAML_FLAG_DEFAULT,
				struct target_struct, test_value_uint),
		CYAML_MAPPING_END
	};
	static const struct cyaml_schema_type top_schema = {
		CYAML_TYPE_MAPPING(CYAML_FLAG_DEFAULT,
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (data_tgt->test_value_uint != value) {
		return ttest_fail(&tc, "Incorrect value");
	}

	return ttest_pass(&tc);
}

static bool test_load_mapping_entry_bool_true(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	bool value = true;
	static const unsigned char yaml[] =
		"test_bool: true\n";
	struct target_struct {
		unsigned test_value_bool;
	} *data_tgt = NULL;
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_BOOL("test_bool", CYAML_FLAG_DEFAULT,
				struct target_struct, test_value_bool),
		CYAML_MAPPING_END
	};
	static const struct cyaml_schema_type top_schema = {
		CYAML_TYPE_MAPPING(CYAML_FLAG_DEFAULT,
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (data_tgt->test_value_bool != value) {
		return ttest_fail(&tc, "Incorrect value");
	}

	return ttest_pass(&tc);
}

static bool test_load_mapping_entry_bool_false(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	bool value = false;
	static const unsigned char yaml[] =
		"test_bool: false\n";
	struct target_struct {
		unsigned test_value_bool;
	} *data_tgt = NULL;
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_BOOL("test_bool", CYAML_FLAG_DEFAULT,
				struct target_struct, test_value_bool),
		CYAML_MAPPING_END
	};
	static const struct cyaml_schema_type top_schema = {
		CYAML_TYPE_MAPPING(CYAML_FLAG_DEFAULT,
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (data_tgt->test_value_bool != value) {
		return ttest_fail(&tc, "Incorrect value");
	}

	return ttest_pass(&tc);
}

static bool test_load_mapping_entry_enum(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	enum test_enum {
		TEST_ENUM_FIRST,
		TEST_ENUM_SECOND,
		TEST_ENUM_THIRD,
		TEST_ENUM__COUNT,
	} value = TEST_ENUM_SECOND;
	static const char * const strings[TEST_ENUM__COUNT] = {
		[TEST_ENUM_FIRST]  = "first",
		[TEST_ENUM_SECOND] = "second",
		[TEST_ENUM_THIRD]  = "third",
	};
	static const unsigned char yaml[] =
		"test_enum: second\n";
	struct target_struct {
		enum test_enum test_value_enum;
	} *data_tgt = NULL;
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_ENUM("test_enum", CYAML_FLAG_DEFAULT,
				struct target_struct, test_value_enum,
				strings, TEST_ENUM__COUNT),
		CYAML_MAPPING_END
	};
	static const struct cyaml_schema_type top_schema = {
		CYAML_TYPE_MAPPING(CYAML_FLAG_DEFAULT,
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (data_tgt->test_value_enum != value) {
		return ttest_fail(&tc, "Incorrect value");
	}

	return ttest_pass(&tc);
}

static bool test_load_mapping_entry_string(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	const char *value = "Hello World!";
	static const unsigned char yaml[] =
		"test_string: Hello World!\n";
	struct target_struct {
		char test_value_string[50];
	} *data_tgt = NULL;
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_STRING("test_string", CYAML_FLAG_DEFAULT,
				struct target_struct, test_value_string, 0),
		CYAML_MAPPING_END
	};
	static const struct cyaml_schema_type top_schema = {
		CYAML_TYPE_MAPPING(CYAML_FLAG_DEFAULT,
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (strcmp(data_tgt->test_value_string, value) != 0) {
		fprintf(stderr, "expected: %s\n", value);
		for (unsigned i = 0; i < strlen(value) + 1; i++) {
			fprintf(stderr, "%2.2x ", value[i]);
		}
		fprintf(stderr, "\n");
		fprintf(stderr, "     got: %s\n", data_tgt->test_value_string);
		for (unsigned i = 0; i < sizeof(data_tgt->test_value_string); i++) {
			fprintf(stderr, "%2.2x ", value[i]);
		}
		fprintf(stderr, "\n");
		return ttest_fail(&tc, "Incorrect value");
	}

	return ttest_pass(&tc);
}

static bool test_load_mapping_entry_string_ptr(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	const char *value = "Hello World!";
	static const unsigned char yaml[] =
		"test_string: Hello World!\n";
	struct target_struct {
		char * test_value_string;
	} *data_tgt = NULL;
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_STRING_PTR("test_string", CYAML_FLAG_POINTER,
				struct target_struct, test_value_string,
				0, CYAML_UNLIMITED),
		CYAML_MAPPING_END
	};
	static const struct cyaml_schema_type top_schema = {
		CYAML_TYPE_MAPPING(CYAML_FLAG_DEFAULT,
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (strcmp(data_tgt->test_value_string, value) != 0) {
		return ttest_fail(&tc, "Incorrect value");
	}

	return ttest_pass(&tc);
}

static bool test_load_mapping_entry_ignore_deep(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	static const unsigned char yaml[] =
		"ignore:\n"
		"    foo: bar\n"
		"    bar:\n"
		"      - 1\n"
		"      - 2\n";
	struct target_struct {
		bool foo;
	} *data_tgt = NULL;
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_IGNORE("ignore", CYAML_FLAG_DEFAULT),
		CYAML_MAPPING_END
	};
	static const struct cyaml_schema_type top_schema = {
		CYAML_TYPE_MAPPING(CYAML_FLAG_DEFAULT,
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (data_tgt->foo != false) {
		return ttest_fail(&tc, "Incorrect value");
	}

	return ttest_pass(&tc);
}

static bool test_load_mapping_entry_ignore_scalar(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	static const unsigned char yaml[] =
		"ignore: foo\n";
	struct target_struct {
		bool foo;
	} *data_tgt = NULL;
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_IGNORE("ignore", CYAML_FLAG_DEFAULT),
		CYAML_MAPPING_END
	};
	static const struct cyaml_schema_type top_schema = {
		CYAML_TYPE_MAPPING(CYAML_FLAG_DEFAULT,
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (data_tgt->foo != false) {
		return ttest_fail(&tc, "Incorrect value");
	}

	return ttest_pass(&tc);
}

bool load_tests(
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

	ttest_heading(rc, "Load single entry mapping tests: simple types");

	pass &= test_load_mapping_entry_enum(rc, &config);
	pass &= test_load_mapping_entry_uint(rc, &config);
	pass &= test_load_mapping_entry_string(rc, &config);
	pass &= test_load_mapping_entry_int_pos(rc, &config);
	pass &= test_load_mapping_entry_int_neg(rc, &config);
	pass &= test_load_mapping_entry_bool_true(rc, &config);
	pass &= test_load_mapping_entry_bool_false(rc, &config);
	pass &= test_load_mapping_entry_string_ptr(rc, &config);
	pass &= test_load_mapping_entry_ignore_deep(rc, &config);
	pass &= test_load_mapping_entry_ignore_scalar(rc, &config);

	return pass;
}
