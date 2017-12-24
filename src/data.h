/*
 * SPDX-License-Identifier: ISC
 *
 * Copyright (C) 2017 Michael Drake <tlsa@netsurf-browser.org>
 */

/**
 * \file
 * \brief CYAML functions for manipulating client data structures.
 */

#ifndef CYAML_DATA_H
#define CYAML_DATA_H

#include "cyaml.h"

/**
 * Write a value of up to eight bytes to data_target.
 *
 * \param[in]  value        The value to write.
 * \param[in]  entry_size   The number of bytes of value to write.
 * \param[in]  data_target  The address to write to.
 * \return \ref CYAML_OK on success, or appropriate error code otherwise.
 */
static inline cyaml_err_t cyaml_data_write(
		uint64_t value,
		uint8_t entry_size,
		uint8_t *data_target)
{
	data_target += entry_size - 1;

	if (entry_size > 8 || entry_size == 0) {
		return CYAML_ERR_INVALID_DATA_SIZE;
	}

	switch (entry_size) {
	case 8: *data_target-- = (value >> 56) & 0xff; /* Fall through. */
	case 7: *data_target-- = (value >> 48) & 0xff; /* Fall through. */
	case 6: *data_target-- = (value >> 40) & 0xff; /* Fall through. */
	case 5: *data_target-- = (value >> 32) & 0xff; /* Fall through. */
	case 4: *data_target-- = (value >> 24) & 0xff; /* Fall through. */
	case 3: *data_target-- = (value >> 16) & 0xff; /* Fall through. */
	case 2: *data_target-- = (value >>  8) & 0xff; /* Fall through. */
	case 1: *data_target-- = (value >>  0) & 0xff;
	}

	return CYAML_OK;
}

/**
 * Read a value of up to eight bytes from data.
 *
 * \param[in]  entry_size  The number of bytes to read.
 * \param[in]  data        The address to read from.
 * \param[out] error_out   Returns the error code.  \ref CYAML_OK on success,
 *                         or appropriate error otherwise.
 * \return On success, returns the value read from data.
 *         On failure, returns 0.
 */
static inline uint64_t cyaml_data_read(
		uint8_t entry_size,
		const uint8_t *data,
		cyaml_err_t *error_out)
{
	uint64_t ret = 0;

	if (entry_size > 8 || entry_size == 0) {
		*error_out = CYAML_ERR_INVALID_DATA_SIZE;
		return ret;
	}

	data += entry_size - 1;

	switch (entry_size) {
	case 8: ret |= ((uint64_t)(*data-- & 0xff)) << 56; /* Fall through. */
	case 7: ret |= ((uint64_t)(*data-- & 0xff)) << 48; /* Fall through. */
	case 6: ret |= ((uint64_t)(*data-- & 0xff)) << 40; /* Fall through. */
	case 5: ret |= ((uint64_t)(*data-- & 0xff)) << 32; /* Fall through. */
	case 4: ret |= ((uint64_t)(*data-- & 0xff)) << 24; /* Fall through. */
	case 3: ret |= ((uint64_t)(*data-- & 0xff)) << 16; /* Fall through. */
	case 2: ret |= ((uint64_t)(*data-- & 0xff)) <<  8; /* Fall through. */
	case 1: ret |= ((uint64_t)(*data-- & 0xff)) <<  0;
	}

	*error_out = CYAML_OK;
	return ret;
}

#endif
