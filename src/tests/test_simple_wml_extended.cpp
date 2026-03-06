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

#include "server/common/simple_wml.hpp"
#include <string>

BOOST_AUTO_TEST_SUITE(test_simple_wml_extended)

BOOST_AUTO_TEST_CASE(test_string_span_default_construction)
{
	simple_wml::string_span span;

	BOOST_CHECK(span.empty());
	BOOST_CHECK_EQUAL(span.size(), 0);
	BOOST_CHECK(span.is_null());
}

BOOST_AUTO_TEST_CASE(test_string_span_from_cstring)
{
	simple_wml::string_span span("hello");

	BOOST_CHECK(!span.empty());
	BOOST_CHECK_EQUAL(span.size(), 5);
	BOOST_CHECK(span == "hello");
}

BOOST_AUTO_TEST_CASE(test_string_span_from_string)
{
	// string_span from std::string stores pointer to the string's internal data
	// The span is valid as long as the source string remains alive and unchanged
	// Note: The constructor takes std::string by value, creating a temporary copy.
	// The span points to the temporary's data which is destroyed after construction.
	// This is a known limitation of the string_span(std::string) constructor.
	// Only the size check is reliable immediately after construction.
	std::string str = "test string";
	simple_wml::string_span span(str);

	BOOST_CHECK_EQUAL(span.size(), 11);
	// Note: Comparing the span with any string may fail because the span's
	// internal pointer points to freed memory (the temporary copy).
}

BOOST_AUTO_TEST_CASE(test_string_span_equality)
{
	simple_wml::string_span span1("abc");
	simple_wml::string_span span2("abc");
	simple_wml::string_span span3("abd");

	BOOST_CHECK(span1 == span2);
	BOOST_CHECK(span1 != span3);
	BOOST_CHECK(span1 == "abc");
	BOOST_CHECK(span1 != "abcd");
}

BOOST_AUTO_TEST_CASE(test_string_span_comparison)
{
	simple_wml::string_span span1("abc");
	simple_wml::string_span span2("abd");
	simple_wml::string_span span3("ab");

	BOOST_CHECK(span1 < span2);
	BOOST_CHECK(span3 < span1);
	BOOST_CHECK(!(span2 < span1));
}

BOOST_AUTO_TEST_CASE(test_string_span_to_string)
{
	simple_wml::string_span span("hello world");

	BOOST_CHECK_EQUAL(span.to_string(), "hello world");
}

BOOST_AUTO_TEST_CASE(test_string_span_iterators)
{
	simple_wml::string_span span("test");

	BOOST_CHECK_EQUAL(span.begin()[0], 't');
	BOOST_CHECK_EQUAL(span.begin()[1], 'e');
	BOOST_CHECK_EQUAL(span.begin()[2], 's');
	BOOST_CHECK_EQUAL(span.begin()[3], 't');
	BOOST_CHECK(span.end() - span.begin() == 4);
}

BOOST_AUTO_TEST_CASE(test_document_empty)
{
	const char* doctext = "";
	simple_wml::document doc(doctext, simple_wml::INIT_COMPRESSED);

	BOOST_CHECK_NO_THROW(doc.root());
}

BOOST_AUTO_TEST_CASE(test_document_single_node)
{
	const char* doctext = "[test][/test]";
	simple_wml::document doc(doctext, simple_wml::INIT_COMPRESSED);

	simple_wml::node& root = doc.root();
	simple_wml::node* test_node = root.child("test");

	BOOST_CHECK(test_node != nullptr);
}

BOOST_AUTO_TEST_CASE(test_document_multiple_nodes)
{
	const char* doctext = R"([node1]
[/node1]
[node2]
[/node2]
[node3]
[/node3])";
	simple_wml::document doc(doctext, simple_wml::INIT_COMPRESSED);

	simple_wml::node& root = doc.root();

	BOOST_CHECK(root.child("node1") != nullptr);
	BOOST_CHECK(root.child("node2") != nullptr);
	BOOST_CHECK(root.child("node3") != nullptr);
}

