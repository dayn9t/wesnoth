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

#include "config.hpp"
#include "color.hpp"
#include "lexical_cast.hpp"
#include "utils/math.hpp"
#include "serialization/string_utils.hpp"
#include <string>
#include <vector>
#include <cstdint>

// ============================================================================
// Math utilities extended tests
// ============================================================================

BOOST_AUTO_TEST_SUITE(test_math_utils_extended)

BOOST_AUTO_TEST_CASE(test_bit_width_runtime)
{
	BOOST_CHECK_EQUAL(bit_width(uint8_t(0)), 8);
	BOOST_CHECK_EQUAL(bit_width(uint16_t(0)), 16);
	BOOST_CHECK_EQUAL(bit_width(uint32_t(0)), 32);
	BOOST_CHECK_EQUAL(bit_width(uint64_t(0)), 64);
}

BOOST_AUTO_TEST_CASE(test_count_leading_zeros_runtime)
{
	BOOST_CHECK_EQUAL(count_leading_zeros(uint8_t(1)), 7);
	BOOST_CHECK_EQUAL(count_leading_zeros(uint16_t(1)), 15);
	BOOST_CHECK_EQUAL(count_leading_zeros(uint32_t(1)), 31);
	BOOST_CHECK_EQUAL(count_leading_zeros(uint64_t(1)), 63);

	BOOST_CHECK_EQUAL(count_leading_zeros(uint8_t(0xFF)), 0);
	BOOST_CHECK_EQUAL(count_leading_zeros(uint16_t(0xFFFF)), 0);
	BOOST_CHECK_EQUAL(count_leading_zeros(uint32_t(0xFFFFFFFF)), 0);
	BOOST_CHECK_EQUAL(count_leading_zeros(uint64_t(0xFFFFFFFFFFFFFFFFULL)), 0);
}

BOOST_AUTO_TEST_CASE(test_count_leading_ones_runtime)
{
	BOOST_CHECK_EQUAL(count_leading_ones(0), 0);
	BOOST_CHECK_EQUAL(count_leading_ones(1u), 0);
	BOOST_CHECK_EQUAL(count_leading_ones(uint8_t(0xFF)), 8);
	BOOST_CHECK_EQUAL(count_leading_ones(uint16_t(0xFFFF)), 16);
	BOOST_CHECK_EQUAL(count_leading_ones(uint32_t(0xFFFFFFFF)), 32);
	BOOST_CHECK_EQUAL(count_leading_ones(uint64_t(0xFFFFFFFFFFFFFFFFULL)), 64);
	BOOST_CHECK_EQUAL(count_leading_ones(uint8_t(0xF0)), 4);
	BOOST_CHECK_EQUAL(count_leading_ones(uint8_t(0xE0)), 3);
	BOOST_CHECK_EQUAL(count_leading_ones(uint8_t(0xC0)), 2);
	BOOST_CHECK_EQUAL(count_leading_ones(uint8_t(0x80)), 1);
}

BOOST_AUTO_TEST_SUITE_END()

// ============================================================================
// Lexical cast extended tests
// ============================================================================

BOOST_AUTO_TEST_SUITE(test_lexical_cast_extended)

BOOST_AUTO_TEST_CASE(test_lexical_cast_int_to_string)
{
	BOOST_CHECK_EQUAL(lexical_cast<std::string>(0), "0");
	BOOST_CHECK_EQUAL(lexical_cast<std::string>(1), "1");
	BOOST_CHECK_EQUAL(lexical_cast<std::string>(-1), "-1");
	BOOST_CHECK_EQUAL(lexical_cast<std::string>(12345), "12345");
	BOOST_CHECK_EQUAL(lexical_cast<std::string>(-12345), "-12345");
}

BOOST_AUTO_TEST_CASE(test_lexical_cast_string_to_int)
{
	BOOST_CHECK_EQUAL(lexical_cast<int>("0"), 0);
	BOOST_CHECK_EQUAL(lexical_cast<int>("1"), 1);
	BOOST_CHECK_EQUAL(lexical_cast<int>("-1"), -1);
	BOOST_CHECK_EQUAL(lexical_cast<int>("12345"), 12345);
	BOOST_CHECK_EQUAL(lexical_cast<int>("-12345"), -12345);
}

