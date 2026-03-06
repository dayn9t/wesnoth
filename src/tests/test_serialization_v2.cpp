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

#include "serialization/parser.hpp"
#include "serialization/string_utils.hpp"
#include "serialization/unicode.hpp"
#include "serialization/binary_or_text.hpp"
#include "serialization/tokenizer.hpp"
#include "config.hpp"

#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <limits>

BOOST_AUTO_TEST_SUITE(test_serialization_v2)

// ============================================================================
// Parser Error Handling Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_parser_empty_input)
{
	std::istringstream input("");
	config result = io::read(input);
	BOOST_CHECK(result.empty());
}

BOOST_AUTO_TEST_CASE(test_parser_whitespace_only)
{
	std::istringstream input("   \n\t  \n  ");
	config result = io::read(input);
	BOOST_CHECK(result.empty());
}

BOOST_AUTO_TEST_CASE(test_parser_simple_tag)
{
	std::istringstream input("[test][/test]");
	config result = io::read(input);
	BOOST_CHECK(result.has_child("test"));
}

BOOST_AUTO_TEST_CASE(test_parser_nested_tags)
{
	std::istringstream input("[outer][inner][/inner][/outer]");
	config result = io::read(input);

	config::const_child_iterator outer = result.find_child("outer");
	BOOST_CHECK(outer != result.child_end());

	const config& outer_cfg = outer->second;
	BOOST_CHECK(outer_cfg.has_child("inner"));
}

BOOST_AUTO_TEST_CASE(test_parser_attribute_with_quotes)
{
	std::istringstream input("[test]\nkey=\"value with spaces\"\n[/test]");
	config result = io::read(input);

	const config::const_child_iterator test_iter = result.find_child("test");
	BOOST_REQUIRE(test_iter != result.child_end());

	const config& test_cfg = test_iter->second;
	BOOST_CHECK_EQUAL(test_cfg["key"].str(), "value with spaces");
}

BOOST_AUTO_TEST_CASE(test_parser_attribute_without_quotes)
{
	std::istringstream input("[test]\nkey=value123\n[/test]");
	config result = io::read(input);

	const config::const_child_iterator test_iter = result.find_child("test");
	BOOST_REQUIRE(test_iter != result.child_end());

	const config& test_cfg = test_iter->second;
	BOOST_CHECK_EQUAL(test_cfg["key"].str(), "value123");
}

BOOST_AUTO_TEST_CASE(test_parser_multiple_attributes)
{
	std::istringstream input("[test]\nattr1=\"value1\"\nattr2=\"value2\"\nattr3=\"value3\"\n[/test]");
	config result = io::read(input);

	const config::const_child_iterator test_iter = result.find_child("test");
	BOOST_REQUIRE(test_iter != result.child_end());

	const config& test_cfg = test_iter->second;
	BOOST_CHECK_EQUAL(test_cfg["attr1"].str(), "value1");
	BOOST_CHECK_EQUAL(test_cfg["attr2"].str(), "value2");
	BOOST_CHECK_EQUAL(test_cfg["attr3"].str(), "value3");
}

BOOST_AUTO_TEST_CASE(test_parser_integer_attribute)
{
	std::istringstream input("[test]\nint_val=42\nneg_int=-10\n[/test]");
	config result = io::read(input);

	const config::const_child_iterator test_iter = result.find_child("test");
	BOOST_REQUIRE(test_iter != result.child_end());

	const config& test_cfg = test_iter->second;
	BOOST_CHECK_EQUAL(test_cfg["int_val"].to_int(), 42);
	BOOST_CHECK_EQUAL(test_cfg["neg_int"].to_int(), -10);
}

BOOST_AUTO_TEST_CASE(test_parser_bool_attribute)
{
	std::istringstream input("[test]\ntrue_val=\"true\"\nfalse_val=\"false\"\nyes_val=\"yes\"\nno_val=\"no\"\n[/test]");
	config result = io::read(input);

	const config::const_child_iterator test_iter = result.find_child("test");
	BOOST_REQUIRE(test_iter != result.child_end());

	const config& test_cfg = test_iter->second;
	BOOST_CHECK(test_cfg["true_val"].to_bool());
	BOOST_CHECK(!test_cfg["false_val"].to_bool());
	BOOST_CHECK(test_cfg["yes_val"].to_bool());
	BOOST_CHECK(!test_cfg["no_val"].to_bool());
}

BOOST_AUTO_TEST_CASE(test_parser_unterminated_tag_throws)
{
	std::istringstream input("[test]");
	BOOST_CHECK_THROW(io::read(input), config::error);
}