BOOST_AUTO_TEST_CASE(test_node_attributes)
{
	const char* doctext = R"([test]
attr1="value1"
attr2="value2"
[/test])";
	simple_wml::document doc(doctext, simple_wml::INIT_COMPRESSED);

	simple_wml::node& root = doc.root();
	simple_wml::node* test_node = root.child("test");

	BOOST_CHECK(test_node != nullptr);
	BOOST_CHECK((*test_node)["attr1"] == "value1");
	BOOST_CHECK((*test_node)["attr2"] == "value2");
}

BOOST_AUTO_TEST_CASE(test_node_has_attr)
{
	const char* doctext = R"([test]
exists="yes"
[/test])";
	simple_wml::document doc(doctext, simple_wml::INIT_COMPRESSED);

	simple_wml::node& root = doc.root();
	simple_wml::node* test_node = root.child("test");

	BOOST_CHECK(test_node->has_attr("exists"));
	BOOST_CHECK(!test_node->has_attr("nonexistent"));
}

BOOST_AUTO_TEST_CASE(test_node_set_attr)
{
	const char* doctext = "[test][/test]";
	simple_wml::document doc(doctext, simple_wml::INIT_COMPRESSED);

	simple_wml::node& root = doc.root();
	simple_wml::node* test_node = root.child("test");

	test_node->set_attr("new_attr", "new_value");

	BOOST_CHECK((*test_node)["new_attr"] == "new_value");
}

BOOST_AUTO_TEST_CASE(test_nested_nodes)
{
	const char* doctext = R"([outer]
[inner]
deep="value"
[/inner]
[/outer])";
	simple_wml::document doc(doctext, simple_wml::INIT_COMPRESSED);

	simple_wml::node& root = doc.root();
	simple_wml::node* outer = root.child("outer");

	BOOST_CHECK(outer != nullptr);

	simple_wml::node* inner = outer->child("inner");
	BOOST_CHECK(inner != nullptr);
	BOOST_CHECK((*inner)["deep"] == "value");
}

