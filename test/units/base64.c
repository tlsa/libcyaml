/*
 * SPDX-License-Identifier: ISC
 *
 * Copyright (C) 2018-2021 Michael Drake <tlsa@netsurf-browser.org>
 */

#include <stdbool.h>
#include <assert.h>
#include <stdio.h>

#include <cyaml/cyaml.h>

#include "../../src/base64.h"

#include "ttest.h"
#include "test.h"

/** Helper macro to squash unused variable warnings. */
#define UNUSED(_x) ((void)(_x))

/** Helper macro to get the length of string string literals. */
#define SLEN(_s) (CYAML_ARRAY_LEN(_s) - 1)

#define STRING_PAIRS(_dec, _enc) \
	{ \
		.dec     = _dec, \
		.dec_len = SLEN(_dec), \
		.enc     = _enc, \
		.enc_len = SLEN(_enc), \
	}

static const struct string_pairs {
	const char *enc;
	size_t enc_len;
	const char *dec;
	size_t dec_len;
} data[] = {
	STRING_PAIRS(""          , ""                ),
	STRING_PAIRS("😸"        , "8J+YuA=="        ),
	STRING_PAIRS("Cat"       , "Q2F0"            ),
	STRING_PAIRS("Cats"      , "Q2F0cw=="        ),
	STRING_PAIRS("Kitty"     , "S2l0dHk="        ),
	STRING_PAIRS("Kitten"    , "S2l0dGVu"        ),
	STRING_PAIRS("Kitties"   , "S2l0dGllcw=="    ),
	STRING_PAIRS("Kittens"   , "S2l0dGVucw=="    ),
	STRING_PAIRS("Kitty cat" , "S2l0dHkgY2F0"    ),
	STRING_PAIRS("Kitty cats", "S2l0dHkgY2F0cw=="),
};

/**
 * Test base64 encoding.
 *
 * \param[in]  report  The test report context.
 * \return true if test passes, false otherwise.
 */
static bool test_base64_encode(
		ttest_report_ctx_t *report)
{
	bool pass = true;

	for (unsigned i = 0; i < CYAML_ARRAY_LEN(data); i++) {
		size_t enc_len;
		ttest_ctx_t tc;
		char name[sizeof(__func__) + 64];
		char encoded[64];

		sprintf(name, "%s_%u", __func__, i);
		if (!ttest_start(report, name, NULL, NULL, &tc)) {
			continue;
		}

		enc_len = cyaml_base64_calc_encoded_size(data[i].dec_len);
		if (enc_len != data[i].enc_len) {
			pass &= ttest_fail(&tc, "Incorrect encoded size: "
					"Got %zu, expected %zu",
					enc_len, data[i].enc_len);
			continue;
		}

		cyaml_base64_encode((const uint8_t *)data[i].dec,
				data[i].dec_len, encoded);
		encoded[enc_len] = '\0';
		if (memcmp(encoded, data[i].enc, enc_len) != 0) {
			pass &= ttest_fail(&tc, "Wrong encoded data:\n"
					"\t     Got: %s\n"
					"\tExpected: %s",
					encoded, data[i].enc);
			continue;
		}

		pass &= ttest_pass(&tc);
	}

	return pass;
}

/**
 * Test base64 decoding.
 *
 * \param[in]  report  The test report context.
 * \return true if test passes, false otherwise.
 */
