#ifndef CYAML_EX_H
#define CYAML_EX_H

#include <stdbool.h>

#if defined(CYAML_H) && !defined(KEEP_BOTH_VERSIONS)
#error "cyaml_ex.h must be included before cyaml.h"
#endif

#ifdef KEEP_BOTH_VERSIONS
#include "cyaml.h"
#define cyaml_schema_field_base cyaml_schema_field
#else
#define cyaml_schema_field cyaml_schema_field_base
#define cyaml_schema_field_t cyaml_schema_field_base_t
#include "cyaml.h"
#undef cyaml_schema_field_t
#undef cyaml_schema_field
#define cyaml_schema_field cyaml_schema_field_ex
#define cyaml_schema_field_t cyaml_schema_field_ex_t
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Expanded schema entry for mapping fields.
 *
 * Includes the base mapping field data, and a default value (if provided).
 */
typedef struct cyaml_schema_field_ex {
	/**
	 * The base schema mapping field data
	 */
	struct cyaml_schema_field_base base;
	/**
	 * Set if a default value is provided for this field
	 */
	bool use_default_value;
	/**
	 * The default value (if provided)
	 */
	union {
		uint64_t default_value_u64; /**< An integer default value. */
		float default_value_f; /**< A floating-point default value. */
		double default_value_d; /**< A double-sized floating-point default value. */
	};
} cyaml_schema_field_ex_t;


#define CYAML_FIELD_DEFAULT_NONE \
	.use_default_value = false, \
	.default_value_u64 = 0,

/**
 * Expanded mapping schema helper macro for keys with \ref CYAML_INT type.
 *
 * Similar to \ref CYAML_FIELD_INT_BASE.
 */
#define CYAML_FIELD_INT_EX(...) \
{ \
	.base = CYAML_FIELD_INT_BASE(__VA_ARGS__), \
	CYAML_FIELD_DEFAULT_NONE \
}

/**
 * Mapping schema helper macro for keys with \ref CYAML_INT type with a default
 * value.
 *
 * Use this for integers contained in structs. Based on \ref CYAML_FIELD_INT.
 *
 * \param[in]  _default    Value to be set as a default when loading.
 * \param[in]  _key        String defining the YAML mapping key for this value.
 * \param[in]  _flags      Any behavioural flags relevant to this value.
 * \param[in]  _structure  The structure corresponding to the mapping.
 * \param[in]  _member     The member in _structure for this mapping value.
 */
#define CYAML_FIELD_INT_DEFAULT( \
	_default, _key, _flags, ...) \
{ \
	.base = CYAML_FIELD_INT_BASE(_key, (_flags | CYAML_FLAG_OPTIONAL), \
			__VA_ARGS__), \
	.use_default_value = true, \
	.default_value_u64 = (uint64_t)_default, \
}

/**
 * Expanded mapping schema helper macro for keys with \ref CYAML_INT type.
 *
 * Similar to \ref CYAML_FIELD_INT_PTR.
 */
#define CYAML_FIELD_INT_PTR_EX(...) \
{ \
	.base = CYAML_FIELD_INT_PTR_BASE(__VA_ARGS__), \
	CYAML_FIELD_DEFAULT_NONE \
}

/**
 * Expanded mapping schema helper macro for keys with \ref CYAML_UINT type.
 *
 * Similar to \ref CYAML_FIELD_UINT.
 */
#define CYAML_FIELD_UINT_EX(...) \
{ \
	.base = CYAML_FIELD_UINT_BASE(__VA_ARGS__), \
	CYAML_FIELD_DEFAULT_NONE \
}

/**
 * Value schema helper macro for keys with \ref CYAML_UINT type with a default
 * value.
 *
 * Use this for unsigned integers contained in structs. Based on
 * \ref CYAML_FIELD_UINT
 *
 * \param[in]  _default    Value to be set as a default when loading.
 * \param[in]  _key        String defining the YAML mapping key for this value.
 * \param[in]  _flags      Any behavioural flags relevant to this value.
 * \param[in]  _structure  The structure corresponding to the mapping.
 * \param[in]  _member     The member in _structure for this mapping value.
 */