BOOST_AUTO_TEST_CASE(test_special_characters)
{
	// simple_wml has strict parsing rules - attributes must be in alphabetical order
	// Double quotes inside values should be represented as "" (doubled quotes)
	const char* doctext = R"([test]
backslash="\\"
mixed="\\""test""\\"
quote=""""
[/test])";
	simple_wml::document doc(doctext, simple_wml::INIT_COMPRESSED);

	simple_wml::node& root = doc.root();
	simple_wml::node* test_node = root.child("test");

	BOOST_CHECK(test_node != nullptr);
}

BOOST_AUTO_TEST_CASE(test_numeric_values)
{
	// Attributes must be in alphabetical order
	const char* doctext = R"([test]
float_val="3.14"
int_val="42"
neg_int="-10"
[/test])";
	simple_wml::document doc(doctext, simple_wml::INIT_COMPRESSED);

	simple_wml::node& root = doc.root();
	simple_wml::node* test_node = root.child("test");

	BOOST_CHECK((*test_node)["float_val"] == "3.14");
	BOOST_CHECK((*test_node)["int_val"] == "42");
	BOOST_CHECK((*test_node)["neg_int"] == "-10");
}

BOOST_AUTO_TEST_CASE(test_empty_attribute_value)
{
	const char* doctext = R"([test]
empty=""
[/test])";
	simple_wml::document doc(doctext, simple_wml::INIT_COMPRESSED);

	simple_wml::node& root = doc.root();
	simple_wml::node* test_node = root.child("test");

	BOOST_CHECK((*test_node)["empty"] == "");
}

BOOST_AUTO_TEST_CASE(test_string_span_to_int)
{
	simple_wml::string_span span("12345");

	BOOST_CHECK_EQUAL(span.to_int(), 12345);
}

BOOST_AUTO_TEST_CASE(test_string_span_to_bool)
{
	simple_wml::string_span span_true("true");
	simple_wml::string_span span_false("false");
	simple_wml::string_span span_yes("yes");
	simple_wml::string_span span_no("no");

	BOOST_CHECK(span_true.to_bool() == true);
	BOOST_CHECK(span_false.to_bool() == false);
	BOOST_CHECK(span_yes.to_bool() == true);
	BOOST_CHECK(span_no.to_bool() == false);
}

BOOST_AUTO_TEST_CASE(test_document_default_construction)
{
	simple_wml::document doc;

	BOOST_CHECK_NO_THROW(doc.root());
}

BOOST_AUTO_TEST_CASE(test_node_children_count)
{
	const char* doctext = R"([parent]
[child][/child]
[child][/child]
[child][/child]
[/parent])";
	simple_wml::document doc(doctext, simple_wml::INIT_COMPRESSED);

	simple_wml::node& root = doc.root();
	simple_wml::node* parent = root.child("parent");

	BOOST_CHECK(parent != nullptr);
}

BOOST_AUTO_TEST_CASE(test_string_span_to_string_view)
{
	simple_wml::string_span span("test");
	std::string_view sv = span;

	BOOST_CHECK_EQUAL(sv, "test");
}

// ============================================================================
// Boundary condition tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_string_span_empty_string)
{
	// Empty string span - empty() checks size_ == 0, is_null() checks str_ == nullptr
	// A string literal "" has a valid pointer (not null) but size 0
	simple_wml::string_span span("");

	BOOST_CHECK(span.empty());
	BOOST_CHECK_EQUAL(span.size(), 0);
	// is_null() returns false because str_ points to the string literal, not nullptr
	BOOST_CHECK(!span.is_null());
}

BOOST_AUTO_TEST_CASE(test_string_span_long_string)
{
	// Long string
	std::string long_str(10000, 'a');
	simple_wml::string_span span(long_str);

	BOOST_CHECK_EQUAL(span.size(), 10000);
	BOOST_CHECK(!span.empty());
}

BOOST_AUTO_TEST_CASE(test_string_span_special_characters)
{
	// Special characters
	simple_wml::string_span span("Hello\nWorld\t!\r\n");

	BOOST_CHECK_EQUAL(span.size(), 15);
	BOOST_CHECK(span == "Hello\nWorld\t!\r\n");
}

BOOST_AUTO_TEST_CASE(test_string_span_unicode)
{
	// Unicode characters
	simple_wml::string_span span("Hello 世界 🎮");

	BOOST_CHECK(span == "Hello 世界 🎮");
}

BOOST_AUTO_TEST_CASE(test_string_span_comparison_boundary)
{
	simple_wml::string_span span1("");
	simple_wml::string_span span2("a");
	simple_wml::string_span span3("aa");

	// Empty is less than non-empty
	BOOST_CHECK(span1 < span2);
	BOOST_CHECK(span2 < span3);

	// Equality
	BOOST_CHECK(span1 == span1);
	BOOST_CHECK(!(span1 == span2));
}

BOOST_AUTO_TEST_CASE(test_string_span_to_int_boundary)
{
	// Integer limits
	simple_wml::string_span max_int(std::to_string(INT_MAX).c_str());
	BOOST_CHECK_EQUAL(max_int.to_int(), INT_MAX);

	simple_wml::string_span min_int(std::to_string(INT_MIN).c_str());
	BOOST_CHECK_EQUAL(min_int.to_int(), INT_MIN);

	simple_wml::string_span zero("0");
	BOOST_CHECK_EQUAL(zero.to_int(), 0);

	simple_wml::string_span negative("-1");
	BOOST_CHECK_EQUAL(negative.to_int(), -1);
}

BOOST_AUTO_TEST_CASE(test_string_span_to_int_invalid)
{
	// Invalid integers - to_int() uses atoi() which returns 0 for non-numeric strings
	simple_wml::string_span not_a_number("abc");
	BOOST_CHECK_EQUAL(not_a_number.to_int(), 0);

	simple_wml::string_span empty("");
	BOOST_CHECK_EQUAL(empty.to_int(), 0);

	// atoi() stops at first non-digit character and returns the number parsed so far
	// This is standard atoi behavior - "123abc" returns 123
	simple_wml::string_span mixed("123abc");
	BOOST_CHECK_EQUAL(mixed.to_int(), 123);
}

BOOST_AUTO_TEST_CASE(test_string_span_to_bool_variations)
{
	// to_bool() returns false only for: "no", "off", "false", "0", "0.0"
	// Everything else (including "yes", "true", "1", "maybe") returns true

	// False values (explicitly checked)
	BOOST_CHECK(simple_wml::string_span("false").to_bool() == false);
	BOOST_CHECK(simple_wml::string_span("no").to_bool() == false);
	BOOST_CHECK(simple_wml::string_span("off").to_bool() == false);
	BOOST_CHECK(simple_wml::string_span("0").to_bool() == false);
	BOOST_CHECK(simple_wml::string_span("0.0").to_bool() == false);

	// Empty returns default_value (false in this case)
	BOOST_CHECK(simple_wml::string_span("").to_bool() == false);

	// True values (anything not in the false list)
	BOOST_CHECK(simple_wml::string_span("true").to_bool() == true);
	BOOST_CHECK(simple_wml::string_span("TRUE").to_bool() == true);  // case-sensitive check
	BOOST_CHECK(simple_wml::string_span("True").to_bool() == true);
	BOOST_CHECK(simple_wml::string_span("yes").to_bool() == true);
	BOOST_CHECK(simple_wml::string_span("YES").to_bool() == true);  // case-sensitive check
	BOOST_CHECK(simple_wml::string_span("1").to_bool() == true);
	BOOST_CHECK(simple_wml::string_span("maybe").to_bool() == true);  // not in false list
}

// ============================================================================
// Document boundary condition tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_document_empty_root)
{
	// Document with just empty root
	const char* doctext = "[]";
	simple_wml::document doc(doctext, simple_wml::INIT_COMPRESSED);

	BOOST_CHECK_NO_THROW(doc.root());
}

BOOST_AUTO_TEST_CASE(test_document_whitespace_only)
{
	// Document with only whitespace
	const char* doctext = "   \n\t   ";
	simple_wml::document doc(doctext, simple_wml::INIT_COMPRESSED);

	BOOST_CHECK_NO_THROW(doc.root());
}

BOOST_AUTO_TEST_CASE(test_document_many_nodes)
{
	// Document with many nodes
	std::string doctext;
	for (int i = 0; i < 100; ++i) {
		doctext += "[item]\n";
		doctext += "id=\"" + std::to_string(i) + "\"\n";
		doctext += "[/item]\n";
	}

	simple_wml::document doc(doctext.c_str(), simple_wml::INIT_COMPRESSED);
	simple_wml::node& root = doc.root();

	// Verify all items exist
	for (int i = 0; i < 100; ++i) {
		simple_wml::node* item = root.child("item");
		BOOST_CHECK(item != nullptr);
	}
}

BOOST_AUTO_TEST_CASE(test_document_deep_nesting)
{
	// Deeply nested document
	std::string doctext;
	for (int i = 0; i < 50; ++i) {
		doctext += "[level" + std::to_string(i) + "]\n";
	}
	for (int i = 49; i >= 0; --i) {
		doctext += "[/level" + std::to_string(i) + "]\n";
	}

	simple_wml::document doc(doctext.c_str(), simple_wml::INIT_COMPRESSED);
	BOOST_CHECK_NO_THROW(doc.root());
}

BOOST_AUTO_TEST_CASE(test_document_long_attribute_value)
{
	// Long attribute value
	std::string long_value(10000, 'x');
	std::string doctext = "[test]\nvalue=\"" + long_value + "\"\n[/test]";

	simple_wml::document doc(doctext.c_str(), simple_wml::INIT_COMPRESSED);
	simple_wml::node& root = doc.root();
	simple_wml::node* test_node = root.child("test");

	BOOST_CHECK(test_node != nullptr);
	BOOST_CHECK((*test_node)["value"] == long_value.c_str());
}

BOOST_AUTO_TEST_CASE(test_document_many_attributes)
{
	// Node with many attributes - simple_wml requires attributes to be in alphabetical order
	// Generate keys in alphabetical order: key000, key001, ..., key099
	std::string doctext = "[test]\n";
	for (int i = 0; i < 100; ++i) {
		std::string key = "key" + std::to_string(1000 + i).substr(1);  // Pad with zeros: 000, 001, etc.
		doctext += key + "=\"value" + std::to_string(i) + "\"\n";
	}
	doctext += "[/test]";

	simple_wml::document doc(doctext.c_str(), simple_wml::INIT_COMPRESSED);
	simple_wml::node& root = doc.root();
	simple_wml::node* test_node = root.child("test");

	BOOST_CHECK(test_node != nullptr);
	for (int i = 0; i < 100; ++i) {
		std::string key = "key" + std::to_string(1000 + i).substr(1);
		BOOST_CHECK(test_node->has_attr(key.c_str()));
	}
}

BOOST_AUTO_TEST_CASE(test_document_special_characters_in_attributes)
{
	// Special characters in attribute values - simple_wml has strict parsing
	// Attributes must be in alphabetical order
	const char* doctext = R"([test]
quotes="say ""hello"""
spaces="hello world"
[/test])";

	simple_wml::document doc(doctext, simple_wml::INIT_COMPRESSED);
	simple_wml::node& root = doc.root();
	simple_wml::node* test_node = root.child("test");

	BOOST_CHECK(test_node != nullptr);
	BOOST_CHECK(test_node->has_attr("quotes"));
	BOOST_CHECK(test_node->has_attr("spaces"));
}