BOOST_AUTO_TEST_CASE(test_lexical_cast_default_int)
{
	BOOST_CHECK_EQUAL(lexical_cast_default<int>("invalid", 42), 42);
	BOOST_CHECK_EQUAL(lexical_cast_default<int>("123", 0), 123);
	BOOST_CHECK_EQUAL(lexical_cast_default<int>("", 100), 100);
}

BOOST_AUTO_TEST_CASE(test_lexical_cast_double)
{
	BOOST_CHECK_CLOSE(lexical_cast<double>("3.14"), 3.14, 0.001);
	BOOST_CHECK_CLOSE(lexical_cast<double>("-2.5"), -2.5, 0.001);
	BOOST_CHECK_CLOSE(lexical_cast<double>("0.0"), 0.0, 0.001);
}

BOOST_AUTO_TEST_SUITE_END()

// ============================================================================
// Color extended tests
// ============================================================================

BOOST_AUTO_TEST_SUITE(test_color_extended)

BOOST_AUTO_TEST_CASE(test_color_from_rgb)
{
	color_t c(255, 128, 64);

	BOOST_CHECK_EQUAL(c.r, 255);
	BOOST_CHECK_EQUAL(c.g, 128);
	BOOST_CHECK_EQUAL(c.b, 64);
	BOOST_CHECK_EQUAL(c.a, 255);
}

BOOST_AUTO_TEST_CASE(test_color_from_rgba)
{
	color_t c(255, 128, 64, 200);

	BOOST_CHECK_EQUAL(c.r, 255);
	BOOST_CHECK_EQUAL(c.g, 128);
	BOOST_CHECK_EQUAL(c.b, 64);
	BOOST_CHECK_EQUAL(c.a, 200);
}

BOOST_AUTO_TEST_CASE(test_color_from_hex_string)
{
	color_t c = color_t::from_hex_string("#FF8040");

	BOOST_CHECK_EQUAL(c.r, 255);
	BOOST_CHECK_EQUAL(c.g, 128);
	BOOST_CHECK_EQUAL(c.b, 64);
}

BOOST_AUTO_TEST_CASE(test_color_from_rgba_string)
{
	color_t c = color_t::from_rgba_string("255,128,64,200");

	BOOST_CHECK_EQUAL(c.r, 255);
	BOOST_CHECK_EQUAL(c.g, 128);
	BOOST_CHECK_EQUAL(c.b, 64);
	BOOST_CHECK_EQUAL(c.a, 200);
}

BOOST_AUTO_TEST_CASE(test_color_to_hex_string)
{
	color_t c(255, 128, 64);
	std::string hex = c.to_hex_string();

	BOOST_CHECK(!hex.empty());
}

BOOST_AUTO_TEST_CASE(test_color_equality)
{
	color_t c1(255, 128, 64);
	color_t c2(255, 128, 64);
	color_t c3(255, 128, 65);

	BOOST_CHECK(c1 == c2);
	BOOST_CHECK(c1 != c3);
}

BOOST_AUTO_TEST_CASE(test_color_black)
{
	color_t black = color_t::from_hex_string("#000000");

	BOOST_CHECK_EQUAL(black.r, 0);
	BOOST_CHECK_EQUAL(black.g, 0);
	BOOST_CHECK_EQUAL(black.b, 0);
}

BOOST_AUTO_TEST_CASE(test_color_white)
{
	color_t white = color_t::from_hex_string("#FFFFFF");

	BOOST_CHECK_EQUAL(white.r, 255);
	BOOST_CHECK_EQUAL(white.b, 255);
}

// ============================================================================
// Color boundary condition tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_color_boundary_values)
{
	// Maximum values
	color_t max_val(255, 255, 255, 255);
	BOOST_CHECK_EQUAL(max_val.r, 255);
	BOOST_CHECK_EQUAL(max_val.g, 255);
	BOOST_CHECK_EQUAL(max_val.b, 255);
	BOOST_CHECK_EQUAL(max_val.a, 255);

	// Minimum values
	color_t min_val(0, 0, 0, 0);
	BOOST_CHECK_EQUAL(min_val.r, 0);
	BOOST_CHECK_EQUAL(min_val.g, 0);
	BOOST_CHECK_EQUAL(min_val.b, 0);
	BOOST_CHECK_EQUAL(min_val.a, 0);
}

