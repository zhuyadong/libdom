/*
 * This file is part of libdom.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2007 John-Mark Bell <jmb@netsurf-browser.org>
 */

/** \file
 * UTF-16 manipulation functions (interface).
 */

#ifndef dom_utils_utf16_h_
#define dom_utils_utf16_h_

#include <inttypes.h>

#include "utils/charset_errors.h"

inline charset_error _dom_utf16_to_ucs4(const uint8_t *s, size_t len,
		uint32_t *ucs4, size_t *clen);
inline charset_error _dom_utf16_from_ucs4(uint32_t ucs4, uint8_t *s,
		size_t *len);

inline charset_error _dom_utf16_length(const uint8_t *s, size_t max,
		size_t *len);
inline charset_error _dom_utf16_char_byte_length(const uint8_t *s,
		size_t *len);

inline charset_error _dom_utf16_prev(const uint8_t *s, uint32_t off,
		uint32_t *prevoff);
inline charset_error _dom_utf16_next(const uint8_t *s, uint32_t len,
		uint32_t off, uint32_t *nextoff);

inline charset_error _dom_utf16_next_paranoid(const uint8_t *s,
		uint32_t len, uint32_t off, uint32_t *nextoff);

#endif