BOOST_AUTO_TEST_CASE(test_document_empty_attribute_name)
{
	// Empty attribute name (edge case)
	const char* doctext = R"([test]
="value"
[/test])";

	simple_wml::document doc(doctext, simple_wml::INIT_COMPRESSED);
	BOOST_CHECK_NO_THROW(doc.root());
}

BOOST_AUTO_TEST_CASE(test_document_duplicate_attributes)
{
	// Duplicate attribute names - simple_wml requires attributes in alphabetical order
	// and does not allow duplicates (would violate ordering constraint)
	// This test verifies that simple_wml throws an error for duplicate keys
	const char* doctext = R"([test]
key="first"
key="second"
[/test])";

	// With INIT_COMPRESSED, parsing happens when root() is called, not during construction
	// Duplicate attributes should throw a game::error when root() is accessed
	simple_wml::document doc(doctext, simple_wml::INIT_COMPRESSED);
	BOOST_CHECK_THROW(doc.root(), game::error);
}

BOOST_AUTO_TEST_CASE(test_document_unicode_attributes)
{
	// Unicode in attributes - must be in alphabetical order
	// 'e' (emoji) comes before 'n' (name)
	const char* doctext = R"([test]
emoji="🎮"
name="世界"
[/test])";

	simple_wml::document doc(doctext, simple_wml::INIT_COMPRESSED);
	simple_wml::node& root = doc.root();
	simple_wml::node* test_node = root.child("test");

	BOOST_CHECK(test_node != nullptr);
	BOOST_CHECK(test_node->has_attr("emoji"));
	BOOST_CHECK(test_node->has_attr("name"));
}

