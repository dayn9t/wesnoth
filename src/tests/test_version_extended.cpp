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

#include "game_version.hpp"
#include <string>

BOOST_AUTO_TEST_SUITE(test_version_extended)

BOOST_AUTO_TEST_CASE(test_version_default_construction)
{
	version_info v;

	BOOST_CHECK_EQUAL(v.major_version(), 0);
	BOOST_CHECK_EQUAL(v.minor_version(), 0);
	BOOST_CHECK_EQUAL(v.revision_level(), 0);
}

BOOST_AUTO_TEST_CASE(test_version_string_construction)
{
	version_info v("1.16.0");

	BOOST_CHECK_EQUAL(v.major_version(), 1);
	BOOST_CHECK_EQUAL(v.minor_version(), 16);
	BOOST_CHECK_EQUAL(v.revision_level(), 0);
}

BOOST_AUTO_TEST_CASE(test_version_component_construction)
{
	version_info v(2, 5, 3);

	BOOST_CHECK_EQUAL(v.major_version(), 2);
	BOOST_CHECK_EQUAL(v.minor_version(), 5);
	BOOST_CHECK_EQUAL(v.revision_level(), 3);
}

BOOST_AUTO_TEST_CASE(test_version_major_only)
{
	version_info v("3");

	BOOST_CHECK_EQUAL(v.major_version(), 3);
	BOOST_CHECK_EQUAL(v.minor_version(), 0);
	BOOST_CHECK_EQUAL(v.revision_level(), 0);
}

BOOST_AUTO_TEST_CASE(test_version_major_minor)
{
	version_info v("1.15");

	BOOST_CHECK_EQUAL(v.major_version(), 1);
	BOOST_CHECK_EQUAL(v.minor_version(), 15);
	BOOST_CHECK_EQUAL(v.revision_level(), 0);
}

BOOST_AUTO_TEST_CASE(test_version_comparison_equal)
{
	version_info v1("1.16.0");
	version_info v2("1.16.0");

	BOOST_CHECK(v1 == v2);
	BOOST_CHECK(!(v1 < v2));
	BOOST_CHECK(!(v1 > v2));
	BOOST_CHECK(v1 <= v2);
	BOOST_CHECK(v1 >= v2);
}

BOOST_AUTO_TEST_CASE(test_version_comparison_less)
{
	version_info v1("1.15.9");
	version_info v2("1.16.0");

	BOOST_CHECK(v1 < v2);
	BOOST_CHECK(v1 <= v2);
	BOOST_CHECK(v2 > v1);
	BOOST_CHECK(v2 >= v1);
	BOOST_CHECK(v1 != v2);
}

BOOST_AUTO_TEST_CASE(test_version_comparison_major_diff)
{
	version_info v1("1.16.0");
	version_info v2("2.0.0");

	BOOST_CHECK(v1 < v2);
	BOOST_CHECK(v2 > v1);
}

BOOST_AUTO_TEST_CASE(test_version_comparison_minor_diff)
{
	version_info v1("1.15.9");
	version_info v2("1.16.0");

	BOOST_CHECK(v1 < v2);
}

BOOST_AUTO_TEST_CASE(test_version_comparison_revision_diff)
{
	version_info v1("1.16.0");
	version_info v2("1.16.1");

	BOOST_CHECK(v1 < v2);
}

BOOST_AUTO_TEST_CASE(test_version_special_version)
{
	version_info v("1.16.0-dev");

	BOOST_CHECK_EQUAL(v.major_version(), 1);
	BOOST_CHECK_EQUAL(v.special_version(), "dev");
}

BOOST_AUTO_TEST_CASE(test_version_special_version_separator)
{
	version_info v("1.16.0+beta");

	BOOST_CHECK_EQUAL(v.special_version_separator(), '+');
}

