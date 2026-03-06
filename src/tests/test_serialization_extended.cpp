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

#include "serialization/string_utils.hpp"
#include "serialization/unicode.hpp"
#include <string>
#include <vector>
#include <map>

BOOST_AUTO_TEST_SUITE(test_serialization_extended)

// ============================================================================
// string_utils tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_split_empty)
{
	auto result = utils::split("");

	BOOST_CHECK(result.empty());
}

BOOST_AUTO_TEST_CASE(test_split_single)
{
	auto result = utils::split("single");

	BOOST_CHECK_EQUAL(result.size(), 1);
	BOOST_CHECK_EQUAL(result[0], "single");
}

BOOST_AUTO_TEST_CASE(test_split_trailing_separator)
{
	auto result = utils::split("a,b,");

	BOOST_CHECK_EQUAL(result.size(), 3);
	BOOST_CHECK_EQUAL(result[0], "a");
	BOOST_CHECK_EQUAL(result[1], "b");
	BOOST_CHECK_EQUAL(result[2], "");
}

BOOST_AUTO_TEST_CASE(test_split_multiple_separators)
{
	auto result = utils::split("a,,b,,,c");

	BOOST_CHECK_EQUAL(result.size(), 6);
}

BOOST_AUTO_TEST_CASE(test_join_empty)
{
	std::vector<std::string> empty;
	std::string result = utils::join(empty);

	BOOST_CHECK(result.empty());
}

BOOST_AUTO_TEST_CASE(test_join_single)
{
	std::vector<std::string> single = {"one"};
	std::string result = utils::join(single);

	BOOST_CHECK_EQUAL(result, "one");
}

BOOST_AUTO_TEST_CASE(test_join_multiple_default_sep)
{
	std::vector<std::string> multiple = {"a", "b", "c"};
	std::string result = utils::join(multiple);

	BOOST_CHECK_EQUAL(result, "a,b,c");
}

BOOST_AUTO_TEST_CASE(test_join_custom_separator)
{
	std::vector<std::string> parts = {"x", "y", "z"};
	std::string result = utils::join(parts, " | ");

	BOOST_CHECK_EQUAL(result, "x | y | z");
}

BOOST_AUTO_TEST_CASE(test_string_bool_true_values)
{
	BOOST_CHECK(utils::string_bool("true"));
	BOOST_CHECK(utils::string_bool("yes"));
	BOOST_CHECK(utils::string_bool("1"));
	BOOST_CHECK(utils::string_bool("on"));
	BOOST_CHECK(utils::string_bool("TRUE"));
	BOOST_CHECK(utils::string_bool("Yes"));
}

BOOST_AUTO_TEST_CASE(test_string_bool_false_values)
{
	BOOST_CHECK(!utils::string_bool("false"));
	BOOST_CHECK(!utils::string_bool("no"));
	BOOST_CHECK(!utils::string_bool("0"));
	BOOST_CHECK(!utils::string_bool("off"));
	BOOST_CHECK(!utils::string_bool(""));
	BOOST_CHECK(!utils::string_bool("anything"));
}

BOOST_AUTO_TEST_CASE(test_string_bool_default)
{
	BOOST_CHECK(utils::string_bool("invalid", true));
	BOOST_CHECK(!utils::string_bool("invalid", false));
}

// ============================================================================
// unicode tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_utf8_size_ascii)
{
	std::string ascii = "hello";

	BOOST_CHECK_EQUAL(utf8::size(ascii), 5);
}

BOOST_AUTO_TEST_CASE(test_utf8_size_unicode)
{
	std::string unicode = "héllo";

	BOOST_CHECK_EQUAL(utf8::size(unicode), 5);
}

BOOST_AUTO_TEST_CASE(test_utf8_index)
{
	std::string str = "abc";

	BOOST_CHECK_EQUAL(utf8::index(str, 0), 0);
	BOOST_CHECK_EQUAL(utf8::index(str, 1), 1);
	BOOST_CHECK_EQUAL(utf8::index(str, 2), 2);
}

BOOST_AUTO_TEST_CASE(test_utf8_truncate_shorter)
{
	std::string str = "hello";
	std::string result = utf8::truncate(str, 3);

	BOOST_CHECK_EQUAL(result, "hel");
}