BOOST_AUTO_TEST_CASE(test_parser_mismatched_tags_throws)
{
	std::istringstream input("[test][/other]");
	BOOST_CHECK_THROW(io::read(input), config::error);
}

BOOST_AUTO_TEST_CASE(test_parser_invalid_characters_at_start_throws)
{
	std::istringstream input("invalid characters");
	BOOST_CHECK_THROW(io::read(input), config::error);
}

// ============================================================================
// String Parsing Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_parse_range_single_value)
{
	auto result = utils::parse_range("5");
	BOOST_CHECK_EQUAL(result.first, 5);
	BOOST_CHECK_EQUAL(result.second, 5);
}

BOOST_AUTO_TEST_CASE(test_parse_range_dash_range)
{
	auto result = utils::parse_range("3-7");
	BOOST_CHECK_EQUAL(result.first, 3);
	BOOST_CHECK_EQUAL(result.second, 7);
}

BOOST_AUTO_TEST_CASE(test_parse_range_negative_value)
{
	auto result = utils::parse_range("-5");
	BOOST_CHECK_EQUAL(result.first, -5);
	BOOST_CHECK_EQUAL(result.second, -5);
}

BOOST_AUTO_TEST_CASE(test_parse_range_negative_range)
{
	auto result = utils::parse_range("-10--5");
	BOOST_CHECK_EQUAL(result.first, -10);
	BOOST_CHECK_EQUAL(result.second, -5);
}

BOOST_AUTO_TEST_CASE(test_parse_range_infinity)
{
	auto result = utils::parse_range("1-infinity");
	BOOST_CHECK_EQUAL(result.first, 1);
	BOOST_CHECK_EQUAL(result.second, std::numeric_limits<int>::max());
}

BOOST_AUTO_TEST_CASE(test_parse_range_real)
{
	auto result = utils::parse_range_real("3.14-6.28");
	BOOST_CHECK_CLOSE(result.first, 3.14, 0.001);
	BOOST_CHECK_CLOSE(result.second, 6.28, 0.001);
}

BOOST_AUTO_TEST_CASE(test_string_bool_true_values)
{
	BOOST_CHECK(utils::string_bool("true"));
	BOOST_CHECK(utils::string_bool("yes"));
	BOOST_CHECK(utils::string_bool("1"));
	BOOST_CHECK(utils::string_bool("on"));
	BOOST_CHECK(utils::string_bool("TRUE"));
	BOOST_CHECK(utils::string_bool("YES"));
}

BOOST_AUTO_TEST_CASE(test_string_bool_false_values)
{
	BOOST_CHECK(!utils::string_bool("false"));
	BOOST_CHECK(!utils::string_bool("no"));
	BOOST_CHECK(!utils::string_bool("0"));
	BOOST_CHECK(!utils::string_bool("off"));
	BOOST_CHECK(!utils::string_bool(""));
	BOOST_CHECK(!utils::string_bool("random"));
}

BOOST_AUTO_TEST_CASE(test_string_bool_default_value)
{
	BOOST_CHECK(utils::string_bool("invalid", true));
	BOOST_CHECK(!utils::string_bool("invalid", false));
}

