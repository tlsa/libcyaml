/*
 * SPDX-License-Identifier: ISC
 *
 * Copyright (C) 2018 Michael Drake <tlsa@netsurf-browser.org>
 */

/**
 * \file
 * \brief CYAML functions for handling utf8 text.
 */

#ifndef CYAML_UTF8_H
#define CYAML_UTF8_H

/**
 * Case insensitive comparason.
 *
 * \note This has some limitations and only performs case insensitive
 *       comparason over some sectons of unicode.
 *
 * \param[in]  str1  First string to be compared.
 * \param[in]  str2  Second string to be compared.
 * \return 0 if and only if strings are equal.
 */
int cyaml_utf8_casecmp(
		const void * const str1,
		const void * const str2);

#endif
