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

/* Common cleanup function to free data loaded by tests. */
static void cyaml_cleanup(void *data)
{
	struct test_data *td = data;

	cyaml_free(td->config, td->schema, *(td->data));
}

/* Test loading a positive signed integer. */
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (data_tgt->test_value_int != value) {
		return ttest_fail(&tc, "Incorrect value");
	}

	return ttest_pass(&tc);
}

/* Test loading a negative signed integer. */
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (data_tgt->test_value_int != value) {
		return ttest_fail(&tc, "Incorrect value");
	}

	return ttest_pass(&tc);
}

/* Test loading an unsigned integer. */
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (data_tgt->test_value_uint != value) {
		return ttest_fail(&tc, "Incorrect value");
	}

	return ttest_pass(&tc);
}

/* Test loading a floating point value as a float. */
static bool test_load_mapping_entry_float(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	float value = 3.14159;
	static const unsigned char yaml[] =
		"test_fp: 3.14159\n";
	struct target_struct {
		float test_value_fp;
	} *data_tgt = NULL;
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_FLOAT("test_fp", CYAML_FLAG_DEFAULT,
				struct target_struct, test_value_fp),
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (data_tgt->test_value_fp != value) {
		return ttest_fail(&tc, "Incorrect value: "
				"expected: %f, got: %f",
				value, data_tgt->test_value_fp);
	}

	return ttest_pass(&tc);
}

/* Test loading a floating point value as a double. */
static bool test_load_mapping_entry_double(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	double value = 3.14159;
	static const unsigned char yaml[] =
		"test_fp: 3.14159\n";
	struct target_struct {
		double test_value_fp;
	} *data_tgt = NULL;
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_FLOAT("test_fp", CYAML_FLAG_DEFAULT,
				struct target_struct, test_value_fp),
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (data_tgt->test_value_fp != value) {
		return ttest_fail(&tc, "Incorrect value: "
				"expected: %lf, got: %lf",
				value, data_tgt->test_value_fp);
	}

	return ttest_pass(&tc);
}

/* Test loading a boolean value (true). */
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (data_tgt->test_value_bool != value) {
		return ttest_fail(&tc, "Incorrect value");
	}

	return ttest_pass(&tc);
}

/* Test loading a boolean value (false). */
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (data_tgt->test_value_bool != value) {
		return ttest_fail(&tc, "Incorrect value");
	}

	return ttest_pass(&tc);
}

/* Test loading an enumeration. */
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (data_tgt->test_value_enum != value) {
		return ttest_fail(&tc, "Incorrect value");
	}

	return ttest_pass(&tc);
}

/* Test loading a string to a character array. */
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

/* Test loading a string to a allocated char pointer. */
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (strcmp(data_tgt->test_value_string, value) != 0) {
		return ttest_fail(&tc, "Incorrect value");
	}

	return ttest_pass(&tc);
}

/* Test loading an ignored value with descendants. */
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (data_tgt->foo != false) {
		return ttest_fail(&tc, "Incorrect value");
	}

	return ttest_pass(&tc);
}

/* Test loading an ignored value. */
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (data_tgt->foo != false) {
		return ttest_fail(&tc, "Incorrect value");
	}

	return ttest_pass(&tc);
}

/* Test loading a flag word. */
static bool test_load_mapping_entry_flags(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	enum test_flags {
		TEST_FLAGS_NONE   = 0,
		TEST_FLAGS_FIRST  = (1 << 0),
		TEST_FLAGS_SECOND = (1 << 1),
		TEST_FLAGS_THIRD  = (1 << 2),
		TEST_FLAGS_FOURTH = (1 << 3),
		TEST_FLAGS_FIFTH  = (1 << 4),
		TEST_FLAGS_SIXTH  = (1 << 5),
	} value = TEST_FLAGS_SECOND | TEST_FLAGS_FIFTH | 1024;
	#define TEST_FLAGS__COUNT 6
	static const char * const strings[TEST_FLAGS__COUNT] = {
		"first",
		"second",
		"third",
		"fourth",
		"fifth",
		"sixth",
	};
	static const unsigned char yaml[] =
		"test_flags:\n"
		"    - second\n"
		"    - fifth\n"
		"    - 1024\n";
	struct target_struct {
		enum test_flags test_value_flags;
	} *data_tgt = NULL;
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_FLAGS("test_flags", CYAML_FLAG_DEFAULT,
				struct target_struct, test_value_flags,
				strings, TEST_FLAGS__COUNT),
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (data_tgt->test_value_flags != value) {
		return ttest_fail(&tc, "Incorrect value: "
				"expected: 0x%x, got: 0x%x\n",
				value, data_tgt->test_value_flags);
	}

	return ttest_pass(&tc);
}

/* Test loading a mapping, to an internal structure. */
static bool test_load_mapping_entry_mapping(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	struct value_s {
		short a;
		long b;
	} value;
	static const unsigned char yaml[] =
		"mapping:\n"
		"    a: 123\n"
		"    b: 9999\n";
	struct target_struct {
		struct value_s test_value_mapping;
	} *data_tgt = NULL;
	static const struct cyaml_schema_mapping test_mapping_schema[] = {
		CYAML_MAPPING_INT("a", CYAML_FLAG_DEFAULT, struct value_s, a),
		CYAML_MAPPING_INT("b", CYAML_FLAG_DEFAULT, struct value_s, b),
		CYAML_MAPPING_END
	};
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_MAPPING("mapping", CYAML_FLAG_DEFAULT,
				struct target_struct, test_value_mapping,
				test_mapping_schema),
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

	memset(&value, 0, sizeof(value));
	value.a = 123;
	value.b = 9999;

	ttest_ctx_t tc = ttest_start(report, __func__, cyaml_cleanup, &td);

	err = cyaml_load_data(yaml, YAML_LEN(yaml), config, &top_schema,
			(cyaml_data_t **) &data_tgt);
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (memcmp(&data_tgt->test_value_mapping, &value, sizeof(value)) != 0) {
		return ttest_fail(&tc, "Incorrect value");
	}

	return ttest_pass(&tc);
}

/* Test loading a mapping, to an allocated structure. */
static bool test_load_mapping_entry_mapping_ptr(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	struct value_s {
		short a;
		long b;
	} value;
	static const unsigned char yaml[] =
		"mapping:\n"
		"    a: 123\n"
		"    b: 9999\n";
	struct target_struct {
		struct value_s *test_value_mapping;
	} *data_tgt = NULL;
	static const struct cyaml_schema_mapping test_mapping_schema[] = {
		CYAML_MAPPING_INT("a", CYAML_FLAG_DEFAULT, struct value_s, a),
		CYAML_MAPPING_INT("b", CYAML_FLAG_DEFAULT, struct value_s, b),
		CYAML_MAPPING_END
	};
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_MAPPING_PTR("mapping", CYAML_FLAG_POINTER,
				struct target_struct, test_value_mapping,
				test_mapping_schema),
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

	memset(&value, 0, sizeof(value));
	value.a = 123;
	value.b = 9999;

	ttest_ctx_t tc = ttest_start(report, __func__, cyaml_cleanup, &td);

	err = cyaml_load_data(yaml, YAML_LEN(yaml), config, &top_schema,
			(cyaml_data_t **) &data_tgt);
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (memcmp(data_tgt->test_value_mapping, &value, sizeof(value)) != 0) {
		return ttest_fail(&tc, "Incorrect value");
	}

	return ttest_pass(&tc);
}

