/*
 * SPDX-License-Identifier: ISC
 *
 * Copyright (c) 2017-2018 Michael Drake <tlsa@netsurf-browser.org>
 */

/**
 * \file
 * \brief CYAML library public header.
 *
 * CYAML is a C library for parsing and serialising structured YAML documents.
 *
 * \todo Add cyaml_save_* functions for serialisation to YAML.
 */

#ifndef CYAML_H
#define CYAML_H

/**
 * CYAML library version string.
 */
extern const char *cyaml_version_str;

/**
 * CYAML library version number suitable for comparisons.
 */
extern const uint32_t cyaml_version;

/**
 * CYAML value types.
 */
typedef enum cyaml_type {
	CYAML_INT,      /**< Value is a signed integer. */
	CYAML_UINT,     /**< Value is an unsigned signed integer. */
	CYAML_BOOL,     /**< Value is a boolean. */
	/**
	 * Value is an enum.  Values of this type require a string list in
	 * the schema entry, to define the list of valid enum values.
	 */
	CYAML_ENUM,
	/**
	 * Value is a flags bit field.  Values of this type require a string
	 * list in the schema entry, to define the list of valid flag values.
	 * In the YAML, a \ref CYAML_FLAGS value must be presented as a
	 * sequence of strings.
	 */
	CYAML_FLAGS,
	CYAML_FLOAT,    /**< Value is floating point. */
	CYAML_STRING,   /**< Value is a string. */
	/**
	 * Value is a mapping.  Values of this type require mapping schema
	 * array in the schema entry.
	 */
	CYAML_MAPPING,
	/**
	 * Value is a sequence.  Values of this type must be the direct
	 * children of a mapping.  They require:
	 *
	 * - A schema describing the type of the sequence entries.
	 * - Offset to the array entry count member in the mapping structure.
	 * - Size in bytes of the count member in the mapping structure.
	 * - The minimum and maximum number of sequence count entries.
	 *   Set `max` to \ref CYAML_UNLIMITED to make array count
	 *   unconstrained.
	 */
	CYAML_SEQUENCE,
	/**
	 * Value is a **fixed length** sequence.  It is similar to \ref
	 * CYAML_SEQUENCE, however:
	 *
	 * - Values of this type do not need to be direct children of a mapping.
	 * - The minimum and maximum entry count must be the same.  If not
	 *   \ref CYAML_ERR_SEQUENCE_FIXED_COUNT will be returned.
	 * - Thee offset and size of the count structure member is unused.
	 *   Because the count is a schema-defined constant, it does not need
	 *   to be recorded.
	 */
	CYAML_SEQUENCE_FIXED,
	/**
	 * Value of this type is completely ignored.  This is most useful for
	 * ignoring particular keys in a mapping, when CYAML client sets
	 * configuration of \ref CYAML_CFG_IGNORE_UNKNOWN_KEYS.
	 */
	CYAML_IGNORE,
	/**
	 * Count of the valid CYAML types.  This value is **not a valid type**
	 * itself.
	 */
	CYAML__TYPE_COUNT,
} cyaml_type_e;

/**
 * CYAML value flags.
 *
 * These may be bitwise-ORed together.
 */
typedef enum cyaml_flag {
	CYAML_FLAG_DEFAULT  = 0,        /**< Default value flags (none set). */
	CYAML_FLAG_OPTIONAL = (1 << 0), /**< Mapping field is optional. */
	CYAML_FLAG_POINTER  = (1 << 1), /**< Value is a pointer to its type. */
	/**
	 * Make value handling strict.
	 *
	 * For \ref CYAML_ENUM and \ref CYAML_FLAGS types, in strict mode
	 * the YAML must contain a matching string.  Without strict, numerical
	 * values are also permitted.
	 *
	 * * For \ref CYAML_ENUM, the value becomes the value of the enum.
	 *   The numerical value is treated as signed.
	 * * For \ref CYAML_FLAGS, the values are bitwise ORed together.
	 *   The numerical values are treated as unsigned,
	 */
	CYAML_FLAG_STRICT   = (1 << 2),
} cyaml_flag_e;

/**
 * Schema definition for a value.
 *
 * There are convenience macros for each of the types to assist in
 * building a CYAML schema data structure for your YAML documents.
 */
