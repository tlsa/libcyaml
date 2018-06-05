/*
 * SPDX-License-Identifier: ISC
 *
 * Copyright (C) 2018 Michael Drake <tlsa@netsurf-browser.org>
 */

/**
 * \file
 * \brief CYAML functions for handling utf8 text.
 */

#include <assert.h>
#include <stdint.h>
#include <stdbool.h>

#include "utf8.h"

/**
 * Get expected byte-length of UTF8 character.
 *
 * Finds the number of bytes expected for the UTF8 sequence starting with
 * the given byte.
 *
 * \param[in]  b  First byte of UTF8 sequence.
 * \return the byte width of the character or 0 if invalid.
 */
static inline int cyaml_utf8_char_len(uint8_t b)
{
	if (!(b & 0x80)) {
		return 1;
	} else switch (b >> 3) {
		case 0x18:
		case 0x19:
		case 0x1a:
		case 0x1b:
			return 2;
		case 0x1c:
		case 0x1d:
			return 3;
		case 0x1e:
			return 4;
	}
	return 0; /* Invalid */
}

/**
 * Get a codepoint from the input string.
 *
 * Caller must provide the expected length given the first input byte.
 *
 * \param[in]  s    String to read first codepoint from.
 * \param[in]  len  Expected length of first character.
 * \return The codepoint or `0xfffd` if character is invalid.
 */
static unsigned cyaml_utf8_get_codepoint(
		const uint8_t *s,
		int len)
{
	if (len > 1) {
		assert(s[0] != 0);
	}

	switch (len) {
	case 1:
		return s[0];
	case 2:
		if (s[1] == 0) {
			break;
		}
		return ((0x1f & s[0]) <<  6) |  (0x3f & s[1]);
	case 3:
		if ((s[1] == 0) || (s[2] == 0)) {
			break;
		}
		return ((0x0f & s[0]) << 12) | ((0x3f & s[1]) <<  6) |
		        (0x3f & s[2]);
	case 4:
		if ((s[1] == 0) || (s[2] == 0) || (s[3] == 0)) {
			break;
		}
		return ((0x07 & s[0]) << 18) | ((0x3f & s[1]) << 12) |
		       ((0x3f & s[2]) <<  6) |  (0x3f & s[3]);
	}

	/* Invalid. */
	return 0xfffd; /* REPLACEMENT CHARACTER */
}

/**
 * Convert a Unicode codepoint to lower case.
 *
 * \note This only handles some of the Unicode blocks.
 *       (Currently the Latin ones.)
 *
 * \param[in]  c  Codepoint to convert to lower-case, if applicable.
 * \return the lower-cased codepoint.
 */
