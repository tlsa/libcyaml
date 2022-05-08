#include <stdbool.h>
#include <assert.h>

#include "data.h"

static void cyaml__update_config(
	const cyaml_config_t *config,
	cyaml_config_t *config_copy)
{
	*config_copy = *config;
	config_copy->flags |= CYAML_CFG_EXTENDED;
}

cyaml_err_t cyaml_load_file_ex(
		const char *path,
		const cyaml_config_t *config,
		const cyaml_schema_value_t *schema,
		cyaml_data_t **data_out,
		unsigned *seq_count_out)
{
	cyaml_config_t cfg_copy;

	cyaml__update_config(config, &cfg_copy);
	return cyaml_load_file(path, &cfg_copy, schema, data_out,
			seq_count_out);
}

cyaml_err_t cyaml_load_data_ex(
		const uint8_t *input,
		size_t input_len,
		const cyaml_config_t *config,
		const cyaml_schema_value_t *schema,
		cyaml_data_t **data_out,
		unsigned *seq_count_out)
{
	cyaml_config_t cfg_copy;

	cyaml__update_config(config, &cfg_copy);
	return cyaml_load_data(input, input_len, &cfg_copy, schema, data_out,
			seq_count_out);
}

cyaml_err_t cyaml_save_file_ex(
		const char *path,
		const cyaml_config_t *config,
		const cyaml_schema_value_t *schema,
		const cyaml_data_t *data,
		unsigned seq_count)
{
	cyaml_config_t cfg_copy;

	cyaml__update_config(config, &cfg_copy);
	return cyaml_save_file(path, &cfg_copy, schema, data, seq_count);
}

cyaml_err_t cyaml_save_data_ex(
		char **output,
		size_t *len,
		const cyaml_config_t *config,
		const cyaml_schema_value_t *schema,
		const cyaml_data_t *data,
		unsigned seq_count)
{
	cyaml_config_t cfg_copy;

	cyaml__update_config(config, &cfg_copy);
	return cyaml_save_data(output, len, &cfg_copy, schema, data, seq_count);
}

cyaml_err_t cyaml_free_ex(
		const cyaml_config_t *config,
		const cyaml_schema_value_t *schema,
		cyaml_data_t *data,
		unsigned seq_count)
{
	cyaml_config_t cfg_copy;

	cyaml__update_config(config, &cfg_copy);
	return cyaml_free(&cfg_copy, schema, data, seq_count);
}