typedef struct cyaml_schema_value {
	/**
	 * The type of the value defined by this schema entry.
	 */
	enum cyaml_type type;
	/** Flags indicating value's characteristics. */
	enum cyaml_flag flags;
	/**
	 * Size of the value's client data type in bytes.
	 *
	 * For example, `short` `int`, `long`, `int8_t`, etc are all signed
	 * integer types, so they would have the type \ref CYAML_INT,
	 * however, they have different sizes.
	 */
	uint32_t data_size;
	/** Anonymous union containing type-specific attributes. */
	union {
		/** \ref CYAML_STRING type-specific schema data. */
		struct {
			/**
			 * Minimum string length (bytes).
			 *
			 * \note Excludes trailing NUL.
			 */
			uint32_t min;
			/**
			 * Maximum string length (bytes).
			 *
			 * \note Excludes trailing NULL, so for character array
			 *       strings (rather than pointer strings), this
			 *       must be no more than `data_size - 1`.
			 */
			uint32_t max;
		} string;
		/** \ref CYAML_MAPPING type-specific schema data. */
		struct {
			/**
			 * Array of cyaml mapping field schema definitions.
			 *
			 * The array must be terminated by an entry with a
			 * NULL key.  See \ref cyaml_schema_field_t
			 * and \ref CYAML_FIELD_END for more info.
			 */
			const struct cyaml_schema_field *schema;
		} mapping;
		/**
		 * \ref CYAML_SEQUENCE and \ref CYAML_SEQUENCE_FIXED
		 * type-specific schema data.
		 */
		struct {
			/**
			 * Schema definition for the type of the entries in the
			 * sequence.
			 *
			 * All of a sequence's entries must be of the same
			 * type, and a sequence can not have an entry type of
			 * type \ref CYAML_SEQUENCE (although \ref
			 * CYAML_SEQUENCE_FIXED is allowed).  That is, you
			 * can't have a sequence of variable-length sequences.
			 */
			const struct cyaml_schema_value *schema;
			/**
			 * Minimum number of sequence entries.
			 *
			 * \note min and max must be the same for \ref
			 *       CYAML_SEQUENCE_FIXED.
			 */
			uint32_t min;
			/**
			 * Maximum number of sequence entries.
			 *
			 * \note min and max must be the same for \ref
			 *       CYAML_SEQUENCE_FIXED.
			 */
			uint32_t max;
		} sequence;
		/**
		 * \ref CYAML_ENUM and \ref CYAML_FLAGS type-specific schema
		 * data.
		 */
		struct {
			/** Array of strings defining enum or flags values. */
			const char * const *strings;
			/** Entry count for strings array. */
			uint32_t count;
		} enumeration;
	};
} cyaml_schema_value_t;

/**
 * Schema definition entry for mapping fields.
 *
 * YAML mappings are key:value pairs.  CYAML only supports scalar mapping keys,
 * that is, keys that are a string.
 *
 * The schema for mappings is composed of an array of entries of this
 * data structure.  It specifies the name of the key, and the type of
 * the value.  It also specifies the offset into the data at which value
 * data should be placed.  The array is terminated by an entry with a NULL key.
 */
typedef struct cyaml_schema_field {
	/**
	 * String for YAML mapping key that his schema entry describes,
	 * or NULL to indicated the end of an array of cyaml_schema_field_t
	 * entries.
	 */
	const char *key;
	/**
	 * Offset in data structure at which the value for this key should
	 * be placed / read from.
	 */
	uint32_t data_offset;
	/**
	 * \ref CYAML_SEQUENCE only: Offset to sequence
	 * entry count member in mapping's data structure.
	 */
	uint32_t count_offset;
	/**
	 * \ref CYAML_SEQUENCE only: Size in bytes of sequence
	 * entry count member in mapping's data structure.
	 */
	uint8_t count_size;
	/**
	 * Defines the schema for the mapping field's value.
	 */
	struct cyaml_schema_value value;
} cyaml_schema_field_t;

/**
 * CYAML behavioural configuration flags for clients
 *
 * These may be bitwise-ORed together.
 */
typedef enum cyaml_cfg_flags {
	/**
	 * This indicates CYAML's default behaviour.
	 */
	CYAML_CFG_DEFAULT             = 0,
	/**
	 * When set, unknown mapping keys are ignored when loading YAML.
	 * Without this flag set, CYAML's default behaviour is to return
	 * with the error \ref CYAML_ERR_INVALID_KEY.
	 */
	CYAML_CFG_IGNORE_UNKNOWN_KEYS = (1 << 0),
} cyaml_cfg_flags_t;

/**
 * CYAML function return codes indicating success or reason for failure.
 *
 * Use \ref cyaml_strerror() to convert and error code to a human-readable
 * string.
 */