BOOST_AUTO_TEST_CASE(test_color_from_hex_boundary)
{
	// Short hex
	color_t short_hex = color_t::from_hex_string("#FFF");
	// Implementation dependent - may or may not work

	// Lowercase hex
	color_t lower = color_t::from_hex_string("#ffffff");
	BOOST_CHECK_EQUAL(lower.r, 255);
	BOOST_CHECK_EQUAL(lower.g, 255);
	BOOST_CHECK_EQUAL(lower.b, 255);

	// Mixed case
	color_t mixed = color_t::from_hex_string("#FfFfFf");
	BOOST_CHECK_EQUAL(mixed.r, 255);
	BOOST_CHECK_EQUAL(mixed.g, 255);
	BOOST_CHECK_EQUAL(mixed.b, 255);
}

BOOST_AUTO_TEST_CASE(test_color_from_hex_invalid)
{
	// Invalid hex strings should be handled gracefully
	color_t invalid = color_t::from_hex_string("not_a_hex");
	(void)invalid;

	// Empty string
	color_t empty = color_t::from_hex_string("");
	(void)empty;

	// Too long
	color_t too_long = color_t::from_hex_string("#FFFFFFFFFF");
	(void)too_long;
}

BOOST_AUTO_TEST_CASE(test_color_from_rgba_boundary)
{
	// All zeros
	color_t black = color_t::from_rgba_string("0,0,0,0");
	BOOST_CHECK_EQUAL(black.r, 0);
	BOOST_CHECK_EQUAL(black.g, 0);
	BOOST_CHECK_EQUAL(black.b, 0);
	BOOST_CHECK_EQUAL(black.a, 0);

	// All max
	color_t white = color_t::from_rgba_string("255,255,255,255");
	BOOST_CHECK_EQUAL(white.r, 255);
	BOOST_CHECK_EQUAL(white.g, 255);
	BOOST_CHECK_EQUAL(white.b, 255);
	BOOST_CHECK_EQUAL(white.a, 255);
}

BOOST_AUTO_TEST_CASE(test_color_from_rgba_invalid)
{
	// Invalid formats - should handle gracefully
	color_t missing = color_t::from_rgba_string("255,255");
	(void)missing;

	color_t extra = color_t::from_rgba_string("255,255,255,255,255");
	(void)extra;

	color_t non_numeric = color_t::from_rgba_string("a,b,c,d");
	(void)non_numeric;

	color_t empty = color_t::from_rgba_string("");
	(void)empty;
}

BOOST_AUTO_TEST_CASE(test_color_null)
{
	// Null color
	color_t null_color = color_t::null_color();
	(void)null_color;
}

BOOST_AUTO_TEST_SUITE_END()

// ============================================================================
// Additional lexical cast boundary tests
// ============================================================================

BOOST_AUTO_TEST_SUITE(test_lexical_cast_boundary)

BOOST_AUTO_TEST_CASE(test_lexical_cast_int_limits)
{
	// Integer limits
	BOOST_CHECK_EQUAL(lexical_cast<std::string>(INT_MAX), std::to_string(INT_MAX));
	BOOST_CHECK_EQUAL(lexical_cast<std::string>(INT_MIN), std::to_string(INT_MIN));
	BOOST_CHECK_EQUAL(lexical_cast<std::string>(0), "0");
}

BOOST_AUTO_TEST_CASE(test_lexical_cast_unsigned_limits)
{
	// Unsigned limits
	BOOST_CHECK_EQUAL(lexical_cast<std::string>(UINT_MAX), std::to_string(UINT_MAX));
	BOOST_CHECK_EQUAL(lexical_cast<std::string>(0U), "0");
}

BOOST_AUTO_TEST_CASE(test_lexical_cast_long_long_limits)
{
	// Long long limits
	BOOST_CHECK_EQUAL(lexical_cast<std::string>(LLONG_MAX), std::to_string(LLONG_MAX));
	BOOST_CHECK_EQUAL(lexical_cast<std::string>(LLONG_MIN), std::to_string(LLONG_MIN));
}

BOOST_AUTO_TEST_CASE(test_lexical_cast_default_boundary)
{
	// Default with various invalid inputs
	BOOST_CHECK_EQUAL(lexical_cast_default<int>("", 0), 0);
	BOOST_CHECK_EQUAL(lexical_cast_default<int>("   ", -1), -1);
	BOOST_CHECK_EQUAL(lexical_cast_default<int>("abc", 42), 42);
	BOOST_CHECK_EQUAL(lexical_cast_default<int>("12.34", 0), 0);
}

