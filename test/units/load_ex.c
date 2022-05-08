#include <inttypes.h>
#include <stdbool.h>
#include <assert.h>

#include <cyaml/cyaml_ex.h>

#include "../../src/data.h"
#include "ttest.h"
#include "test.h"

/**
 * Unit test context data.
 */
typedef struct test_data {
	cyaml_data_t **data;
	unsigned *seq_count;
	const struct cyaml_config *config;
	const struct cyaml_schema_value *schema;
} test_data_t;

/** Helper macro to count bytes of YAML input data. */
#define YAML_LEN(_y) (sizeof(_y) - 1)

/**
 * Common clean up function to free data loaded by tests.
 *
 * \param[in]  data  The unit test context data.
 */
static void cyaml_cleanup(void *data)
{
	struct test_data *td = data;
	unsigned seq_count = 0;

	if (td->seq_count != NULL) {
		seq_count = *(td->seq_count);
	}

	if (td->data != NULL) {
		cyaml_free(td->config, td->schema, *(td->data), seq_count);
	}
}

/**
 * Test loading a positive signed integer.
 *
 * \param[in]  report  The test report context.
 * \param[in]  config  The CYAML config to use for the test.
 * \return true if test passes, false otherwise.
 */
static bool test_load_ex_mapping_entry_int_pos(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	static const int value = 90;
	static const unsigned char yaml[] =
		"test_int: 90\n";
	struct target_struct {
		int test_value_int;
	} *data_tgt = NULL;
	static const struct cyaml_schema_field_ex mapping_schema[] = {
		CYAML_FIELD_INT_EX("test_int", CYAML_FLAG_DEFAULT,
				struct target_struct, test_value_int),
		CYAML_FIELD_END_EX
	};
	static const struct cyaml_schema_value top_schema = {
		CYAML_VALUE_MAPPING_EX(CYAML_FLAG_POINTER,
				struct target_struct, mapping_schema),
	};
	test_data_t td = {
		.data = (cyaml_data_t **) &data_tgt,
		.config = config,
		.schema = &top_schema,
	};
	cyaml_err_t err;
	ttest_ctx_t tc;

	if (!ttest_start(report, __func__, cyaml_cleanup, &td, &tc)) {
		return true;
	}

	err = cyaml_load_data_ex(yaml, YAML_LEN(yaml), config, &top_schema,
			(cyaml_data_t **) &data_tgt, NULL);
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (data_tgt->test_value_int != value) {
		return ttest_fail(&tc, "Incorrect value");
	}

	return ttest_pass(&tc);
}

/**
 * Test default of a positive signed integer.
 *
 * \param[in]  report  The test report context.
 * \param[in]  config  The CYAML config to use for the test.
 * \return true if test passes, false otherwise.
 */
static bool test_load_ex_mapping_entry_int_pos_default(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	static const int value = 90, default_value = 50;
	static const unsigned char yaml[] =
		"test_int: 90\n";
	struct target_struct {
		int test_value_int;
		int test_value_here;
	} *data_tgt = NULL;
	static const struct cyaml_schema_field_ex mapping_schema[] = {
		CYAML_FIELD_INT_DEFAULT(default_value, "not_here",
				CYAML_FLAG_DEFAULT, struct target_struct,
				test_value_here),
		CYAML_FIELD_INT_EX("test_int", CYAML_FLAG_DEFAULT,
				struct target_struct, test_value_int),
		CYAML_FIELD_END_EX
	};
	static const struct cyaml_schema_value top_schema = {
		CYAML_VALUE_MAPPING_EX(CYAML_FLAG_POINTER,
				struct target_struct, mapping_schema),
	};
	test_data_t td = {
		.data = (cyaml_data_t **) &data_tgt,
		.config = config,
		.schema = &top_schema,
	};
	cyaml_err_t err;
	ttest_ctx_t tc;

	if (!ttest_start(report, __func__, cyaml_cleanup, &td, &tc)) {
		return true;
	}

	err = cyaml_load_data_ex(yaml, YAML_LEN(yaml), config, &top_schema,
			(cyaml_data_t **) &data_tgt, NULL);
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (data_tgt->test_value_int != value) {
		return ttest_fail(&tc, "Incorrect value");
	}
	if (data_tgt->test_value_here != default_value) {
		return ttest_fail(&tc, "Incorrect value");
	}

	return ttest_pass(&tc);
}

/**
 * Test default for an enumeration.
 *
 * \param[in]  report  The test report context.
 * \param[in]  config  The CYAML config to use for the test.
 * \return true if test passes, false otherwise.
 */