static bool test_base64_decode(
		ttest_report_ctx_t *report)
{
	bool pass = true;

	for (unsigned i = 0; i < CYAML_ARRAY_LEN(data); i++) {
		size_t dec_len;
		ttest_ctx_t tc;
		cyaml_err_t err;
		char name[sizeof(__func__) + 64];
		char decoded[64];

		sprintf(name, "%s_%u", __func__, i);
		if (!ttest_start(report, name, NULL, NULL, &tc)) {
			continue;
		}

		err = cyaml_base64_calc_decoded_size(data[i].enc,
				data[i].enc_len, &dec_len);
		if (err != CYAML_OK) {
			pass &= ttest_fail(&tc, "Failed to calc decoded size: "
					"%s", cyaml_strerror(err));
			continue;
		}
		if (dec_len != data[i].dec_len) {
			pass &= ttest_fail(&tc, "Incorrect decoded size: "
					"Got %zu, expected %zu",
					dec_len, data[i].dec_len);
			continue;
		}

		cyaml_base64_decode(data[i].enc, data[i].enc_len,
				(uint8_t *)decoded);
		if (memcmp(decoded, data[i].dec, dec_len) != 0) {
			pass &= ttest_fail(&tc, "Wrong decoded data:\n"
					"\t     Got: %*.*s\n"
					"\tExpected: %*.*s",
					(int) dec_len, (int) dec_len, decoded,
					(int) dec_len, (int) dec_len, data[i].dec);
			continue;
		}

		pass &= ttest_pass(&tc);
	}

	return pass;
}

/**
 * Test base64 decoding oddities.
 *
 * \param[in]  report  The test report context.
 * \return true if test passes, false otherwise.
 */