static unsigned cyaml_utf8_to_lower(unsigned c)
{
	if (((c >= 0x0041) && (c <= 0x005a)) /* Basic Latin */        ||
	    ((c >= 0x00c0) && (c <= 0x00d6)) /* Latin-1 Supplement */ ||
	    ((c >= 0x00d8) && (c <= 0x00de)) /* Latin-1 Supplement */ ) {
		return c + 32;

	} else if (((c >= 0x0100) && (c <= 0x012f)) /* Latin Extended-A */ ||
	           ((c >= 0x0132) && (c <= 0x0137)) /* Latin Extended-A */ ||
	           ((c >= 0x014a) && (c <= 0x0177)) /* Latin Extended-A */ ||
	           ((c >= 0x0182) && (c <= 0x0185)) /* Latin Extended-B */ ||
	           ((c >= 0x01a0) && (c <= 0x01a5)) /* Latin Extended-B */ ||
	           ((c >= 0x01de) && (c <= 0x01ef)) /* Latin Extended-B */ ||
	           ((c >= 0x01f8) && (c <= 0x021f)) /* Latin Extended-B */ ||
	           ((c >= 0x0222) && (c <= 0x0233)) /* Latin Extended-B */ ||
	           ((c >= 0x0246) && (c <= 0x024f)) /* Latin Extended-B */ ) {
		return c & ~0x1;

	} else if (((c >= 0x0139) && (c <= 0x0148) /* Latin Extended-A */) ||
	           ((c >= 0x0179) && (c <= 0x017e) /* Latin Extended-A */) ||
	           ((c >= 0x01b3) && (c <= 0x01b6) /* Latin Extended-B */) ||
	           ((c >= 0x01cd) && (c <= 0x01dc) /* Latin Extended-B */)) {
		return (c + 1) & ~0x1;

	} else switch (c) {
		case 0x0178: return 0x00ff; /* Latin Extended-A */
		case 0x0187: return 0x0188; /* Latin Extended-B */
		case 0x018b: return 0x018c; /* Latin Extended-B */
		case 0x018e: return 0x01dd; /* Latin Extended-B */
		case 0x0191: return 0x0192; /* Latin Extended-B */
		case 0x0198: return 0x0199; /* Latin Extended-B */
		case 0x01a7: return 0x01a8; /* Latin Extended-B */
		case 0x01ac: return 0x01ad; /* Latin Extended-B */
		case 0x01af: return 0x01b0; /* Latin Extended-B */
		case 0x01b7: return 0x0292; /* Latin Extended-B */
		case 0x01b8: return 0x01b9; /* Latin Extended-B */
		case 0x01bc: return 0x01bd; /* Latin Extended-B */
		case 0x01c4: return 0x01c6; /* Latin Extended-B */
		case 0x01c5: return 0x01c6; /* Latin Extended-B */
		case 0x01c7: return 0x01c9; /* Latin Extended-B */
		case 0x01c8: return 0x01c9; /* Latin Extended-B */
		case 0x01ca: return 0x01cc; /* Latin Extended-B */
		case 0x01cb: return 0x01cc; /* Latin Extended-B */
		case 0x01f1: return 0x01f3; /* Latin Extended-B */
		case 0x01f2: return 0x01f3; /* Latin Extended-B */
		case 0x01f4: return 0x01f5; /* Latin Extended-B */
		case 0x01f7: return 0x01bf; /* Latin Extended-B */
		case 0x0220: return 0x019e; /* Latin Extended-B */
		case 0x023b: return 0x023c; /* Latin Extended-B */
		case 0x023d: return 0x019a; /* Latin Extended-B */
		case 0x0241: return 0x0242; /* Latin Extended-B */
		case 0x0243: return 0x0180; /* Latin Extended-B */
	}

	return c;
}

/* Exported function, documented in utf8.h. */
int cyaml_utf8_casecmp(
		const void * const str1,
		const void * const str2)
{
	const uint8_t *s1 = str1;
	const uint8_t *s2 = str2;

	while (true) {
		int len1;
		int len2;
		int cmp1;
		int cmp2;

		/* Check for end of strings. */
		if ((*s1 == 0) && (*s2 == 0)) {
			return 0; /* Both strings ended; match. */

		} else if (*s1 == 0) {
			return 1; /* String 1 has ended. */

		} else if (*s2 == 0) {
			return -1;/* String 2 has ended. */
		}

		/* Get byte lengths of these characters. */
		len1 = cyaml_utf8_char_len(*s1);
		len2 = cyaml_utf8_char_len(*s2);

		/* Compare values. */
		if ((len1 == 1) && (len2 == 1)) {
			/* Common case: Both strings have ASCII values. */
			if (*s1 != *s2) {
				/* They're different; need to lower case. */
				cmp1 = ((*s1 >= 'A') && (*s1 <= 'Z')) ?
						(*s1 + 'a' - 'A') : *s1;
				cmp2 = ((*s2 >= 'A') && (*s2 <= 'Z')) ?
						(*s2 + 'a' - 'A') : *s2;
				if (cmp1 != cmp2) {
					return cmp1 - cmp2;
				}
			}
		} else if ((len1 == 0) || (len2 == 0)) {
			/* Invalid UTF8 sequence. */
			return len1 - len2;
		} else {
			/* Otherwise convert to UCS4 for comparison. */
			cmp1 = cyaml_utf8_get_codepoint(s1, len1);
			cmp2 = cyaml_utf8_get_codepoint(s2, len2);

			if (cmp1 != cmp2) {
				/* They're different; need to lower case. */
				cmp1 = cyaml_utf8_to_lower(cmp1);
				cmp2 = cyaml_utf8_to_lower(cmp2);
				if (cmp1 != cmp2) {
					return cmp1 - cmp2;
				}
			}
		}

		/* Advance each string by their current character length. */
		while ((*s1 != 0) && len1) {
			len1--;
			s1++;
		}
		while ((*s2 != 0) && len2) {
			len2--;
			s2++;
		}
	}
}