#define CYAML_FIELD_UINT_DEFAULT( \
		_default, _key, _flags, ...) \
{ \
	.base = CYAML_FIELD_UINT_BASE(_key, (_flags | CYAML_FLAG_OPTIONAL), \
			__VA_ARGS__), \
	.use_default_value = true, \
	.default_value_u64 = (uint64_t)_default, \
}

/**
 * Expanded mapping schema helper macro for keys with \ref CYAML_UINT type.
 *
 * Similar to \ref CYAML_FIELD_UINT_PTR.
 */
#define CYAML_FIELD_UINT_PTR_EX(...) \
{ \
	.base = CYAML_FIELD_UINT_PTR_BASE(__VA_ARGS__), \
	CYAML_FIELD_DEFAULT_NONE \
}

/**
 * Expanded mapping schema helper macro for keys with \ref CYAML_BOOL type.
 *
 * Similar to \ref CYAML_FIELD_BOOL.
 */
#define CYAML_FIELD_BOOL_EX(...) \
{ \
	.base = CYAML_FIELD_BOOL_BASE(__VA_ARGS__), \
	CYAML_FIELD_DEFAULT_NONE \
}

/**
 * Value schema helper macro for keys with \ref CYAML_BOOL type with a default
 * value.
 *
 * Use this for boolean types contained in structs. Based on
 * \ref CYAML_FIELD_BOOL
 *
 * \param[in]  _default    Value to be set as a default when loading.
 * \param[in]  _key        String defining the YAML mapping key for this value.
 * \param[in]  _flags      Any behavioural flags relevant to this value.
 * \param[in]  _structure  The structure corresponding to the mapping.
 * \param[in]  _member     The member in _structure for this mapping value.
 */
#define CYAML_FIELD_BOOL_DEFAULT( \
		_default, _key, _flags, ...) \
{ \
	.base = CYAML_FIELD_BOOL_BASE(_key, (_flags | CYAML_FLAG_OPTIONAL), \
			__VA_ARGS__), \
	.use_default_value = true, \
	.default_value_u64 = (uint64_t)_default, \
}

/**
 * Expanded mapping schema helper macro for keys with \ref CYAML_BOOL type.
 *
 * Similar to \ref CYAML_FIELD_BOOL_PTR.
 */
#define CYAML_FIELD_BOOL_PTR_EX(...) \
{ \
	.base = CYAML_FIELD_BOOL_PTR_BASE(__VA_ARGS__), \
	CYAML_FIELD_DEFAULT_NONE \
}

/**
 * Expanded mapping schema helper macro for keys with \ref CYAML_ENUM type.
 *
 * Similar to \ref CYAML_FIELD_ENUM.
 */
#define CYAML_FIELD_ENUM_EX(...) \
{ \
	.base = CYAML_FIELD_ENUM_BASE(__VA_ARGS__), \
	CYAML_FIELD_DEFAULT_NONE \
}

/**
 * Mapping schema helper macro for keys with \ref CYAML_ENUM type with a default
 * value.
 *
 * Use this for enumerated types contained in structs. Based on
 * \ref CYAML_FIELD_ENUM.
 *
 * \param[in]  _default    Value to be set as a default when loading.
 * \param[in]  _key        String defining the YAML mapping key for this value.
 * \param[in]  _flags      Any behavioural flags relevant to this value.
 * \param[in]  _structure  The structure corresponding to the mapping.
 * \param[in]  _member     The member in _structure for this mapping value.
 * \param[in]  _strings       Array of string data for enumeration values.
 * \param[in]  _strings_count Number of entries in _strings.
 */
#define CYAML_FIELD_ENUM_DEFAULT( \
		_default, _key, _flags, ...) \
{ \
	.base = CYAML_FIELD_ENUM_BASE(_key, (_flags | CYAML_FLAG_OPTIONAL), \
			__VA_ARGS__), \
	.use_default_value = true, \
	.default_value_u64 = (uint64_t)_default, \
}

/**
 * Expanded mapping schema helper macro for keys with \ref CYAML_ENUM type.
 *
 * Similar to \ref CYAML_FIELD_ENUM_PTR.
 */
