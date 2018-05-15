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
 * Test saving an unsigned integer.
 *
 * \param[in]  report  The test report context.
 * \param[in]  config  The CYAML config to use for the test.
 * \return true if test passes, false otherwise.
 */
static bool test_save_mapping_entry_uint(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	static const unsigned char ref[] =
		"---\n"
		"test_uint: 555\n"
		"...\n";
	static const struct target_struct {
		unsigned test_uint;
	} data = {
		.test_uint = 555,
	};
	static const struct cyaml_schema_field mapping_schema[] = {
		CYAML_FIELD_UINT("test_uint", CYAML_FLAG_DEFAULT,
				struct target_struct, test_uint),
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

	if (len != YAML_LEN(ref) || memcmp(ref, buffer, len) != 0) {
		return ttest_fail(&tc, "Bad data:\n"
				"EXPECTED (%zu):\n\n%*s\n\n"
				"GOT (%zu):\n\n%*s\n",
				YAML_LEN(ref), YAML_LEN(ref), ref,
				len, len, buffer);
	}

	return ttest_pass(&tc);
}

/**
 * Test saving a float.
 *
 * \param[in]  report  The test report context.
 * \param[in]  config  The CYAML config to use for the test.
 * \return true if test passes, false otherwise.
 */
static bool test_save_mapping_entry_float(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	static const unsigned char ref[] =
		"---\n"
		"test_float: 3.14\n"
		"...\n";
	static const struct target_struct {
		float test_float;
	} data = {
		.test_float = 3.14,
	};
	static const struct cyaml_schema_field mapping_schema[] = {
		CYAML_FIELD_FLOAT("test_float", CYAML_FLAG_DEFAULT,
				struct target_struct, test_float),
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

	if (len != YAML_LEN(ref) || memcmp(ref, buffer, len) != 0) {
		return ttest_fail(&tc, "Bad data:\n"
				"EXPECTED (%zu):\n\n%*s\n\n"
				"GOT (%zu):\n\n%*s\n",
				YAML_LEN(ref), YAML_LEN(ref), ref,
				len, len, buffer);
	}

	return ttest_pass(&tc);
}

/**
 * Test saving a double.
 *
 * \param[in]  report  The test report context.
 * \param[in]  config  The CYAML config to use for the test.
 * \return true if test passes, false otherwise.
 */
static bool test_save_mapping_entry_double(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	static const unsigned char ref[] =
		"---\n"
		"test_float: 3.14\n"
		"...\n";
	static const struct target_struct {
		double test_float;
	} data = {
		.test_float = 3.14,
	};
	static const struct cyaml_schema_field mapping_schema[] = {
		CYAML_FIELD_FLOAT("test_float", CYAML_FLAG_DEFAULT,
				struct target_struct, test_float),
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

	if (len != YAML_LEN(ref) || memcmp(ref, buffer, len) != 0) {
		return ttest_fail(&tc, "Bad data:\n"
				"EXPECTED (%zu):\n\n%*s\n\n"
				"GOT (%zu):\n\n%*s\n",
				YAML_LEN(ref), YAML_LEN(ref), ref,
				len, len, buffer);
	}

	return ttest_pass(&tc);
}

/**
 * Test saving a string.
 *
 * \param[in]  report  The test report context.
 * \param[in]  config  The CYAML config to use for the test.
 * \return true if test passes, false otherwise.
 */
static bool test_save_mapping_entry_string(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	static const unsigned char ref[] =
		"---\n"
		"test_string: This is a test, of sorts.\n"
		"...\n";
	static const struct target_struct {
		char test_string[32];
	} data = {
		.test_string = "This is a test, of sorts.",
	};
	static const struct cyaml_schema_field mapping_schema[] = {
		CYAML_FIELD_STRING("test_string", CYAML_FLAG_DEFAULT,
				struct target_struct, test_string, 0),
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

	if (len != YAML_LEN(ref) || memcmp(ref, buffer, len) != 0) {
		return ttest_fail(&tc, "Bad data:\n"
				"EXPECTED (%zu):\n\n%*s\n\n"
				"GOT (%zu):\n\n%*s\n",
				YAML_LEN(ref), YAML_LEN(ref), ref,
				len, len, buffer);
	}

	return ttest_pass(&tc);
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

	if (len != YAML_LEN(ref) || memcmp(ref, buffer, len) != 0) {
		return ttest_fail(&tc, "Bad data:\n"
				"EXPECTED (%zu):\n\n%*s\n\n"
				"GOT (%zu):\n\n%*s\n",
				YAML_LEN(ref), YAML_LEN(ref), ref,
				len, len, buffer);
	}

	return ttest_pass(&tc);
}

/**
 * Test saving a negative signed integer.
 *
 * \param[in]  report  The test report context.
 * \param[in]  config  The CYAML config to use for the test.
 * \return true if test passes, false otherwise.
 */
static bool test_save_mapping_entry_int_neg(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	static const unsigned char ref[] =
		"---\n"
		"test_int: -14\n"
		"...\n";
	static const struct target_struct {
		int test_int;
	} data = {
		.test_int = -14,
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

	if (len != YAML_LEN(ref) || memcmp(ref, buffer, len) != 0) {
		return ttest_fail(&tc, "Bad data:\n"
				"EXPECTED (%zu):\n\n%*s\n\n"
				"GOT (%zu):\n\n%*s\n",
				YAML_LEN(ref), YAML_LEN(ref), ref,
				len, len, buffer);
	}

	return ttest_pass(&tc);
}

/**
 * Test saving a boolean.
 *
 * \param[in]  report  The test report context.
 * \param[in]  config  The CYAML config to use for the test.
 * \return true if test passes, false otherwise.
 */
static bool test_save_mapping_entry_bool_true(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	static const unsigned char ref[] =
		"---\n"
		"test_bool: true\n"
		"...\n";
	static const struct target_struct {
		bool test_bool;
	} data = {
		.test_bool = true,
	};
	static const struct cyaml_schema_field mapping_schema[] = {
		CYAML_FIELD_BOOL("test_bool", CYAML_FLAG_DEFAULT,
				struct target_struct, test_bool),
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

	if (len != YAML_LEN(ref) || memcmp(ref, buffer, len) != 0) {
		return ttest_fail(&tc, "Bad data:\n"
				"EXPECTED (%zu):\n\n%*s\n\n"
				"GOT (%zu):\n\n%*s\n",
				YAML_LEN(ref), YAML_LEN(ref), ref,
				len, len, buffer);
	}

	return ttest_pass(&tc);
}

/**
 * Test saving a boolean.
 *
 * \param[in]  report  The test report context.
 * \param[in]  config  The CYAML config to use for the test.
 * \return true if test passes, false otherwise.
 */
static bool test_save_mapping_entry_bool_false(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	static const unsigned char ref[] =
		"---\n"
		"test_bool: false\n"
		"...\n";
	static const struct target_struct {
		bool test_bool;
	} data = {
		.test_bool = false,
	};
	static const struct cyaml_schema_field mapping_schema[] = {
		CYAML_FIELD_BOOL("test_bool", CYAML_FLAG_DEFAULT,
				struct target_struct, test_bool),
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

	if (len != YAML_LEN(ref) || memcmp(ref, buffer, len) != 0) {
		return ttest_fail(&tc, "Bad data:\n"
				"EXPECTED (%zu):\n\n%*s\n\n"
				"GOT (%zu):\n\n%*s\n",
				YAML_LEN(ref), YAML_LEN(ref), ref,
				len, len, buffer);
	}

	return ttest_pass(&tc);
}

/**
 * Test saving a string pointer.
 *
 * \param[in]  report  The test report context.
 * \param[in]  config  The CYAML config to use for the test.
 * \return true if test passes, false otherwise.
 */
static bool test_save_mapping_entry_string_ptr(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	static const unsigned char ref[] =
		"---\n"
		"test_string: This is a test, of sorts.\n"
		"...\n";
	static const struct target_struct {
		const char *test_string;
	} data = {
		.test_string = "This is a test, of sorts.",
	};
	static const struct cyaml_schema_field mapping_schema[] = {
		CYAML_FIELD_STRING_PTR("test_string", CYAML_FLAG_POINTER,
				struct target_struct, test_string,
				0, CYAML_UNLIMITED),
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

	if (len != YAML_LEN(ref) || memcmp(ref, buffer, len) != 0) {
		return ttest_fail(&tc, "Bad data:\n"
				"EXPECTED (%zu):\n\n%*s\n\n"
				"GOT (%zu):\n\n%*s\n",
				YAML_LEN(ref), YAML_LEN(ref), ref,
				len, len, buffer);
	}

	return ttest_pass(&tc);
}

/**
 * Test saving a strict enum.
 *
 * \param[in]  report  The test report context.
 * \param[in]  config  The CYAML config to use for the test.
 * \return true if test passes, false otherwise.
 */
static bool test_save_mapping_entry_enum_strict(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	enum test_e {
		FIRST, SECOND, THIRD, FOURTH, COUNT
	};
	static const char *strings[COUNT] = {
		[FIRST]  = "first",
		[SECOND] = "second",
		[THIRD]  = "third",
		[FOURTH] = "fourth"
	};
	static const unsigned char ref[] =
		"---\n"
		"test_enum: third\n"
		"...\n";
	static const struct target_struct {
		enum test_e test_enum;
	} data = {
		.test_enum = THIRD,
	};
	static const struct cyaml_schema_field mapping_schema[] = {
		CYAML_FIELD_ENUM("test_enum", CYAML_FLAG_STRICT,
				struct target_struct, test_enum, strings, 4),
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

	if (len != YAML_LEN(ref) || memcmp(ref, buffer, len) != 0) {
		return ttest_fail(&tc, "Bad data:\n"
				"EXPECTED (%zu):\n\n%*s\n\n"
				"GOT (%zu):\n\n%*s\n",
				YAML_LEN(ref), YAML_LEN(ref), ref,
				len, len, buffer);
	}

	return ttest_pass(&tc);
}

/**
 * Test saving a numerical enum.
 *
 * \param[in]  report  The test report context.
 * \param[in]  config  The CYAML config to use for the test.
 * \return true if test passes, false otherwise.
 */
static bool test_save_mapping_entry_enum_number(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	enum test_e {
		FIRST, SECOND, THIRD, FOURTH
	};
	static const char *strings[] = {
		"first", "second", "third", "fourth"
	};
	static const unsigned char ref[] =
		"---\n"
		"test_enum: 99\n"
		"...\n";
	static const struct target_struct {
		enum test_e test_enum;
	} data = {
		.test_enum = 99,
	};
	static const struct cyaml_schema_field mapping_schema[] = {
		CYAML_FIELD_ENUM("test_enum", CYAML_FLAG_DEFAULT,
				struct target_struct, test_enum, strings, 4),
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

	if (len != YAML_LEN(ref) || memcmp(ref, buffer, len) != 0) {
		return ttest_fail(&tc, "Bad data:\n"
				"EXPECTED (%zu):\n\n%*s\n\n"
				"GOT (%zu):\n\n%*s\n",
				YAML_LEN(ref), YAML_LEN(ref), ref,
				len, len, buffer);
	}

	return ttest_pass(&tc);
}

/**
 * Test saving a mapping.
 *
 * \param[in]  report  The test report context.
 * \param[in]  config  The CYAML config to use for the test.
 * \return true if test passes, false otherwise.
 */
static bool test_save_mapping_entry_mapping(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	struct value_s {
		short a;
		long b;
	};
	static const unsigned char ref[] =
		"---\n"
		"mapping:\n"
		"  a: 123\n"
		"  b: 9999\n"
		"...\n";
	static const struct target_struct {
		struct value_s test_mapping;
	} data = {
		.test_mapping = {
			.a = 123,
			.b = 9999
		},
	};
	static const struct cyaml_schema_field test_mapping_schema[] = {
		CYAML_FIELD_INT("a", CYAML_FLAG_DEFAULT, struct value_s, a),
		CYAML_FIELD_INT("b", CYAML_FLAG_DEFAULT, struct value_s, b),
		CYAML_FIELD_END
	};
	static const struct cyaml_schema_field mapping_schema[] = {
		CYAML_FIELD_MAPPING("mapping", CYAML_FLAG_DEFAULT,
				struct target_struct, test_mapping,
				test_mapping_schema),
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

	if (len != YAML_LEN(ref) || memcmp(ref, buffer, len) != 0) {
		return ttest_fail(&tc, "Bad data:\n"
				"EXPECTED (%zu):\n\n%*s\n\n"
				"GOT (%zu):\n\n%*s\n",
				YAML_LEN(ref), YAML_LEN(ref), ref,
				len, len, buffer);
	}

	return ttest_pass(&tc);
}

/**
 * Test saving a mapping pointer.
 *
 * \param[in]  report  The test report context.
 * \param[in]  config  The CYAML config to use for the test.
 * \return true if test passes, false otherwise.
 */
static bool test_save_mapping_entry_mapping_ptr(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	struct value_s {
		short a;
		long b;
	} value = {
		.a = 123,
		.b = 9999
	};
	static const unsigned char ref[] =
		"---\n"
		"mapping:\n"
		"  a: 123\n"
		"  b: 9999\n"
		"...\n";
	const struct target_struct {
		struct value_s *test_mapping;
	} data = {
		.test_mapping = &value,
	};
	static const struct cyaml_schema_field test_mapping_schema[] = {
		CYAML_FIELD_INT("a", CYAML_FLAG_DEFAULT, struct value_s, a),
		CYAML_FIELD_INT("b", CYAML_FLAG_DEFAULT, struct value_s, b),
		CYAML_FIELD_END
	};
	static const struct cyaml_schema_field mapping_schema[] = {
		CYAML_FIELD_MAPPING_PTR("mapping", CYAML_FLAG_POINTER,
				struct target_struct, test_mapping,
				test_mapping_schema),
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

	if (len != YAML_LEN(ref) || memcmp(ref, buffer, len) != 0) {
		return ttest_fail(&tc, "Bad data:\n"
				"EXPECTED (%zu):\n\n%*s\n\n"
				"GOT (%zu):\n\n%*s\n",
				YAML_LEN(ref), YAML_LEN(ref), ref,
				len, len, buffer);
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

	pass &= test_save_mapping_entry_uint(rc, &config);
	pass &= test_save_mapping_entry_float(rc, &config);
	pass &= test_save_mapping_entry_double(rc, &config);
	pass &= test_save_mapping_entry_string(rc, &config);
	pass &= test_save_mapping_entry_int_pos(rc, &config);
	pass &= test_save_mapping_entry_int_neg(rc, &config);
	pass &= test_save_mapping_entry_bool_true(rc, &config);
	pass &= test_save_mapping_entry_bool_false(rc, &config);
	pass &= test_save_mapping_entry_string_ptr(rc, &config);
	pass &= test_save_mapping_entry_enum_strict(rc, &config);
	pass &= test_save_mapping_entry_enum_number(rc, &config);

	ttest_heading(rc, "Save single entry mapping tests: complex types");

	pass &= test_save_mapping_entry_mapping(rc, &config);
	pass &= test_save_mapping_entry_mapping_ptr(rc, &config);

	return pass;
}