/* Test loading a sequence of integers into an int[]. */
static bool test_load_mapping_entry_sequence_int(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	int ref[] = { 1, 1, 2, 3, 5, 8 };
	static const unsigned char yaml[] =
		"sequence:\n"
		"    - 1\n"
		"    - 1\n"
		"    - 2\n"
		"    - 3\n"
		"    - 5\n"
		"    - 8\n";
	struct target_struct {
		int seq[CYAML_ARRAY_LEN(ref)];
		uint32_t seq_count;
	} *data_tgt = NULL;
	static const struct cyaml_schema_type entry_schema = {
		CYAML_TYPE_INT(CYAML_FLAG_DEFAULT, *(data_tgt->seq)),
	};
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_SEQUENCE("sequence", CYAML_FLAG_DEFAULT,
				struct target_struct, seq, &entry_schema,
				0, CYAML_ARRAY_LEN(ref)),
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (CYAML_ARRAY_LEN(ref) != data_tgt->seq_count) {
		return ttest_fail(&tc, "Incorrect sequence count");
	}

	for (unsigned i = 0; i < CYAML_ARRAY_LEN(ref); i++) {
		if (data_tgt->seq[i] != ref[i]) {
			return ttest_fail(&tc, "Incorrect value (i=%u): "
					"got: %i, expected: %i", i,
					data_tgt->seq[i], ref[i]);
		}
	}

	return ttest_pass(&tc);
}

/* Test loading a sequence of enums into an enum test_enum[]. */
static bool test_load_mapping_entry_sequence_enum(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	enum test_enum {
		TEST_ENUM_FIRST,
		TEST_ENUM_SECOND,
		TEST_ENUM_THIRD,
		TEST_ENUM__COUNT,
	} ref[] = { TEST_ENUM_FIRST, TEST_ENUM_SECOND, TEST_ENUM_THIRD };
	static const char * const strings[TEST_ENUM__COUNT] = {
		[TEST_ENUM_FIRST]  = "first",
		[TEST_ENUM_SECOND] = "second",
		[TEST_ENUM_THIRD]  = "third",
	};
	static const unsigned char yaml[] =
		"sequence:\n"
		"    - first\n"
		"    - second\n"
		"    - third\n";
	struct target_struct {
		enum test_enum seq[CYAML_ARRAY_LEN(ref)];
		uint32_t seq_count;
	} *data_tgt = NULL;
	static const struct cyaml_schema_type entry_schema = {
		CYAML_TYPE_ENUM(CYAML_FLAG_DEFAULT,
				*(data_tgt->seq), strings, TEST_ENUM__COUNT),
	};
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_SEQUENCE("sequence", CYAML_FLAG_DEFAULT,
				struct target_struct, seq, &entry_schema,
				0, CYAML_ARRAY_LEN(ref)),
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (CYAML_ARRAY_LEN(ref) != data_tgt->seq_count) {
		return ttest_fail(&tc, "Incorrect sequence count");
	}

	for (unsigned i = 0; i < CYAML_ARRAY_LEN(ref); i++) {
		if (data_tgt->seq[i] != ref[i]) {
			return ttest_fail(&tc, "Incorrect value (i=%u): "
					"got: %i, expected: %i", i,
					data_tgt->seq[i], ref[i]);
		}
	}

	return ttest_pass(&tc);
}

/* Test loading a sequence of unsigned integers into an unsigned[]. */
static bool test_load_mapping_entry_sequence_uint(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	unsigned ref[] = { 99999, 99998, 99997, 99996, 99995, 99994 };
	static const unsigned char yaml[] =
		"sequence:\n"
		"    - 99999\n"
		"    - 99998\n"
		"    - 99997\n"
		"    - 99996\n"
		"    - 99995\n"
		"    - 99994\n";
	struct target_struct {
		unsigned seq[CYAML_ARRAY_LEN(ref)];
		uint32_t seq_count;
	} *data_tgt = NULL;
	static const struct cyaml_schema_type entry_schema = {
		CYAML_TYPE_UINT(CYAML_FLAG_DEFAULT, *(data_tgt->seq)),
	};
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_SEQUENCE("sequence", CYAML_FLAG_DEFAULT,
				struct target_struct, seq, &entry_schema,
				0, CYAML_ARRAY_LEN(ref)),
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (CYAML_ARRAY_LEN(ref) != data_tgt->seq_count) {
		return ttest_fail(&tc, "Incorrect sequence count");
	}

	for (unsigned i = 0; i < CYAML_ARRAY_LEN(ref); i++) {
		if (data_tgt->seq[i] != ref[i]) {
			return ttest_fail(&tc, "Incorrect value (i=%u): "
					"got: %i, expected: %i", i,
					data_tgt->seq[i], ref[i]);
		}
	}

	return ttest_pass(&tc);
}

/* Test loading a sequence of boolean values into an bool[]. */
static bool test_load_mapping_entry_sequence_bool(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	bool ref[] = { true, false, true, false, true, false, true, false };
	static const unsigned char yaml[] =
		"sequence:\n"
		"    - true\n"
		"    - false\n"
		"    - yes\n"
		"    - no\n"
		"    - enable\n"
		"    - disable\n"
		"    - 1\n"
		"    - 0\n";
	struct target_struct {
		bool seq[CYAML_ARRAY_LEN(ref)];
		uint32_t seq_count;
	} *data_tgt = NULL;
	static const struct cyaml_schema_type entry_schema = {
		CYAML_TYPE_BOOL(CYAML_FLAG_DEFAULT, *(data_tgt->seq)),
	};
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_SEQUENCE("sequence", CYAML_FLAG_DEFAULT,
				struct target_struct, seq, &entry_schema,
				0, CYAML_ARRAY_LEN(ref)),
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (CYAML_ARRAY_LEN(ref) != data_tgt->seq_count) {
		return ttest_fail(&tc, "Incorrect sequence count");
	}

	for (unsigned i = 0; i < CYAML_ARRAY_LEN(ref); i++) {
		if (data_tgt->seq[i] != ref[i]) {
			return ttest_fail(&tc, "Incorrect value (i=%u): "
					"got: %i, expected: %i", i,
					data_tgt->seq[i], ref[i]);
		}
	}

	return ttest_pass(&tc);
}

/* Test loading a sequence of flag sequences into an array of flag words. */
static bool test_load_mapping_entry_sequence_flags(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	enum test_flags {
		TEST_FLAGS_NONE   = 0,
		TEST_FLAGS_FIRST  = (1 << 0),
		TEST_FLAGS_SECOND = (1 << 1),
		TEST_FLAGS_THIRD  = (1 << 2),
		TEST_FLAGS_FOURTH = (1 << 3),
		TEST_FLAGS_FIFTH  = (1 << 4),
		TEST_FLAGS_SIXTH  = (1 << 5),
	} ref[3] = {
		TEST_FLAGS_SECOND | TEST_FLAGS_FIFTH | 1024,
		TEST_FLAGS_FIRST,
		TEST_FLAGS_FOURTH | TEST_FLAGS_SIXTH
	};
	#define TEST_FLAGS__COUNT 6
	static const char * const strings[TEST_FLAGS__COUNT] = {
		"first",
		"second",
		"third",
		"fourth",
		"fifth",
		"sixth",
	};
	static const unsigned char yaml[] =
		"sequence:\n"
		"    - - second\n"
		"      - fifth\n"
		"      - 1024\n"
		"    - - first\n"
		"    - - fourth\n"
		"      - sixth\n";
	struct target_struct {
		enum test_flags seq[3];
		uint32_t seq_count;
	} *data_tgt = NULL;
	static const struct cyaml_schema_type entry_schema = {
		CYAML_TYPE_FLAGS(CYAML_FLAG_DEFAULT, *(data_tgt->seq),
				strings, TEST_FLAGS__COUNT),
	};
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_SEQUENCE("sequence", CYAML_FLAG_DEFAULT,
				struct target_struct, seq, &entry_schema,
				0, CYAML_ARRAY_LEN(data_tgt->seq)),
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (CYAML_ARRAY_LEN(ref) != data_tgt->seq_count) {
		return ttest_fail(&tc, "Incorrect sequence count");
	}

	for (unsigned i = 0; i < CYAML_ARRAY_LEN(ref); i++) {
		if (data_tgt->seq[i] != ref[i]) {
			return ttest_fail(&tc, "Incorrect value (i=%u): "
					"got: %i, expected: %i", i,
					data_tgt->seq[i], ref[i]);
		}
	}

	return ttest_pass(&tc);
}