#define CYAML_FIELD_ENUM_PTR_EX(...) \
{ \
	.base = CYAML_FIELD_ENUM_PTR_BASE(__VA_ARGS__), \
	CYAML_FIELD_DEFAULT_NONE \
}

/**
 * Expanded mapping schema helper macro for keys with \ref CYAML_FLAGS type.
 *
 * Similar to \ref CYAML_FIELD_FLAGS.
 */
#define CYAML_FIELD_FLAGS_EX(...) \
{ \
	.base = CYAML_FIELD_FLAGS_BASE(__VA_ARGS__), \
	CYAML_FIELD_DEFAULT_NONE \
}

/**
 * Mapping schema helper macro for keys with \ref CYAML_FLAGS type with a
 * default value.
 *
 * Use this for flag types contained in structs. Based on
 * \ref CYAML_FIELD_FLAGS.
 *
 * \param[in]  _default    Value to be set as a default when loading.
 * \param[in]  _key        String defining the YAML mapping key for this value.
 * \param[in]  _flags      Any behavioural flags relevant to this value.
 * \param[in]  _structure  The structure corresponding to the mapping.
 * \param[in]  _member     The member in _structure for this mapping value.
 * \param[in]  _strings       Array of string data for flag values.
 * \param[in]  _strings_count Number of entries in _strings.
 */
#define CYAML_FIELD_FLAGS_DEFAULT( \
		_default, _key, _flags, ...) \
{ \
	.base = CYAML_FIELD_FLAGS_BASE(_key, (_flags | CYAML_FLAG_OPTIONAL), \
			__VA_ARGS__), \
	.use_default_value = true, \
	.default_value_u64 = _default, \
}

/**
 * Expanded mapping schema helper macro for keys with \ref CYAML_FLAGS type.
 *
 * Similar to \ref CYAML_FIELD_FLAGS_PTR.
 */
#define CYAML_FIELD_FLAGS_PTR_EX(...) \
{ \
	.base = CYAML_FIELD_FLAGS_PTR_BASE(__VA_ARGS__), \
	CYAML_FIELD_DEFAULT_NONE \
}

/**
 * Expanded mapping schema helper macro for keys with \ref CYAML_BITFIELD type.
 *
 * Similar to \ref CYAML_FIELD_BITFIELD.
 */
#define CYAML_FIELD_BITFIELD_EX(...) \
{ \
	.base = CYAML_FIELD_BITFIELD_BASE(__VA_ARGS__), \
	CYAML_FIELD_DEFAULT_NONE \
}

/**
 * Mapping schema helper macro for keys with \ref CYAML_BITFIELD type with a
 * default value.
 *
 * Use this for bit field types contained in structs. Based on
 * \ref CYAML_FIELD_BITFIELD.
 *
 * \param[in]  _default    Value to be set as a default when loading.
 * \param[in]  _key        String defining the YAML mapping key for this value.
 * \param[in]  _flags      Any behavioural flags relevant to this value.
 * \param[in]  _structure  The structure corresponding to the mapping.
 * \param[in]  _member     The member in _structure for this mapping value.
 * \param[in]  _bitvals       Array of bit field value data for the bit field.
 * \param[in]  _bitvals_count Number of entries in _bitvals.
 */
#define CYAML_FIELD_BITFIELD_DEFAULT( \
		_default, _key, _flags, ...) \
{ \
	.base = CYAML_FIELD_BITFIELD_BASE(_key, \
			(_flags | CYAML_FLAG_OPTIONAL), __VA_ARGS__), \
	.use_default_value = true, \
	.default_value_u64 = (uint64_t)_default, \
}

/**
 * Expanded mapping schema helper macro for keys with \ref CYAML_BITFIELD type.
 *
 * Similar to \ref CYAML_FIELD_BITFIELD_PTR.
 */
#define CYAML_FIELD_BITFIELD_PTR_EX(...) \
{ \
	.base = CYAML_FIELD_BITFIELD_PTR_BASE(__VA_ARGS__), \
	CYAML_FIELD_DEFAULT_NONE \
}

/**
 * Expanded mapping schema helper macro for keys with \ref CYAML_FLOAT type.
 *
 * Similar to \ref CYAML_FIELD_FLOAT.
 */