static bool test_load_ex_mapping_entry_enum_default(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	static const enum test_enum {
		TEST_ENUM_FIRST,
		TEST_ENUM_SECOND,
		TEST_ENUM_THIRD,
		TEST_ENUM__COUNT,
	} value = TEST_ENUM_SECOND, default_value = TEST_ENUM_THIRD;
	static const cyaml_strval_t strings[TEST_ENUM__COUNT] = {
		[TEST_ENUM_FIRST]  = { "first",  0 },
		[TEST_ENUM_SECOND] = { "second", 1 },
		[TEST_ENUM_THIRD]  = { "third",  2 },
	};
	static const unsigned char yaml[] =
		"test_enum: second\n";
	struct target_struct {
		enum test_enum test_value_enum;
		enum test_enum default_value_enum;
	} *data_tgt = NULL;
	static const struct cyaml_schema_field_ex mapping_schema[] = {
		CYAML_FIELD_ENUM_EX("test_enum", CYAML_FLAG_DEFAULT,
				struct target_struct, test_value_enum,
				strings, TEST_ENUM__COUNT),
		CYAML_FIELD_ENUM_DEFAULT(default_value, "test_def_enum",
				CYAML_FLAG_DEFAULT, struct target_struct,
				default_value_enum, strings, TEST_ENUM__COUNT),
		CYAML_FIELD_END_EX
	};
	static const struct cyaml_schema_value top_schema = {
		CYAML_VALUE_MAPPING_EX(CYAML_FLAG_POINTER,
				struct target_struct, mapping_schema),
	};
	test_data_t td = {
		.data = (cyaml_data_t **) &data_tgt,
		.config = config,
		.schema = &top_schema,
	};
	cyaml_err_t err;
	ttest_ctx_t tc;

	if (!ttest_start(report, __func__, cyaml_cleanup, &td, &tc)) {
		return true;
	}

	err = cyaml_load_data_ex(yaml, YAML_LEN(yaml), config, &top_schema,
			(cyaml_data_t **) &data_tgt, NULL);
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (data_tgt->test_value_enum != value) {
		return ttest_fail(&tc, "Incorrect value");
	}
	if (data_tgt->default_value_enum != default_value) {
		return ttest_fail(&tc, "Incorrect value");
	}

	return ttest_pass(&tc);
}

/**
 * Test default floating point value.
 *
 * \param[in]  report  The test report context.
 * \param[in]  config  The CYAML config to use for the test.
 * \return true if test passes, false otherwise.
 */
static bool test_load_ex_mapping_entry_float_default(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	static const float value = 3.14159f, def_value = 2.71828f;
	static const unsigned char yaml[] =
		"test_fp: 3.14159\n";
	struct target_struct {
		float test_value_fp;
		float test_def_value_fp;
	} *data_tgt = NULL;
	static const struct cyaml_schema_field_ex mapping_schema[] = {
		CYAML_FIELD_FLOAT_EX("test_fp", CYAML_FLAG_DEFAULT,
				struct target_struct, test_value_fp),
		CYAML_FIELD_FLOAT_DEFAULT(def_value, "def_test_fp",
				CYAML_FLAG_DEFAULT, struct target_struct,
				test_def_value_fp),
		CYAML_FIELD_END_EX
	};
	static const struct cyaml_schema_value top_schema = {
		CYAML_VALUE_MAPPING_EX(CYAML_FLAG_POINTER,
				struct target_struct, mapping_schema),
	};
	test_data_t td = {
		.data = (cyaml_data_t **) &data_tgt,
		.config = config,
		.schema = &top_schema,
	};
	cyaml_err_t err;
	ttest_ctx_t tc;

	if (!ttest_start(report, __func__, cyaml_cleanup, &td, &tc)) {
		return true;
	}

	err = cyaml_load_data_ex(yaml, YAML_LEN(yaml), config, &top_schema,
			(cyaml_data_t **) &data_tgt, NULL);
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (data_tgt->test_value_fp != value) {
		return ttest_fail(&tc, "Incorrect value: "
				"expected: %f, got: %f",
				value, data_tgt->test_value_fp);
	}
	if (data_tgt->test_def_value_fp != def_value) {
		return ttest_fail(&tc, "Incorrect value: "
				"expected: %f, got: %f",
				def_value, data_tgt->test_def_value_fp);
	}

	return ttest_pass(&tc);
}

/**
 * Test default floating point value as a double.
 *
 * \param[in]  report  The test report context.
 * \param[in]  config  The CYAML config to use for the test.
 * \return true if test passes, false otherwise.
 */
static bool test_load_ex_mapping_entry_double_default(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	static const double value = 3.14159, def_value = 2.71828;
	static const unsigned char yaml[] =
		"test_fp: 3.14159\n";
	struct target_struct {
		double test_value_fp;
		double test_def_value_fp;
	} *data_tgt = NULL;
	static const struct cyaml_schema_field_ex mapping_schema[] = {
		CYAML_FIELD_FLOAT_EX("test_fp", CYAML_FLAG_DEFAULT,
				struct target_struct, test_value_fp),
		CYAML_FIELD_DOUBLE_DEFAULT(def_value, "def_test_fp",
				CYAML_FLAG_DEFAULT, struct target_struct,
				test_def_value_fp),
		CYAML_FIELD_END_EX
	};
	static const struct cyaml_schema_value top_schema = {
		CYAML_VALUE_MAPPING_EX(CYAML_FLAG_POINTER,
				struct target_struct, mapping_schema),
	};
	test_data_t td = {
		.data = (cyaml_data_t **) &data_tgt,
		.config = config,
		.schema = &top_schema,
	};
	cyaml_err_t err;
	ttest_ctx_t tc;

	if (!ttest_start(report, __func__, cyaml_cleanup, &td, &tc)) {
		return true;
	}

	err = cyaml_load_data_ex(yaml, YAML_LEN(yaml), config, &top_schema,
			(cyaml_data_t **) &data_tgt, NULL);
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (data_tgt->test_value_fp != value) {
		return ttest_fail(&tc, "Incorrect value: "
				"expected: %lf, got: %lf",
				value, data_tgt->test_value_fp);
	}
	if (data_tgt->test_def_value_fp != def_value) {
		return ttest_fail(&tc, "Incorrect value: "
				"expected: %lf, got: %lf",
				def_value, data_tgt->test_def_value_fp);
	}

	return ttest_pass(&tc);
}