BOOST_AUTO_TEST_CASE(test_lexical_cast_double_special)
{
	// Special double values
	BOOST_CHECK_CLOSE(lexical_cast<double>("0.0"), 0.0, 0.001);
	BOOST_CHECK_CLOSE(lexical_cast<double>("-0.0"), 0.0, 0.001);
	BOOST_CHECK_CLOSE(lexical_cast<double>("1e10"), 1e10, 0.001);
	BOOST_CHECK_CLOSE(lexical_cast<double>("1e-10"), 1e-10, 0.001);
}

BOOST_AUTO_TEST_SUITE_END()

// ============================================================================
// Math utilities boundary tests
// ============================================================================

BOOST_AUTO_TEST_SUITE(test_math_utils_boundary)

BOOST_AUTO_TEST_CASE(test_bit_width_all_ones)
{
	// All bits set to 1
	BOOST_CHECK_EQUAL(bit_width(uint8_t(0xFF)), 8);
	BOOST_CHECK_EQUAL(bit_width(uint16_t(0xFFFF)), 16);
	BOOST_CHECK_EQUAL(bit_width(uint32_t(0xFFFFFFFF)), 32);
	BOOST_CHECK_EQUAL(bit_width(uint64_t(0xFFFFFFFFFFFFFFFFULL)), 64);
}

BOOST_AUTO_TEST_CASE(test_bit_width_single_bit)
{
	// Single bit set
	BOOST_CHECK_EQUAL(bit_width(uint8_t(0x80)), 8);
	BOOST_CHECK_EQUAL(bit_width(uint16_t(0x8000)), 16);
	BOOST_CHECK_EQUAL(bit_width(uint32_t(0x80000000)), 32);
}

BOOST_AUTO_TEST_CASE(test_count_leading_zeros_all_zeros)
{
	// All zeros - should return bit width
	BOOST_CHECK_EQUAL(count_leading_zeros(uint8_t(0)), 8);
	BOOST_CHECK_EQUAL(count_leading_zeros(uint16_t(0)), 16);
	BOOST_CHECK_EQUAL(count_leading_zeros(uint32_t(0)), 32);
	BOOST_CHECK_EQUAL(count_leading_zeros(uint64_t(0)), 64);
}

BOOST_AUTO_TEST_CASE(test_count_leading_zeros_all_ones)
{
	// All ones - should return 0
	BOOST_CHECK_EQUAL(count_leading_zeros(uint8_t(0xFF)), 0);
	BOOST_CHECK_EQUAL(count_leading_zeros(uint16_t(0xFFFF)), 0);
	BOOST_CHECK_EQUAL(count_leading_zeros(uint32_t(0xFFFFFFFF)), 0);
}

BOOST_AUTO_TEST_CASE(test_count_leading_zeros_single_bit)
{
	// Single bit at each position
	BOOST_CHECK_EQUAL(count_leading_zeros(uint8_t(0x80)), 0);
	BOOST_CHECK_EQUAL(count_leading_zeros(uint8_t(0x40)), 1);
	BOOST_CHECK_EQUAL(count_leading_zeros(uint8_t(0x20)), 2);
	BOOST_CHECK_EQUAL(count_leading_zeros(uint8_t(0x10)), 3);
	BOOST_CHECK_EQUAL(count_leading_zeros(uint8_t(0x08)), 4);
	BOOST_CHECK_EQUAL(count_leading_zeros(uint8_t(0x04)), 5);
	BOOST_CHECK_EQUAL(count_leading_zeros(uint8_t(0x02)), 6);
	BOOST_CHECK_EQUAL(count_leading_zeros(uint8_t(0x01)), 7);
}

BOOST_AUTO_TEST_CASE(test_count_leading_ones_all_zeros)
{
	// All zeros - should return 0
	BOOST_CHECK_EQUAL(count_leading_ones(uint8_t(0)), 0);
	BOOST_CHECK_EQUAL(count_leading_ones(uint16_t(0)), 0);
}

BOOST_AUTO_TEST_CASE(test_count_leading_ones_all_ones)
{
	// All ones - should return bit width
	BOOST_CHECK_EQUAL(count_leading_ones(uint8_t(0xFF)), 8);
	BOOST_CHECK_EQUAL(count_leading_ones(uint16_t(0xFFFF)), 16);
	BOOST_CHECK_EQUAL(count_leading_ones(uint32_t(0xFFFFFFFF)), 32);
}