#define CYAML_FIELD_FLOAT_EX(...) \
{ \
	.base = CYAML_FIELD_FLOAT_BASE(__VA_ARGS__), \
	CYAML_FIELD_DEFAULT_NONE \
}

/**
 * Mapping schema helper macro for keys with \ref CYAML_FLOAT type with a
 * default floating-point value.
 *
 * Use this for floating point types contained in structs. Based on
 * \ref CYAML_FIELD_FLOAT.
 *
 * \param[in]  _default    Value to be set as a default when loading.
 * \param[in]  _key        String defining the YAML mapping key for this value.
 * \param[in]  _flags      Any behavioural flags relevant to this value.
 * \param[in]  _structure  The structure corresponding to the mapping.
 * \param[in]  _member     The member in _structure for this mapping value.
 */
#define CYAML_FIELD_FLOAT_DEFAULT( \
		_default, _key, _flags, _structure, _member) \
{ \
	.base = CYAML_FIELD_FLOAT_BASE(_key, (_flags | CYAML_FLAG_OPTIONAL), \
			_structure, _member), \
	.use_default_value = true, \
	.default_value_f = (float)_default, \
}

/**
 * Mapping schema helper macro for keys with \ref CYAML_FLOAT type with a
 * default double-sized floating-point value.
 *
 * Use this for double-sized floating point types contained in structs. Based on
 * \ref CYAML_FIELD_FLOAT.
 *
 * \param[in]  _default    Value to be set as a default when loading.
 * \param[in]  _key        String defining the YAML mapping key for this value.
 * \param[in]  _flags      Any behavioural flags relevant to this value.
 * \param[in]  _structure  The structure corresponding to the mapping.
 * \param[in]  _member     The member in _structure for this mapping value.
 */
#define CYAML_FIELD_DOUBLE_DEFAULT( \
		_default, _key, _flags, _structure, _member) \
{ \
	.base = CYAML_FIELD_FLOAT_BASE(_key, (_flags | CYAML_FLAG_OPTIONAL), \
			_structure, _member), \
	.use_default_value = true, \
	.default_value_d = (double)_default, \
}

/**
 * Exapnded apping schema helper macro for keys with \ref CYAML_FLOAT type.
 *
 * Similar to \ref CYAML_FIELD_FLOAT.
 */
#define CYAML_FIELD_FLOAT_PTR_EX(...) \
{ \
	.base = CYAML_FIELD_FLOAT_PTR_BASE(__VA_ARGS__), \
	CYAML_FIELD_DEFAULT_NONE \
}

/**
 * Expanded mapping schema helper macro for keys with \ref CYAML_STRING type.
 *
 * Similar to \ref CYAML_FIELD_STRING.
 */
#define CYAML_FIELD_STRING_EX(...) \
{ \
	.base = CYAML_FIELD_STRING_BASE(__VA_ARGS__), \
	CYAML_FIELD_DEFAULT_NONE \
}

/**
 * Expanded mapping schema helper macro for keys with \ref CYAML_STRING type.
 *
 * Similar to \ref CYAML_FIELD_STRING_PTR.
 */
#define CYAML_FIELD_STRING_PTR_EX(...) \
{ \
	.base = CYAML_FIELD_STRING_PTR_BASE(__VA_ARGS__), \
	CYAML_FIELD_DEFAULT_NONE \
}

/**
 * Value schema helper macro for values with \ref CYAML_MAPPING type, when
 * the fields allow for default values.
 *
 * \param[in]  _flags         Any behavioural flags relevant to this value.
 * \param[in]  _type          The C type of structure corresponding to mapping.
 * \param[in]  _fields        Pointer to mapping fields schema array.
 */
#define CYAML_VALUE_MAPPING_EX( \
		_flags, _type, _fields) \
	CYAML_VALUE_MAPPING_BASE(_flags, _type, &_fields[0].base)

/**
 * Expanded mapping schema helper macro for keys with \ref CYAML_MAPPING type.
 *
 * Similar to /ref CYAML_FIELD_MAPPING.
 */