/* Test loading a sequence of strings into an array of char[7]. */
static bool test_load_mapping_entry_sequence_string(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	const char *ref[] = {
		"This",
		"is",
		"merely",
		"a",
		"test",
	};
	static const unsigned char yaml[] =
		"sequence:\n"
		"    - This\n"
		"    - is\n"
		"    - merely\n"
		"    - a\n"
		"    - test\n";
	struct target_struct {
		char seq[5][7];
		uint32_t seq_count;
	} *data_tgt = NULL;
	static const struct cyaml_schema_type entry_schema = {
		CYAML_TYPE_STRING(CYAML_FLAG_DEFAULT, *(data_tgt->seq), 0, 6),
	};
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_SEQUENCE("sequence", CYAML_FLAG_DEFAULT,
				struct target_struct, seq, &entry_schema,
				0, CYAML_ARRAY_LEN(ref)),
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (CYAML_ARRAY_LEN(ref) != data_tgt->seq_count) {
		return ttest_fail(&tc, "Incorrect sequence count");
	}

	for (unsigned i = 0; i < CYAML_ARRAY_LEN(ref); i++) {
		if (strcmp(data_tgt->seq[i], ref[i]) != 0) {
			return ttest_fail(&tc, "Incorrect value (i=%u)", i);
		}
	}

	return ttest_pass(&tc);
}

/* Test loading a sequence of strings into an array of allocated strings. */
static bool test_load_mapping_entry_sequence_string_ptr(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	const char *ref[] = {
		"This",
		"is",
		"merely",
		"a",
		"test",
	};
	static const unsigned char yaml[] =
		"sequence:\n"
		"    - This\n"
		"    - is\n"
		"    - merely\n"
		"    - a\n"
		"    - test\n";
	struct target_struct {
		char *seq[5];
		uint32_t seq_count;
	} *data_tgt = NULL;
	static const struct cyaml_schema_type entry_schema = {
		CYAML_TYPE_STRING(CYAML_FLAG_POINTER, *(data_tgt->seq),
				0, CYAML_UNLIMITED),
	};
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_SEQUENCE("sequence", CYAML_FLAG_DEFAULT,
				struct target_struct, seq, &entry_schema,
				0, CYAML_ARRAY_LEN(ref)),
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (CYAML_ARRAY_LEN(ref) != data_tgt->seq_count) {
		return ttest_fail(&tc, "Incorrect sequence count");
	}

	for (unsigned i = 0; i < CYAML_ARRAY_LEN(ref); i++) {
		if (strcmp(data_tgt->seq[i], ref[i]) != 0) {
			return ttest_fail(&tc, "Incorrect value (i=%u)", i);
		}
	}

	return ttest_pass(&tc);
}

/* Test loading a sequence of mappings into an array of structures. */
static bool test_load_mapping_entry_sequence_mapping(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	struct value_s {
		short a;
		long b;
	} ref[3];
	static const unsigned char yaml[] =
		"sequence:\n"
		"    - a: 123\n"
		"      b: 9999\n"
		"    - a: 4000\n"
		"      b: 62000\n"
		"    - a: 1\n"
		"      b: 765\n";
	struct target_struct {
		struct value_s seq[3];
		uint32_t seq_count;
	} *data_tgt = NULL;
	static const struct cyaml_schema_mapping test_mapping_schema[] = {
		CYAML_MAPPING_INT("a", CYAML_FLAG_DEFAULT, struct value_s, a),
		CYAML_MAPPING_INT("b", CYAML_FLAG_DEFAULT, struct value_s, b),
		CYAML_MAPPING_END
	};
	static const struct cyaml_schema_type entry_schema = {
		CYAML_TYPE_MAPPING(CYAML_FLAG_DEFAULT,
				struct value_s, test_mapping_schema),
	};
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_SEQUENCE("sequence", CYAML_FLAG_DEFAULT,
				struct target_struct, seq, &entry_schema,
				0, CYAML_ARRAY_LEN(data_tgt->seq)),
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

	memset(ref, 0, sizeof(ref));
	ref[0].a = 123;
	ref[0].b = 9999;
	ref[1].a = 4000;
	ref[1].b = 62000;
	ref[2].a = 1;
	ref[2].b = 765;

	ttest_ctx_t tc = ttest_start(report, __func__, cyaml_cleanup, &td);

	err = cyaml_load_data(yaml, YAML_LEN(yaml), config, &top_schema,
			(cyaml_data_t **) &data_tgt);
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (CYAML_ARRAY_LEN(ref) != data_tgt->seq_count) {
		return ttest_fail(&tc, "Incorrect sequence count");
	}

	if (memcmp(data_tgt->seq, ref, sizeof(ref)) != 0) {
		return ttest_fail(&tc, "Incorrect value");
	}

	return ttest_pass(&tc);
}

/* Test loading a sequence of mappings into an array of pointers to structs. */
static bool test_load_mapping_entry_sequence_mapping_ptr(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	struct value_s {
		short a;
		long b;
	} ref[3] = {
		{ .a =  123, .b =  9999, },
		{ .a = 4000, .b = 62000, },
		{ .a =    1, .b =   765, },
	};
	static const unsigned char yaml[] =
		"sequence:\n"
		"    - a: 123\n"
		"      b: 9999\n"
		"    - a: 4000\n"
		"      b: 62000\n"
		"    - a: 1\n"
		"      b: 765\n";
	struct target_struct {
		struct value_s *seq[3];
		uint32_t seq_count;
	} *data_tgt = NULL;
	static const struct cyaml_schema_mapping test_mapping_schema[] = {
		CYAML_MAPPING_INT("a", CYAML_FLAG_DEFAULT, struct value_s, a),
		CYAML_MAPPING_INT("b", CYAML_FLAG_DEFAULT, struct value_s, b),
		CYAML_MAPPING_END
	};
	static const struct cyaml_schema_type entry_schema = {
		CYAML_TYPE_MAPPING(CYAML_FLAG_POINTER,
				struct value_s, test_mapping_schema),
	};
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_SEQUENCE("sequence", CYAML_FLAG_DEFAULT,
				struct target_struct, seq, &entry_schema,
				0, CYAML_ARRAY_LEN(data_tgt->seq)),
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (CYAML_ARRAY_LEN(ref) != data_tgt->seq_count) {
		return ttest_fail(&tc, "Incorrect sequence count");
	}

	for (unsigned i = 0; i < CYAML_ARRAY_LEN(ref); i++) {
		if (ref[i].a != data_tgt->seq[i]->a) {
			return ttest_fail(&tc, "Incorrect value");
		}
		if (ref[i].b != data_tgt->seq[i]->b) {
			return ttest_fail(&tc, "Incorrect value");
		}
	}

	return ttest_pass(&tc);
}

/* Test loading a sequence of sequences of int into int[4][3]. */
static bool test_load_mapping_entry_sequence_sequence_fixed_int(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	int ref[4][3] = {
		{  1,  2,  3 },
		{  4,  5,  6 },
		{  7,  8,  9 },
		{ 10, 11, 12 },
	};
	static const unsigned char yaml[] =
		"sequence:\n"
		"    - [  1,  2,  3 ]\n"
		"    - [  4,  5,  6 ]\n"
		"    - [  7,  8,  9 ]\n"
		"    - [ 10, 11, 12 ]\n";
	struct target_struct {
		int seq[4][3];
		uint32_t seq_count;
	} *data_tgt = NULL;
	static const struct cyaml_schema_type entry_schema_int = {
		CYAML_TYPE_INT(CYAML_FLAG_DEFAULT, **(data_tgt->seq)),
	};
	static const struct cyaml_schema_type entry_schema = {
		CYAML_TYPE_SEQUENCE_FIXED(
				CYAML_FLAG_DEFAULT, **(data_tgt->seq),
				&entry_schema_int, CYAML_ARRAY_LEN(*ref)),
	};
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_SEQUENCE("sequence", CYAML_FLAG_DEFAULT,
				struct target_struct, seq, &entry_schema,
				0, CYAML_ARRAY_LEN(ref)),
		CYAML_MAPPING_END,
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (CYAML_ARRAY_LEN(ref) != data_tgt->seq_count) {
		return ttest_fail(&tc, "Incorrect sequence count: "
				"expected %u, got %u",
				CYAML_ARRAY_LEN(ref), data_tgt->seq_count);
	}

	for (unsigned j = 0; j < CYAML_ARRAY_LEN(ref); j++) {
		for (unsigned i = 0; i < CYAML_ARRAY_LEN(*ref); i++) {
			if (data_tgt->seq[j][i] != ref[j][i]) {
				return ttest_fail(&tc,
						"Incorrect value "
						"(i=%u, j=%u): "
						"got: %i, expected: %i", i, j,
						data_tgt->seq[j][i], ref[j][i]);
			}
		}
	}

	return ttest_pass(&tc);
}