static bool test_base64_decode_odd(
		ttest_report_ctx_t *report)
{
	bool pass = true;
	static const struct odd_data {
		const char *name;
		const char *enc;
		size_t enc_len;
		const char *dec;
		size_t dec_len;
		cyaml_err_t err;
	} odd_data[] = {
		{
			.name = "whitespace_only",
			.enc = " \t\r\n",
			.enc_len = 4,
			.dec = "",
			.dec_len = 0,
			.err = CYAML_OK,
		},
		{
			.name = "padding_only_1",
			.enc = "=",
			.enc_len = 1,
			.err = CYAML_ERR_INVALID_BASE64,
		},
		{
			.name = "padding_only_2",
			.enc = "==",
			.enc_len = 2,
			.err = CYAML_ERR_INVALID_BASE64,
		},
		{
			.name = "str_len",
			.enc = "C",
			.enc_len = 1,
			.err = CYAML_ERR_INVALID_BASE64,
		},
		{
			.name = "bad_char",
			.enc = "Q2F0!",
			.enc_len = SLEN("Q2F0!"),
			.err = CYAML_ERR_INVALID_BASE64,
		},
		{
			.name = "bad_char_hyphen",
			.enc = "Q2F0-cw==",
			.enc_len = SLEN("Q2F0-cw=="),
			.err = CYAML_ERR_INVALID_BASE64,
		},
		{
			.name = "bad_char_vt",
			.enc = "S2l0dHkg\vY2F0cw==",
			.enc_len = SLEN("S2l0dHkg\vY2F0cw=="),
			.err = CYAML_ERR_INVALID_BASE64,
		},
		{
			.name = "bad_char_ff",
			.enc = "S2l0dHkg\fY2F0cw==",
			.enc_len = SLEN("S2l0dHkg\fY2F0cw=="),
			.err = CYAML_ERR_INVALID_BASE64,
		},
		{
			.name = "bad_char_nul",
			.enc = "S2l0dHkg\0Y2F0cw==",
			.enc_len = SLEN("S2l0dHkg\0Y2F0cw=="),
			.err = CYAML_ERR_INVALID_BASE64,
		},
		{
			.name = "bad_char_nbsp",
			.enc = "S2l0dHkg\xC2\xA0Y2F0cw==",
			.enc_len = SLEN("S2l0dHkg\xC2\xA0Y2F0cw=="),
			.err = CYAML_ERR_INVALID_BASE64,
		},
		{
			.name = "bad_char_e_acute",
			.enc = "S2l0dHkg\xC3\xA9Y2F0cw==",
			.enc_len = SLEN("S2l0dHkg\xC3\xA9Y2F0cw=="),
			.err = CYAML_ERR_INVALID_BASE64,
		},
		{
			.name = "bad_char_ellipsis",
			.enc = "S2l0dHkg\xE2\x80\xA6Y2F0cw==",
			.enc_len = SLEN("S2l0dHkg\xE2\x80\xA6Y2F0cw=="),
			.err = CYAML_ERR_INVALID_BASE64,
		},
		{
			.name = "truncated_nel",
			.enc = "S2l0dHkgY2F0cw==\xC2",
			.enc_len = SLEN("S2l0dHkgY2F0cw==\xC2"),
			.err = CYAML_ERR_INVALID_BASE64,
		},
		{
			.name = "truncated_ls",
			.enc = "S2l0dHkgY2F0cw==\xE2\x80",
			.enc_len = SLEN("S2l0dHkgY2F0cw==\xE2\x80"),
			.err = CYAML_ERR_INVALID_BASE64,
		},
		{
			.name = "space",
			.enc = "S2l0dHkg Y2F0cw==",
			.enc_len = SLEN("S2l0dHkg Y2F0cw=="),
			.dec = "Kitty cats",
			.dec_len = SLEN("Kitty cats"),
			.err = CYAML_OK,
		},
		{
			.name = "tab",
			.enc = "S2l0dHkg\tY2F0cw==",
			.enc_len = SLEN("S2l0dHkg\tY2F0cw=="),
			.dec = "Kitty cats",
			.dec_len = SLEN("Kitty cats"),
			.err = CYAML_OK,
		},
		{
			.name = "lf",
			.enc = "S2l0dHkg\nY2F0cw==\n",
			.enc_len = SLEN("S2l0dHkg\nY2F0cw==\n"),
			.dec = "Kitty cats",
			.dec_len = SLEN("Kitty cats"),
			.err = CYAML_OK,
		},
		{
			.name = "cr",
			.enc = "S2l0dHkg\rY2F0cw==",
			.enc_len = SLEN("S2l0dHkg\rY2F0cw=="),
			.dec = "Kitty cats",
			.dec_len = SLEN("Kitty cats"),
			.err = CYAML_OK,
		},
		{
			.name = "crlf",
			.enc = "S2l0dHkg\r\nY2F0cw==\r\n",
			.enc_len = SLEN("S2l0dHkg\r\nY2F0cw==\r\n"),
			.dec = "Kitty cats",
			.dec_len = SLEN("Kitty cats"),
			.err = CYAML_OK,
		},
		{
			.name = "bad_char_nel",
			.enc = "S2l0dHkg\xC2\x85Y2F0cw==",
			.enc_len = SLEN("S2l0dHkg\xC2\x85Y2F0cw=="),
			.err = CYAML_ERR_INVALID_BASE64,
		},
		{
			.name = "bad_char_ls",
			.enc = "S2l0dHkg\xE2\x80\xA8Y2F0cw==",
			.enc_len = SLEN("S2l0dHkg\xE2\x80\xA8Y2F0cw=="),
			.err = CYAML_ERR_INVALID_BASE64,
		},
		{
			.name = "bad_char_ps",
			.enc = "S2l0dHkg\xE2\x80\xA9Y2F0cw==",
			.enc_len = SLEN("S2l0dHkg\xE2\x80\xA9Y2F0cw=="),
			.err = CYAML_ERR_INVALID_BASE64,
		},
		{
			.name = "bad_char_trailing_ps",
			.enc = "S2l0dHkgY2F0cw==\xE2\x80\xA9",
			.enc_len = SLEN("S2l0dHkgY2F0cw==\xE2\x80\xA9"),
			.err = CYAML_ERR_INVALID_BASE64,
		},
		{
			.name = "break_inside_group",
			.enc = "S2l0d\nHkgY2F0cw==",
			.enc_len = SLEN("S2l0d\nHkgY2F0cw=="),
			.dec = "Kitty cats",
			.dec_len = SLEN("Kitty cats"),
			.err = CYAML_OK,
		},
		{
			.name = "break_inside_padding",
			.enc = "S2l0dHkgY2F0cw=\n=",
			.enc_len = SLEN("S2l0dHkgY2F0cw=\n="),
			.dec = "Kitty cats",
			.dec_len = SLEN("Kitty cats"),
			.err = CYAML_OK,
		},
		{
			.name = "mixed",
			.enc = " \tS2l0\r\ndHkg Y2F0\n\t" "cw==\r\n",
			.enc_len = SLEN(" \tS2l0\r\ndHkg Y2F0\n\t" "cw==\r\n"),
			.dec = "Kitty cats",
			.dec_len = SLEN("Kitty cats"),
			.err = CYAML_OK,
		},
		{
			.name = "no_padding_1",
			.enc = "S2l0dHk",
			.enc_len = 7,
			.dec = "Kitty",
			.dec_len = 5,
			.err = CYAML_OK,
		},
		{
			.name = "no_padding_2",
			.enc = "8J+YuA",
			.enc_len = 6,
			.dec = "😸",
			.dec_len = 4,
			.err = CYAML_OK,
		},
		{
			.name = "padding_count1",
			.enc = "S2l0dHk==",
			.enc_len = 9,
			.err = CYAML_ERR_INVALID_BASE64,
		},
		{
			.name = "padding_count2",
			.enc = "Q2F0cw=",
			.enc_len = 7,
			.err = CYAML_ERR_INVALID_BASE64,
		},
		{
			.name = "padding_count3",
			.enc = "Q2F00",
			.enc_len = 5,
			.err = CYAML_ERR_INVALID_BASE64,
		},
		{
			.name = "excess_padding",
			.enc = "C===",
			.enc_len = 4,
			.err = CYAML_ERR_INVALID_BASE64,
		},
		{
			.name = "unnecessary_padding",
			.enc = "Q2F0=",
			.enc_len = 5,
			.err = CYAML_ERR_INVALID_BASE64,
		},
		{
			.name = "internal_padding",
			.enc = "C=at",
			.enc_len = 4,
			.err = CYAML_ERR_INVALID_BASE64,
		},
	};

	for (unsigned i = 0; i < CYAML_ARRAY_LEN(odd_data); i++) {
		size_t dec_len;
		ttest_ctx_t tc;
		cyaml_err_t err;
		char name[sizeof(__func__) + 64];
		char decoded[64];

		sprintf(name, "%s_%s", __func__, odd_data[i].name);
		if (!ttest_start(report, name, NULL, NULL, &tc)) {
			continue;
		}

		err = cyaml_base64_calc_decoded_size(odd_data[i].enc,
				odd_data[i].enc_len, &dec_len);
		if (err != odd_data[i].err) {
			pass &= ttest_fail(&tc, "Unexpected return value: %s",
					cyaml_strerror(err));
			continue;
		}
		if (err != CYAML_OK) {
			pass &= ttest_pass(&tc);
			continue;
		}

		if (dec_len != odd_data[i].dec_len) {
			pass &= ttest_fail(&tc, "Incorrect decoded size: "
					"Got %zu, expected %zu",
					dec_len, odd_data[i].dec_len);
			continue;
		}

		cyaml_base64_decode(odd_data[i].enc, odd_data[i].enc_len,
				(uint8_t *)decoded);
		if (memcmp(decoded, odd_data[i].dec, dec_len) != 0) {
			pass &= ttest_fail(&tc, "Wrong decoded data:\n"
					"\t     Got: %*.*s\n"
					"\tExpected: %*.*s",
					(int) dec_len, (int) dec_len, decoded,
					(int) dec_len, (int) dec_len, odd_data[i].dec);
			continue;
		}

		pass &= ttest_pass(&tc);
	}

	return pass;
}

/**
 * Run the CYAML base64 unit tests.
 *
 * \param[in]  rc         The ttest report context.
 * \param[in]  log_level  CYAML log level.
 * \param[in]  log_fn     CYAML logging function, or NULL.
 * \return true iff all unit tests pass, otherwise false.
 */
bool base64_tests(
		ttest_report_ctx_t *rc,
		cyaml_log_t log_level,
		cyaml_log_fn_t log_fn)
{
	bool pass = true;

	UNUSED(log_level);
	UNUSED(log_fn);

	ttest_heading(rc, "Base64 tests: Encode & decode");

	pass &= test_base64_encode(rc);
	pass &= test_base64_decode(rc);

	ttest_heading(rc, "Base64 tests: Decode errors");

	pass &= test_base64_decode_odd(rc);

	return pass;
}