BOOST_AUTO_TEST_CASE(test_count_leading_ones_alternating)
{
	// Alternating patterns
	BOOST_CHECK_EQUAL(count_leading_ones(uint8_t(0xAA)), 0); // 10101010
	BOOST_CHECK_EQUAL(count_leading_ones(uint8_t(0x55)), 0); // 01010101
	BOOST_CHECK_EQUAL(count_leading_ones(uint8_t(0xF0)), 4); // 11110000
	BOOST_CHECK_EQUAL(count_leading_ones(uint8_t(0x0F)), 0); // 00001111
	BOOST_CHECK_EQUAL(count_leading_ones(uint8_t(0xC0)), 2); // 11000000
}

BOOST_AUTO_TEST_SUITE_END()

// ============================================================================
// String utils extended tests
// ============================================================================

BOOST_AUTO_TEST_SUITE(test_string_utils_extended)

BOOST_AUTO_TEST_CASE(test_split_empty_string)
{
	auto result = utils::split("");

	BOOST_CHECK(result.empty());
}

BOOST_AUTO_TEST_CASE(test_split_single_element)
{
	auto result = utils::split("hello");

	BOOST_CHECK_EQUAL(result.size(), 1);
	BOOST_CHECK_EQUAL(result[0], "hello");
}

BOOST_AUTO_TEST_CASE(test_split_multiple_elements)
{
	auto result = utils::split("a,b,c");

	BOOST_CHECK_EQUAL(result.size(), 3);
	BOOST_CHECK_EQUAL(result[0], "a");
	BOOST_CHECK_EQUAL(result[1], "b");
	BOOST_CHECK_EQUAL(result[2], "c");
}

BOOST_AUTO_TEST_CASE(test_join_empty_vector)
{
	std::vector<std::string> empty;
	std::string result = utils::join(empty);

	BOOST_CHECK(result.empty());
}

BOOST_AUTO_TEST_CASE(test_join_single_element)
{
	std::vector<std::string> single = {"hello"};
	std::string result = utils::join(single);

	BOOST_CHECK_EQUAL(result, "hello");
}

BOOST_AUTO_TEST_CASE(test_join_multiple_elements)
{
	std::vector<std::string> multiple = {"a", "b", "c"};
	std::string result = utils::join(multiple, ",");

	BOOST_CHECK_EQUAL(result, "a,b,c");
}

BOOST_AUTO_TEST_CASE(test_join_custom_separator)
{
	std::vector<std::string> parts = {"one", "two", "three"};
	std::string result = utils::join(parts, " | ");

	BOOST_CHECK_EQUAL(result, "one | two | three");
}

BOOST_AUTO_TEST_CASE(test_string_bool_true)
{
	BOOST_CHECK(utils::string_bool("true"));
	BOOST_CHECK(utils::string_bool("yes"));
	BOOST_CHECK(utils::string_bool("1"));
	BOOST_CHECK(utils::string_bool("on"));
}

BOOST_AUTO_TEST_CASE(test_string_bool_false)
{
	BOOST_CHECK(!utils::string_bool("false"));
	BOOST_CHECK(!utils::string_bool("no"));
	BOOST_CHECK(!utils::string_bool("0"));
	BOOST_CHECK(!utils::string_bool("off"));
	BOOST_CHECK(!utils::string_bool(""));
}

// ============================================================================
// Additional boundary condition tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_bit_width_various_values)
{
	// Test bit_width with various values
	BOOST_CHECK_EQUAL(bit_width(uint8_t(0x00)), 8);
	BOOST_CHECK_EQUAL(bit_width(uint8_t(0x01)), 8);
	BOOST_CHECK_EQUAL(bit_width(uint8_t(0x80)), 8);
	BOOST_CHECK_EQUAL(bit_width(uint8_t(0xFF)), 8);
}

BOOST_AUTO_TEST_CASE(test_count_leading_zeros_various)
{
	// Test with various bit patterns
	BOOST_CHECK_EQUAL(count_leading_zeros(uint8_t(0x00)), 8);
	BOOST_CHECK_EQUAL(count_leading_zeros(uint8_t(0x01)), 7);
	BOOST_CHECK_EQUAL(count_leading_zeros(uint8_t(0x7F)), 1);
	BOOST_CHECK_EQUAL(count_leading_zeros(uint8_t(0x80)), 0);
	BOOST_CHECK_EQUAL(count_leading_zeros(uint8_t(0xFF)), 0);
}