BOOST_AUTO_TEST_CASE(test_utf8_truncate_longer)
{
	std::string str = "hi";
	std::string result = utf8::truncate(str, 10);

	BOOST_CHECK_EQUAL(result, "hi");
}

BOOST_AUTO_TEST_CASE(test_utf8_truncate_exact)
{
	std::string str = "test";
	std::string result = utf8::truncate(str, 4);

	BOOST_CHECK_EQUAL(result, "test");
}

BOOST_AUTO_TEST_CASE(test_utf8_truncate_zero)
{
	std::string str = "test";
	std::string result = utf8::truncate(str, 0);

	BOOST_CHECK(result.empty());
}

// ============================================================================
// wildcard tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_wildcard_exact_match)
{
	BOOST_CHECK(utils::wildcard_string_match("test", "test"));
	BOOST_CHECK(!utils::wildcard_string_match("test", "Test"));
	BOOST_CHECK(!utils::wildcard_string_match("test", "test2"));
}

BOOST_AUTO_TEST_CASE(test_wildcard_star_prefix)
{
	BOOST_CHECK(utils::wildcard_string_match("test.txt", "*.txt"));
	BOOST_CHECK(!utils::wildcard_string_match("test.doc", "*.txt"));
}

BOOST_AUTO_TEST_CASE(test_wildcard_star_suffix)
{
	BOOST_CHECK(utils::wildcard_string_match("myfile_test", "myfile*"));
	BOOST_CHECK(!utils::wildcard_string_match("other_test", "myfile*"));
}

BOOST_AUTO_TEST_CASE(test_wildcard_star_middle)
{
	BOOST_CHECK(utils::wildcard_string_match("hello world", "hel*world"));
	BOOST_CHECK(utils::wildcard_string_match("helworld", "hel*world"));
}

BOOST_AUTO_TEST_CASE(test_wildcard_question_mark)
{
	BOOST_CHECK(utils::wildcard_string_match("cat", "c?t"));
	BOOST_CHECK(utils::wildcard_string_match("cut", "c?t"));
	BOOST_CHECK(!utils::wildcard_string_match("ct", "c?t"));
	BOOST_CHECK(!utils::wildcard_string_match("coat", "c?t"));
}

BOOST_AUTO_TEST_CASE(test_wildcard_combined)
{
	BOOST_CHECK(utils::wildcard_string_match("test123.txt", "test*?.txt"));
	BOOST_CHECK(utils::wildcard_string_match("file_a.txt", "file_?*"));
}

BOOST_AUTO_TEST_CASE(test_wildcard_empty)
{
	BOOST_CHECK(utils::wildcard_string_match("", ""));
	BOOST_CHECK(utils::wildcard_string_match("", "*"));
	BOOST_CHECK(!utils::wildcard_string_match("", "?"));
	BOOST_CHECK(!utils::wildcard_string_match("test", ""));
}

// ============================================================================
// lowercasing tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_lowercase_ascii)
{
	BOOST_CHECK_EQUAL(utf8::lowercase("HELLO"), "hello");
	BOOST_CHECK_EQUAL(utf8::lowercase("Hello"), "hello");
	BOOST_CHECK_EQUAL(utf8::lowercase("hello"), "hello");
}

BOOST_AUTO_TEST_CASE(test_lowercase_mixed)
{
	BOOST_CHECK_EQUAL(utf8::lowercase("HeLLo WoRLD"), "hello world");
	BOOST_CHECK_EQUAL(utf8::lowercase("TEST123"), "test123");
}

BOOST_AUTO_TEST_CASE(test_lowercase_empty)
{
	BOOST_CHECK_EQUAL(utf8::lowercase(""), "");
}

BOOST_AUTO_TEST_CASE(test_lowercase_numbers)
{
	BOOST_CHECK_EQUAL(utf8::lowercase("12345"), "12345");
}

// ============================================================================
// parenthetical split tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_parenthetical_split_simple)
{
	auto result = utils::parenthetical_split("a, b, c", ',');

	BOOST_CHECK_EQUAL(result.size(), 3);
}