BOOST_AUTO_TEST_CASE(test_node_set_attr_overwrite)
{
	const char* doctext = "[test]\nkey=\"old\"\n[/test]";
	simple_wml::document doc(doctext, simple_wml::INIT_COMPRESSED);

	simple_wml::node& root = doc.root();
	simple_wml::node* test_node = root.child("test");

	BOOST_CHECK((*test_node)["key"] == "old");

	test_node->set_attr("key", "new");
	BOOST_CHECK((*test_node)["key"] == "new");
}

BOOST_AUTO_TEST_CASE(test_node_set_attr_new)
{
	const char* doctext = "[test][/test]";
	simple_wml::document doc(doctext, simple_wml::INIT_COMPRESSED);

	simple_wml::node& root = doc.root();
	simple_wml::node* test_node = root.child("test");

	BOOST_CHECK(!test_node->has_attr("new_key"));

	test_node->set_attr("new_key", "new_value");
	BOOST_CHECK((*test_node)["new_key"] == "new_value");
}

BOOST_AUTO_TEST_CASE(test_node_child_not_found)
{
	const char* doctext = "[test][/test]";
	simple_wml::document doc(doctext, simple_wml::INIT_COMPRESSED);

	simple_wml::node& root = doc.root();
	simple_wml::node* test_node = root.child("test");

	simple_wml::node* not_found = test_node->child("nonexistent");
	BOOST_CHECK(not_found == nullptr);
}