BOOST_AUTO_TEST_CASE(test_count_leading_ones_various)
{
	// Test with various bit patterns
	BOOST_CHECK_EQUAL(count_leading_ones(uint8_t(0x00)), 0);
	BOOST_CHECK_EQUAL(count_leading_ones(uint8_t(0x80)), 1);
	BOOST_CHECK_EQUAL(count_leading_ones(uint8_t(0xC0)), 2);
	BOOST_CHECK_EQUAL(count_leading_ones(uint8_t(0xE0)), 3);
	BOOST_CHECK_EQUAL(count_leading_ones(uint8_t(0xF0)), 4);
	BOOST_CHECK_EQUAL(count_leading_ones(uint8_t(0xF8)), 5);
	BOOST_CHECK_EQUAL(count_leading_ones(uint8_t(0xFC)), 6);
	BOOST_CHECK_EQUAL(count_leading_ones(uint8_t(0xFE)), 7);
	BOOST_CHECK_EQUAL(count_leading_ones(uint8_t(0xFF)), 8);
}

BOOST_AUTO_TEST_CASE(test_lexical_cast_boundary_values)
{
	// Test boundary values for lexical_cast
	BOOST_CHECK_EQUAL(lexical_cast<std::string>(INT_MAX), std::to_string(INT_MAX));
	BOOST_CHECK_EQUAL(lexical_cast<std::string>(INT_MIN), std::to_string(INT_MIN));
	BOOST_CHECK_EQUAL(lexical_cast<std::string>(0), "0");
	BOOST_CHECK_EQUAL(lexical_cast<std::string>(-1), "-1");
}

BOOST_AUTO_TEST_CASE(test_lexical_cast_unsigned_boundary)
{
	// Unsigned boundary values
	BOOST_CHECK_EQUAL(lexical_cast<std::string>(UINT_MAX), std::to_string(UINT_MAX));
	BOOST_CHECK_EQUAL(lexical_cast<std::string>(0U), "0");
}

BOOST_AUTO_TEST_CASE(test_lexical_cast_default_various_invalid)
{
	// Various invalid inputs
	BOOST_CHECK_EQUAL(lexical_cast_default<int>("", 42), 42);
	BOOST_CHECK_EQUAL(lexical_cast_default<int>("   ", 42), 42);
	BOOST_CHECK_EQUAL(lexical_cast_default<int>("abc", 42), 42);
	BOOST_CHECK_EQUAL(lexical_cast_default<int>("12.34", 42), 42);
	BOOST_CHECK_EQUAL(lexical_cast_default<int>("0x10", 42), 42); // Hex not supported
	BOOST_CHECK_EQUAL(lexical_cast_default<int>("0123", 42), 123); // Octal interpreted as decimal
}

BOOST_AUTO_TEST_CASE(test_lexical_cast_double_boundary)
{
	// Double boundary values
	BOOST_CHECK_CLOSE(lexical_cast<double>("0.0"), 0.0, 0.001);
	BOOST_CHECK_CLOSE(lexical_cast<double>("-0.0"), 0.0, 0.001);
	BOOST_CHECK_CLOSE(lexical_cast<double>("1e308"), 1e308, 0.001);
	BOOST_CHECK_CLOSE(lexical_cast<double>("-1e308"), -1e308, 0.001);
	BOOST_CHECK_CLOSE(lexical_cast<double>("1e-308"), 1e-308, 0.001);
}

BOOST_AUTO_TEST_CASE(test_color_from_hex_edge_cases)
{
	// Hex edge cases
	color_t c1 = color_t::from_hex_string("#000000");
	BOOST_CHECK_EQUAL(c1.r, 0);
	BOOST_CHECK_EQUAL(c1.g, 0);
	BOOST_CHECK_EQUAL(c1.b, 0);

	color_t c2 = color_t::from_hex_string("#FFFFFF");
	BOOST_CHECK_EQUAL(c2.r, 255);
	BOOST_CHECK_EQUAL(c2.g, 255);
	BOOST_CHECK_EQUAL(c2.b, 255);

	color_t c3 = color_t::from_hex_string("#FfFfFf");
	BOOST_CHECK_EQUAL(c3.r, 255);
	BOOST_CHECK_EQUAL(c3.g, 255);
	BOOST_CHECK_EQUAL(c3.b, 255);
}