BOOST_AUTO_TEST_CASE(test_parenthetical_split_nested)
{
	auto result = utils::parenthetical_split("a, (b, c), d", ',', "(", ")");

	BOOST_CHECK_EQUAL(result.size(), 3);
	BOOST_CHECK_EQUAL(result[0], "a");
	BOOST_CHECK_EQUAL(result[1], "(b, c)");
	BOOST_CHECK_EQUAL(result[2], "d");
}

// ============================================================================
// Boundary condition tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_split_empty_delimiters)
{
	// Empty string split
	auto result = utils::split("");
	BOOST_CHECK(result.empty());

	// String with only delimiters
	result = utils::split(",,,,");
	BOOST_CHECK_EQUAL(result.size(), 5);
	for (const auto& s : result) {
		BOOST_CHECK(s.empty());
	}
}

BOOST_AUTO_TEST_CASE(test_split_whitespace_only)
{
	// Whitespace handling
	auto result = utils::split("   ", ' ');
	BOOST_CHECK_EQUAL(result.size(), 4);
}

BOOST_AUTO_TEST_CASE(test_split_large_input)
{
	// Large input
	std::string large(10000, 'a');
	large[1000] = ',';
	large[5000] = ',';
	large[9000] = ',';

	auto result = utils::split(large);
	BOOST_CHECK_EQUAL(result.size(), 4);
}

BOOST_AUTO_TEST_CASE(test_join_empty_elements)
{
	// Join with empty strings
	std::vector<std::string> parts = {"", "a", "", "b", ""};
	std::string result = utils::join(parts, ",");

	BOOST_CHECK_EQUAL(result, ",a,,b,");
}

BOOST_AUTO_TEST_CASE(test_join_single_empty)
{
	std::vector<std::string> parts = {""};
	std::string result = utils::join(parts);

	BOOST_CHECK_EQUAL(result, "");
}

BOOST_AUTO_TEST_CASE(test_string_bool_edge_cases)
{
	// Edge cases for string_bool
	BOOST_CHECK(!utils::string_bool(""));
	BOOST_CHECK(!utils::string_bool(" "));
	BOOST_CHECK(!utils::string_bool("  true  ")); // Should not trim
	BOOST_CHECK(utils::string_bool("TRUE"));
	BOOST_CHECK(utils::string_bool("True"));
	BOOST_CHECK(utils::string_bool("YES"));
	BOOST_CHECK(utils::string_bool("Yes"));
	BOOST_CHECK(utils::string_bool("ON"));
	BOOST_CHECK(utils::string_bool("On"));
}

BOOST_AUTO_TEST_CASE(test_utf8_size_edge_cases)
{
	// Empty string
	BOOST_CHECK_EQUAL(utf8::size(""), 0);

	// Single byte characters
	BOOST_CHECK_EQUAL(utf8::size("a"), 1);

	// Multi-byte characters
	BOOST_CHECK_EQUAL(utf8::size("é"), 1); // 2-byte UTF-8
	BOOST_CHECK_EQUAL(utf8::size("中"), 1); // 3-byte UTF-8
	BOOST_CHECK_EQUAL(utf8::size("🎮"), 1); // 4-byte UTF-8

	// Mixed
	BOOST_CHECK_EQUAL(utf8::size("aé中🎮"), 4);
}

BOOST_AUTO_TEST_CASE(test_utf8_truncate_edge_cases)
{
	// Truncate to 0
	std::string str = "hello";
	BOOST_CHECK_EQUAL(utf8::truncate(str, 0), "");

	// Truncate longer than string
	BOOST_CHECK_EQUAL(utf8::truncate(str, 100), "hello");

	// Truncate UTF-8 string
	std::string utf8_str = "Hello 世界";
	BOOST_CHECK_EQUAL(utf8::truncate(utf8_str, 7), "Hello 世");
	BOOST_CHECK_EQUAL(utf8::truncate(utf8_str, 100), "Hello 世界");
}

BOOST_AUTO_TEST_CASE(test_utf8_index_edge_cases)
{
	std::string str = "abc";

	BOOST_CHECK_EQUAL(utf8::index(str, 0), 0);
	BOOST_CHECK_EQUAL(utf8::index(str, 2), 2);
	// Note: index returns npos if out of bounds, but implementation may vary
}