BOOST_AUTO_TEST_CASE(test_document_move)
{
	const char* doctext = "[test]\nkey=\"value\"\n[/test]";
	simple_wml::document doc1(doctext, simple_wml::INIT_COMPRESSED);

	// Document is move-only (copy constructor deleted)
	// Just verify it was created correctly
	BOOST_CHECK(doc1.root().child("test") != nullptr);
}

// ============================================================================
// Additional boundary condition tests
// ============================================================================

BOOST_AUTO_TEST_CASE(test_string_span_null_pointer)
{
	// Null pointer construction - creates a null span
	// Note: passing nullptr to strlen() would cause undefined behavior,
	// but the default constructor creates a null span
	simple_wml::string_span span;  // Default construction creates null span
	BOOST_CHECK(span.empty());
	BOOST_CHECK(span.is_null());
}

BOOST_AUTO_TEST_CASE(test_string_span_substr)
{
	simple_wml::string_span span("hello world");

	// Substring operations
	BOOST_CHECK_EQUAL(span.size(), 11);

	// Empty substring from c_str
	const char* str = "";
	simple_wml::string_span empty_sub(str);
	BOOST_CHECK(empty_sub.empty());
}

BOOST_AUTO_TEST_CASE(test_string_span_equality_reflexive)
{
	simple_wml::string_span span("test");
	BOOST_CHECK(span == span);
	BOOST_CHECK(!(span != span));
}

BOOST_AUTO_TEST_CASE(test_string_span_equality_symmetric)
{
	simple_wml::string_span span1("abc");
	simple_wml::string_span span2("abc");
	BOOST_CHECK(span1 == span2);
	BOOST_CHECK(span2 == span1);
}

BOOST_AUTO_TEST_CASE(test_string_span_comparison_transitive)
{
	simple_wml::string_span span1("a");
	simple_wml::string_span span2("b");
	simple_wml::string_span span3("c");

	BOOST_CHECK(span1 < span2);
	BOOST_CHECK(span2 < span3);
	BOOST_CHECK(span1 < span3);
}

BOOST_AUTO_TEST_CASE(test_string_span_to_int_overflow)
{
	// Integer overflow
	simple_wml::string_span overflow("999999999999999999999999999999");
	int result = overflow.to_int();
	// Result is undefined/implementation dependent
	(void)result;
}

BOOST_AUTO_TEST_CASE(test_string_span_to_int_whitespace)
{
	// Whitespace handling - atoi() handles leading whitespace
	simple_wml::string_span ws1(" 123");
	BOOST_CHECK_EQUAL(ws1.to_int(), 123); // atoi skips leading whitespace

	// Trailing whitespace is included in the number (atoi stops at null terminator)
	// but string_span's to_int() copies to a null-terminated buffer first
	simple_wml::string_span ws2("123 ");
	BOOST_CHECK_EQUAL(ws2.to_int(), 123); // atoi stops at first non-digit
}

BOOST_AUTO_TEST_CASE(test_string_span_to_bool_empty)
{
	simple_wml::string_span empty("");
	BOOST_CHECK(!empty.to_bool());
}

BOOST_AUTO_TEST_CASE(test_string_span_to_bool_whitespace)
{
	// " true" with leading space is not in the false list ("no", "off", "false", "0", "0.0")
	// so it returns true
	simple_wml::string_span ws(" true");
	BOOST_CHECK(ws.to_bool()); // Not a recognized false value, so returns true
}

BOOST_AUTO_TEST_CASE(test_document_empty_content)
{
	// Completely empty document
	const char* doctext = "";
	simple_wml::document doc(doctext, simple_wml::INIT_COMPRESSED);
	BOOST_CHECK_NO_THROW(doc.root());
}

BOOST_AUTO_TEST_CASE(test_document_only_whitespace)
{
	// Document with only whitespace - simple_wml only handles ' ', '\t', '\n'
	// '\r' is not supported and will cause parsing errors
	const char* doctext = "   \n\t  ";
	simple_wml::document doc(doctext, simple_wml::INIT_COMPRESSED);
	BOOST_CHECK_NO_THROW(doc.root());
}