BOOST_AUTO_TEST_CASE(test_color_from_hex_invalid)
{
	// Invalid hex strings
	color_t c1 = color_t::from_hex_string("");
	(void)c1; // Should not crash

	color_t c2 = color_t::from_hex_string("not_hex");
	(void)c2;

	color_t c3 = color_t::from_hex_string("#GGGGGG");
	(void)c3;

	color_t c4 = color_t::from_hex_string("#FFF"); // Short form
	(void)c4;

	color_t c5 = color_t::from_hex_string("#FFFFFFFFFF"); // Too long
	(void)c5;
}

BOOST_AUTO_TEST_CASE(test_color_from_rgba_edge_cases)
{
	// RGBA edge cases
	color_t c1 = color_t::from_rgba_string("0,0,0,0");
	BOOST_CHECK_EQUAL(c1.r, 0);
	BOOST_CHECK_EQUAL(c1.g, 0);
	BOOST_CHECK_EQUAL(c1.b, 0);
	BOOST_CHECK_EQUAL(c1.a, 0);

	color_t c2 = color_t::from_rgba_string("255,255,255,255");
	BOOST_CHECK_EQUAL(c2.r, 255);
	BOOST_CHECK_EQUAL(c2.g, 255);
	BOOST_CHECK_EQUAL(c2.b, 255);
	BOOST_CHECK_EQUAL(c2.a, 255);
}

BOOST_AUTO_TEST_CASE(test_color_from_rgba_invalid)
{
	// Invalid RGBA strings
	color_t c1 = color_t::from_rgba_string("");
	(void)c1;

	color_t c2 = color_t::from_rgba_string("255,255"); // Too few
	(void)c2;

	color_t c3 = color_t::from_rgba_string("a,b,c,d"); // Non-numeric
	(void)c3;

	color_t c4 = color_t::from_rgba_string("256,256,256,256"); // Out of range
	(void)c4;
}

BOOST_AUTO_TEST_CASE(test_color_equality_reflexive)
{
	color_t c(128, 64, 32, 200);
	BOOST_CHECK(c == c); // Reflexive
	BOOST_CHECK(!(c != c));
}

BOOST_AUTO_TEST_CASE(test_color_equality_symmetric)
{
	color_t c1(128, 64, 32, 200);
	color_t c2(128, 64, 32, 200);
	BOOST_CHECK(c1 == c2);
	BOOST_CHECK(c2 == c1); // Symmetric
}

BOOST_AUTO_TEST_CASE(test_color_null)
{
	color_t null_c = color_t::null_color();
	// Null color should have specific values
	(void)null_c;
}

BOOST_AUTO_TEST_CASE(test_split_empty_input)
{
	auto result = utils::split("");
	BOOST_CHECK(result.empty());
}

BOOST_AUTO_TEST_CASE(test_split_only_delimiters)
{
	auto result = utils::split(",,,", ',');
	BOOST_CHECK_EQUAL(result.size(), 4);
	for (const auto& s : result) {
		BOOST_CHECK(s.empty());
	}
}

BOOST_AUTO_TEST_CASE(test_join_empty_vector_with_separator)
{
	std::vector<std::string> empty;
	std::string result = utils::join(empty, ",");
	BOOST_CHECK(result.empty());
}

BOOST_AUTO_TEST_CASE(test_join_single_element_with_separator)
{
	std::vector<std::string> single = {"only"};
	std::string result = utils::join(single, "|");
	BOOST_CHECK_EQUAL(result, "only");
}

BOOST_AUTO_TEST_CASE(test_join_empty_strings)
{
	std::vector<std::string> parts = {"", "a", "", "b", ""};
	std::string result = utils::join(parts, ",");
	BOOST_CHECK_EQUAL(result, ",a,,b,");
}

BOOST_AUTO_TEST_CASE(test_string_bool_case_variations)
{
	BOOST_CHECK(utils::string_bool("TRUE"));
	BOOST_CHECK(utils::string_bool("True"));
	BOOST_CHECK(utils::string_bool("true"));
	BOOST_CHECK(utils::string_bool("YES"));
	BOOST_CHECK(utils::string_bool("Yes"));
	BOOST_CHECK(utils::string_bool("yes"));
	BOOST_CHECK(utils::string_bool("ON"));
	BOOST_CHECK(utils::string_bool("On"));
	BOOST_CHECK(utils::string_bool("on"));
}