/* Test loading a sequence of sequences of int into int*[4]. */
static bool test_load_mapping_entry_sequence_sequence_fixed_ptr_int(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	int ref[4][3] = {
		{  1,  2,  3 },
		{  4,  5,  6 },
		{  7,  8,  9 },
		{ 10, 11, 12 },
	};
	static const unsigned char yaml[] =
		"sequence:\n"
		"    - [  1,  2,  3 ]\n"
		"    - [  4,  5,  6 ]\n"
		"    - [  7,  8,  9 ]\n"
		"    - [ 10, 11, 12 ]\n";
	struct target_struct {
		int *seq[4];
		uint32_t seq_count;
	} *data_tgt = NULL;
	static const struct cyaml_schema_type entry_schema_int = {
		CYAML_TYPE_INT(CYAML_FLAG_DEFAULT, **(data_tgt->seq)),
	};
	static const struct cyaml_schema_type entry_schema = {
		CYAML_TYPE_SEQUENCE_FIXED(
				CYAML_FLAG_POINTER, **(data_tgt->seq),
				&entry_schema_int, CYAML_ARRAY_LEN(*ref)),
	};
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_SEQUENCE("sequence", CYAML_FLAG_DEFAULT,
				struct target_struct, seq, &entry_schema,
				0, CYAML_ARRAY_LEN(ref)),
		CYAML_MAPPING_END,
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (CYAML_ARRAY_LEN(ref) != data_tgt->seq_count) {
		return ttest_fail(&tc, "Incorrect sequence count: "
				"expected %u, got %u",
				CYAML_ARRAY_LEN(ref), data_tgt->seq_count);
	}

	for (unsigned j = 0; j < CYAML_ARRAY_LEN(ref); j++) {
		for (unsigned i = 0; i < CYAML_ARRAY_LEN(*ref); i++) {
			if (data_tgt->seq[j][i] != ref[j][i]) {
				return ttest_fail(&tc,
						"Incorrect value "
						"(i=%u, j=%u): "
						"got: %i, expected: %i", i, j,
						data_tgt->seq[j][i], ref[j][i]);
			}
		}
	}

	return ttest_pass(&tc);
}