typedef enum cyaml_err {
	CYAML_OK,                        /**< Success. */
	CYAML_ERR_OOM,                   /**< Memory allocation failed. */
	CYAML_ERR_ALIAS,                 /**< YAML alias is unsupported. */
	CYAML_ERR_FILE_OPEN,             /**< Failed to open file. */
	CYAML_ERR_INVALID_KEY,           /**< Mapping key rejected by schema. */
	CYAML_ERR_INVALID_VALUE,         /**< Value rejected by schema. */
	CYAML_ERR_INTERNAL_ERROR,        /**< Internal error in LibCYAML. */
	CYAML_ERR_UNEXPECTED_EVENT,      /**< YAML event rejected by schema. */
	CYAML_ERR_STRING_LENGTH_MIN,     /**< String length too short. */
	CYAML_ERR_STRING_LENGTH_MAX,     /**< String length too long. */
	CYAML_ERR_INVALID_DATA_SIZE,     /**< Value's data size unsupported. */
	CYAML_ERR_TOP_LEVEL_NON_PTR,     /**< Top level type must be pointer. */
	CYAML_ERR_BAD_TYPE_IN_SCHEMA,    /**< Schema contains invalid type. */
	CYAML_ERR_BAD_MIN_MAX_SCHEMA,    /**< Schema minimum exceeds maximum. */
	CYAML_ERR_BAD_PARAM_SEQ_COUNT,   /**< Bad seq_count param for schema. */
	CYAML_ERR_BAD_PARAM_NULL_DATA,   /**< Client gave NULL data argument. */
	CYAML_ERR_SEQUENCE_ENTRIES_MIN,  /**< Too few sequence entries. */
	CYAML_ERR_SEQUENCE_ENTRIES_MAX,  /**< Too many sequence entries. */
	CYAML_ERR_SEQUENCE_FIXED_COUNT,  /**< Mismatch between min and max. */
	CYAML_ERR_SEQUENCE_IN_SEQUENCE,  /**< Non-fixed sequence in sequence. */
	CYAML_ERR_MAPPING_FIELD_MISSING, /**< Required mapping field missing. */
	CYAML_ERR_BAD_CONFIG_NULL_MEMFN, /**< Client gave NULL mem function. */
	CYAML_ERR_BAD_PARAM_NULL_CONFIG, /**< Client gave NULL config arg. */
	CYAML_ERR_BAD_PARAM_NULL_SCHEMA, /**< Client gave NULL schema arg. */
	CYAML_ERR_LIBYAML_PARSER_INIT,   /**< Failed to initialise libyaml. */
	CYAML_ERR_LIBYAML_PARSER,        /**< Error inside libyaml. */
	CYAML_ERR__COUNT,                /**< Count of CYAML return codes.
	                                  *   This is **not a valid return
	                                  *   code** itself.
	                                  */
} cyaml_err_t;

/**
 * Value schema helper macro for values with \ref CYAML_INT type.
 *
 * \param[in]  _flags         Any behavioural flags relevant to this value.
 * \param[in]  _type          The C type for this value.
 */
#define CYAML_VALUE_INT( \
		_flags, _type) \
	.type = CYAML_INT, \
	.flags = (_flags), \
	.data_size = sizeof(_type)

/**
 * Mapping schema helper macro for keys with \ref CYAML_INT type.
 *
 * \param[in]  _key        String defining the YAML mapping key for this value.
 * \param[in]  _flags      Any behavioural flags relevant to this value.
 * \param[in]  _structure  The structure corresponding to the mapping.
 * \param[in]  _member     The member in _structure for this mapping value.
 */
#define CYAML_FIELD_INT( \
		_key, _flags, _structure, _member) \
{ \
	.key = _key, \
	.value = { \
		CYAML_VALUE_INT((_flags), ((_structure *)NULL)->_member), \
	}, \
	.data_offset = offsetof(_structure, _member) \
}

/**
 * Value schema helper macro for values with \ref CYAML_UINT type.
 *
 * \param[in]  _flags         Any behavioural flags relevant to this value.
 * \param[in]  _type          The C type for this value.
 */
#define CYAML_VALUE_UINT( \
		_flags, _type) \
	.type = CYAML_UINT, \
	.flags = (_flags), \
	.data_size = sizeof(_type)

/**
 * Mapping schema helper macro for keys with \ref CYAML_UINT type.
 *
 * \param[in]  _key        String defining the YAML mapping key for this value.
 * \param[in]  _flags      Any behavioural flags relevant to this value.
 * \param[in]  _structure  The structure corresponding to the mapping.
 * \param[in]  _member     The member in _structure for this mapping value.
 */
#define CYAML_FIELD_UINT( \
		_key, _flags, _structure, _member) \
{ \
	.key = _key, \
	.value = { \
		CYAML_VALUE_UINT((_flags), ((_structure *)NULL)->_member), \
	}, \
	.data_offset = offsetof(_structure, _member) \
}

/**
 * Value schema helper macro for values with \ref CYAML_BOOL type.
 *
 * \param[in]  _flags         Any behavioural flags relevant to this value.
 * \param[in]  _type          The C type for this value.
 */
