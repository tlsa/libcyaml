/*
 * SPDX-License-Identifier: ISC
 *
 * Copyright (C) 2023 Michael Drake <tlsa@netsurf-browser.org>
 */

#include <assert.h>
#include <string.h>

#include "base64.h"
#include "util.h"

/**
 * \file
 * \brief CYAML functions for handling base64 encode and decode.
 */

/** Base64 value to character mapping. */
static const char enc[] =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz"
	"0123456789"
	"+/";

/* Exported function, documented in base64.h. */
size_t cyaml_base64_calc_encoded_size(
		size_t data_len)
{
	return (data_len + 2) / 3 * 4;
}

/**
 * Read up to 3 input bytes and write up to 4 output characters.
 *
 * \param[in]  count  Number of input bytes to encode (1, 2, or 3).
 * \param[in]  i      Input: Array of at least `count` bytes.
 * \param[out] o      Output: Storage to write the encoded data.
 * \return Number of output characters written.
*/
static inline size_t cyaml_base64__encode(
		size_t count,
		const uint8_t *i,
		char *o)
{
	uint32_t combined = 0;

	assert(count >= 1);
	assert(count <= 3);

	switch (count) {
		case 3: combined |= ((uint32_t)i[2] <<  0); /* Fall through. */
		case 2: combined |= ((uint32_t)i[1] <<  8); /* Fall through. */
		case 1:	combined |= ((uint32_t)i[0] << 16);
	}

	switch (count) {
		case 3: o[3] = enc[(combined >>  0) & 0x3f]; /* Fall through. */
		case 2: o[2] = enc[(combined >>  6) & 0x3f]; /* Fall through. */
		case 1: o[1] = enc[(combined >> 12) & 0x3f];
		        o[0] = enc[(combined >> 18) & 0x3f];
	}

	return count + 1;
}

/* Exported function, documented in base64.h. */
void cyaml_base64_encode(
		const uint8_t *data,
		size_t data_len,
		char *str)
{
	size_t str_len = 0;

	while (data_len >= 3) {
		str_len += cyaml_base64__encode(3, data, str + str_len);
		data_len -= 3;
		data += 3;
	}

	if (data_len != 0) {
		str_len += cyaml_base64__encode(data_len, data, str + str_len);
	}

	assert((str_len & 0x3U) != 1);
	switch (str_len & 0x3U) {
		case 2: str[str_len++] = '='; /* Fall through. */
		case 3: str[str_len++] = '=';
	}
}

/** Non-base64 character mapping values. */
enum {
	BAD = 64, /**< Invalid character. */
	PAD = 65, /**< Padding character ('='). */
};

/** Base64 character to value mapping. */
static const uint8_t dec[256] = {
	BAD, BAD, BAD, BAD,    BAD, BAD, BAD, BAD, /* ....  .... */
	BAD, BAD, BAD, BAD,    BAD, BAD, BAD, BAD, /* ....  .... */
	BAD, BAD, BAD, BAD,    BAD, BAD, BAD, BAD, /* ....  .... */
	BAD, BAD, BAD, BAD,    BAD, BAD, BAD, BAD, /* ....  .... */
	BAD, BAD, BAD, BAD,    BAD, BAD, BAD, BAD, /* ....  .... */
	BAD, BAD, BAD,  62,    BAD, BAD, BAD,  63, /* ...+  .../ */
	 52,  53,  54,  55,     56,  57,  58,  59, /* 0123  4567 */
	 60,  61, BAD, BAD,    BAD, PAD, BAD, BAD, /* 89..  .=.. */
	BAD,   0,   1,   2,      3,   4,   5,   6, /* .ABC  DEFG */
	  7,   8,   9,  10,     11,  12,  13,  14, /* HIJK  LMNO */
	 15,  16,  17,  18,     19,  20,  21,  22, /* PQRS  TUVW */
	 23,  24,  25, BAD,    BAD, BAD, BAD, BAD, /* XYZ.  .... */
	BAD,  26,  27,  28,     29,  30,  31,  32, /* .abc  defg */
	 33,  34,  35,  36,     37,  38,  39,  40, /* hijk  lmno */
	 41,  42,  43,  44,     45,  46,  47,  48, /* pqrs  tuvw */
	 49,  50,  51, BAD,    BAD, BAD, BAD, BAD, /* zyz.  .... */
	BAD, BAD, BAD, BAD,    BAD, BAD, BAD, BAD, /* ....  .... */
	BAD, BAD, BAD, BAD,    BAD, BAD, BAD, BAD, /* ....  .... */
	BAD, BAD, BAD, BAD,    BAD, BAD, BAD, BAD, /* ....  .... */
	BAD, BAD, BAD, BAD,    BAD, BAD, BAD, BAD, /* ....  .... */
	BAD, BAD, BAD, BAD,    BAD, BAD, BAD, BAD, /* ....  .... */
	BAD, BAD, BAD, BAD,    BAD, BAD, BAD, BAD, /* ....  .... */
	BAD, BAD, BAD, BAD,    BAD, BAD, BAD, BAD, /* ....  .... */
	BAD, BAD, BAD, BAD,    BAD, BAD, BAD, BAD, /* ....  .... */
	BAD, BAD, BAD, BAD,    BAD, BAD, BAD, BAD, /* ....  .... */
	BAD, BAD, BAD, BAD,    BAD, BAD, BAD, BAD, /* ....  .... */
	BAD, BAD, BAD, BAD,    BAD, BAD, BAD, BAD, /* ....  .... */
	BAD, BAD, BAD, BAD,    BAD, BAD, BAD, BAD, /* ....  .... */
	BAD, BAD, BAD, BAD,    BAD, BAD, BAD, BAD, /* ....  .... */
	BAD, BAD, BAD, BAD,    BAD, BAD, BAD, BAD, /* ....  .... */
	BAD, BAD, BAD, BAD,    BAD, BAD, BAD, BAD, /* ....  .... */
	BAD, BAD, BAD, BAD,    BAD, BAD, BAD, BAD, /* ....  .... */
};