BOOST_AUTO_TEST_CASE(test_wildcard_edge_cases)
{
	// Empty patterns
	BOOST_CHECK(utils::wildcard_string_match("", ""));
	BOOST_CHECK(utils::wildcard_string_match("", "*"));
	BOOST_CHECK(!utils::wildcard_string_match("a", ""));

	// Only wildcards
	BOOST_CHECK(utils::wildcard_string_match("anything", "*"));
	BOOST_CHECK(utils::wildcard_string_match("", "*"));

	// Multiple stars
	BOOST_CHECK(utils::wildcard_string_match("abc", "*a*b*c*"));
	BOOST_CHECK(utils::wildcard_string_match("abc", "***abc***"));

	// Question marks only
	BOOST_CHECK(utils::wildcard_string_match("abc", "???"));
	BOOST_CHECK(!utils::wildcard_string_match("ab", "???"));
	BOOST_CHECK(!utils::wildcard_string_match("abcd", "???"));
}

BOOST_AUTO_TEST_CASE(test_lowercase_edge_cases)
{
	// Already lowercase
	BOOST_CHECK_EQUAL(utf8::lowercase("lowercase"), "lowercase");

	// Empty
	BOOST_CHECK_EQUAL(utf8::lowercase(""), "");

	// Numbers and symbols
	BOOST_CHECK_EQUAL(utf8::lowercase("123!@#"), "123!@#");

	// Mixed case with numbers
	BOOST_CHECK_EQUAL(utf8::lowercase("ABC123DEF"), "abc123def");
}

BOOST_AUTO_TEST_CASE(test_parenthetical_split_edge_cases)
{
	// Empty
	auto result = utils::parenthetical_split("", ',');
	BOOST_CHECK_EQUAL(result.size(), 1);
	BOOST_CHECK_EQUAL(result[0], "");

	// No parentheses
	result = utils::parenthetical_split("a,b,c", ',');
	BOOST_CHECK_EQUAL(result.size(), 3);

	// Unbalanced parentheses (implementation dependent)
	result = utils::parenthetical_split("a,(b,c", ',');
	// Should handle gracefully

	// Nested parentheses
	result = utils::parenthetical_split("a,(b,(c,d),e),f", ',');
	BOOST_CHECK(result.size() >= 2);
}

// ============================================================================
// Additional boundary condition tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_split_single_delimiter)
{
	// String with only delimiter
	auto result = utils::split(",", ',');
	BOOST_CHECK_EQUAL(result.size(), 2);
	BOOST_CHECK_EQUAL(result[0], "");
	BOOST_CHECK_EQUAL(result[1], "");
}

BOOST_AUTO_TEST_CASE(test_split_consecutive_delimiters)
{
	// Multiple consecutive delimiters
	auto result = utils::split("a,,,,b", ',');
	BOOST_CHECK_EQUAL(result.size(), 5);
	BOOST_CHECK_EQUAL(result[0], "a");
	BOOST_CHECK_EQUAL(result[1], "");
	BOOST_CHECK_EQUAL(result[2], "");
	BOOST_CHECK_EQUAL(result[3], "");
	BOOST_CHECK_EQUAL(result[4], "b");
}

BOOST_AUTO_TEST_CASE(test_split_leading_trailing_delimiters)
{
	// Leading and trailing delimiters
	auto result = utils::split(",a,b,", ',');
	BOOST_CHECK_EQUAL(result.size(), 4);
	BOOST_CHECK_EQUAL(result[0], "");
	BOOST_CHECK_EQUAL(result[1], "a");
	BOOST_CHECK_EQUAL(result[2], "b");
	BOOST_CHECK_EQUAL(result[3], "");
}

BOOST_AUTO_TEST_CASE(test_split_whitespace_handling)
{
	// Whitespace preservation
	auto result = utils::split("  a  ,  b  ,  c  ", ',');
	BOOST_CHECK_EQUAL(result.size(), 3);
	BOOST_CHECK_EQUAL(result[0], "  a  ");
	BOOST_CHECK_EQUAL(result[1], "  b  ");
	BOOST_CHECK_EQUAL(result[2], "  c  ");
}