#define CYAML_VALUE_BOOL( \
		_flags, _type) \
	.type = CYAML_BOOL, \
	.flags = (_flags), \
	.data_size = sizeof(_type)

/**
 * Mapping schema helper macro for keys with \ref CYAML_BOOL type.
 *
 * \param[in]  _key        String defining the YAML mapping key for this value.
 * \param[in]  _flags      Any behavioural flags relevant to this value.
 * \param[in]  _structure  The structure corresponding to the mapping.
 * \param[in]  _member     The member in _structure for this mapping value.
 */
#define CYAML_FIELD_BOOL( \
		_key, _flags, _structure, _member) \
{ \
	.key = _key, \
	.value = { \
		CYAML_VALUE_BOOL((_flags), ((_structure *)NULL)->_member), \
	}, \
	.data_offset = offsetof(_structure, _member) \
}

/**
 * Value schema helper macro for values with \ref CYAML_ENUM type.
 *
 * \param[in]  _flags         Any behavioural flags relevant to this value.
 * \param[in]  _type          The C type for this value.
 * \param[in]  _strings       Array of string data for enumeration values.
 * \param[in]  _strings_count Number of entries in _strings.
 */
#define CYAML_VALUE_ENUM( \
		_flags, _type, _strings, _strings_count) \
	.type = CYAML_ENUM, \
	.flags = (_flags), \
	.data_size = sizeof(_type), \
	.enumeration = { \
		.strings = _strings, \
		.count = _strings_count, \
	}

/**
 * Mapping schema helper macro for keys with \ref CYAML_ENUM type.
 *
 * \param[in]  _key        String defining the YAML mapping key for this value.
 * \param[in]  _flags      Any behavioural flags relevant to this value.
 * \param[in]  _structure  The structure corresponding to the mapping.
 * \param[in]  _member     The member in _structure for this mapping value.
 * \param[in]  _strings       Array of string data for enumeration values.
 * \param[in]  _strings_count Number of entries in _strings.
 */
#define CYAML_FIELD_ENUM( \
		_key, _flags, _structure, _member, _strings, _strings_count) \
{ \
	.key = _key, \
	.value = { \
		CYAML_VALUE_ENUM((_flags), ((_structure *)NULL)->_member, \
				_strings, _strings_count), \
	}, \
	.data_offset = offsetof(_structure, _member) \
}

/**
 * Value schema helper macro for values with \ref CYAML_FLAGS type.
 *
 * \param[in]  _flags         Any behavioural flags relevant to this value.
 * \param[in]  _type          The C type for this value.
 * \param[in]  _strings       Array of string data for flag values.
 * \param[in]  _strings_count Number of entries in _strings.
 */
#define CYAML_VALUE_FLAGS( \
		_flags, _type, _strings, _strings_count) \
	.type = CYAML_FLAGS, \
	.flags = (_flags), \
	.data_size = sizeof(_type), \
	.enumeration = { \
		.strings = _strings, \
		.count = _strings_count, \
	}

/**
 * Mapping schema helper macro for keys with \ref CYAML_FLAGS type.
 *
 * \param[in]  _key        String defining the YAML mapping key for this value.
 * \param[in]  _flags      Any behavioural flags relevant to this value.
 * \param[in]  _structure  The structure corresponding to the mapping.
 * \param[in]  _member     The member in _structure for this mapping value.
 * \param[in]  _strings       Array of string data for flag values.
 * \param[in]  _strings_count Number of entries in _strings.
 */
#define CYAML_FIELD_FLAGS( \
		_key, _flags, _structure, _member, _strings, _strings_count) \
{ \
	.key = _key, \
	.value = { \
		CYAML_VALUE_FLAGS((_flags), ((_structure *)NULL)->_member, \
				_strings, _strings_count), \
	}, \
	.data_offset = offsetof(_structure, _member) \
}

/**
 * Value schema helper macro for values with \ref CYAML_FLOAT type.
 *
 * \param[in]  _flags         Any behavioural flags relevant to this value.
 * \param[in]  _type          The C type for this value.
 */
#define CYAML_VALUE_FLOAT( \
		_flags, _type) \
	.type = CYAML_FLOAT, \
	.flags = (_flags), \
	.data_size = sizeof(_type)

/**
 * Mapping schema helper macro for keys with \ref CYAML_FLOAT type.
 *
 * \param[in]  _key        String defining the YAML mapping key for this value.
 * \param[in]  _flags      Any behavioural flags relevant to this value.
 * \param[in]  _structure  The structure corresponding to the mapping.
 * \param[in]  _member     The member in _structure for this mapping value.
 */