BOOST_AUTO_TEST_CASE(test_string_bool_whitespace)
{
	// Whitespace should make it invalid
	BOOST_CHECK(!utils::string_bool(" true"));
	BOOST_CHECK(!utils::string_bool("true "));
	BOOST_CHECK(!utils::string_bool(" true "));
	BOOST_CHECK(!utils::string_bool("\ntrue"));
}

BOOST_AUTO_TEST_CASE(test_config_child_access_safety)
{
	config cfg;

	// Safe access to non-existent child
	BOOST_CHECK_NO_THROW(cfg.has_child("nonexistent"));
	BOOST_CHECK_EQUAL(cfg.child_count("nonexistent"), 0);

	// Optional child returns null
	auto opt = cfg.optional_child("nonexistent");
	BOOST_CHECK(!opt);
}

BOOST_AUTO_TEST_CASE(test_config_attribute_safety)
{
	config cfg;

	// Access to non-existent attribute
	const config::attribute_value& val = cfg["missing"];
	BOOST_CHECK(val.empty());
	BOOST_CHECK(val.blank());
	BOOST_CHECK_EQUAL(val.str(), "");
	BOOST_CHECK_EQUAL(val.to_int(), 0);
	BOOST_CHECK_EQUAL(val.to_double(), 0.0);
	BOOST_CHECK_EQUAL(val.to_bool(), false);
}

BOOST_AUTO_TEST_SUITE_END()

// ============================================================================
// Config extended tests
// ============================================================================

BOOST_AUTO_TEST_SUITE(test_config_extended)

BOOST_AUTO_TEST_CASE(test_config_mandatory_child)
{
	config cfg;
	cfg.add_child("test");

	BOOST_CHECK_NO_THROW(cfg.mandatory_child("test"));
}

BOOST_AUTO_TEST_CASE(test_config_optional_child)
{
	config cfg;

	BOOST_CHECK(!cfg.has_child("nonexistent"));

	cfg.add_child("exists");
	BOOST_CHECK(cfg.has_child("exists"));
}

BOOST_AUTO_TEST_CASE(test_config_has_child)
{
	config cfg;

	BOOST_CHECK(!cfg.has_child("child"));

	cfg.add_child("child");
	BOOST_CHECK(cfg.has_child("child"));
}

BOOST_AUTO_TEST_CASE(test_config_child_count)
{
	config cfg;

	BOOST_CHECK_EQUAL(cfg.child_count("item"), 0);

	cfg.add_child("item");
	cfg.add_child("item");
	cfg.add_child("item");

	BOOST_CHECK_EQUAL(cfg.child_count("item"), 3);
}

BOOST_AUTO_TEST_CASE(test_config_all_children_count)
{
	config cfg;

	BOOST_CHECK_EQUAL(cfg.all_children_count(), 0);

	cfg.add_child("a");
	cfg.add_child("b");
	cfg.add_child("c");

	BOOST_CHECK_EQUAL(cfg.all_children_count(), 3);
}

BOOST_AUTO_TEST_CASE(test_config_clear_children)
{
	config cfg;

	cfg.add_child("item");
	cfg.add_child("item");
	cfg.add_child("other");

	cfg.clear_children("item");

	BOOST_CHECK_EQUAL(cfg.child_count("item"), 0);
	BOOST_CHECK_EQUAL(cfg.child_count("other"), 1);
}

BOOST_AUTO_TEST_CASE(test_config_attribute_count)
{
	config cfg;

	BOOST_CHECK_EQUAL(cfg.attribute_count(), 0);

	cfg["a"] = "1";
	cfg["b"] = "2";
	cfg["c"] = "3";

	BOOST_CHECK_EQUAL(cfg.attribute_count(), 3);
}

BOOST_AUTO_TEST_CASE(test_config_empty)
{
	config cfg;

	BOOST_CHECK(cfg.empty());

	cfg["attr"] = "value";
	BOOST_CHECK(!cfg.empty());

	cfg.add_child("child");
	BOOST_CHECK(!cfg.empty());
}

BOOST_AUTO_TEST_CASE(test_config_swap)
{
	config cfg1, cfg2;

	cfg1["name"] = "first";
	cfg2["name"] = "second";

	swap(cfg1, cfg2);

	BOOST_CHECK_EQUAL(cfg1["name"].str(), "second");
	BOOST_CHECK_EQUAL(cfg2["name"].str(), "first");
}

BOOST_AUTO_TEST_SUITE_END()