/**
 * Test default of a positive signed integer.
 *
 * \param[in]  report  The test report context.
 * \param[in]  config  The CYAML config to use for the test.
 * \return true if test passes, false otherwise.
 */
static bool test_load_ex_sequence_with_default_int(
		ttest_report_ctx_t *report,
		const cyaml_config_t *config)
{
	static const int default_value = 50;
	static const unsigned char yaml[] =
		"test:\n"
		"- test_int: 100\n"
		"- test_int: 70\n"
		"  default_int: 20\n"
		"- test_int: 90\n";
	struct target_entry {
		int test_value_int;
		int test_value_here;
	};
	struct target_struct {
		struct target_entry *entries;
		uint8_t entries_count;
	};
	struct target_struct *data_tgt = NULL;
	struct target_entry expected[] = {
		{ 100, default_value },
		{ 70, 20 },
		{ 90, default_value },
	};
	static const struct cyaml_schema_field_ex mapping_entry[] = {
		CYAML_FIELD_INT_DEFAULT(default_value, "default_int",
				CYAML_FLAG_DEFAULT, struct target_entry,
				test_value_here),
		CYAML_FIELD_INT_EX("test_int", CYAML_FLAG_DEFAULT,
				struct target_entry, test_value_int),
		CYAML_FIELD_END_EX
	};
	static const struct cyaml_schema_value mapping_entry_schema = {
		CYAML_VALUE_MAPPING_EX(CYAML_FLAG_DEFAULT, struct target_entry,
				mapping_entry),
	};
	static const struct cyaml_schema_field_ex mapping_schema[] = {
		CYAML_FIELD_SEQUENCE_EX("test", CYAML_FLAG_POINTER,
				struct target_struct, entries,
				&mapping_entry_schema, 0, CYAML_UNLIMITED),
	};
	static const struct cyaml_schema_value top_schema = {
		CYAML_VALUE_MAPPING_EX(CYAML_FLAG_POINTER, struct target_struct,
				mapping_schema),
	};
	test_data_t td = {
		.data = (cyaml_data_t **) &data_tgt,
		.config = config,
		.schema = &top_schema,
	};
	cyaml_err_t err;
	ttest_ctx_t tc;

	if (!ttest_start(report, __func__, cyaml_cleanup, &td, &tc)) {
		return true;
	}

	err = cyaml_load_data_ex(yaml, YAML_LEN(yaml), config, &top_schema,
			(cyaml_data_t **) &data_tgt, NULL);
	if (err != CYAML_OK) {
		return ttest_fail(&tc, cyaml_strerror(err));
	}

	if (data_tgt->entries_count != CYAML_ARRAY_LEN(expected)) {
		return ttest_fail(&tc, "Incorrect number of entries");
	}
	for (unsigned i = 0; i < data_tgt->entries_count; i++) {
		if (data_tgt->entries[i].test_value_int !=
		    expected[i].test_value_int) {
			return ttest_fail(&tc, "Incorrect value");
		}
		if (data_tgt->entries[i].test_value_here !=
		    expected[i].test_value_here) {
			return ttest_fail(&tc, "Incorrect value");
		}
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
bool load_ex_tests(
		ttest_report_ctx_t *rc,
		cyaml_log_t log_level,
		cyaml_log_fn_t log_fn)
{
	bool pass = true;
	cyaml_config_t config = {
		.log_fn = log_fn,
		.mem_fn = cyaml_mem,
		.log_level = log_level,
		.flags = CYAML_CFG_DEFAULT | CYAML_CFG_EXTENDED,
	};

	ttest_heading(rc, "Load single entry mapping tests (extended): simple types");

	pass &= test_load_ex_mapping_entry_int_pos(rc, &config);
	pass &= test_load_ex_mapping_entry_int_pos_default(rc, &config);
	pass &= test_load_ex_mapping_entry_enum_default(rc, &config);
	pass &= test_load_ex_mapping_entry_float_default(rc, &config);
	pass &= test_load_ex_mapping_entry_double_default(rc, &config);
	pass &= test_load_ex_sequence_with_default_int(rc, &config);

	return pass;
}