/* Test loading a sequence of sequences of int into one-dimensional int[]. */
static bool test_load_mapping_entry_sequence_sequence_fixed_flat_int(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	int ref[4][3] = {
		{  1,  2,  3 },
		{  4,  5,  6 },
		{  7,  8,  9 },
		{ 10, 11, 12 },
	};
	static const unsigned char yaml[] =
		"sequence:\n"
		"    - [  1,  2,  3 ]\n"
		"    - [  4,  5,  6 ]\n"
		"    - [  7,  8,  9 ]\n"
		"    - [ 10, 11, 12 ]\n";
	struct target_struct {
		int seq[12];
		uint32_t seq_count;
	} *data_tgt = NULL;
	static const struct cyaml_schema_type entry_schema_int = {
		CYAML_TYPE_INT(CYAML_FLAG_DEFAULT, int),
	};
	static const struct cyaml_schema_type entry_schema = {
		CYAML_TYPE_SEQUENCE_FIXED(
				CYAML_FLAG_DEFAULT, int,
				&entry_schema_int, CYAML_ARRAY_LEN(*ref)),
	};
	static const struct cyaml_schema_mapping mapping_schema[] = {
		{
			.key = "sequence",
			.value = {
				.type = CYAML_SEQUENCE,
				.flags = CYAML_FLAG_DEFAULT,
				.data_size = sizeof(int[3]),
				.sequence = {
					.schema = &entry_schema,
					.count_size = sizeof(data_tgt->seq_count),
					.count_offset = offsetof(struct target_struct, seq_count),
					.min = 0,
					.max = CYAML_UNLIMITED,
				}
			},
			.data_offset = offsetof(struct target_struct, seq),
		},
		CYAML_MAPPING_END,
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	/* Note: count is count of entries of the outer sequence entries,
	 * so, 4, not 12. */
	if (CYAML_ARRAY_LEN(ref) != data_tgt->seq_count) {
		return ttest_fail(&tc, "Incorrect sequence count: "
				"expected %u, got %u",
				CYAML_ARRAY_LEN(ref), data_tgt->seq_count);
	}

	for (unsigned j = 0; j < CYAML_ARRAY_LEN(ref); j++) {
		for (unsigned i = 0; i < CYAML_ARRAY_LEN(*ref); i++) {
			if (data_tgt->seq[j * CYAML_ARRAY_LEN(*ref) + i] != ref[j][i]) {
				return ttest_fail(&tc,
						"Incorrect value "
						"(i=%u, j=%u): "
						"got: %i, expected: %i", i, j,
						data_tgt->seq[j * CYAML_ARRAY_LEN(*ref) + i],
						ref[j][i]);
			}
		}
	}

	return ttest_pass(&tc);
}

/* Test loading a sequence of integers to allocated int* array. */
static bool test_load_mapping_entry_sequence_ptr_int(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	int ref[] = { 1, 1, 2, 3, 5, 8 };
	static const unsigned char yaml[] =
		"sequence:\n"
		"    - 1\n"
		"    - 1\n"
		"    - 2\n"
		"    - 3\n"
		"    - 5\n"
		"    - 8\n";
	struct target_struct {
		int *seq;
		uint32_t seq_count;
	} *data_tgt = NULL;
	static const struct cyaml_schema_type entry_schema = {
		CYAML_TYPE_INT(CYAML_FLAG_DEFAULT, *(data_tgt->seq)),
	};
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_SEQUENCE("sequence", CYAML_FLAG_POINTER,
				struct target_struct, seq, &entry_schema,
				0, CYAML_UNLIMITED),
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (CYAML_ARRAY_LEN(ref) != data_tgt->seq_count) {
		return ttest_fail(&tc, "Incorrect sequence count");
	}

	for (unsigned i = 0; i < CYAML_ARRAY_LEN(ref); i++) {
		if (data_tgt->seq[i] != ref[i]) {
			return ttest_fail(&tc, "Incorrect value (i=%u): "
					"got: %i, expected: %i", i,
					data_tgt->seq[i], ref[i]);
		}
	}

	return ttest_pass(&tc);
}

/* Test loading a sequence of enums to allocated enum test_enum* array. */
static bool test_load_mapping_entry_sequence_ptr_enum(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	enum test_enum {
		TEST_ENUM_FIRST,
		TEST_ENUM_SECOND,
		TEST_ENUM_THIRD,
		TEST_ENUM__COUNT,
	} ref[] = { TEST_ENUM_FIRST, TEST_ENUM_SECOND, TEST_ENUM_THIRD };
	static const char * const strings[TEST_ENUM__COUNT] = {
		[TEST_ENUM_FIRST]  = "first",
		[TEST_ENUM_SECOND] = "second",
		[TEST_ENUM_THIRD]  = "third",
	};
	static const unsigned char yaml[] =
		"sequence:\n"
		"    - first\n"
		"    - second\n"
		"    - third\n";
	struct target_struct {
		enum test_enum *seq;
		uint32_t seq_count;
	} *data_tgt = NULL;
	static const struct cyaml_schema_type entry_schema = {
		CYAML_TYPE_ENUM(CYAML_FLAG_DEFAULT,
				*(data_tgt->seq), strings, TEST_ENUM__COUNT),
	};
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_SEQUENCE("sequence", CYAML_FLAG_POINTER,
				struct target_struct, seq, &entry_schema,
				0, CYAML_UNLIMITED),
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (CYAML_ARRAY_LEN(ref) != data_tgt->seq_count) {
		return ttest_fail(&tc, "Incorrect sequence count");
	}

	for (unsigned i = 0; i < CYAML_ARRAY_LEN(ref); i++) {
		if (data_tgt->seq[i] != ref[i]) {
			return ttest_fail(&tc, "Incorrect value (i=%u): "
					"got: %i, expected: %i", i,
					data_tgt->seq[i], ref[i]);
		}
	}

	return ttest_pass(&tc);
}

/* Test loading a sequence of unsigned integers to allocated unsigned* array. */
static bool test_load_mapping_entry_sequence_ptr_uint(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	unsigned ref[] = { 99999, 99998, 99997, 99996, 99995, 99994 };
	static const unsigned char yaml[] =
		"sequence:\n"
		"    - 99999\n"
		"    - 99998\n"
		"    - 99997\n"
		"    - 99996\n"
		"    - 99995\n"
		"    - 99994\n";
	struct target_struct {
		unsigned *seq;
		uint32_t seq_count;
	} *data_tgt = NULL;
	static const struct cyaml_schema_type entry_schema = {
		CYAML_TYPE_UINT(CYAML_FLAG_DEFAULT, *(data_tgt->seq)),
	};
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_SEQUENCE("sequence", CYAML_FLAG_POINTER,
				struct target_struct, seq, &entry_schema,
				0, CYAML_UNLIMITED),
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (CYAML_ARRAY_LEN(ref) != data_tgt->seq_count) {
		return ttest_fail(&tc, "Incorrect sequence count");
	}

	for (unsigned i = 0; i < CYAML_ARRAY_LEN(ref); i++) {
		if (data_tgt->seq[i] != ref[i]) {
			return ttest_fail(&tc, "Incorrect value (i=%u): "
					"got: %i, expected: %i", i,
					data_tgt->seq[i], ref[i]);
		}
	}

	return ttest_pass(&tc);
}

/* Test loading a sequence of boolean values to allocated bool* array. */
static bool test_load_mapping_entry_sequence_ptr_bool(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	bool ref[] = { true, false, true, false, true, false, true, false };
	static const unsigned char yaml[] =
		"sequence:\n"
		"    - true\n"
		"    - false\n"
		"    - yes\n"
		"    - no\n"
		"    - enable\n"
		"    - disable\n"
		"    - 1\n"
		"    - 0\n";
	struct target_struct {
		bool *seq;
		uint32_t seq_count;
	} *data_tgt = NULL;
	static const struct cyaml_schema_type entry_schema = {
		CYAML_TYPE_BOOL(CYAML_FLAG_DEFAULT, *(data_tgt->seq)),
	};
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_SEQUENCE("sequence", CYAML_FLAG_POINTER,
				struct target_struct, seq, &entry_schema,
				0, CYAML_UNLIMITED),
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (CYAML_ARRAY_LEN(ref) != data_tgt->seq_count) {
		return ttest_fail(&tc, "Incorrect sequence count");
	}

	for (unsigned i = 0; i < CYAML_ARRAY_LEN(ref); i++) {
		if (data_tgt->seq[i] != ref[i]) {
			return ttest_fail(&tc, "Incorrect value (i=%u): "
					"got: %i, expected: %i", i,
					data_tgt->seq[i], ref[i]);
		}
	}

	return ttest_pass(&tc);
}

/* Test loading a sequence of flag sequences to allocated flag words array. */
static bool test_load_mapping_entry_sequence_ptr_flags(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	enum test_flags {
		TEST_FLAGS_NONE   = 0,
		TEST_FLAGS_FIRST  = (1 << 0),
		TEST_FLAGS_SECOND = (1 << 1),
		TEST_FLAGS_THIRD  = (1 << 2),
		TEST_FLAGS_FOURTH = (1 << 3),
		TEST_FLAGS_FIFTH  = (1 << 4),
		TEST_FLAGS_SIXTH  = (1 << 5),
	} ref[3] = {
		TEST_FLAGS_SECOND | TEST_FLAGS_FIFTH | 1024,
		TEST_FLAGS_FIRST,
		TEST_FLAGS_FOURTH | TEST_FLAGS_SIXTH
	};
	#define TEST_FLAGS__COUNT 6
	static const char * const strings[TEST_FLAGS__COUNT] = {
		"first",
		"second",
		"third",
		"fourth",
		"fifth",
		"sixth",
	};
	static const unsigned char yaml[] =
		"sequence:\n"
		"    - - second\n"
		"      - fifth\n"
		"      - 1024\n"
		"    - - first\n"
		"    - - fourth\n"
		"      - sixth\n";
	struct target_struct {
		enum test_flags *seq;
		uint32_t seq_count;
	} *data_tgt = NULL;
	static const struct cyaml_schema_type entry_schema = {
		CYAML_TYPE_FLAGS(CYAML_FLAG_DEFAULT, *(data_tgt->seq),
				strings, TEST_FLAGS__COUNT),
	};
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_SEQUENCE("sequence", CYAML_FLAG_POINTER,
				struct target_struct, seq, &entry_schema,
				0, CYAML_UNLIMITED),
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (CYAML_ARRAY_LEN(ref) != data_tgt->seq_count) {
		return ttest_fail(&tc, "Incorrect sequence count");
	}

	for (unsigned i = 0; i < CYAML_ARRAY_LEN(ref); i++) {
		if (data_tgt->seq[i] != ref[i]) {
			return ttest_fail(&tc, "Incorrect value (i=%u): "
					"got: %i, expected: %i", i,
					data_tgt->seq[i], ref[i]);
		}
	}

	return ttest_pass(&tc);
}

/* Test loading a sequence of strings to allocated array of char[7]. */
static bool test_load_mapping_entry_sequence_ptr_string(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	const char *ref[] = {
		"This",
		"is",
		"merely",
		"a",
		"test",
	};
	static const unsigned char yaml[] =
		"sequence:\n"
		"    - This\n"
		"    - is\n"
		"    - merely\n"
		"    - a\n"
		"    - test\n";
	struct target_struct {
		char (*seq)[7];
		uint32_t seq_count;
	} *data_tgt = NULL;
	static const struct cyaml_schema_type entry_schema = {
		CYAML_TYPE_STRING(CYAML_FLAG_DEFAULT, *(data_tgt->seq), 0, 6),
	};
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_SEQUENCE("sequence", CYAML_FLAG_POINTER,
				struct target_struct, seq, &entry_schema,
				0, CYAML_ARRAY_LEN(ref)),
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (CYAML_ARRAY_LEN(ref) != data_tgt->seq_count) {
		return ttest_fail(&tc, "Incorrect sequence count");
	}

	for (unsigned i = 0; i < CYAML_ARRAY_LEN(ref); i++) {
		if (strcmp(data_tgt->seq[i], ref[i]) != 0) {
			return ttest_fail(&tc, "Incorrect value (i=%u)", i);
		}
	}

	return ttest_pass(&tc);
}

/* Test loading a sequence of strings to allocated array of
 * allocated strings. */
static bool test_load_mapping_entry_sequence_ptr_string_ptr(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	const char *ref[] = {
		"This",
		"is",
		"merely",
		"a",
		"test",
	};
	static const unsigned char yaml[] =
		"sequence:\n"
		"    - This\n"
		"    - is\n"
		"    - merely\n"
		"    - a\n"
		"    - test\n";
	struct target_struct {
		char **seq;
		uint32_t seq_count;
	} *data_tgt = NULL;
	static const struct cyaml_schema_type entry_schema = {
		CYAML_TYPE_STRING(CYAML_FLAG_POINTER, *(data_tgt->seq),
				0, CYAML_UNLIMITED),
	};
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_SEQUENCE("sequence", CYAML_FLAG_POINTER,
				struct target_struct, seq, &entry_schema,
				0, CYAML_ARRAY_LEN(ref)),
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (CYAML_ARRAY_LEN(ref) != data_tgt->seq_count) {
		return ttest_fail(&tc, "Incorrect sequence count");
	}

	for (unsigned i = 0; i < CYAML_ARRAY_LEN(ref); i++) {
		if (strcmp(data_tgt->seq[i], ref[i]) != 0) {
			return ttest_fail(&tc, "Incorrect value (i=%u)", i);
		}
	}

	return ttest_pass(&tc);
}

/* Test loading a sequence of mappings to allocated array mapping structs. */
static bool test_load_mapping_entry_sequence_ptr_mapping(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	struct value_s {
		short a;
		long b;
	} ref[3] = {
		{ .a =  123, .b =  9999, },
		{ .a = 4000, .b = 62000, },
		{ .a =    1, .b =   765, },
	};
	static const unsigned char yaml[] =
		"sequence:\n"
		"    - a: 123\n"
		"      b: 9999\n"
		"    - a: 4000\n"
		"      b: 62000\n"
		"    - a: 1\n"
		"      b: 765\n";
	struct target_struct {
		struct value_s *seq;
		uint32_t seq_count;
	} *data_tgt = NULL;
	static const struct cyaml_schema_mapping test_mapping_schema[] = {
		CYAML_MAPPING_INT("a", CYAML_FLAG_DEFAULT, struct value_s, a),
		CYAML_MAPPING_INT("b", CYAML_FLAG_DEFAULT, struct value_s, b),
		CYAML_MAPPING_END
	};
	static const struct cyaml_schema_type entry_schema = {
		CYAML_TYPE_MAPPING(CYAML_FLAG_DEFAULT,
				struct value_s, test_mapping_schema),
	};
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_SEQUENCE("sequence", CYAML_FLAG_POINTER,
				struct target_struct, seq, &entry_schema,
				0, CYAML_ARRAY_LEN(ref)),
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (CYAML_ARRAY_LEN(ref) != data_tgt->seq_count) {
		return ttest_fail(&tc, "Incorrect sequence count");
	}

	for (unsigned i = 0; i < CYAML_ARRAY_LEN(ref); i++) {
		if (ref[i].a != data_tgt->seq[i].a) {
			return ttest_fail(&tc, "Incorrect value");
		}
		if (ref[i].b != data_tgt->seq[i].b) {
			return ttest_fail(&tc, "Incorrect value");
		}
	}

	return ttest_pass(&tc);
}

/* Test loading a sequence of mappings to allocated array of pointers to
 * mapping structs. */
static bool test_load_mapping_entry_sequence_ptr_mapping_ptr(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	struct value_s {
		short a;
		long b;
	} ref[3] = {
		{ .a =  123, .b =  9999, },
		{ .a = 4000, .b = 62000, },
		{ .a =    1, .b =   765, },
	};
	static const unsigned char yaml[] =
		"sequence:\n"
		"    - a: 123\n"
		"      b: 9999\n"
		"    - a: 4000\n"
		"      b: 62000\n"
		"    - a: 1\n"
		"      b: 765\n";
	struct target_struct {
		struct value_s **seq;
		uint32_t seq_count;
	} *data_tgt = NULL;
	static const struct cyaml_schema_mapping test_mapping_schema[] = {
		CYAML_MAPPING_INT("a", CYAML_FLAG_DEFAULT, struct value_s, a),
		CYAML_MAPPING_INT("b", CYAML_FLAG_DEFAULT, struct value_s, b),
		CYAML_MAPPING_END
	};
	static const struct cyaml_schema_type entry_schema = {
		CYAML_TYPE_MAPPING(CYAML_FLAG_POINTER,
				struct value_s, test_mapping_schema),
	};
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_SEQUENCE("sequence", CYAML_FLAG_POINTER,
				struct target_struct, seq, &entry_schema,
				0, CYAML_ARRAY_LEN(ref)),
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (CYAML_ARRAY_LEN(ref) != data_tgt->seq_count) {
		return ttest_fail(&tc, "Incorrect sequence count");
	}

	for (unsigned i = 0; i < CYAML_ARRAY_LEN(ref); i++) {
		if (ref[i].a != data_tgt->seq[i]->a) {
			return ttest_fail(&tc, "Incorrect value");
		}
		if (ref[i].b != data_tgt->seq[i]->b) {
			return ttest_fail(&tc, "Incorrect value");
		}
	}

	return ttest_pass(&tc);
}

/* Test loading a sequence of sequences of integers to allocated array
 * of int[3]. */
static bool test_load_mapping_entry_sequence_ptr_sequence_fixed_int(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	int ref[4][3] = {
		{  1,  2,  3 },
		{  4,  5,  6 },
		{  7,  8,  9 },
		{ 10, 11, 12 },
	};
	static const unsigned char yaml[] =
		"sequence:\n"
		"    - [  1,  2,  3 ]\n"
		"    - [  4,  5,  6 ]\n"
		"    - [  7,  8,  9 ]\n"
		"    - [ 10, 11, 12 ]\n";
	struct target_struct {
		int (*seq)[3];
		uint32_t seq_count;
	} *data_tgt = NULL;
	static const struct cyaml_schema_type entry_schema_int = {
		CYAML_TYPE_INT(CYAML_FLAG_DEFAULT, int),
	};
	static const struct cyaml_schema_type entry_schema = {
		CYAML_TYPE_SEQUENCE_FIXED(
				CYAML_FLAG_DEFAULT, int,
				&entry_schema_int, CYAML_ARRAY_LEN(*ref)),
	};
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_SEQUENCE("sequence", CYAML_FLAG_POINTER,
				struct target_struct, seq, &entry_schema,
				0, CYAML_UNLIMITED),
		CYAML_MAPPING_END,
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (CYAML_ARRAY_LEN(ref) != data_tgt->seq_count) {
		return ttest_fail(&tc, "Incorrect sequence count: "
				"expected %u, got %u",
				CYAML_ARRAY_LEN(ref), data_tgt->seq_count);
	}

	for (unsigned j = 0; j < CYAML_ARRAY_LEN(ref); j++) {
		for (unsigned i = 0; i < CYAML_ARRAY_LEN(*ref); i++) {
			if (data_tgt->seq[j][i] != ref[j][i]) {
				return ttest_fail(&tc,
						"Incorrect value "
						"(i=%u, j=%u): "
						"got: %i, expected: %i", i, j,
						data_tgt->seq[j][i], ref[j][i]);
			}
		}
	}

	return ttest_pass(&tc);
}

/* Test loading a sequence of sequences of integers to allocated array
 * of allocated arrays of integers. */
static bool test_load_mapping_entry_sequence_ptr_sequence_fixed_ptr_int(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	int ref[4][3] = {
		{  1,  2,  3 },
		{  4,  5,  6 },
		{  7,  8,  9 },
		{ 10, 11, 12 },
	};
	static const unsigned char yaml[] =
		"sequence:\n"
		"    - [  1,  2,  3 ]\n"
		"    - [  4,  5,  6 ]\n"
		"    - [  7,  8,  9 ]\n"
		"    - [ 10, 11, 12 ]\n";
	struct target_struct {
		int **seq;
		uint32_t seq_count;
	} *data_tgt = NULL;
	static const struct cyaml_schema_type entry_schema_int = {
		CYAML_TYPE_INT(CYAML_FLAG_DEFAULT, int),
	};
	static const struct cyaml_schema_type entry_schema = {
		CYAML_TYPE_SEQUENCE_FIXED(
				CYAML_FLAG_POINTER, int,
				&entry_schema_int, CYAML_ARRAY_LEN(*ref)),
	};
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_SEQUENCE("sequence", CYAML_FLAG_POINTER,
				struct target_struct, seq, &entry_schema,
				0, CYAML_UNLIMITED),
		CYAML_MAPPING_END,
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (CYAML_ARRAY_LEN(ref) != data_tgt->seq_count) {
		return ttest_fail(&tc, "Incorrect sequence count: "
				"expected %u, got %u",
				CYAML_ARRAY_LEN(ref), data_tgt->seq_count);
	}

	for (unsigned j = 0; j < CYAML_ARRAY_LEN(ref); j++) {
		for (unsigned i = 0; i < CYAML_ARRAY_LEN(*ref); i++) {
			if (data_tgt->seq[j][i] != ref[j][i]) {
				return ttest_fail(&tc,
						"Incorrect value "
						"(i=%u, j=%u): "
						"got: %i, expected: %i", i, j,
						data_tgt->seq[j][i], ref[j][i]);
			}
		}
	}

	return ttest_pass(&tc);
}

/* Test loading a sequence of sequences of integers a one-dimensional allocated
 * array of integers. */
static bool test_load_mapping_entry_sequence_ptr_sequence_fixed_flat_int(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	int ref[4][3] = {
		{  1,  2,  3 },
		{  4,  5,  6 },
		{  7,  8,  9 },
		{ 10, 11, 12 },
	};
	static const unsigned char yaml[] =
		"sequence:\n"
		"    - [  1,  2,  3 ]\n"
		"    - [  4,  5,  6 ]\n"
		"    - [  7,  8,  9 ]\n"
		"    - [ 10, 11, 12 ]\n";
	struct target_struct {
		int *seq;
		uint32_t seq_count;
	} *data_tgt = NULL;
	static const struct cyaml_schema_type entry_schema_int = {
		CYAML_TYPE_INT(CYAML_FLAG_DEFAULT, int),
	};
	static const struct cyaml_schema_type entry_schema = {
		CYAML_TYPE_SEQUENCE_FIXED(
				CYAML_FLAG_DEFAULT, int,
				&entry_schema_int, CYAML_ARRAY_LEN(*ref)),
	};
	static const struct cyaml_schema_mapping mapping_schema[] = {
		{
			.key = "sequence",
			.value = {
				.type = CYAML_SEQUENCE,
				.flags = CYAML_FLAG_POINTER,
				.data_size = sizeof(int[3]),
				.sequence = {
					.schema = &entry_schema,
					.count_size = sizeof(data_tgt->seq_count),
					.count_offset = offsetof(struct target_struct, seq_count),
					.min = 0,
					.max = CYAML_UNLIMITED,
				}
			},
			.data_offset = offsetof(struct target_struct, seq),
		},
		CYAML_MAPPING_END,
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	/* Note: count is count of entries of the outer sequence entries,
	 * so, 4, not 12. */
	if (CYAML_ARRAY_LEN(ref) != data_tgt->seq_count) {
		return ttest_fail(&tc, "Incorrect sequence count: "
				"expected %u, got %u",
				CYAML_ARRAY_LEN(ref), data_tgt->seq_count);
	}

	for (unsigned j = 0; j < CYAML_ARRAY_LEN(ref); j++) {
		for (unsigned i = 0; i < CYAML_ARRAY_LEN(*ref); i++) {
			if (data_tgt->seq[j * CYAML_ARRAY_LEN(*ref) + i] != ref[j][i]) {
				return ttest_fail(&tc,
						"Incorrect value "
						"(i=%u, j=%u): "
						"got: %i, expected: %i", i, j,
						data_tgt->seq[j * CYAML_ARRAY_LEN(*ref) + i],
						ref[j][i]);
			}
		}
	}

	return ttest_pass(&tc);
}

/* Test loading a mapping multiple fields. */
static bool test_load_mapping_with_multiple_fields(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	struct target_struct {
		signed char a;
		short b;
		int c;
		long d;
		long long e;
	} data = {
		.a = 9,
		.b = 90,
		.c = 900,
		.d = 9000,
		.e = 90000,
	};
	static const unsigned char yaml[] =
		"a: 9\n"
		"b: 90\n"
		"c: 900\n"
		"d: 9000\n"
		"e: 90000\n";
	struct target_struct *data_tgt = NULL;
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_INT("a", CYAML_FLAG_DEFAULT,
				struct target_struct, a),
		CYAML_MAPPING_INT("b", CYAML_FLAG_DEFAULT,
				struct target_struct, b),
		CYAML_MAPPING_INT("c", CYAML_FLAG_DEFAULT,
				struct target_struct, c),
		CYAML_MAPPING_INT("d", CYAML_FLAG_DEFAULT,
				struct target_struct, d),
		CYAML_MAPPING_INT("e", CYAML_FLAG_DEFAULT,
				struct target_struct, e),
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (data_tgt->a != data.a) {
		return ttest_fail(&tc, "Incorrect value for entry a");
	}
	if (data_tgt->b != data.b) {
		return ttest_fail(&tc, "Incorrect value for entry b");
	}
	if (data_tgt->c != data.c) {
		return ttest_fail(&tc, "Incorrect value for entry c");
	}
	if (data_tgt->d != data.d) {
		return ttest_fail(&tc, "Incorrect value for entry d");
	}
	if (data_tgt->e != data.e) {
		return ttest_fail(&tc, "Incorrect value for entry e");
	}

	return ttest_pass(&tc);
}

/* Test loading a mapping with optional fields. */
static bool test_load_mapping_with_optional_fields(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	long values[] = { 4, 3, 2, 1 };
	struct target_struct {
		char *a;
		char b[10];
		int c;
		long d[4];
		long *e;
		unsigned e_count;
		char *f;
		char *g;
		char h[10];
		int i;
		long j[4];
		long *k;
		unsigned k_count;
	} data = {
		.a = "Hello",
		.b = "World!",
		.c = 0,
		.d = { 0, 0, 0, 0 },
		.e = values,
		.f = "Required!",
		.g = NULL,
		.h = "\0",
		.i = 9876,
		.j = { 1, 2, 3, 4 },
		.k = NULL,
	};
	static const unsigned char yaml[] =
		"a: Hello\n"
		"b: World!\n"
		"e: [ 4, 3, 2, 1 ]\n"
		"f: Required!\n"
		"i: 9876\n"
		"j: [ 1, 2, 3, 4 ]\n";
	struct target_struct *data_tgt = NULL;
	static const struct cyaml_schema_type sequence_entry = {
		CYAML_TYPE_INT(CYAML_FLAG_DEFAULT, sizeof(long)),
	};
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_STRING_PTR("a",
				CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL,
				struct target_struct, a, 0, CYAML_UNLIMITED),
		CYAML_MAPPING_STRING("b", CYAML_FLAG_OPTIONAL,
				struct target_struct, b, 0),
		CYAML_MAPPING_INT("c", CYAML_FLAG_OPTIONAL,
				struct target_struct, c),
		CYAML_MAPPING_SEQUENCE_FIXED("d", CYAML_FLAG_OPTIONAL,
				struct target_struct, d, &sequence_entry, 4),
		CYAML_MAPPING_SEQUENCE("e",
				CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL,
				struct target_struct, e, &sequence_entry,
				0, CYAML_UNLIMITED),
		CYAML_MAPPING_STRING_PTR("f",
				CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL,
				struct target_struct, f, 0, CYAML_UNLIMITED),
		CYAML_MAPPING_STRING_PTR("g",
				CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL,
				struct target_struct, g, 0, CYAML_UNLIMITED),
		CYAML_MAPPING_STRING("h", CYAML_FLAG_OPTIONAL,
				struct target_struct, h, 0),
		CYAML_MAPPING_INT("i", CYAML_FLAG_OPTIONAL,
				struct target_struct, i),
		CYAML_MAPPING_SEQUENCE_FIXED("j", CYAML_FLAG_OPTIONAL,
				struct target_struct, j, &sequence_entry, 4),
		CYAML_MAPPING_SEQUENCE("k",
				CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL,
				struct target_struct, k, &sequence_entry,
				0, CYAML_UNLIMITED),
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (strcmp(data_tgt->a, data.a) != 0) {
		return ttest_fail(&tc, "Incorrect value for entry a: "
				"Expected: %s, got: %s",
				data.a, data_tgt->a);
	}
	if (strcmp(data_tgt->b, data.b) != 0) {
		return ttest_fail(&tc, "Incorrect value for entry b");
	}
	if (data_tgt->c != data.c) {
		return ttest_fail(&tc, "Incorrect value for entry c");
	}
	for (unsigned i = 0; i < 4; i++) {
		if (data_tgt->d[i] != data.d[i]) {
			return ttest_fail(&tc, "Incorrect value for entry d");
		}
	}
	for (unsigned i = 0; i < 4; i++) {
		if (data_tgt->e[i] != data.e[i]) {
			return ttest_fail(&tc, "Incorrect value for entry e "
					"Index: %u: Expected: %ld, got: %ld",
					i, data.e[i], data_tgt->e[i]);
		}
	}
	if (strcmp(data_tgt->f, data.f) != 0) {
		return ttest_fail(&tc, "Incorrect value for entry f: "
				"Expected: %s, got: %s",
				data.f, data_tgt->f);
	}
	if (data_tgt->g != data.g) {
		return ttest_fail(&tc, "Incorrect value for entry g: "
				"Expected: %s, got: %s",
				data.g, data_tgt->g);
	}
	if (strcmp(data_tgt->h, data.h) != 0) {
		return ttest_fail(&tc, "Incorrect value for entry h");
	}
	if (data_tgt->i != data.i) {
		return ttest_fail(&tc, "Incorrect value for entry i");
	}
	for (unsigned i = 0; i < 4; i++) {
		if (data_tgt->j[i] != data.j[i]) {
			return ttest_fail(&tc, "Incorrect value for entry j");
		}
	}
	if (data_tgt->k != data.k) {
		return ttest_fail(&tc, "Incorrect value for entry k");
	}

	return ttest_pass(&tc);
}

/* Test loading a mapping with only optional fields. */
static bool test_load_mapping_only_optional_fields(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	struct target_struct {
		int c;
		int i;
	};
	static const unsigned char yaml[] =
		"\n";
	struct target_struct *data_tgt = NULL;
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_INT("c", CYAML_FLAG_OPTIONAL,
				struct target_struct, c),
		CYAML_MAPPING_INT("i", CYAML_FLAG_OPTIONAL,
				struct target_struct, i),
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
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (data_tgt != NULL) {
		return ttest_fail(&tc, "Shouldn't have allocated anything");
	}

	return ttest_pass(&tc);
}

/* Test loading a mapping with unknown keys ignored by config. */
static bool test_load_mapping_ignored_unknown_keys(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	struct target_struct {
		short b;
		int c;
		long d;
		long long e;
	} data = {
		.b = 90,
		.c = 900,
		.d = 9000,
		.e = 90000,
	};
	static const unsigned char yaml[] =
		"a: 9\n"
		"b: 90\n"
		"c: 900\n"
		"d: 9000\n"
		"e: 90000\n"
		"f: 900000\n";
	struct target_struct *data_tgt = NULL;
	static const struct cyaml_schema_mapping mapping_schema[] = {
		CYAML_MAPPING_INT("b", CYAML_FLAG_DEFAULT,
				struct target_struct, b),
		CYAML_MAPPING_INT("c", CYAML_FLAG_DEFAULT,
				struct target_struct, c),
		CYAML_MAPPING_INT("d", CYAML_FLAG_DEFAULT,
				struct target_struct, d),
		CYAML_MAPPING_INT("e", CYAML_FLAG_DEFAULT,
				struct target_struct, e),
		CYAML_MAPPING_END
	};
	static const struct cyaml_schema_type top_schema = {
		CYAML_TYPE_MAPPING(CYAML_FLAG_POINTER,
				struct target_struct, mapping_schema),
	};
	cyaml_config_t cfg = *config;
	test_data_t td = {
		.data = (cyaml_data_t **) &data_tgt,
		.config = &cfg,
		.schema = &top_schema,
	};
	cyaml_err_t err;

	ttest_ctx_t tc = ttest_start(report, __func__, cyaml_cleanup, &td);

	cfg.flags |= CYAML_CFG_IGNORE_UNKNOWN_KEYS;
	err = cyaml_load_data(yaml, YAML_LEN(yaml), &cfg, &top_schema,
			(cyaml_data_t **) &data_tgt);
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (data_tgt->b != data.b) {
		return ttest_fail(&tc, "Incorrect value for entry b");
	}
	if (data_tgt->c != data.c) {
		return ttest_fail(&tc, "Incorrect value for entry c");
	}
	if (data_tgt->d != data.d) {
		return ttest_fail(&tc, "Incorrect value for entry d");
	}
	if (data_tgt->e != data.e) {
		return ttest_fail(&tc, "Incorrect value for entry e");
	}

	return ttest_pass(&tc);
}

/**
 * Run the YAML loading unit tests.
 *
 * \param[in]  rc         The ttest report context.
 * \param[in]  log_level  CYAML log level.
 * \param[in]  log_fn     CYAML logging function, or NULL.
 * \return true iff all unit tests pass, otherwise false.
 */
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
	pass &= test_load_mapping_entry_float(rc, &config);
	pass &= test_load_mapping_entry_double(rc, &config);
	pass &= test_load_mapping_entry_string(rc, &config);
	pass &= test_load_mapping_entry_int_pos(rc, &config);
	pass &= test_load_mapping_entry_int_neg(rc, &config);
	pass &= test_load_mapping_entry_bool_true(rc, &config);
	pass &= test_load_mapping_entry_bool_false(rc, &config);
	pass &= test_load_mapping_entry_string_ptr(rc, &config);
	pass &= test_load_mapping_entry_ignore_deep(rc, &config);
	pass &= test_load_mapping_entry_ignore_scalar(rc, &config);

	ttest_heading(rc, "Load single entry mapping tests: complex types");

	pass &= test_load_mapping_entry_flags(rc, &config);
	pass &= test_load_mapping_entry_mapping(rc, &config);
	pass &= test_load_mapping_entry_mapping_ptr(rc, &config);

	ttest_heading(rc, "Load single entry mapping tests: sequences");

	pass &= test_load_mapping_entry_sequence_int(rc, &config);
	pass &= test_load_mapping_entry_sequence_enum(rc, &config);
	pass &= test_load_mapping_entry_sequence_uint(rc, &config);
	pass &= test_load_mapping_entry_sequence_bool(rc, &config);
	pass &= test_load_mapping_entry_sequence_flags(rc, &config);
	pass &= test_load_mapping_entry_sequence_string(rc, &config);
	pass &= test_load_mapping_entry_sequence_mapping(rc, &config);
	pass &= test_load_mapping_entry_sequence_string_ptr(rc, &config);
	pass &= test_load_mapping_entry_sequence_mapping_ptr(rc, &config);
	pass &= test_load_mapping_entry_sequence_sequence_fixed_int(rc, &config);
	pass &= test_load_mapping_entry_sequence_sequence_fixed_ptr_int(rc, &config);
	pass &= test_load_mapping_entry_sequence_sequence_fixed_flat_int(rc, &config);

	ttest_heading(rc, "Load single entry mapping tests: ptr sequences");

	pass &= test_load_mapping_entry_sequence_ptr_int(rc, &config);
	pass &= test_load_mapping_entry_sequence_ptr_enum(rc, &config);
	pass &= test_load_mapping_entry_sequence_ptr_uint(rc, &config);
	pass &= test_load_mapping_entry_sequence_ptr_bool(rc, &config);
	pass &= test_load_mapping_entry_sequence_ptr_flags(rc, &config);
	pass &= test_load_mapping_entry_sequence_ptr_string(rc, &config);
	pass &= test_load_mapping_entry_sequence_ptr_mapping(rc, &config);
	pass &= test_load_mapping_entry_sequence_ptr_string_ptr(rc, &config);
	pass &= test_load_mapping_entry_sequence_ptr_mapping_ptr(rc, &config);
	pass &= test_load_mapping_entry_sequence_ptr_sequence_fixed_int(rc, &config);
	pass &= test_load_mapping_entry_sequence_ptr_sequence_fixed_ptr_int(rc, &config);
	pass &= test_load_mapping_entry_sequence_ptr_sequence_fixed_flat_int(rc, &config);

	ttest_heading(rc, "Load tests: various");

	pass &= test_load_mapping_with_multiple_fields(rc, &config);
	pass &= test_load_mapping_with_optional_fields(rc, &config);
	pass &= test_load_mapping_only_optional_fields(rc, &config);
	pass &= test_load_mapping_ignored_unknown_keys(rc, &config);

	return pass;
}