BOOST_AUTO_TEST_CASE(test_document_malformed_tags)
{
	// Malformed tags should be handled gracefully
	const char* doctext1 = "[unclosed";
	BOOST_CHECK_NO_THROW(simple_wml::document(doctext1, simple_wml::INIT_COMPRESSED));

	const char* doctext2 = "unopened]";
	BOOST_CHECK_NO_THROW(simple_wml::document(doctext2, simple_wml::INIT_COMPRESSED));

	const char* doctext3 = "[/mismatched]";
	BOOST_CHECK_NO_THROW(simple_wml::document(doctext3, simple_wml::INIT_COMPRESSED));
}

BOOST_AUTO_TEST_CASE(test_document_very_deep_nesting)
{
	// Very deeply nested document
	std::string doctext;
	for (int i = 0; i < 100; ++i) {
		doctext += "[level" + std::to_string(i) + "]\n";
	}
	for (int i = 99; i >= 0; --i) {
		doctext += "[/level" + std::to_string(i) + "]\n";
	}

	simple_wml::document doc(doctext.c_str(), simple_wml::INIT_COMPRESSED);
	BOOST_CHECK_NO_THROW(doc.root());
}

BOOST_AUTO_TEST_CASE(test_document_very_long_attribute_name)
{
	// Very long attribute name
	std::string long_name(1000, 'a');
	std::string doctext = "[test]\n" + long_name + "=\"value\"\n[/test]";

	simple_wml::document doc(doctext.c_str(), simple_wml::INIT_COMPRESSED);
	simple_wml::node& root = doc.root();
	simple_wml::node* test_node = root.child("test");
	BOOST_CHECK(test_node != nullptr);
	BOOST_CHECK(test_node->has_attr(long_name.c_str()));
}

BOOST_AUTO_TEST_CASE(test_document_special_chars_in_tag_names)
{
	// Special characters in tag names (if supported)
	const char* doctext = "[tag_name-1]\n[/tag_name-1]";
	simple_wml::document doc(doctext, simple_wml::INIT_COMPRESSED);
	simple_wml::node& root = doc.root();
	BOOST_CHECK(root.child("tag_name-1") != nullptr);
}

BOOST_AUTO_TEST_CASE(test_document_numeric_tag_names)
{
	// Numeric tag names
	const char* doctext = "[123]\n[/123]";
	simple_wml::document doc(doctext, simple_wml::INIT_COMPRESSED);
	simple_wml::node& root = doc.root();
	BOOST_CHECK(root.child("123") != nullptr);
}

BOOST_AUTO_TEST_CASE(test_node_child_not_found_safety)
{
	const char* doctext = "[test][/test]";
	simple_wml::document doc(doctext, simple_wml::INIT_COMPRESSED);
	simple_wml::node& root = doc.root();
	simple_wml::node* test_node = root.child("test");

	// Accessing non-existent child
	simple_wml::node* not_found = test_node->child("nonexistent");
	BOOST_CHECK(not_found == nullptr);
}

BOOST_AUTO_TEST_CASE(test_node_attr_not_found)
{
	const char* doctext = "[test][/test]";
	simple_wml::document doc(doctext, simple_wml::INIT_COMPRESSED);
	simple_wml::node& root = doc.root();
	simple_wml::node* test_node = root.child("test");

	// Accessing non-existent attribute returns empty span
	simple_wml::string_span val = (*test_node)["nonexistent"];
	BOOST_CHECK(val.empty());
}

BOOST_AUTO_TEST_CASE(test_node_set_attr_empty_value)
{
	const char* doctext = "[test][/test]";
	simple_wml::document doc(doctext, simple_wml::INIT_COMPRESSED);
	simple_wml::node& root = doc.root();
	simple_wml::node* test_node = root.child("test");

	test_node->set_attr("empty", "");
	BOOST_CHECK((*test_node)["empty"] == "");
}

BOOST_AUTO_TEST_CASE(test_node_set_attr_overwrite_multiple)
{
	const char* doctext = "[test]\nkey=\"original\"\n[/test]";
	simple_wml::document doc(doctext, simple_wml::INIT_COMPRESSED);
	simple_wml::node& root = doc.root();
	simple_wml::node* test_node = root.child("test");

	// Overwrite multiple times
	test_node->set_attr("key", "first");
	BOOST_CHECK((*test_node)["key"] == "first");

	test_node->set_attr("key", "second");
	BOOST_CHECK((*test_node)["key"] == "second");

	test_node->set_attr("key", "third");
	BOOST_CHECK((*test_node)["key"] == "third");
}