#define CYAML_FIELD_FLOAT( \
		_key, _flags, _structure, _member) \
{ \
	.key = _key, \
	.value = { \
		CYAML_VALUE_FLOAT((_flags), ((_structure *)NULL)->_member), \
	}, \
	.data_offset = offsetof(_structure, _member) \
}

/**
 * Value schema helper macro for values with \ref CYAML_STRING type.
 *
 * \note If the string is an array (`char str[N];`) then the \ref
 *       CYAML_FLAG_POINTER flag must **not** be set, and the max
 *       length must be no more than `N-1`.
 *
 *       If the string is a pointer (`char *str;`), then the \ref
 *       CYAML_FLAG_POINTER flag **must be set**.
 *
 * \param[in]  _flags         Any behavioural flags relevant to this value.
 * \param[in]  _type          The C type for this value.
 * \param[in]  _min           Minimum string length in bytes.
 *                            Excludes trailing '\0'.
 * \param[in]  _max           The C type for this value.
 *                            Excludes trailing '\0'.
 */
#define CYAML_VALUE_STRING( \
		_flags, _type, _min, _max) \
	.type = CYAML_STRING, \
	.flags = (_flags), \
	.data_size = sizeof(_type), \
	.string = { \
		.min = _min, \
		.max = _max, \
	}

/**
 * Mapping schema helper macro for keys with \ref CYAML_STRING type.
 *
 * Use this for fields with C array type, e.g. `char str[N];`.  This fills the
 * maximum string length (`N-1`) out automatically.
 *
 * \param[in]  _key        String defining the YAML mapping key for this value.
 * \param[in]  _flags      Any behavioural flags relevant to this value.
 * \param[in]  _structure  The structure corresponding to the mapping.
 * \param[in]  _member     The member in _structure for this mapping value.
 * \param[in]  _min        Minimum string length in bytes.  Excludes '\0'.
 */
#define CYAML_FIELD_STRING( \
		_key, _flags, _structure, _member, _min) \
{ \
	.key = _key, \
	.value = { \
		CYAML_VALUE_STRING(((_flags) & (~CYAML_FLAG_POINTER)), \
				(((_structure *)NULL)->_member), _min, \
				sizeof(((_structure *)NULL)->_member) - 1), \
	}, \
	.data_offset = offsetof(_structure, _member) \
}

/**
 * Mapping schema helper macro for keys with \ref CYAML_STRING type.
 *
 * Use this for fields with C pointer type, e.g. `char *str;`.  This creates
 * a separate allocation for the string data, and fills in the pointer.
 *
 * Use `0` for _min and \ref CYAML_UNLIMITED for _max for unconstrained string
 * lengths.
 *
 * \param[in]  _key        String defining the YAML mapping key for this value.
 * \param[in]  _flags      Any behavioural flags relevant to this value.
 * \param[in]  _structure  The structure corresponding to the mapping.
 * \param[in]  _member     The member in _structure for this mapping value.
 * \param[in]  _min        Minimum string length in bytes.  Excludes '\0'.
 * \param[in]  _max        Maximum string length in bytes.  Excludes '\0'.
 */
#define CYAML_FIELD_STRING_PTR( \
		_key, _flags, _structure, _member, _min, _max) \
{ \
	.key = _key, \
	.value = { \
		CYAML_VALUE_STRING(((_flags) | CYAML_FLAG_POINTER), \
				(((_structure *)NULL)->_member), \
				_min, _max), \
	}, \
	.data_offset = offsetof(_structure, _member) \
}

/**
 * Value schema helper macro for values with \ref CYAML_MAPPING type.
 *
 * \param[in]  _flags         Any behavioural flags relevant to this value.
 * \param[in]  _type          The C type of structure corresponding to mapping.
 * \param[in]  _schema        Pointer to mapping schema array.
 */
#define CYAML_VALUE_MAPPING( \
		_flags, _type, _schema) \
	.type = CYAML_MAPPING, \
	.flags = (_flags), \
	.data_size = sizeof(_type), \
	.mapping = { \
		.schema = _schema, \
	}

/**
 * Mapping schema helper macro for keys with \ref CYAML_MAPPING type.
 *
 * Use this for structures contained within other structures.
 *
 * \param[in]  _key        String defining the YAML mapping key for this value.
 * \param[in]  _flags      Any behavioural flags relevant to this value.
 * \param[in]  _structure  The structure corresponding to the containing mapping.
 * \param[in]  _member     The member in _structure for this mapping value.
 * \param[in]  _schema     Pointer to mapping schema array.
 */
