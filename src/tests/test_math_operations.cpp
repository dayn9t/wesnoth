/*
	Copyright (C) 2025
	Part of the Battle for Wesnoth Project https://www.wesnoth.org/

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY.

	See the COPYING file for more details.
*/

#define GETTEXT_DOMAIN "wesnoth-test"

#include <boost/test/unit_test.hpp>

#include "utils/math.hpp"
#include <cstdint>

BOOST_AUTO_TEST_SUITE(test_math_operations)

BOOST_AUTO_TEST_CASE(test_bit_width_uint8)
{
	BOOST_CHECK_EQUAL(bit_width(uint8_t(0)), 8);
	BOOST_CHECK_EQUAL(bit_width(uint8_t(1)), 8);
	BOOST_CHECK_EQUAL(bit_width(uint8_t(255)), 8);
}

BOOST_AUTO_TEST_CASE(test_bit_width_uint16)
{
	BOOST_CHECK_EQUAL(bit_width(uint16_t(0)), 16);
	BOOST_CHECK_EQUAL(bit_width(uint16_t(1)), 16);
	BOOST_CHECK_EQUAL(bit_width(uint16_t(65535)), 16);
}

BOOST_AUTO_TEST_CASE(test_bit_width_uint32)
{
	BOOST_CHECK_EQUAL(bit_width(uint32_t(0)), 32);
	BOOST_CHECK_EQUAL(bit_width(uint32_t(1)), 32);
}

BOOST_AUTO_TEST_CASE(test_bit_width_uint64)
{
	BOOST_CHECK_EQUAL(bit_width(uint64_t(0)), 64);
	BOOST_CHECK_EQUAL(bit_width(uint64_t(1)), 64);
}

BOOST_AUTO_TEST_CASE(test_count_leading_zeros_uint8)
{
	BOOST_CHECK_EQUAL(count_leading_zeros(uint8_t(1)), 7);
	BOOST_CHECK_EQUAL(count_leading_zeros(uint8_t(2)), 6);
	BOOST_CHECK_EQUAL(count_leading_zeros(uint8_t(128)), 0);
	BOOST_CHECK_EQUAL(count_leading_zeros(uint8_t(255)), 0);
}

BOOST_AUTO_TEST_CASE(test_count_leading_zeros_uint16)
{
	BOOST_CHECK_EQUAL(count_leading_zeros(uint16_t(1)), 15);
	BOOST_CHECK_EQUAL(count_leading_zeros(uint16_t(256)), 7);
	BOOST_CHECK_EQUAL(count_leading_zeros(uint16_t(65535)), 0);
}

BOOST_AUTO_TEST_CASE(test_count_leading_zeros_uint32)
{
	BOOST_CHECK_EQUAL(count_leading_zeros(uint32_t(1)), 31);
	BOOST_CHECK_EQUAL(count_leading_zeros(uint32_t(65536)), 15);
}

BOOST_AUTO_TEST_CASE(test_count_leading_zeros_uint64)
{
	BOOST_CHECK_EQUAL(count_leading_zeros(uint64_t(1)), 63);
	BOOST_CHECK_EQUAL(count_leading_zeros(uint64_t(0xFFFFFFFF)), 32);
}

BOOST_AUTO_TEST_CASE(test_count_leading_ones_zero)
{
	BOOST_CHECK_EQUAL(count_leading_ones(0), 0);
	BOOST_CHECK_EQUAL(count_leading_ones(1u), 0);
}

BOOST_AUTO_TEST_CASE(test_count_leading_ones_all_ones)
{
	BOOST_CHECK_EQUAL(count_leading_ones(uint8_t(0xFF)), 8);
	BOOST_CHECK_EQUAL(count_leading_ones(uint16_t(0xFFFF)), 16);
	BOOST_CHECK_EQUAL(count_leading_ones(uint32_t(0xFFFFFFFF)), 32);
}

BOOST_AUTO_TEST_CASE(test_count_leading_ones_partial)
{
	BOOST_CHECK_EQUAL(count_leading_ones(uint8_t(0xF0)), 4);
	BOOST_CHECK_EQUAL(count_leading_ones(uint8_t(0xE0)), 3);
	BOOST_CHECK_EQUAL(count_leading_ones(uint8_t(0xC0)), 2);
	BOOST_CHECK_EQUAL(count_leading_ones(uint8_t(0x80)), 1);
}

BOOST_AUTO_TEST_CASE(test_is_even)
{
	BOOST_CHECK(is_even(0));
	BOOST_CHECK(is_even(2));
	BOOST_CHECK(is_even(4));
	BOOST_CHECK(is_even(-2));
	BOOST_CHECK(!is_even(1));
	BOOST_CHECK(!is_even(3));
	BOOST_CHECK(!is_even(-1));
}

BOOST_AUTO_TEST_CASE(test_is_odd)
{
	BOOST_CHECK(is_odd(1));
	BOOST_CHECK(is_odd(3));
	BOOST_CHECK(is_odd(-1));
	BOOST_CHECK(!is_odd(0));
	BOOST_CHECK(!is_odd(2));
	BOOST_CHECK(!is_odd(-2));
}

BOOST_AUTO_TEST_SUITE_END()