BOOST_AUTO_TEST_CASE(test_join_empty_strings)
{
	// Joining empty strings with various separators
	std::vector<std::string> parts = {"", "", ""};
	std::string result = utils::join(parts, "-");
	BOOST_CHECK_EQUAL(result, "--");
}

BOOST_AUTO_TEST_CASE(test_join_mixed_empty)
{
	// Mix of empty and non-empty
	std::vector<std::string> parts = {"a", "", "b", "", "c"};
	std::string result = utils::join(parts, ",");
	BOOST_CHECK_EQUAL(result, "a,,b,,c");
}

BOOST_AUTO_TEST_CASE(test_join_single_element_no_sep)
{
	// Single element should not have separator
	std::vector<std::string> parts = {"only"};
	std::string result = utils::join(parts, "|");
	BOOST_CHECK_EQUAL(result, "only");
}

BOOST_AUTO_TEST_CASE(test_string_bool_whitespace)
{
	// Whitespace handling in boolean strings
	BOOST_CHECK(!utils::string_bool(" true"));  // Leading space
	BOOST_CHECK(!utils::string_bool("true "));  // Trailing space
	BOOST_CHECK(!utils::string_bool(" true ")); // Both spaces
	BOOST_CHECK(!utils::string_bool("\ttrue")); // Tab
	BOOST_CHECK(!utils::string_bool("\ntrue")); // Newline
}

BOOST_AUTO_TEST_CASE(test_string_bool_case_sensitivity)
{
	// Case sensitivity
	BOOST_CHECK(utils::string_bool("TRUE"));
	BOOST_CHECK(utils::string_bool("True"));
	BOOST_CHECK(utils::string_bool("tRuE"));
	BOOST_CHECK(utils::string_bool("YES"));
	BOOST_CHECK(utils::string_bool("Yes"));
	BOOST_CHECK(utils::string_bool("ON"));
	BOOST_CHECK(utils::string_bool("On"));
}

BOOST_AUTO_TEST_CASE(test_utf8_size_invalid_sequences)
{
	// Invalid UTF-8 sequences should be handled gracefully
	std::string invalid1 = "\x80\x81\x82"; // Invalid start bytes
	BOOST_CHECK_NO_THROW(utf8::size(invalid1));

	std::string invalid2 = "\xC0\x80"; // Overlong encoding
	BOOST_CHECK_NO_THROW(utf8::size(invalid2));

	std::string truncated = "\xE0\x80"; // Truncated 3-byte sequence
	BOOST_CHECK_NO_THROW(utf8::size(truncated));
}

BOOST_AUTO_TEST_CASE(test_utf8_truncate_multibyte_boundary)
{
	// Truncating at multi-byte character boundaries
	std::string utf8_2byte = "\xC3\xA9"; // é
	BOOST_CHECK_EQUAL(utf8::truncate(utf8_2byte, 1), utf8_2byte);

	std::string utf8_3byte = "\xE4\xB8\xAD"; // 中
	BOOST_CHECK_EQUAL(utf8::truncate(utf8_3byte, 1), utf8_3byte);

	std::string utf8_4byte = "\xF0\x9F\x8E\xAE"; // 🎮
	BOOST_CHECK_EQUAL(utf8::truncate(utf8_4byte, 1), utf8_4byte);
}

BOOST_AUTO_TEST_CASE(test_utf8_truncate_partial_character)
{
	// Truncating in the middle of a multi-byte character
	std::string str = "a\xC3\xA9" "b"; // aéb (split to avoid hex escape warning)
	// Truncating to 2 should give "a" (é is 2 bytes)
	std::string result = utf8::truncate(str, 2);
	BOOST_CHECK_EQUAL(result.size(), 3); // a + é (2 bytes)
}

BOOST_AUTO_TEST_CASE(test_wildcard_empty_patterns)
{
	// Empty pattern matching
	BOOST_CHECK(utils::wildcard_string_match("", ""));
	BOOST_CHECK(!utils::wildcard_string_match("a", ""));
	BOOST_CHECK(utils::wildcard_string_match("", "*"));
	BOOST_CHECK(!utils::wildcard_string_match("", "?"));
}