#define CYAML_FIELD_MAPPING( \
		_key, _flags, _structure, _member, _schema) \
{ \
	.key = _key, \
	.value = { \
		CYAML_VALUE_MAPPING(((_flags) & (~CYAML_FLAG_POINTER)), \
				(((_structure *)NULL)->_member), _schema), \
	}, \
	.data_offset = offsetof(_structure, _member) \
}

/**
 * Mapping schema helper macro for keys with \ref CYAML_MAPPING type.
 *
 * Use this for pointers to structures.
 *
 * \param[in]  _key        String defining the YAML mapping key for this value.
 * \param[in]  _flags      Any behavioural flags relevant to this value.
 * \param[in]  _structure  The structure corresponding to the containing mapping.
 * \param[in]  _member     The member in _structure for this mapping value.
 * \param[in]  _schema     Pointer to mapping schema array.
 */
#define CYAML_FIELD_MAPPING_PTR( \
		_key, _flags, _structure, _member, _schema) \
{ \
	.key = _key, \
	.value = { \
		CYAML_VALUE_MAPPING(((_flags) | CYAML_FLAG_POINTER), \
				(*(((_structure *)NULL)->_member)), _schema), \
	}, \
	.data_offset = offsetof(_structure, _member) \
}

/**
 * Value schema helper macro for values with \ref CYAML_SEQUENCE type.
 *
 * \param[in]  _flags      Any behavioural flags relevant to this value.
 * \param[in]  _type       The C type of sequence **entries**.
 * \param[in]  _schema     Pointer to schema for the **entries** in sequence.
 * \param[in]  _min        Minimum number of sequence entries required.
 * \param[in]  _max        Maximum number of sequence entries required.
 */
#define CYAML_VALUE_SEQUENCE( \
		_flags, _type, _schema, _min, _max) \
	.type = CYAML_SEQUENCE, \
	.flags = (_flags), \
	.data_size = sizeof(_type), \
	.sequence = { \
		.schema = _schema, \
		.min = _min, \
		.max = _max, \
	}

/**
 * Mapping schema helper macro for keys with \ref CYAML_SEQUENCE type.
 *
 * \param[in]  _key        String defining the YAML mapping key for this value.
 * \param[in]  _flags      Any behavioural flags relevant to this value.
 * \param[in]  _structure  The structure corresponding to the mapping.
 * \param[in]  _member     The member in _structure for this mapping value.
 * \param[in]  _schema     Pointer to schema for the **entries** in sequence.
 * \param[in]  _min        Minimum number of sequence entries required.
 * \param[in]  _max        Maximum number of sequence entries required.
 */
#define CYAML_FIELD_SEQUENCE( \
		_key, _flags, _structure, _member, _schema, _min, _max) \
{ \
	.key = _key, \
	.value = { \
		CYAML_VALUE_SEQUENCE((_flags), \
				(*(((_structure *)NULL)->_member)), \
				_schema, _min, _max), \
	}, \
	.data_offset = offsetof(_structure, _member), \
	.count_size = sizeof(((_structure *)NULL)->_member ## _count), \
	.count_offset = offsetof(_structure, _member ## _count), \
}

/**
 * Value schema helper macro for values with \ref CYAML_SEQUENCE_FIXED type.
 *
 * \param[in]  _flags      Any behavioural flags relevant to this value.
 * \param[in]  _type       The C type of sequence **entries**.
 * \param[in]  _schema     Pointer to schema for the **entries** in sequence.
 * \param[in]  _count      Number of sequence entries required.
 */
#define CYAML_VALUE_SEQUENCE_FIXED( \
		_flags, _type, _schema, _count) \
	.type = CYAML_SEQUENCE_FIXED, \
	.flags = (_flags), \
	.data_size = sizeof(_type), \
	.sequence = { \
		.schema = _schema, \
		.min = _count, \
		.max = _count, \
	}

/**
 * Mapping schema helper macro for keys with \ref CYAML_SEQUENCE_FIXED type.
 *
 * \param[in]  _key        String defining the YAML mapping key for this value.
 * \param[in]  _flags      Any behavioural flags relevant to this value.
 * \param[in]  _structure  The structure corresponding to the mapping.
 * \param[in]  _member     The member in _structure for this mapping value.
 * \param[in]  _schema     Pointer to schema for the **entries** in sequence.
 * \param[in]  _count      Number of sequence entries required.
 */
#define CYAML_FIELD_SEQUENCE_FIXED( \
		_key, _flags, _structure, _member, _schema, _count) \
{ \
	.key = _key, \
	.value = { \
		CYAML_VALUE_SEQUENCE_FIXED((_flags), \
				(*(((_structure *)NULL)->_member)), \
				_schema, _count), \
	}, \
	.data_offset = offsetof(_structure, _member) \
}