#define CYAML_FIELD_MAPPING_EX(...) \
{ \
	.base = CYAML_FIELD_MAPPING_BASE(__VA_ARGS__), \
	CYAML_FIELD_DEFAULT_NONE \
}

/**
 * Expanded mapping schema helper macro for keys with \ref CYAML_MAPPING type.
 *
 * Similar to /ref CYAML_FIELD_MAPPING.
 */
#define CYAML_FIELD_MAPPING_PTR_EX(...) \
{ \
	.base = CYAML_FIELD_MAPPING_PTR_BASE(__VA_ARGS__), \
	CYAML_FIELD_DEFAULT_NONE \
}

/**
 * Expanded mapping schema helper macro for keys with \ref CYAML_SEQUENCE type.
 *
 * Similar to /ref CYAML_FIELD_SEQUENCE.
 */
#define CYAML_FIELD_SEQUENCE_EX(...) \
{ \
	.base = CYAML_FIELD_SEQUENCE_BASE(__VA_ARGS__), \
	CYAML_FIELD_DEFAULT_NONE \
}

/**
 * Expanded mapping schema helper macro for keys with \ref CYAML_SEQUENCE type.
 *
 * Similar to /ref CYAML_FIELD_SEQUENCE_PTR.
 */
#define CYAML_FIELD_SEQUENCE_PTR_EX(...) \
{ \
	.base = CYAML_FIELD_SEQUENCE_PTR_BASE(__VA_ARGS__), \
	CYAML_FIELD_DEFAULT_NONE \
}

/**
 * Expanded mapping schema helper macro for keys with \ref CYAML_SEQUENCE type.
 *
 * Similar to /ref CYAML_FIELD_SEQUENCE_COUNT.
 */
#define CYAML_FIELD_SEQUENCE_COUNT_EX(...) \
{ \
	.base = CYAML_FIELD_SEQUENCE_COUNT_BASE(__VA_ARGS__), \
	CYAML_FIELD_DEFAULT_NONE \
}

/**
 * Expanded mapping schema helper macro for keys with \ref CYAML_SEQUENCE_FIXED
 * type.
 *
 * Similar to /ref CYAML_FIELD_SEQUENCE_FIXED.
 */
#define CYAML_FIELD_SEQUENCE_FIXED_EX(...) \
{ \
	.base = CYAML_FIELD_SEQUENCE_FIXED_BASE(__VA_ARGS__), \
	CYAML_FIELD_DEFAULT_NONE \
}

/**
 * Mapping schema helper macro for keys with \ref CYAML_IGNORE type.
 *
 * Similar to /ref CYAML_FIELD_IGNORE.
 */
#define CYAML_FIELD_IGNORE_EX(...) \
{ \
	.base = CYAML_FIELD_IGNORE_BASE(__VA_ARGS__), \
	CYAML_FIELD_DEFAULT_NONE \
}

/**
 * Expanded mapping schema helper macro for terminating an array of mapping
 * fields.
 *
 * Similar to /ref CYAML_FIELD_END.
 */
#define CYAML_FIELD_END_EX { \
	.base = CYAML_FIELD_END_BASE, \
	CYAML_FIELD_DEFAULT_NONE \
}

/**
 * Load a YAML document from a file at the given path, using expanded cyaml
 * field structures.
 *
 * Similar to \ref cyaml_load_file, but will set up the expanded library flag by
 * default.
 */
extern cyaml_err_t cyaml_load_file_ex(
		const char *path,
		const cyaml_config_t *config,
		const cyaml_schema_value_t *schema,
		cyaml_data_t **data_out,
		unsigned *seq_count_out);

/**
 * Load a YAML document from a data buffer, using expanded cyaml field
 * structures.
 *
 * Similar to \ref cyaml_load_data, but will set up the expanded library flag by
 * default.
 */
extern cyaml_err_t cyaml_load_data_ex(
		const uint8_t *input,
		size_t input_len,
		const cyaml_config_t *config,
		const cyaml_schema_value_t *schema,
		cyaml_data_t **data_out,
		unsigned *seq_count_out);