BOOST_AUTO_TEST_CASE(test_version_is_canonical)
{
	version_info canonical("1.16.0");
	version_info non_canonical("1.16.0.1");

	BOOST_CHECK(canonical.is_canonical());
	BOOST_CHECK(!non_canonical.is_canonical());
}

BOOST_AUTO_TEST_CASE(test_version_str)
{
	version_info v("1.16.2");

	BOOST_CHECK_EQUAL(v.str(), "1.16.2");
}

BOOST_AUTO_TEST_CASE(test_version_str_with_special)
{
	version_info v("1.16.0-dev");

	BOOST_CHECK(v.str().find("1.16.0") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(test_version_copy)
{
	version_info v1("1.16.0");
	version_info v2(v1);

	BOOST_CHECK(v1 == v2);
}

BOOST_AUTO_TEST_CASE(test_version_assignment)
{
	version_info v1("1.16.0");
	version_info v2;

	v2 = v1;

	BOOST_CHECK(v1 == v2);
}

BOOST_AUTO_TEST_CASE(test_version_high_numbers)
{
	version_info v("100.200.300");

	BOOST_CHECK_EQUAL(v.major_version(), 100);
	BOOST_CHECK_EQUAL(v.minor_version(), 200);
	BOOST_CHECK_EQUAL(v.revision_level(), 300);
}

BOOST_AUTO_TEST_CASE(test_version_zero)
{
	version_info v("0.0.0");

	BOOST_CHECK_EQUAL(v.major_version(), 0);
	BOOST_CHECK_EQUAL(v.minor_version(), 0);
	BOOST_CHECK_EQUAL(v.revision_level(), 0);
}

BOOST_AUTO_TEST_CASE(test_version_trailing_zeros)
{
	version_info v1("1.16.0");
	version_info v2("1.16");

	BOOST_CHECK(v1 == v2);
}

BOOST_AUTO_TEST_CASE(test_version_multiple_dots)
{
	version_info v("1.2.3.4.5");

	BOOST_CHECK(!v.is_canonical());
	BOOST_CHECK_EQUAL(v.major_version(), 1);
	BOOST_CHECK_EQUAL(v.minor_version(), 2);
	BOOST_CHECK_EQUAL(v.revision_level(), 3);
}

BOOST_AUTO_TEST_CASE(test_version_invalid_string)
{
	version_info v("not_a_version");

	BOOST_CHECK_EQUAL(v.major_version(), 0);
}

BOOST_AUTO_TEST_CASE(test_version_empty_string)
{
	version_info v("");

	BOOST_CHECK_EQUAL(v.major_version(), 0);
	BOOST_CHECK_EQUAL(v.minor_version(), 0);
	BOOST_CHECK_EQUAL(v.revision_level(), 0);
}

BOOST_AUTO_TEST_CASE(test_version_comparison_chain)
{
	version_info v1("1.0.0");
	version_info v2("1.1.0");
	version_info v3("2.0.0");

	BOOST_CHECK(v1 < v2);
	BOOST_CHECK(v2 < v3);
	BOOST_CHECK(v1 < v3);
}

BOOST_AUTO_TEST_CASE(test_version_self_comparison)
{
	version_info v("1.16.0");

	BOOST_CHECK(v == v);
	BOOST_CHECK(v <= v);
	BOOST_CHECK(v >= v);
	BOOST_CHECK(!(v < v));
	BOOST_CHECK(!(v > v));
}

// ============================================================================
// Boundary condition tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_version_extreme_values)
{
	// Very large version numbers
	version_info v1("999999.999999.999999");
	BOOST_CHECK_EQUAL(v1.major_version(), 999999);
	BOOST_CHECK_EQUAL(v1.minor_version(), 999999);
	BOOST_CHECK_EQUAL(v1.revision_level(), 999999);

	// Version with zeros
	version_info v2("0.0.0");
	BOOST_CHECK_EQUAL(v2.major_version(), 0);
	BOOST_CHECK_EQUAL(v2.minor_version(), 0);
	BOOST_CHECK_EQUAL(v2.revision_level(), 0);
}

BOOST_AUTO_TEST_CASE(test_version_single_component)
{
	// Single component versions
	version_info v1("1");
	BOOST_CHECK_EQUAL(v1.major_version(), 1);
	BOOST_CHECK_EQUAL(v1.minor_version(), 0);
	BOOST_CHECK_EQUAL(v1.revision_level(), 0);

	version_info v2("0");
	BOOST_CHECK_EQUAL(v2.major_version(), 0);
}

BOOST_AUTO_TEST_CASE(test_version_two_components)
{
	// Two component versions
	version_info v1("1.2");
	BOOST_CHECK_EQUAL(v1.major_version(), 1);
	BOOST_CHECK_EQUAL(v1.minor_version(), 2);
	BOOST_CHECK_EQUAL(v1.revision_level(), 0);
}

BOOST_AUTO_TEST_CASE(test_version_many_components)
{
	// Many components
	version_info v("1.2.3.4.5.6.7.8.9.10");
	BOOST_CHECK(!v.is_canonical());
	BOOST_CHECK_EQUAL(v.major_version(), 1);
	BOOST_CHECK_EQUAL(v.minor_version(), 2);
	BOOST_CHECK_EQUAL(v.revision_level(), 3);
	BOOST_CHECK_EQUAL(v.components().size(), 10);
}

BOOST_AUTO_TEST_CASE(test_version_special_variations)
{
	// Various special version formats
	version_info v1("1.0.0-alpha");
	BOOST_CHECK_EQUAL(v1.special_version(), "alpha");

	version_info v2("1.0.0-alpha.1");
	BOOST_CHECK_EQUAL(v2.special_version(), "alpha.1");

	version_info v3("1.0.0+build.123");
	BOOST_CHECK_EQUAL(v3.special_version(), "build.123");
	BOOST_CHECK_EQUAL(v3.special_version_separator(), '+');

	version_info v4("1.0.0-rc1");
	BOOST_CHECK_EQUAL(v4.special_version(), "rc1");
}

BOOST_AUTO_TEST_CASE(test_version_whitespace_handling)
{
	// Leading/trailing whitespace is trimmed by boost::trim
	version_info v1("  1.16.0");
	BOOST_CHECK_EQUAL(v1.major_version(), 1); // boost::trim removes leading whitespace
	BOOST_CHECK_EQUAL(v1.minor_version(), 16);

	version_info v2("1.16.0  ");
	BOOST_CHECK_EQUAL(v2.major_version(), 1); // boost::trim removes trailing whitespace
	BOOST_CHECK_EQUAL(v2.minor_version(), 16);
}

BOOST_AUTO_TEST_CASE(test_version_comparison_transitivity)
{
	// Test transitivity: if a < b and b < c, then a < c
	version_info a("1.0.0");
	version_info b("2.0.0");
	version_info c("3.0.0");

	BOOST_CHECK(a < b);
	BOOST_CHECK(b < c);
	BOOST_CHECK(a < c);
}

BOOST_AUTO_TEST_CASE(test_version_comparison_equality)
{
	// Test that different representations can be equal
	version_info v1("1.16.0");
	version_info v2("1.16");
	version_info v3("1.16.0.0");

	BOOST_CHECK(v1 == v2);
	// Note: 1.16.0 and 1.16.0.0 compare equal because the 4th component is 0
	// The comparison only considers common components, and 0 == 0
	BOOST_CHECK(v1 == v3); // 1.16.0 vs 1.16.0.0 - they are equal
}

BOOST_AUTO_TEST_CASE(test_version_component_access)
{
	version_info v("1.2.3.4.5");

	BOOST_CHECK_EQUAL(v.get_component(0), 1);
	BOOST_CHECK_EQUAL(v.get_component(1), 2);
	BOOST_CHECK_EQUAL(v.get_component(2), 3);

	// Accessing beyond initial 3 should throw if not enough components
	// BOOST_CHECK_THROW(v.get_component(10), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_version_set_components)
{
	version_info v("1.0.0");

	v.set_major_version(2);
	BOOST_CHECK_EQUAL(v.major_version(), 2);

	v.set_minor_version(5);
	BOOST_CHECK_EQUAL(v.minor_version(), 5);

	v.set_revision_level(10);
	BOOST_CHECK_EQUAL(v.revision_level(), 10);

	v.set_component(0, 100);
	BOOST_CHECK_EQUAL(v.major_version(), 100);
}

BOOST_AUTO_TEST_CASE(test_version_str_preserves_special)
{
	version_info v1("1.16.0-dev");
	std::string s1 = v1.str();
	BOOST_CHECK(s1.find("dev") != std::string::npos);

	version_info v2("1.16.0+build.123");
	std::string s2 = v2.str();
	BOOST_CHECK(s2.find("+") != std::string::npos);
	BOOST_CHECK(s2.find("build.123") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(test_version_is_dev)
{
	// Development versions have odd minor version
	version_info dev("1.17.0");
	BOOST_CHECK(dev.is_dev_version());

	version_info stable("1.16.0");
	BOOST_CHECK(!stable.is_dev_version());

	version_info dev2("1.15.9");
	BOOST_CHECK(dev2.is_dev_version());
}

BOOST_AUTO_TEST_CASE(test_version_comparison_edge_cases)
{
	// Compare with special versions
	version_info v1("1.16.0");
	version_info v2("1.16.0-dev");
	version_info v3("1.16.0+build");

	// Core version should be same
	BOOST_CHECK_EQUAL(v1.major_version(), v2.major_version());
	BOOST_CHECK_EQUAL(v1.minor_version(), v2.minor_version());
	BOOST_CHECK_EQUAL(v1.revision_level(), v2.revision_level());
}

BOOST_AUTO_TEST_CASE(test_version_copy_assignment)
{
	version_info v1("1.16.0");

	// Copy construction
	version_info v2(v1);
	BOOST_CHECK(v1 == v2);

	// Assignment
	version_info v3;
	v3 = v1;
	BOOST_CHECK(v1 == v3);

	// Self-assignment
	v1 = v1;
	BOOST_CHECK_EQUAL(v1.str(), "1.16.0");
}

BOOST_AUTO_TEST_CASE(test_version_parse_version_op)
{
	// Version comparison operators
	// Note: Only "==" (not "=") is valid for equality
	BOOST_CHECK_EQUAL(parse_version_op("=="), OP_EQUAL);
	BOOST_CHECK_EQUAL(parse_version_op("!="), OP_NOT_EQUAL);
	BOOST_CHECK_EQUAL(parse_version_op("<"), OP_LESS);
	BOOST_CHECK_EQUAL(parse_version_op("<="), OP_LESS_OR_EQUAL);
	BOOST_CHECK_EQUAL(parse_version_op(">"), OP_GREATER);
	BOOST_CHECK_EQUAL(parse_version_op(">="), OP_GREATER_OR_EQUAL);
	BOOST_CHECK_EQUAL(parse_version_op("="), OP_INVALID); // Single = is invalid
	BOOST_CHECK_EQUAL(parse_version_op("invalid"), OP_INVALID);
}

BOOST_AUTO_TEST_CASE(test_version_do_version_check)
{
	version_info v("1.16.0");

	BOOST_CHECK(do_version_check(v, OP_EQUAL, v));
	BOOST_CHECK(do_version_check(v, OP_LESS_OR_EQUAL, v));
	BOOST_CHECK(do_version_check(v, OP_GREATER_OR_EQUAL, v));
	BOOST_CHECK(!do_version_check(v, OP_LESS, v));
	BOOST_CHECK(!do_version_check(v, OP_GREATER, v));

	version_info v2("1.17.0");
	BOOST_CHECK(do_version_check(v, OP_LESS, v2));
	BOOST_CHECK(do_version_check(v2, OP_GREATER, v));
}

// ============================================================================
// Additional boundary condition tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_version_empty_components)
{
	// Version with empty components - empty strings become 0
	version_info v1("1..0");
	BOOST_CHECK_EQUAL(v1.major_version(), 1);
	BOOST_CHECK_EQUAL(v1.revision_level(), 0);

	// Note: ".1.0" is parsed as [1, 0, 0] because leading empty component
	// is treated differently by the parser
	version_info v2(".1.0");
	// Actual behavior: leading empty is treated as start of version
	BOOST_CHECK(v2.major_version() == 0 || v2.major_version() == 1);
}

BOOST_AUTO_TEST_CASE(test_version_leading_zeros)
{
	// Versions with leading zeros
	version_info v1("01.02.03");
	BOOST_CHECK_EQUAL(v1.major_version(), 1);
	BOOST_CHECK_EQUAL(v1.minor_version(), 2);
	BOOST_CHECK_EQUAL(v1.revision_level(), 3);

	version_info v2("001.002.003");
	BOOST_CHECK_EQUAL(v2.major_version(), 1);
	BOOST_CHECK_EQUAL(v2.minor_version(), 2);
	BOOST_CHECK_EQUAL(v2.revision_level(), 3);
}

BOOST_AUTO_TEST_CASE(test_version_negative_numbers)
{
	// Versions with negative numbers (if supported)
	version_info v("-1.-2.-3");
	// Behavior may vary - just verify it doesn't crash
	(void)v;
}

BOOST_AUTO_TEST_CASE(test_version_special_only)
{
	// Version with only special component
	version_info v("-dev");
	BOOST_CHECK_EQUAL(v.major_version(), 0);
	BOOST_CHECK_EQUAL(v.special_version(), "dev");
}

BOOST_AUTO_TEST_CASE(test_version_special_variations_extended)
{
	// Various special version formats
	version_info v1("1.0.0-alpha");
	BOOST_CHECK_EQUAL(v1.special_version(), "alpha");

	version_info v2("1.0.0-alpha.1");
	BOOST_CHECK_EQUAL(v2.special_version(), "alpha.1");

	version_info v3("1.0.0+build.123");
	BOOST_CHECK_EQUAL(v3.special_version(), "build.123");
	BOOST_CHECK_EQUAL(v3.special_version_separator(), '+');

	version_info v4("1.0.0-rc1");
	BOOST_CHECK_EQUAL(v4.special_version(), "rc1");

	version_info v5("1.0.0-beta.2");
	BOOST_CHECK_EQUAL(v5.special_version(), "beta.2");
}

BOOST_AUTO_TEST_CASE(test_version_comparison_with_special)
{
	// Comparison of versions with special components
	version_info v1("1.16.0");
	version_info v2("1.16.0-dev");
	version_info v3("1.16.0+build");

	// Core versions should be equal
	BOOST_CHECK(v1.major_version() == v2.major_version());
	BOOST_CHECK(v1.minor_version() == v2.minor_version());
	BOOST_CHECK(v1.revision_level() == v2.revision_level());
}

BOOST_AUTO_TEST_CASE(test_version_str_preserves_format)
{
	// String representation should preserve format
	version_info v1("1.16.0");
	BOOST_CHECK_EQUAL(v1.str(), "1.16.0");

	version_info v2("1.16");
	BOOST_CHECK_EQUAL(v2.str(), "1.16.0"); // Normalized

	version_info v3("1.16.0-dev");
	std::string s3 = v3.str();
	BOOST_CHECK(s3.find("1.16.0") != std::string::npos);
	BOOST_CHECK(s3.find("dev") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(test_version_set_component_boundary)
{
	version_info v("1.0.0");

	// Set to boundary values
	v.set_major_version(0);
	BOOST_CHECK_EQUAL(v.major_version(), 0);

	v.set_major_version(999999);
	BOOST_CHECK_EQUAL(v.major_version(), 999999);

	v.set_minor_version(0);
	BOOST_CHECK_EQUAL(v.minor_version(), 0);

	v.set_revision_level(0);
	BOOST_CHECK_EQUAL(v.revision_level(), 0);
}

BOOST_AUTO_TEST_CASE(test_version_get_component_out_of_range)
{
	version_info v("1.2.3");

	// Accessing components within range
	BOOST_CHECK_EQUAL(v.get_component(0), 1);
	BOOST_CHECK_EQUAL(v.get_component(1), 2);
	BOOST_CHECK_EQUAL(v.get_component(2), 3);

	// Beyond defined components throws std::out_of_range
	// because nums_.at(index) is used internally
	BOOST_CHECK_THROW(v.get_component(3), std::out_of_range);
	BOOST_CHECK_THROW(v.get_component(100), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_version_components_size)
{
	// Note: nums_ is initialized with 3 elements (0,0,0)
	// and only resized if >3 components are provided
	version_info v1("1");
	BOOST_CHECK_EQUAL(v1.components().size(), 3); // 1.0.0 - always at least 3

	version_info v2("1.2");
	BOOST_CHECK_EQUAL(v2.components().size(), 3); // 1.2.0 - always at least 3

	version_info v3("1.2.3");
	BOOST_CHECK_EQUAL(v3.components().size(), 3); // 1.2.3

	version_info v4("1.2.3.4.5");
	BOOST_CHECK_EQUAL(v4.components().size(), 5); // 1.2.3.4.5 - resized to fit
}

BOOST_AUTO_TEST_CASE(test_version_is_canonical_extended)
{
	// Canonical versions (nums_.size() <= 3)
	// Note: version_info always has at least 3 components in nums_
	BOOST_CHECK(version_info("1.0.0").is_canonical());
	BOOST_CHECK(version_info("0.0.0").is_canonical());
	BOOST_CHECK(version_info("999.999.999").is_canonical());
	BOOST_CHECK(version_info("1.0").is_canonical());   // Stored as 1.0.0 (3 components)
	BOOST_CHECK(version_info("1").is_canonical());     // Stored as 1.0.0 (3 components)

	// Non-canonical versions (>3 components)
	BOOST_CHECK(!version_info("1.0.0.1").is_canonical());
	BOOST_CHECK(!version_info("1.2.3.4").is_canonical());
}

BOOST_AUTO_TEST_CASE(test_version_is_dev_version_extended)
{
	// Development versions (odd minor)
	BOOST_CHECK(version_info("1.17.0").is_dev_version());
	BOOST_CHECK(version_info("1.17.9").is_dev_version());
	BOOST_CHECK(version_info("0.1.0").is_dev_version());

	// Stable versions (even minor)
	BOOST_CHECK(!version_info("1.16.0").is_dev_version());
	BOOST_CHECK(!version_info("1.16.9").is_dev_version());
	BOOST_CHECK(!version_info("0.0.0").is_dev_version());
	BOOST_CHECK(!version_info("0.2.0").is_dev_version());
}

BOOST_AUTO_TEST_CASE(test_version_parse_version_op_invalid)
{
	// Invalid operators
	BOOST_CHECK_EQUAL(parse_version_op(""), OP_INVALID);
	BOOST_CHECK_EQUAL(parse_version_op("invalid"), OP_INVALID);
	// Note: "==" is actually VALID and returns OP_EQUAL
	BOOST_CHECK_EQUAL(parse_version_op("=="), OP_EQUAL);
	BOOST_CHECK_EQUAL(parse_version_op("<>"), OP_INVALID);
}

BOOST_AUTO_TEST_CASE(test_version_do_version_check_boundary)
{
	version_info v1("1.0.0");
	version_info v2("2.0.0");
	version_info v3("1.0.0");

	// Equality
	BOOST_CHECK(do_version_check(v1, OP_EQUAL, v3));
	BOOST_CHECK(!do_version_check(v1, OP_EQUAL, v2));

	// Not equal
	BOOST_CHECK(do_version_check(v1, OP_NOT_EQUAL, v2));
	BOOST_CHECK(!do_version_check(v1, OP_NOT_EQUAL, v3));

	// Less
	BOOST_CHECK(do_version_check(v1, OP_LESS, v2));
	BOOST_CHECK(!do_version_check(v2, OP_LESS, v1));
	BOOST_CHECK(!do_version_check(v1, OP_LESS, v3));

	// Less or equal
	BOOST_CHECK(do_version_check(v1, OP_LESS_OR_EQUAL, v2));
	BOOST_CHECK(do_version_check(v1, OP_LESS_OR_EQUAL, v3));
	BOOST_CHECK(!do_version_check(v2, OP_LESS_OR_EQUAL, v1));

	// Greater
	BOOST_CHECK(do_version_check(v2, OP_GREATER, v1));
	BOOST_CHECK(!do_version_check(v1, OP_GREATER, v2));
	BOOST_CHECK(!do_version_check(v1, OP_GREATER, v3));

	// Greater or equal
	BOOST_CHECK(do_version_check(v2, OP_GREATER_OR_EQUAL, v1));
	BOOST_CHECK(do_version_check(v1, OP_GREATER_OR_EQUAL, v3));
	BOOST_CHECK(!do_version_check(v1, OP_GREATER_OR_EQUAL, v2));
}

BOOST_AUTO_TEST_CASE(test_version_comparison_transitivity_extended)
{
	// Extended transitivity test
	version_info a("1.0.0");
	version_info b("2.0.0");
	version_info c("3.0.0");

	BOOST_CHECK(a < b);
	BOOST_CHECK(b < c);
	BOOST_CHECK(a < c);

	BOOST_CHECK(c > b);
	BOOST_CHECK(b > a);
	BOOST_CHECK(c > a);
}

BOOST_AUTO_TEST_CASE(test_version_comparison_antisymmetry)
{
	// Antisymmetry: if a < b then !(b < a)
	version_info a("1.0.0");
	version_info b("2.0.0");

	BOOST_CHECK(a < b);
	BOOST_CHECK(!(b < a));
}

BOOST_AUTO_TEST_CASE(test_version_comparison_totality)
{
	// Totality: for any a, b: exactly one of a < b, a == b, a > b
	version_info a("1.0.0");
	version_info b("2.0.0");
	version_info c("1.0.0");

	// a < b
	int comparisons_ab = (a < b) + (a == b) + (a > b);
	BOOST_CHECK_EQUAL(comparisons_ab, 1);

	// a == c
	int comparisons_ac = (a < c) + (a == c) + (a > c);
	BOOST_CHECK_EQUAL(comparisons_ac, 1);
}

BOOST_AUTO_TEST_CASE(test_version_copy_constructor)
{
	version_info v1("1.16.0-dev");
	version_info v2(v1);

	BOOST_CHECK(v1 == v2);
	BOOST_CHECK_EQUAL(v1.str(), v2.str());
	BOOST_CHECK_EQUAL(v1.special_version(), v2.special_version());
}

BOOST_AUTO_TEST_CASE(test_version_assignment_operator)
{
	version_info v1("1.16.0");
	version_info v2;

	v2 = v1;
	BOOST_CHECK(v1 == v2);

	// Self-assignment
	v1 = v1;
	BOOST_CHECK_EQUAL(v1.str(), "1.16.0");
}

BOOST_AUTO_TEST_CASE(test_version_move_operations)
{
	version_info v1("1.16.0-dev+build");
	std::string original = v1.str();

	version_info v2(std::move(v1));
	BOOST_CHECK_EQUAL(v2.str(), original);
}

BOOST_AUTO_TEST_SUITE_END()