/**
 * Mapping schema helper macro for keys with \ref CYAML_IGNORE type.
 *
 * \param[in]  _key    String defining the YAML mapping key to ignore.
 * \param[in]  _flags  Any behavioural flags relevant to this key.
 */
#define CYAML_FIELD_IGNORE( \
		_key, _flags) \
{ \
	.key = _key, \
	.value = { \
		.type = CYAML_IGNORE, \
		.flags = (_flags), \
	}, \
}

/**
 * Mapping schema helper macro for terminating an array of mapping fields.
 *
 * CYAML mapping schemas are formed from an array of \ref cyaml_schema_field
 * entries, and an entry with a NULL key indicates the end of the array.
 */
#define CYAML_FIELD_END { .key = NULL }

/**
 * Identifies that a \ref CYAML_SEQUENCE has unconstrained maximum entry
 * count.
 */
#define CYAML_UNLIMITED 0xffffffff

/**
 * Helper macro for counting array elements.
 *
 * \note Don't use this macro on pointers.
 *
 * \param[in] _a  A C array.
 * \return Array element count.
 */
#define CYAML_ARRAY_LEN(_a) ((sizeof(_a)) / (sizeof(_a[0])))

/**
 * Data loaded or saved by CYAML has this type.  CYAML schemas are used
 * to describe the data contained.
 */
typedef void cyaml_data_t;

/** CYAML logging levels. */
typedef enum cyaml_log_e {
	CYAML_LOG_DEBUG,   /**< Debug level logging. */
	CYAML_LOG_INFO,    /**< Info level logging. */
	CYAML_LOG_NOTICE,  /**< Notice level logging. */
	CYAML_LOG_WARNING, /**< Warning level logging. */
	CYAML_LOG_ERROR,   /**< Error level logging. */
} cyaml_log_t;

/**
 * CYAML logging function prototype.
 *
 * Clients may implement this to manage logging from CYAML themselves.
 * Otherwise, consider using the standard logging function, \ref cyaml_log.
 *
 * \param[in] level  Log level of message to log.
 * \param[in] fmt    Format string for message to log.
 * \param[in] args   Additional arguments used by fmt.
 */
typedef void (*cyaml_log_fn_t)(
		cyaml_log_t level,
		const char *fmt,
		va_list args);

/**
 * CYAML memory allocation / freeing function.
 *
 * Clients may implement this to handle memory allocation / freeing.
 *
 * \param[in] ptr    Existing allocation to resize, or NULL.
 * \param[in] size   The new size for the allocation.  \note setting 0 must
 *                   be treated as free().
 * \return If `size == 0`, returns NULL.  If `size > 0`, returns NULL on failure,
 *         and any existing allocation is left untouched, or return non-NULL as
 *         the new allocation on success, and the original pointer becomes
 *         invalid.
 */
typedef void * (*cyaml_mem_fn_t)(
		void *ptr,
		size_t size);

/**
 * Client CYAML configuration data.
 *
 * \todo Should provide facility for client to provide its own custom
 *       allocation functions.
 */
typedef struct cyaml_config {
	/**
	 * Client function to use for logging.
	 *
	 * Clients can implement their own logging function and set it here.
	 * Otherwise, set `log_fn` to \ref cyaml_log if CYAML's default
	 * logging to `stderr` is suitable (see its documentation for more
	 * details), or set to `NULL` to suppress all logging.
	 *
	 * \note Useful backtraces are issued through the `log_fn` at
	 *       \ref CYAML_LOG_ERROR level.  If your application needs
	 *       to load user YAML data, these backtraces can help users
	 *       figure out what's wrong with their YAML, causing it to
	 *       be rejected by your schema.
	 */
	cyaml_log_fn_t log_fn;
	/**
	 * Client function to use for memory allocation handling.
	 *
	 * Clients can implement their own, or pass \ref cyaml_mem to use
	 * CYAML's default allocator.
	 *
	 * \note Depending on platform, when using CYAML's default allocator,
	 *       clients may need to take care to ensure any allocated memory
	 *       is freed using \ref cyaml_mem too.
	 */
	cyaml_mem_fn_t mem_fn;
	/**
	 * Minimum logging priority level to be issued.
	 *
	 * Specifying e.g. \ref CYAML_LOG_WARNING will cause only warnings and
	 * errors to emerge.
	 */
	cyaml_log_t log_level;
	/** CYAML behaviour flags. */
	cyaml_cfg_flags_t flags;
} cyaml_config_t;

