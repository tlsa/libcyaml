test_sources = files('free.c', 'load.c', 'test.c', 'util.c','errs.c', 'file.c',
		     'save.c', 'utf8.c')
test_units = [{
	'name': 'utf-8_codepoint_composition',
	'test_list': [
		{'prefix': 'test_utf8_get_codepoint_', 'range': range(0,5)}
	]
},{
	'name': 'utf-8_string_comparison',
	'test_list': [
		{'prefix': 'test_utf8_strcmp_same_', 'range': range(0,12)},
		{'prefix': 'test_utf8_strcmp_matches_', 'range': range(0,51)},
		{'prefix': 'test_utf8_strcmp_mismatches_', 'range': range(0,8)}
	]
},{
	'name': 'memory_utility_functions',
	'test_list': [
		{'name': 'test_util_strdup'},
		{'name': 'test_util_strdup_len'},
		{'name': 'test_util_memory_funcs'}
	]
},{
	'name': 'error_code_tests',
	'test_list': [
		{'name': 'test_util_state_invalid'},
		{'name': 'test_util_err_success_zero'},
		{'name': 'test_util_err_strings_valid'},
		{'name': 'test_util_err_strings_invalid'}
	]
},{
	'name': 'free_tests',
	'test_list': [
		{'prefix': 'test_free_null_',
		 'enum': ['data','mem_fn','config','schema']}
	]
},{
	'name': 'load_single_entry_mapping_tests_simple_types',
	'test_list': [
		{'prefix': 'test_load_mapping_entry_',
		 'enum': [
			'enum','uint','float','double','string','int_pos',
			'int_neg','enum_ptr','uint_ptr','float_ptr','bool_true',
			'bool_false','double_ptr','string_ptr','int_pos_ptr',
			'int_neg_ptr','enum_sparse','ignore_deep','ignore_scalar',
			'enum_fallback','bool_true_ptr','bool_false_ptr',
			'float_underflow','double_underflow','string_ptr_empty',
			'string_ptr_null_str','string_ptr_null_empty'
		]}
	]
},{
	'name': 'load_single_entry_mapping_tests_complex_types',
	'test_list': [
		{'prefix': 'test_load_mapping_entry_',
		 'enum': [
			'flags','mapping','bitfield','flags_ptr','mapping_ptr',
			'flags_empty','bitfield_ptr','flags_sparse'
		]}
	]
},{
	'name': 'load_single_entry_mapping_tests_sequences',
	'test_list': [
		{'prefix': 'test_load_mapping_entry_sequence_',
		 'enum': [
			'int','enum','uint','bool','flags','string','mapping',
			'string_ptr','mapping_ptr','sequence_fixed_int',
			'sequence_fixed_ptr_int','sequence_fixed_flat_int'
		]}
	]
},{
	'name': 'load_single_entry_mapping_tests_ptr_sequences',
	'test_list': [
		{'prefix': 'test_load_mapping_entry_sequence_ptr_',
		 'enum': [
			'int','enum','uint','bool','flags','string','mapping',
			'string_ptr','mapping_ptr','sequence_fixed_int',
			'sequence_fixed_ptr_int','sequence_fixed_flat_int'
		]}
	]
},{
	'name': 'load_tests_ptr_sequence_with_null_values',
	'test_list': [
		{'name': 'test_load_sequence_null_values_int'},
		{'prefix': 'test_load_sequence_null_str_values_',
		 'enum': ['int','uint','mapping']}
	]
},{
	'name': 'load_tests_sequence_count_sizes',
	'test_list': [
		{'prefix': 'test_load_mapping_entry_sequence_count_',
		 'range': range(0,9)}
	]
},{
	'name': 'load_tests_various',
	'test_list': [
		{'prefix': 'test_load_schema_top_level_',
		 'enum': ['scalar','string','sequence','sequence_fixed']},
		{'prefix': 'test_load_mapping_', 'enum': [
			'without_any_fields','with_multiple_fields',
			'with_optional_fields','only_optional_fields',
			'ignored_unknown_keys'
		]},
		{'prefix': 'test_load_', 'enum': [
			'no_log','duplicate_ignored','multiple_documents_ignored',
			'sequence_without_max_entries',
			'schema_sequence_entry_count_member'
		]}
	]
},{
	'name': 'load_tests_case_sensitivity',
	'test_list': [
		{'name': 'test_load_enum_insensitive'},
		{'name': 'test_load_flags_insensitive'},
		{'prefix': 'test_load_mapping_fields_cfg_insensitive_',
		 'range': range(1,4)},
		{'prefix': 'test_load_mapping_fields_value_',
		 'enum': ['sensitive_1','insensitive_1']}
	]
},{
	'name': 'load_tests_anchors_and_aliases_scalars',
	'test_list': [
		{'name': 'test_load_unused_anchor'},
		{'prefix': 'test_load_anchor_scalar_', 'enum': ['int','string']},
		{'name': 'test_load_anchor_multiple_scalars'}
	]
},{
	'name': 'load_tests_anchors_and_aliases_non_scalars',
	'test_list': [
		{'prefix': 'test_load_anchor_', 'enum': [
			'mapping','sequence','deep_mapping_sequence'
		]}
	]
},{
	'name': 'load_tests_anchors_and_aliases_edge_cases',
	'test_list': [{'name': 'test_load_anchor_updated_anchor'}]
},{
	'name': 'bad_parameter_tests',
	'test_list': [
		{'prefix': 'test_err_load_null_',
		 'enum': ['data','config','mem_fn','schema']},
		{'prefix': 'test_err_save_null_',
		 'enum': ['data','config','mem_fn','schema']},
		{'prefix': 'test_err_load_schema_top_level_',
		 'enum': ['non_pointer','sequence_no_count','not_sequence_count']},
		{'prefix': 'test_err_save_schema_top_level_',
		 'enum': ['non_pointer','not_sequence_count']}
	]
},{
	'name': 'bad_schema_tests',
	'test_list': [
		{'prefix': 'test_err_load_schema_bad_', 'enum': ['type','bitfield']},
		{'prefix': 'test_err_save_schema_bad_', 'enum': ['type','bitfield']},
		{'name': 'test_err_load_schema_string_min_max'},
		{'prefix': 'test_err_load_schema_bad_data_size_',
		 'range': range(1,9)},
		{'prefix': 'test_err_load_schema_sequence_',
		 'enum': ['min_max','in_sequence']},
		{'prefix': 'test_err_save_schema_sequence_',
		 'enum': ['min_max','in_sequence']},
		{'name': 'test_err_load_schema_bad_data_size_float'}
	]
},{
	'name': 'yaml_schema_mismatch_bad_values',
	'test_list': [
		{'name': 'test_err_load_non_scalar_mapping_key'},
		{'prefix': 'test_err_load_schema_invalid_value_', 'enum': [
			'uint','string','null_ptr','float_invalid','double_invalid'
		]},
		{'prefix': 'test_err_load_schema_invalid_value_flags_',
		 'range': range(1,4)},
		{'prefix': 'test_err_load_schema_invalid_value_bitfield_',
		 'range': range(1,7)},
		{'prefix': 'test_err_load_schema_invalid_value_int_range_',
		 'range': range(1,6)},
		{'prefix': 'test_err_load_schema_invalid_value_uint_range_',
		 'range': range(1,6)},
		{'prefix': 'test_err_load_schema_invalid_value_float_range',
		 'range': range(1,5)},
		{'prefix': 'test_err_load_schema_invalid_value_double_range',
		 'range': range(1,3)}
	]
},{
	'name': 'yaml_schema_mismatch_string_lengths',
	'test_list': [
		{'prefix': 'test_err_load_schema_string_',
		 'enum': ['min_length','max_length']}
	]
},{
	'name': 'yaml_schema_mismatch_mapping_fields',
	'test_list': [
		{'prefix': 'test_err_load_schema_', 'enum': [
			'missing_mapping_field','unknown_mapping_field',
			'duplicate_mapping_field'
		]}
	]
},{
	'name': 'yaml_schema_mismatch_sequence_counts',
	'test_list': [
		{'prefix': 'test_err_load_schema_sequence_', 'enum': [
			'min_entries','max_entries'
		]}
	]
},{
	'name': 'yaml_schema_mismatch_bad_flags_bad_enum_strings',
	'test_list': [
		{'prefix': 'test_err_load_schema_', 'enum': [
			'flags_mapping','enum_bad_string','flags_bad_string',
			'strict_enum_bad_string','strict_flags_bad_string'
		]},
		{'prefix': 'test_err_save_schema_', 'enum': [
			'strict_enum_bad_value','strict_flags_bad_value'
		]}
	]
},{
	'name': 'yaml_schema_mismatch_expected_value_type_tests',
	'test_list': [
		{'prefix': 'test_err_load_schema_expect_', 'enum': [
			'int_read_seq','int_read_end_1','int_read_end_2',
			'flags_read_scalar','mapping_read_scalar',
			'sequence_read_scalar'
		]}
	]
},{
	'name': 'memory_allocation_handling_tests',
	'test_list': [
		{'prefix': 'test_err_', 'enum': ['load_log','free_null']},
		{'prefix': 'test_err_load_alloc_oom_', 'range': range(1,3)},
		{'prefix': 'test_err_save_alloc_oom_', 'range': range(1,3)}
	]
},{
	'name': 'alias_tests',
	'test_list': [
		{'prefix': 'test_err_load_', 'enum': [
			'invalid_alias','incomplete_alias','flag_value_alias',
			'mapping_key_alias'
		]},
		{'prefix': 'test_err_load_mapping_value_alias_', 'range': range(1,4)},
		{'prefix': 'test_err_load_bitfield_value_alias_', 'range': range(1,3)}
	]
},{
	'name': 'file_loading_tests',
	'test_list': [
		{'prefix': 'test_file_load_', 'enum': [
			'basic','save_basic','bad_path','basic_invalid'
		]},
		{'prefix': 'test_file_save_', 'enum': ['bad_path','basic_invalid']}
	]
},{
	'name': 'save_single_entry_mapping_tests_simple_types',
	'test_list': [
		{'prefix': 'test_save_mapping_entry_', 'enum': [
			'uint','float','double','string','int_64','int_pos',
			'int_neg','bool_true','bool_false','string_ptr',
			'enum_strict','enum_number','enum_sparse'
		]}
	]
},{
	'name': 'save_single_entry_mapping_tests_scalar_types',
	'test_list': [
		{'prefix': 'test_save_mapping_entry_string_ptr_', 'enum': [
			'plain','single','double','folded','literal'
		]}
	]
},{
	'name': 'save_single_entry_mapping_tests_complex_types',
	'test_list': [
		{'prefix': 'test_save_mapping_entry_', 'enum': [
			'mapping','bitfield','mapping_ptr','flags_strict',
			'flags_number','flags_sparse','bitfield_sparse'
		]}
	]
},{
	'name': 'save_single_entry_mapping_tests_sequences',
	'test_list': [
		{'prefix': 'test_save_mapping_entry_sequence_', 'enum': [
			'int','uint','enum','bool','flags','string','mapping',
			'string_ptr','mapping_ptr','sequence_fixed_int',
			'sequence_fixed_ptr_int','sequence_fixed_flat_int'
		]}
	]
},{
	'name': 'save_single_entry_mapping_tests_ptr_sequences',
	'test_list': [
		{'prefix': 'test_save_mapping_entry_sequence_ptr_', 'enum': [
			'int','enum','uint','bool','flags','string','mapping',
			'string_ptr','mapping_ptr','sequence_fixed_int',
			'sequence_fixed_ptr_int','sequence_fixed_flat_int'
		]}
	]
},{
	'name': 'save_tests_ptr_sequence_with_null_values',
	'test_list': [
		{'prefix': 'test_save_sequence_null_',
		 'enum': ['values_int','str_values_int']}
	]
},{
	'name': 'save_tests_optional_mapping_fields',
	'test_list': [
		{'prefix': 'test_save_mapping_entry_optional_',
		 'enum': ['uint','uint_ptr','uint_ptr_null']}
	]
},{
	'name': 'save_tests_various',
	'test_list': [
		{'prefix': 'test_save_mapping_', 'enum': [
			'entry_ignored','value_flow_style','value_block_style'
		]},
		{'prefix': 'test_save_sequence_config_', 'enum': [
			'flow_style','block_style'
		]},
		{'prefix': 'test_save_', 'enum': [
			'no_document_delimiters','schema_top_level_sequence_fixed'
		]}
	]
}]