/* Exported function, documented in base64.h. */
cyaml_err_t cyaml_base64_calc_decoded_size(
		const char *str,
		size_t str_len,
		size_t *size)
{
	const uint8_t *raw = (const uint8_t *) str;
	size_t padding = 0;
	size_t extra = 0;
	size_t len = 0;

	for (size_t i = 0; i < str_len; i++) {
		switch (dec[raw[i]]) {
		case PAD:
			if (padding >= 2) {
				return CYAML_ERR_INVALID_BASE64;
			}
			padding++;
			break;
		case BAD:
			break;
		default:
			if (padding) {
				return CYAML_ERR_INVALID_BASE64;
			}
			len++;
			break;
		}
	}

	if (len < 2) {
		return CYAML_ERR_INVALID_BASE64;
	}

	switch (len & 0x3U) {
		case 3:
			if (padding && padding != 1) {
				return CYAML_ERR_INVALID_BASE64;
			}
			extra = 2;
			break;
		case 2:
			if (padding && padding != 2) {
				return CYAML_ERR_INVALID_BASE64;
			}
			extra = 1;
			break;
		case 1:
			return CYAML_ERR_INVALID_BASE64;
		case 0:
			if (padding != 0) {
				return CYAML_ERR_INVALID_BASE64;
			}
			break;
	}

	*size = len / 4 * 3 + extra;
	return CYAML_OK;
}

/**
 * Read up to 4 input characters and write up to 3 output bytes.
 *
 * \param[in]  count  Number of input characters to decode (2, 3, or 4).
 * \param[in]  i      Input: Array of at least `count` characters.
 * \param[out] o      Output: Storage to write the decoded data.
 * \return Number of output bytes written.
 */
static inline size_t cyaml_base64__decode(
		size_t count,
		const uint8_t *i,
		uint8_t *o)
{
	assert(count <= 4);
	assert(count >= 2);

	switch (count) {
	case 4: o[2] = (uint8_t)((dec[i[3]]     ) | (dec[i[2]] << 6)); /* Fall through. */
	case 3: o[1] = (uint8_t)((dec[i[2]] >> 2) | (dec[i[1]] << 4)); /* Fall through. */
	case 2: o[0] = (uint8_t)((dec[i[1]] >> 4) | (dec[i[0]] << 2));
	}

	return count - 1;
}

/* Exported function, documented in base64.h. */
void cyaml_base64_decode(
		const char *str,
		size_t str_len,
		uint8_t *data)
{
	const uint8_t *input = (const uint8_t *) str;
	size_t buf_count = 0;
	uint8_t buf[4];

	while (str_len > 0) {
		if (dec[*input] < 64) {
			buf[buf_count++] = *(input);
			if (buf_count == 4) {
				data += cyaml_base64__decode(4, buf, data);
				buf_count = 0;
			}
		}
		str_len--;
		input++;
	}

	if (buf_count != 0) {
		cyaml_base64__decode(buf_count, buf, data);
	}
}