/**
 * Standard CYAML logging function.
 *
 * This logs to `stderr`.  It clients want to log elsewhere they must
 * implement their own logging function, and pass it to CYAML in the
 * \ref cyaml_config_t structure.
 *
 * \note This default logging function composes single log messages from
 *       multiple separate fprintfs to `stderr`.  If the client application
 *       writes to `stderr` from multiple threads, individual \ref cyaml_log
 *       messages may get broken up by the client applications logging.  To
 *       avoid this, clients should implement their own \ref cyaml_log_fn_t and
 *       pass it in via \ref cyaml_config_t.
 *
 * \param[in] level  Log level of message to log.
 * \param[in] fmt    Format string for message to log.
 * \param[in] args   Additional arguments used by fmt.
 */
extern void cyaml_log(
		cyaml_log_t level,
		const char *fmt,
		va_list args);

/**
 * CYAML default memory allocation / freeing function.
 *
 * This is used when clients don't supply their own.  It is exposed to
 * enable clients to use the same allocator as libcyaml used internally
 * to allocate/free memory when they have not provided their own allocation
 * function.
 *
 * \param[in] ptr    Existing allocation to resize, or NULL.
 * \param[in] size   The new size for the allocation.  \note When `size == 0`
 *                   this frees `ptr`.
 * \return If `size == 0`, returns NULL.  If `size > 0`, returns NULL on failure,
 *         and any existing allocation is left untouched, or return non-NULL as
 *         the new alloctation on success, and the original pointer becomes
 *         invalid.
 */
extern void * cyaml_mem(
		void *ptr,
		size_t size);

/**
 * Load a YAML document from a file at the given path.
 *
 * \note In the event of the top-level mapping having only optional fields,
 *       and the YAML not setting any of them, this function can return \ref
 *       CYAML_OK, and `NULL` in the `data_out` parameter.
 *
 * \param[in]  path           Path to YAML file to load.
 * \param[in]  config         Client's CYAML configuration structure.
 * \param[in]  schema         CYAML schema for the YAML to be loaded.
 * \param[out] data_out       Returns the caller-owned loaded data on success.
 *                            Untouched on failure.
 * \param[out] seq_count_out  On success, returns the sequence entry count.
 *                            Untouched on failure.
 *                            Must be non-NULL if top-level schema type is
 *                            \ref CYAML_SEQUENCE, otherwise, must be NULL.
 * \return \ref CYAML_OK on success, or appropriate error code otherwise.
 */
extern cyaml_err_t cyaml_load_file(
		const char *path,
		const cyaml_config_t *config,
		const cyaml_schema_value_t *schema,
		cyaml_data_t **data_out,
		unsigned *seq_count_out);

/**
 * Load a YAML document from a data buffer.
 *
 * \note In the event of the top-level mapping having only optional fields,
 *       and the YAML not setting any of them, this function can return \ref
 *       CYAML_OK, and `NULL` in the `data_out` parameter.
 *
 * \param[in]  input          Buffer to load YAML data from.
 * \param[in]  input_len      Length of input in bytes.
 * \param[in]  config         Client's CYAML configuration structure.
 * \param[in]  schema         CYAML schema for the YAML to be loaded.
 * \param[out] data_out       Returns the caller-owned loaded data on success.
 *                            Untouched on failure.
 * \param[out] seq_count_out  On success, returns the sequence entry count.
 *                            Untouched on failure.
 *                            Must be non-NULL if top-level schema type is
 *                            \ref CYAML_SEQUENCE, otherwise, must be NULL.
 * \return \ref CYAML_OK on success, or appropriate error code otherwise.
 */
extern cyaml_err_t cyaml_load_data(
		const uint8_t *input,
		size_t input_len,
		const cyaml_config_t *config,
		const cyaml_schema_value_t *schema,
		cyaml_data_t **data_out,
		unsigned *seq_count_out);

/**
 * Free data returned by a CYAML load function.
 *
 * This is a convenience function, which is here purely to minimise the
 * amount of code required in clients.  Clients would be better off writing
 * their own free function for the specific data once loaded.
 *
 * \note This is a recursive operation, freeing all nested data.
 *
 * \param[in] config     The client's CYAML library config.
 * \param[in] schema     The schema describing the content of data.  Must match
 *                       the schema given to the CYAML load function used to
 *                       load the data.
 * \param[in] data       The data structure to free.
 * \param[in] seq_count  If top level type is sequence, this should be the
 *                       entry count, otherwise it is ignored.
 * \return \ref CYAML_OK on success, or appropriate error code otherwise.
 */
extern cyaml_err_t cyaml_free(
		const cyaml_config_t *config,
		const cyaml_schema_value_t *schema,
		cyaml_data_t *data,
		unsigned seq_count);

/**
 * Convert a cyaml error code to a human-readable string.
 *
 * \param[in] err  Error code code to convert.
 * \return String representing err.  The string is '\0' terminated, and owned
 *         by libcyaml.
 */
extern const char * cyaml_strerror(
		cyaml_err_t err);

#endif
