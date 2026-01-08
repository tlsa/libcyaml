/*
 * SPDX-License-Identifier: ISC
 *
 * Copyright (C) 2023 Michael Drake <tlsa@netsurf-browser.org>
 */

/**
 * \file
 * \brief CYAML functions for handling base64 encode and decode.
 */

#ifndef CYAML_BASE64_H
#define CYAML_BASE64_H

#include "cyaml/cyaml.h"

/**
 * Calculate the size of a base64 encoded string.
 *
 * \param[in]  data_len  The length of the data to encode.
 * \return The size of the base64 encoded string in bytes.
 *         Excludes space for any trailing '\0' character.
 */
size_t cyaml_base64_calc_encoded_size(
		size_t data_len);

/**
 * Encode data as base64.
 *
 * \param[in]  data      The data to encode.
 * \param[in]  data_len  The length of the data to encode.
 * \param[out] str       Pointer to buffer to receive encoded string.
 *                       Must be at least cyaml_base64_calc_encoded_size().
 *                       No trailing '\0' character is written.
 */
void cyaml_base64_encode(
		const uint8_t *data,
		size_t data_len,
		char *str);

/**
 * Calculate the size of a decoded base64 string.
 *
 * \param[in]  str      The base64 string to decode.
 * \param[in]  str_len  The length of the base64 string to decode.
 * \param[out] size     Pointer to variable to receive decoded size.
 * \return CYAML_OK on success, or appropriate error code otherwise.
 */
cyaml_err_t cyaml_base64_calc_decoded_size(
		const char *str,
		size_t str_len,
		size_t *size);

/**
 * Decode a base64 string.
 *
 * \param[in]  str      The base64 string to decode.
 * \param[in]  str_len  The length of the base64 string to decode.
 * \param[out] data     Pointer to buffer to receive decoded data.
 *                      Must be at least cyaml_base64_calc_decoded_size().
 */
void cyaml_base64_decode(
		const char *str,
		size_t str_len,
		uint8_t *data);

#endif
