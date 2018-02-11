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

	if (data_tgt->test_value_flags != value) {
		return ttest_fail(&tc, "Incorrect value: "
				"expected: 0x%x, got: 0x%x\n",
				value, data_tgt->test_value_flags);
	}

	return ttest_pass(&tc);
}

static bool test_load_mapping_entry_mapping(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	UNUSED(config);
	ttest_ctx_t tc = ttest_start(report, __func__, NULL, NULL);
	return ttest_todo(&tc);
}

static bool test_load_mapping_entry_mapping_ptr(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	UNUSED(config);
	ttest_ctx_t tc = ttest_start(report, __func__, NULL, NULL);
	return ttest_todo(&tc);
}

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

	ttest_heading(rc, "Load single entry mapping tests: complex types");

	pass &= test_load_mapping_entry_flags(rc, &config);
	pass &= test_load_mapping_entry_mapping(rc, &config);
	pass &= test_load_mapping_entry_mapping_ptr(rc, &config);

	ttest_heading(rc, "Load single entry mapping tests: sequences");

	pass &= test_load_mapping_entry_sequence_int(rc, &config);
	pass &= test_load_mapping_entry_sequence_enum(rc, &config);
	pass &= test_load_mapping_entry_sequence_uint(rc, &config);
	pass &= test_load_mapping_entry_sequence_bool(rc, &config);
	pass &= test_load_mapping_entry_sequence_sequence_fixed_int(rc, &config);
	pass &= test_load_mapping_entry_sequence_sequence_fixed_ptr_int(rc, &config);
	pass &= test_load_mapping_entry_sequence_sequence_fixed_flat_int(rc, &config);

	ttest_heading(rc, "Load single entry mapping tests: ptr sequences");

	pass &= test_load_mapping_entry_sequence_ptr_int(rc, &config);
	pass &= test_load_mapping_entry_sequence_ptr_enum(rc, &config);
	pass &= test_load_mapping_entry_sequence_ptr_uint(rc, &config);
	pass &= test_load_mapping_entry_sequence_ptr_bool(rc, &config);

	return pass;
}