/**
 * Save a YAML document to a file at the given path, using expanded cyaml field
 * structures.
 *
 * Similar to \ref cyaml_save_file, but will set up the expanded library flag by
 * default.
 */
extern cyaml_err_t cyaml_save_file_ex(
		const char *path,
		const cyaml_config_t *config,
		const cyaml_schema_value_t *schema,
		const cyaml_data_t *data,
		unsigned seq_count);

/**
 * Save a YAML document into a string in memory, using expanded cyaml field
 * structures.
 *
 * Similar to \ref cyaml_save_data, but will set up the expanded library flag by
 * default.
 */
extern cyaml_err_t cyaml_save_data_ex(
		char **output,
		size_t *len,
		const cyaml_config_t *config,
		const cyaml_schema_value_t *schema,
		const cyaml_data_t *data,
		unsigned seq_count);

/**
 * Free data returned by a CYAML load function, using expanded cyaml field
 * structures.
 *
 * Similar to \ref cyaml_free, but will set up the expanded library flag by
 * default.
 */
extern cyaml_err_t cyaml_free_ex(
		const cyaml_config_t *config,
		const cyaml_schema_value_t *schema,
		cyaml_data_t *data,
		unsigned seq_count);

#ifndef KEEP_BOTH_VERSIONS
#define CYAML_FIELD_INT CYAML_FIELD_INT_EX
#define CYAML_FIELD_INT_PTR CYAML_FIELD_INT_PTR_EX
#define CYAML_FIELD_UINT CYAML_FIELD_UINT_EX
#define CYAML_FIELD_UINT_PTR CYAML_FIELD_UINT_PTR_EX
#define CYAML_FIELD_BOOL CYAML_FIELD_BOOL_EX
#define CYAML_FIELD_BOOL_PTR CYAML_FIELD_BOOL_PTR_EX
#define CYAML_FIELD_ENUM CYAML_FIELD_ENUM_EX
#define CYAML_FIELD_ENUM_PTR CYAML_FIELD_ENUM_PTR_EX
#define CYAML_FIELD_FLAGS CYAML_FIELD_FLAGS_EX
#define CYAML_FIELD_FLAGS_PTR CYAML_FIELD_FLAGS_PTR_EX
#define CYAML_FIELD_BITFIELD CYAML_FIELD_BITFIELD_EX
#define CYAML_FIELD_BITFIELD_PTR CYAML_FIELD_BITFIELD_PTR_EX
#define CYAML_FIELD_FLOAT CYAML_FIELD_FLOAT_EX
#define CYAML_FIELD_FLOAT_PTR CYAML_FIELD_FLOAT_PTR_EX
#define CYAML_FIELD_STRING CYAML_FIELD_STRING_EX
#define CYAML_FIELD_STRING_PTR CYAML_FIELD_STRING_PTR_EX
#define CYAML_VALUE_MAPPING CYAML_VALUE_MAPPING_EX
#define CYAML_FIELD_MAPPING CYAML_FIELD_MAPPING_EX
#define CYAML_FIELD_MAPPING_PTR CYAML_FIELD_MAPPING_PTR_EX
#define CYAML_FIELD_SEQUENCE CYAML_FIELD_SEQUENCE_EX
#define CYAML_FIELD_SEQUENCE_PTR CYAML_FIELD_SEQUENCE_PTR_EX
#define CYAML_FIELD_SEQUENCE_COUNT CYAML_FIELD_SEQUENCE_COUNT_EX
#define CYAML_FIELD_SEQUENCE_FIXED CYAML_FIELD_SEQUENCE_FIXED_EX
#define CYAML_FIELD_IGNORE CYAML_FIELD_IGNORE_EX
#define CYAML_FIELD_END CYAML_FIELD_END_EX
#undef cyaml_load_file
#define cyaml_load_file cyaml_load_file_ex
#undef cyaml_load_data
#define cyaml_load_data cyaml_load_data_ex
#undef cyaml_save_file
#define cyaml_save_file cyaml_save_file_ex
#undef cyaml_save_data
#define cyaml_save_data cyaml_save_data_ex
#undef cymal_free
#define cymal_free cymal_free_ex
#endif

#ifdef __cplusplus
}
#endif

#endif