// ============================================================================
// Binary/Text Format Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_config_write_text_format)
{
	config cfg;
	cfg["test_key"] = "test_value";

	std::ostringstream output;
	io::write(output, cfg);

	std::string result = output.str();
	BOOST_CHECK(result.find("test_key") != std::string::npos);
	BOOST_CHECK(result.find("test_value") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(test_config_write_nested)
{
	config cfg;
	config& child = cfg.add_child("child");
	child["attr"] = "value";

	std::ostringstream output;
	io::write(output, cfg);

	std::string result = output.str();
	BOOST_CHECK(result.find("[child]") != std::string::npos);
	BOOST_CHECK(result.find("[/child]") != std::string::npos);
	BOOST_CHECK(result.find("attr") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(test_config_write_open_close_child)
{
	std::ostringstream output;
	io::write_open_child(output, "test_tag", 0);
	io::write_close_child(output, "test_tag", 0);

	std::string result = output.str();
	BOOST_CHECK(result.find("[test_tag]") != std::string::npos);
	BOOST_CHECK(result.find("[/test_tag]") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(test_config_writer_construction)
{
	std::ostringstream output;
	config_writer writer(output, compression::format::none);
	BOOST_CHECK(writer.good());
}

BOOST_AUTO_TEST_CASE(test_config_writer_write_config)
{
	config cfg;
	cfg["key1"] = "value1";
	cfg["key2"] = "value2";

	std::ostringstream output;
	config_writer writer(output, compression::format::none);
	writer.write(cfg);

	BOOST_CHECK(writer.good());
	std::string result = output.str();
	BOOST_CHECK(result.find("key1") != std::string::npos);
	BOOST_CHECK(result.find("key2") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(test_config_writer_write_child)
{
	config cfg;
	config& child = cfg.add_child("my_child");
	child["child_attr"] = "child_value";

	std::ostringstream output;
	config_writer writer(output, compression::format::none);
	writer.write_child("my_child", child);

	BOOST_CHECK(writer.good());
	std::string result = output.str();
	BOOST_CHECK(result.find("my_child") != std::string::npos);
	BOOST_CHECK(result.find("child_attr") != std::string::npos);
}

// ============================================================================
// Unicode Handling Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_utf8_size_ascii)
{
	std::string str = "hello";
	BOOST_CHECK_EQUAL(utf8::size(str), 5);
}

BOOST_AUTO_TEST_CASE(test_utf8_size_empty)
{
	std::string str = "";
	BOOST_CHECK_EQUAL(utf8::size(str), 0);
}

BOOST_AUTO_TEST_CASE(test_utf8_size_2byte_chars)
{
	// 'e' with acute accent (2 bytes in UTF-8)
	std::string str = "cafe"; // cafe with accent on e
	str += "\xC3\xA9"; // explicit UTF-8 for e with acute
	BOOST_CHECK_EQUAL(utf8::size(str), 5); // c, a, f, e, e-acute
}

BOOST_AUTO_TEST_CASE(test_utf8_size_3byte_chars)
{
	// Chinese character (3 bytes in UTF-8)
	std::string str = "\xE4\xB8\xAD"; // Middle character
	BOOST_CHECK_EQUAL(utf8::size(str), 1);
}

BOOST_AUTO_TEST_CASE(test_utf8_size_4byte_chars)
{
	// Emoji (4 bytes in UTF-8)
	std::string str = "\xF0\x9F\x8E\xAE"; // Video game emoji
	BOOST_CHECK_EQUAL(utf8::size(str), 1);
}

BOOST_AUTO_TEST_CASE(test_utf8_index_ascii)
{
	std::string str = "hello";
	BOOST_CHECK_EQUAL(utf8::index(str, 0), 0);
	BOOST_CHECK_EQUAL(utf8::index(str, 1), 1);
	BOOST_CHECK_EQUAL(utf8::index(str, 4), 4);
}

BOOST_AUTO_TEST_CASE(test_utf8_index_out_of_bounds)
{
	std::string str = "hello";
	BOOST_CHECK_EQUAL(utf8::index(str, 100), str.length());
}

BOOST_AUTO_TEST_CASE(test_utf8_truncate_basic)
{
	std::string str = "hello world";
	std::string result = utf8::truncate(str, 5);
	BOOST_CHECK_EQUAL(result, "hello");
}

BOOST_AUTO_TEST_CASE(test_utf8_truncate_empty)
{
	std::string str = "";
	std::string result = utf8::truncate(str, 5);
	BOOST_CHECK(result.empty());
}

BOOST_AUTO_TEST_CASE(test_utf8_truncate_longer_than_string)
{
	std::string str = "hello";
	std::string result = utf8::truncate(str, 100);
	BOOST_CHECK_EQUAL(result, "hello");
}

BOOST_AUTO_TEST_CASE(test_utf8_truncate_zero)
{
	std::string str = "hello";
	std::string result = utf8::truncate(str, 0);
	BOOST_CHECK(result.empty());
}

BOOST_AUTO_TEST_CASE(test_utf8_lowercase_ascii)
{
	BOOST_CHECK_EQUAL(utf8::lowercase("HELLO"), "hello");
	BOOST_CHECK_EQUAL(utf8::lowercase("HeLLo"), "hello");
	BOOST_CHECK_EQUAL(utf8::lowercase("hello"), "hello");
}

BOOST_AUTO_TEST_CASE(test_utf8_lowercase_empty)
{
	BOOST_CHECK_EQUAL(utf8::lowercase(""), "");
}

BOOST_AUTO_TEST_CASE(test_utf8_insert_basic)
{
	std::string str = "hello world";
	utf8::insert(str, 6, "beautiful ");
	BOOST_CHECK_EQUAL(str, "hello beautiful world");
}

BOOST_AUTO_TEST_CASE(test_utf8_erase_basic)
{
	std::string str = "hello beautiful world";
	utf8::erase(str, 6, 10);
	BOOST_CHECK_EQUAL(str, "hello world");
}

BOOST_AUTO_TEST_CASE(test_utf8_erase_to_end)
{
	std::string str = "hello world";
	utf8::erase(str, 5);
	BOOST_CHECK_EQUAL(str, "hello");
}

// ============================================================================
// Additional String Utils Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_isnewline)
{
	BOOST_CHECK(utils::isnewline('\n'));
	BOOST_CHECK(!utils::isnewline('a'));
	BOOST_CHECK(!utils::isnewline(' '));
}

BOOST_AUTO_TEST_CASE(test_portable_isspace)
{
	BOOST_CHECK(utils::portable_isspace(' '));
	BOOST_CHECK(utils::portable_isspace('\t'));
	BOOST_CHECK(utils::portable_isspace('\n'));
	BOOST_CHECK(utils::portable_isspace('\r'));
	BOOST_CHECK(!utils::portable_isspace('a'));
	BOOST_CHECK(!utils::portable_isspace('1'));
}

BOOST_AUTO_TEST_CASE(test_escape_special_chars)
{
	std::string input = "test#string@with{special}chars";
	std::string result = utils::escape(input);

	BOOST_CHECK(result.find("\\#") != std::string::npos);
	BOOST_CHECK(result.find("\\@") != std::string::npos);
	BOOST_CHECK(result.find("\\{") != std::string::npos);
	BOOST_CHECK(result.find("\\}") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(test_unescape)
{
	std::string input = "test\\#string\\@with\\{special\\}chars";
	std::string result = utils::unescape(input);

	BOOST_CHECK(result.find("\\") == std::string::npos);
	BOOST_CHECK(result.find("#") != std::string::npos);
	BOOST_CHECK(result.find("@") != std::string::npos);
	BOOST_CHECK(result.find("{") != std::string::npos);
	BOOST_CHECK(result.find("}") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(test_urlencode_basic)
{
	std::string input = "hello world";
	std::string result = utils::urlencode(input);

	BOOST_CHECK(result.find("hello") != std::string::npos);
	BOOST_CHECK(result.find("%20") != std::string::npos); // space encoded
	BOOST_CHECK(result.find("world") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(test_urlencode_special_chars)
{
	std::string input = "key=value&other=test";
	std::string result = utils::urlencode(input);

	BOOST_CHECK(result.find("%3D") != std::string::npos); // = encoded
	BOOST_CHECK(result.find("%26") != std::string::npos); // & encoded
}

BOOST_AUTO_TEST_CASE(test_bool_string)
{
	BOOST_CHECK_EQUAL(utils::bool_string(true), "true");
	BOOST_CHECK_EQUAL(utils::bool_string(false), "false");
}

BOOST_AUTO_TEST_CASE(test_join_map)
{
	std::map<std::string, std::string> test_map;
	test_map["key1"] = "value1";
	test_map["key2"] = "value2";

	std::string result = utils::join_map(test_map, ",", ":" );

	BOOST_CHECK(result.find("key1:value1") != std::string::npos);
	BOOST_CHECK(result.find("key2:value2") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(test_indent_basic)
{
	std::string text = "line1\nline2\nline3";
	std::string result = utils::indent(text, 2);

	// Should indent by 2 tabs (8 spaces worth)
	BOOST_CHECK(result.substr(0, 2) == std::string(2, '\t'));
}

BOOST_AUTO_TEST_CASE(test_indent_empty_string)
{
	std::string text = "";
	std::string result = utils::indent(text, 2);

	// Empty string should return indentation unit
	BOOST_CHECK_EQUAL(result, std::string(2, '\t'));
}

// ============================================================================
// Tokenizer Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_tokenizer_simple_string)
{
	std::istringstream input("hello");
	tokenizer tok(input);

	const token& t = tok.next_token();
	BOOST_CHECK_EQUAL(t.type, token::STRING);
	BOOST_CHECK_EQUAL(t.value, "hello");
}

BOOST_AUTO_TEST_CASE(test_tokenizer_equals_sign)
{
	std::istringstream input("=");
	tokenizer tok(input);

	const token& t = tok.next_token();
	BOOST_CHECK_EQUAL(t.type, token::EQUALS);
}

BOOST_AUTO_TEST_CASE(test_tokenizer_brackets)
{
	std::istringstream input("[test]");
	tokenizer tok(input);

	const token& t1 = tok.next_token();
	BOOST_CHECK_EQUAL(t1.type, token::OPEN_BRACKET);

	const token& t2 = tok.next_token();
	BOOST_CHECK_EQUAL(t2.type, token::STRING);
	BOOST_CHECK_EQUAL(t2.value, "test");

	const token& t3 = tok.next_token();
	BOOST_CHECK_EQUAL(t3.type, token::CLOSE_BRACKET);
}

BOOST_AUTO_TEST_CASE(test_tokenizer_newline)
{
	std::istringstream input("\n");
	tokenizer tok(input);

	const token& t = tok.next_token();
	BOOST_CHECK_EQUAL(t.type, token::NEWLINE);
}

BOOST_AUTO_TEST_CASE(test_tokenizer_end_of_file)
{
	std::istringstream input("");
	tokenizer tok(input);

	const token& t = tok.next_token();
	BOOST_CHECK_EQUAL(t.type, token::END);
}

BOOST_AUTO_TEST_CASE(test_tokenizer_quoted_string)
{
	std::istringstream input("\"quoted value\"");
	tokenizer tok(input);

	const token& t = tok.next_token();
	BOOST_CHECK_EQUAL(t.type, token::QSTRING);
}

// ============================================================================
// Round-trip Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_roundtrip_simple_config)
{
	config original;
	original["key1"] = "value1";
	original["key2"] = "value2";

	// Write to string
	std::ostringstream output;
	io::write(output, original);

	// Read back
	std::istringstream input(output.str());
	config parsed = io::read(input);

	// Compare
	BOOST_CHECK_EQUAL(parsed["key1"].str(), "value1");
	BOOST_CHECK_EQUAL(parsed["key2"].str(), "value2");
}

BOOST_AUTO_TEST_CASE(test_roundtrip_nested_config)
{
	config original;
	config& child = original.add_child("child");
	child["attr1"] = "val1";
	child["attr2"] = "val2";

	// Write to string
	std::ostringstream output;
	io::write(output, original);

	// Read back
	std::istringstream input(output.str());
	config parsed = io::read(input);

	// Compare
	BOOST_CHECK(parsed.has_child("child"));
	const config::const_child_iterator child_iter = parsed.find_child("child");
	BOOST_REQUIRE(child_iter != parsed.child_end());

	const config& parsed_child = child_iter->second;
	BOOST_CHECK_EQUAL(parsed_child["attr1"].str(), "val1");
	BOOST_CHECK_EQUAL(parsed_child["attr2"].str(), "val2");
}

BOOST_AUTO_TEST_CASE(test_roundtrip_deep_nesting)
{
	config original;
	config* current = &original.add_child("level0");

	for (int i = 1; i < 5; ++i) {
		current = &current->add_child("level" + std::to_string(i));
		current->["depth"] = std::to_string(i);
	}

	// Write to string
	std::ostringstream output;
	io::write(output, original);

	// Read back
	std::istringstream input(output.str());
	config parsed = io::read(input);

	// Verify structure
	BOOST_CHECK(parsed.has_child("level0"));
}

// ============================================================================
// WML Escape Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_wml_escape_string_quotes)
{
	std::string input = "say \"hello\"";
	std::string result = utils::wml_escape_string(input);

	BOOST_CHECK_EQUAL(result, "say \"\"hello\"\"");
}

BOOST_AUTO_TEST_CASE(test_wml_escape_string_no_quotes)
{
	std::string input = "no quotes here";
	std::string result = utils::wml_escape_string(input);

	BOOST_CHECK_EQUAL(result, input);
}

BOOST_AUTO_TEST_CASE(test_wml_escape_strong_basic)
{
	std::string input = "test >> value";
	std::string result = utils::wml_escape_strong(input);

	BOOST_CHECK(result.find(">>") != std::string::npos);
	BOOST_CHECK(result.find("\">>\"<") != std::string::npos);
}

// ============================================================================
// Signed Value Tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_signed_value_positive)
{
	std::string result = utils::signed_value(42);
	BOOST_CHECK(result.find("42") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(test_signed_value_negative)
{
	std::string result = utils::signed_value(-42);
	BOOST_CHECK(result.find("-") != std::string::npos || result.find("\xE2\x88\x92") != std::string::npos); // Unicode minus
}

BOOST_AUTO_TEST_CASE(test_signed_value_zero)
{
	std::string result = utils::signed_value(0);
	BOOST_CHECK(result.find("0") != std::string::npos || result.find("+0") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(test_half_signed_value_positive)
{
	std::string result = utils::half_signed_value(42);
	BOOST_CHECK(result.find("42") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(test_half_signed_value_negative)
{
	std::string result = utils::half_signed_value(-42);
	BOOST_CHECK(result.find("-") != std::string::npos || result.find("\xE2\x88\x92") != std::string::npos);
}

BOOST_AUTO_TEST_SUITE_END()