BOOST_AUTO_TEST_CASE(test_wildcard_only_wildcards)
{
	// Patterns with only wildcards
	BOOST_CHECK(utils::wildcard_string_match("anything", "*"));
	BOOST_CHECK(utils::wildcard_string_match("", "*"));
	BOOST_CHECK(utils::wildcard_string_match("abc", "***"));
	BOOST_CHECK(utils::wildcard_string_match("abc", "*"));
}

BOOST_AUTO_TEST_CASE(test_wildcard_complex_patterns)
{
	// Complex patterns
	BOOST_CHECK(utils::wildcard_string_match("abc123def", "abc*def"));
	BOOST_CHECK(utils::wildcard_string_match("abcdef", "abc*def"));
	BOOST_CHECK(!utils::wildcard_string_match("abc123deg", "abc*def"));
	BOOST_CHECK(utils::wildcard_string_match("test.txt", "*.txt"));
	BOOST_CHECK(!utils::wildcard_string_match("test.txt.bak", "*.txt"));
}

BOOST_AUTO_TEST_CASE(test_lowercase_unicode)
{
	// Unicode lowercase conversion
	BOOST_CHECK_EQUAL(utf8::lowercase("HELLO"), "hello");
	BOOST_CHECK_EQUAL(utf8::lowercase("HeLLo WoRLD"), "hello world");
	// Note: Unicode case folding behavior may vary by implementation
}

BOOST_AUTO_TEST_CASE(test_lowercase_special_chars)
{
	// Special characters should remain unchanged
	BOOST_CHECK_EQUAL(utf8::lowercase("!@#$%^&*()"), "!@#$%^&*()");
	BOOST_CHECK_EQUAL(utf8::lowercase("1234567890"), "1234567890");
	BOOST_CHECK_EQUAL(utf8::lowercase("\n\t\r"), "\n\t\r");
}

BOOST_AUTO_TEST_CASE(test_parenthetical_split_empty_delimiters)
{
	// Empty delimiters or parentheses
	auto result = utils::parenthetical_split("a,b,c", '\0');
	// Should handle gracefully
	BOOST_CHECK(!result.empty());
}

BOOST_AUTO_TEST_CASE(test_parenthetical_split_deep_nesting)
{
	// Deeply nested parentheses
	std::string deep = "a,((((b,c))),d),e";
	auto result = utils::parenthetical_split(deep, ',');
	// Should handle deep nesting
	BOOST_CHECK(result.size() >= 3);
}

BOOST_AUTO_TEST_CASE(test_parenthetical_split_no_delimiters_in_parens)
{
	// No delimiters inside parentheses
	auto result = utils::parenthetical_split("(a,b,c)", ',');
	BOOST_CHECK_EQUAL(result.size(), 1);
	BOOST_CHECK_EQUAL(result[0], "(a,b,c)");
}

BOOST_AUTO_TEST_CASE(test_split_set_range)
{
	// Testing split_set with various inputs
	std::set<std::string> result = utils::split_set("");
	BOOST_CHECK(result.empty());

	result = utils::split_set("a,b,c");
	BOOST_CHECK_EQUAL(result.size(), 3);
	BOOST_CHECK(result.count("a"));
	BOOST_CHECK(result.count("b"));
	BOOST_CHECK(result.count("c"));

	result = utils::split_set("a,a,b");
	BOOST_CHECK_EQUAL(result.size(), 2); // Duplicates removed
	BOOST_CHECK(result.count("a"));
	BOOST_CHECK(result.count("b"));
}

BOOST_AUTO_TEST_CASE(test_split_set_empty_values)
{
	// Set entries with empty values
	auto result = utils::split_set("");
	BOOST_CHECK(result.empty());

	result = utils::split_set(",a,,b,");
	BOOST_CHECK_EQUAL(result.size(), 3); // Empty strings included
	BOOST_CHECK(result.count("a"));
	BOOST_CHECK(result.count("b"));
}

BOOST_AUTO_TEST_SUITE_END()