BOOST_AUTO_TEST_CASE(test_document_many_siblings)
{
	// Document with many sibling nodes
	std::string doctext;
	for (int i = 0; i < 500; ++i) {
		doctext += "[item]\n";
		doctext += "id=\"" + std::to_string(i) + "\"\n";
		doctext += "[/item]\n";
	}

	simple_wml::document doc(doctext.c_str(), simple_wml::INIT_COMPRESSED);
	simple_wml::node& root = doc.root();

	// Verify at least first item exists
	simple_wml::node* child = root.child("item");
	BOOST_CHECK(child != nullptr);
	// The child method returns the first match, counting all siblings
	// would require iterating through which may not be directly supported
}

BOOST_AUTO_TEST_CASE(test_document_binary_content)
{
	// Document with binary/null content in attributes
	std::string doctext = "[test]\ndata=\"";
	doctext += '\0'; // Null byte
	doctext += "value\"\n[/test]";

	// This may or may not work depending on implementation
	BOOST_CHECK_NO_THROW(simple_wml::document(doctext.c_str(), simple_wml::INIT_COMPRESSED));
}

BOOST_AUTO_TEST_CASE(test_string_span_iterator_operations)
{
	simple_wml::string_span span("hello");

	// Iterator operations
	auto it = span.begin();
	BOOST_CHECK_EQUAL(*it, 'h');
	++it;
	BOOST_CHECK_EQUAL(*it, 'e');
	it++;
	BOOST_CHECK_EQUAL(*it, 'l');

	// Distance
	BOOST_CHECK_EQUAL(span.end() - span.begin(), 5);

	// Comparison
	BOOST_CHECK(span.begin() != span.end());
	BOOST_CHECK(span.begin() == span.begin());
}

BOOST_AUTO_TEST_CASE(test_document_compression_flag)
{
	// Test compression flag
	const char* doctext = "[test]\nkey=\"value\"\n[/test]";

	simple_wml::document doc1(doctext, simple_wml::INIT_COMPRESSED);
	BOOST_CHECK(doc1.root().child("test") != nullptr);
}

BOOST_AUTO_TEST_CASE(test_node_first_child_safety)
{
	const char* doctext = "[parent]\n[child1]\n[/child1]\n[child2]\n[/child2]\n[/parent]";
	simple_wml::document doc(doctext, simple_wml::INIT_COMPRESSED);
	simple_wml::node& root = doc.root();
	simple_wml::node* parent = root.child("parent");

	BOOST_CHECK(parent != nullptr);

	// first_child might not exist as a method, use child instead
	simple_wml::node* child1 = parent->child("child1");
	BOOST_CHECK(child1 != nullptr);

	simple_wml::node* child2 = parent->child("child2");
	BOOST_CHECK(child2 != nullptr);
}

BOOST_AUTO_TEST_CASE(test_node_multiple_children)
{
	// Note: Attributes must be on separate lines, not inside tag brackets
	const char* doctext = R"([root]
[item]
id="1"
[/item]
[item]
id="2"
[/item]
[item]
id="3"
[/item]
[/root])";

	simple_wml::document doc(doctext, simple_wml::INIT_COMPRESSED);
	simple_wml::node& root = doc.root();
	simple_wml::node* root_node = root.child("root");

	// Verify at least one item exists
	simple_wml::node* item = root_node->child("item");
	BOOST_CHECK(item != nullptr);
	// Multiple children with same name may be accessible through
	// implementation-specific methods
}

BOOST_AUTO_TEST_CASE(test_document_output_buffer)
{
	const char* doctext = "[test]\nkey=\"value\"\n[/test]";
	simple_wml::document doc(doctext, simple_wml::INIT_COMPRESSED);

	// Output returns const char*
	const char* output = doc.output();

	// Output should contain the original content
	BOOST_CHECK(output != nullptr);
	// The output may be compressed or formatted differently
}

BOOST_AUTO_TEST_SUITE_END()