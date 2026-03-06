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
#include "help/help_impl.hpp"
#include <memory>
#include <string>

BOOST_AUTO_TEST_SUITE( help_v2 )

// ============================================================================
// Help Topic Structure Tests
// ============================================================================

BOOST_AUTO_TEST_CASE( test_topic_creation )
{
	help::topic t("Test Topic", "test_id", "This is test content");

	BOOST_CHECK_EQUAL(t.title, "Test Topic");
	BOOST_CHECK_EQUAL(t.id, "test_id");
}

BOOST_AUTO_TEST_CASE( test_topic_equality )
{
	help::topic t1("First Title", "same_id", "Content 1");
	help::topic t2("Second Title", "same_id", "Content 2");

	// Topics are equal if their IDs are equal
	BOOST_CHECK(t1 == t2);
	BOOST_CHECK(!(t1 != t2));
}

BOOST_AUTO_TEST_CASE( test_topic_inequality )
{
	help::topic t1("Title", "id_one", "Content");
	help::topic t2("Title", "id_two", "Content");

	BOOST_CHECK(!(t1 == t2));
	BOOST_CHECK(t1 != t2);
}

BOOST_AUTO_TEST_CASE( test_topic_less_than )
{
	help::topic t1("Title A", "alpha", "Content");
	help::topic t2("Title B", "beta", "Content");

	// Comparison should be on ID
	BOOST_CHECK(t1 < t2 || t2 < t1); // One should be less than the other
	BOOST_CHECK(!(t1 < t2 && t2 < t1)); // But not both
}

// ============================================================================
// Help Section Structure Tests
// ============================================================================

BOOST_AUTO_TEST_CASE( test_section_creation )
{
	help::section sec;
	sec.id = "test_section";
	sec.title = "Test Section";

	BOOST_CHECK_EQUAL(sec.id, "test_section");
	BOOST_CHECK_EQUAL(sec.title, "Test Section");
	BOOST_CHECK(sec.topics.empty());
	BOOST_CHECK(sec.sections.empty());
}

BOOST_AUTO_TEST_CASE( test_section_add_section )
{
	help::section parent;
	parent.id = "parent_section";
	parent.title = "Parent";

	help::section child;
	child.id = "child_section";
	child.title = "Child";

	parent.add_section(child);

	BOOST_CHECK_EQUAL(parent.sections.size(), 1u);
	BOOST_CHECK_EQUAL(parent.sections.front().id, "child_section");
}

BOOST_AUTO_TEST_CASE( test_section_add_section_move )
{
	help::section parent;
	parent.id = "parent";

	help::section child;
	child.id = "child";
	child.title = "Child Title";

	parent.add_section(std::move(child));

	BOOST_CHECK_EQUAL(parent.sections.size(), 1u);
	BOOST_CHECK_EQUAL(parent.sections.front().id, "child");
}

BOOST_AUTO_TEST_CASE( test_section_equality )
{
	help::section s1;
	s1.id = "same_id";

	help::section s2;
	s2.id = "same_id";

	// Sections are equal if their IDs are equal
	BOOST_CHECK(s1 == s2);
}

BOOST_AUTO_TEST_CASE( test_section_clear )
{
	help::section sec;
	sec.id = "test";
	sec.title = "Test";

	help::topic t("Topic", "topic_id", "Content");
	sec.topics.push_back(t);

	help::section child;
	child.id = "child";
	sec.sections.push_back(child);

	BOOST_CHECK(!sec.topics.empty());
	BOOST_CHECK(!sec.sections.empty());

	sec.clear();

	BOOST_CHECK(sec.topics.empty());
	BOOST_CHECK(sec.sections.empty());
}

// ============================================================================
// has_id Function Object Tests
// ============================================================================

BOOST_AUTO_TEST_CASE( test_has_id_with_topic )
{
	help::topic t("Title", "target_id", "Content");
	help::has_id predicate("target_id");

	BOOST_CHECK(predicate(t));
}

BOOST_AUTO_TEST_CASE( test_has_id_with_topic_negative )
{
	help::topic t("Title", "some_id", "Content");
	help::has_id predicate("different_id");

	BOOST_CHECK(!predicate(t));
}

BOOST_AUTO_TEST_CASE( test_has_id_with_section )
{
	help::section s;
	s.id = "target_section";
	help::has_id predicate("target_section");

	BOOST_CHECK(predicate(s));
}

BOOST_AUTO_TEST_CASE( test_has_id_with_section_pointer )
{
	help::section s;
	s.id = "pointer_section";
	help::has_id predicate("pointer_section");

	BOOST_CHECK(predicate(&s));
}

BOOST_AUTO_TEST_CASE( test_has_id_with_null_section_pointer )
{
	help::section* s = nullptr;
	help::has_id predicate("any_id");

	BOOST_CHECK(!predicate(s));
}

// ============================================================================
// ID Visibility Tests
// ============================================================================

BOOST_AUTO_TEST_CASE( test_is_visible_id_normal )
{
	BOOST_CHECK(help::is_visible_id("normal_id"));
	BOOST_CHECK(help::is_visible_id("visible_topic"));
	BOOST_CHECK(help::is_visible_id("some_section"));
}

BOOST_AUTO_TEST_CASE( test_is_visible_id_hidden )
{
	// IDs starting with '.' are hidden
	BOOST_CHECK(!help::is_visible_id(".hidden_id"));
	BOOST_CHECK(!help::is_visible_id(".another_hidden"));
}

BOOST_AUTO_TEST_CASE( test_is_visible_id_empty )
{
	// Empty ID is considered visible (or at least not hidden by '.')
	BOOST_CHECK(help::is_visible_id(""));
}

// ============================================================================
// ID Validation Tests
// ============================================================================

BOOST_AUTO_TEST_CASE( test_is_valid_id_normal )
{
	BOOST_CHECK(help::is_valid_id("normal_id"));
	BOOST_CHECK(help::is_valid_id("custom_topic"));
	BOOST_CHECK(help::is_valid_id("my_section"));
}

BOOST_AUTO_TEST_CASE( test_is_valid_id_toplevel_invalid )
{
	// 'toplevel' is a special ID and not valid for user-defined topics
	BOOST_CHECK(!help::is_valid_id("toplevel"));
}

BOOST_AUTO_TEST_CASE( test_is_valid_id_unit_prefix_invalid )
{
	// IDs starting with unit_prefix are invalid
	BOOST_CHECK(!help::is_valid_id(help::unit_prefix + "Archer"));
	BOOST_CHECK(!help::is_valid_id(help::unit_prefix + "Fighter"));
}

BOOST_AUTO_TEST_CASE( test_is_valid_id_hidden_unit_prefix_invalid )
{
	// Hidden unit IDs are also invalid
	std::string hidden_unit = help::hidden_symbol() + help::unit_prefix + "HiddenUnit";
	BOOST_CHECK(!help::is_valid_id(hidden_unit));
}

BOOST_AUTO_TEST_CASE( test_is_valid_id_ability_prefix_invalid )
{
	// IDs starting with ability_prefix are invalid
	BOOST_CHECK(!help::is_valid_id(help::ability_prefix + "heals"));
	BOOST_CHECK(!help::is_valid_id(help::ability_prefix + "skirmisher"));
}

BOOST_AUTO_TEST_CASE( test_is_valid_id_weaponspecial_prefix_invalid )
{
	// IDs starting with weaponspecial_prefix are invalid (using the constant defined in help_impl.cpp)
	const std::string weaponspecial_prefix = "weaponspecial_";
	BOOST_CHECK(!help::is_valid_id(weaponspecial_prefix + "magical"));
	BOOST_CHECK(!help::is_valid_id(weaponspecial_prefix + "poison"));
}

BOOST_AUTO_TEST_CASE( test_is_valid_id_hidden_invalid )
{
	// 'hidden' is a special ID and not valid
	BOOST_CHECK(!help::is_valid_id("hidden"));
}

// ============================================================================
// Hidden Symbol Tests
// ============================================================================

BOOST_AUTO_TEST_CASE( test_hidden_symbol_true )
{
	BOOST_CHECK_EQUAL(help::hidden_symbol(true), ".");
}

BOOST_AUTO_TEST_CASE( test_hidden_symbol_false )
{
	BOOST_CHECK_EQUAL(help::hidden_symbol(false), "");
}

BOOST_AUTO_TEST_CASE( test_hidden_symbol_default )
{
	// Default parameter is true
	BOOST_CHECK_EQUAL(help::hidden_symbol(), ".");
}

// ============================================================================
// Prefix Constants Tests
// ============================================================================

BOOST_AUTO_TEST_CASE( test_prefix_constants )
{
	BOOST_CHECK_EQUAL(help::unit_prefix, "unit_");
	BOOST_CHECK_EQUAL(help::terrain_prefix, "terrain_");
	BOOST_CHECK_EQUAL(help::race_prefix, "race_");
	BOOST_CHECK_EQUAL(help::faction_prefix, "faction_");
	BOOST_CHECK_EQUAL(help::era_prefix, "era_");
	BOOST_CHECK_EQUAL(help::variation_prefix, "variation_");
	BOOST_CHECK_EQUAL(help::ability_prefix, "ability_");
}

BOOST_AUTO_TEST_CASE( test_default_show_topic )
{
	BOOST_CHECK_EQUAL(help::default_show_topic, "..introduction");
}

BOOST_AUTO_TEST_CASE( test_unknown_unit_topic )
{
	BOOST_CHECK_EQUAL(help::unknown_unit_topic, ".unknown_unit");
}

BOOST_AUTO_TEST_CASE( test_max_section_level )
{
	BOOST_CHECK_EQUAL(help::max_section_level, 15);
}

// ============================================================================
// Topic Generator Tests
// ============================================================================

BOOST_AUTO_TEST_CASE( test_text_topic_generator )
{
	std::string content = "Generated content text";
	help::text_topic_generator gen(content);

	BOOST_CHECK_EQUAL(gen(), content);
}

BOOST_AUTO_TEST_CASE( test_text_topic_generator_empty )
{
	std::string content = "";
	help::text_topic_generator gen(content);

	BOOST_CHECK_EQUAL(gen(), content);
}

BOOST_AUTO_TEST_CASE( test_topic_with_text_generator )
{
	std::string text = "Topic text content";
	auto gen = std::make_shared<help::text_topic_generator>(text);
	help::topic t("Generated Topic", "gen_id", gen);

	BOOST_CHECK_EQUAL(t.title, "Generated Topic");
	BOOST_CHECK_EQUAL(t.id, "gen_id");
}

// ============================================================================
// Title Less Comparator Tests
// ============================================================================

BOOST_AUTO_TEST_CASE( test_title_less )
{
	help::title_less comparator;

	help::topic t1("Alpha Topic", "id1", "content");
	help::topic t2("Beta Topic", "id2", "content");

	BOOST_CHECK(comparator(t1, t2));
	BOOST_CHECK(!comparator(t2, t1));
}

BOOST_AUTO_TEST_CASE( test_title_less_equal_titles )
{
	help::title_less comparator;

	help::topic t1("Same Title", "id1", "content");
	help::topic t2("Same Title", "id2", "content");

	// Equal titles should return false
	BOOST_CHECK(!comparator(t1, t2));
	BOOST_CHECK(!comparator(t2, t1));
}

// ============================================================================
// Section Less Comparator Tests
// ============================================================================

BOOST_AUTO_TEST_CASE( test_section_less )
{
	help::section_less comparator;

	help::section s1;
	s1.title = "Alpha Section";

	help::section s2;
	s2.title = "Beta Section";

	BOOST_CHECK(comparator(s1, s2));
	BOOST_CHECK(!comparator(s2, s1));
}

// ============================================================================
// String Less Comparator Tests
// ============================================================================

BOOST_AUTO_TEST_CASE( test_string_less )
{
	help::string_less comparator;

	BOOST_CHECK(comparator("alpha", "beta"));
	BOOST_CHECK(!comparator("beta", "alpha"));
	BOOST_CHECK(!comparator("same", "same"));
}

// ============================================================================
// Section Reference Tests (Config-based)
// ============================================================================

BOOST_AUTO_TEST_CASE( test_section_is_referenced_from_toplevel )
{
	config cfg;

	config& toplevel = cfg.add_child("toplevel");
	toplevel["sections"] = "referenced_section";

	BOOST_CHECK(help::section_is_referenced("referenced_section", cfg));
	BOOST_CHECK(!help::section_is_referenced("unreferenced_section", cfg));
}

BOOST_AUTO_TEST_CASE( test_section_is_referenced_from_section )
{
	config cfg;

	config& section_cfg = cfg.add_child("section");
	section_cfg["id"] = "parent_section";
	section_cfg["sections"] = "child_section";

	BOOST_CHECK(help::section_is_referenced("child_section", cfg));
	BOOST_CHECK(!help::section_is_referenced("orphan_section", cfg));
}

BOOST_AUTO_TEST_CASE( test_section_is_referenced_not_found )
{
	config cfg;

	// Empty config - nothing is referenced
	BOOST_CHECK(!help::section_is_referenced("any_section", cfg));
}

// ============================================================================
// Topic Reference Tests (Config-based)
// ============================================================================

BOOST_AUTO_TEST_CASE( test_topic_is_referenced_from_toplevel )
{
	config cfg;

	config& toplevel = cfg.add_child("toplevel");
	toplevel["topics"] = "referenced_topic";

	BOOST_CHECK(help::topic_is_referenced("referenced_topic", cfg));
	BOOST_CHECK(!help::topic_is_referenced("unreferenced_topic", cfg));
}

BOOST_AUTO_TEST_CASE( test_topic_is_referenced_from_section )
{
	config cfg;

	config& section_cfg = cfg.add_child("section");
	section_cfg["id"] = "parent_section";
	section_cfg["topics"] = "child_topic";

	BOOST_CHECK(help::topic_is_referenced("child_topic", cfg));
	BOOST_CHECK(!help::topic_is_referenced("orphan_topic", cfg));
}

BOOST_AUTO_TEST_CASE( test_topic_is_referenced_not_found )
{
	config cfg;

	// Empty config - nothing is referenced
	BOOST_CHECK(!help::topic_is_referenced("any_topic", cfg));
}

// ============================================================================
// Find Topic Tests
// ============================================================================

BOOST_AUTO_TEST_CASE( test_find_topic_direct )
{
	help::section sec;
	sec.id = "parent";

	help::topic t("Found Topic", "target_id", "Content");
	sec.topics.push_back(t);

	const help::topic* found = help::find_topic(sec, "target_id");

	BOOST_CHECK(found != nullptr);
	BOOST_CHECK_EQUAL(found->id, "target_id");
	BOOST_CHECK_EQUAL(found->title, "Found Topic");
}

BOOST_AUTO_TEST_CASE( test_find_topic_not_found )
{
	help::section sec;
	sec.id = "parent";

	help::topic t("Some Topic", "some_id", "Content");
	sec.topics.push_back(t);

	const help::topic* found = help::find_topic(sec, "nonexistent_id");

	BOOST_CHECK(found == nullptr);
}

BOOST_AUTO_TEST_CASE( test_find_topic_in_subsection )
{
	help::section parent;
	parent.id = "parent";

	help::section child;
	child.id = "child";

	help::topic t("Nested Topic", "nested_id", "Content");
	child.topics.push_back(t);

	parent.add_section(child);

	const help::topic* found = help::find_topic(parent, "nested_id");

	BOOST_CHECK(found != nullptr);
	BOOST_CHECK_EQUAL(found->id, "nested_id");
}

BOOST_AUTO_TEST_CASE( test_find_topic_deeply_nested )
{
	help::section root;
	root.id = "root";

	help::section level1;
	level1.id = "level1";

	help::section level2;
	level2.id = "level2";

	help::topic t("Deep Topic", "deep_id", "Content");
	level2.topics.push_back(t);

	level1.add_section(level2);
	root.add_section(level1);

	const help::topic* found = help::find_topic(root, "deep_id");

	BOOST_CHECK(found != nullptr);
	BOOST_CHECK_EQUAL(found->id, "deep_id");
}

// ============================================================================
// Find Section Tests
// ============================================================================

BOOST_AUTO_TEST_CASE( test_find_section_direct )
{
	help::section parent;
	parent.id = "parent";

	help::section child;
	child.id = "target_section";
	child.title = "Target Section";

	parent.add_section(child);

	const help::section* found = help::find_section(parent, "target_section");

	BOOST_CHECK(found != nullptr);
	BOOST_CHECK_EQUAL(found->id, "target_section");
	BOOST_CHECK_EQUAL(found->title, "Target Section");
}

BOOST_AUTO_TEST_CASE( test_find_section_not_found )
{
	help::section parent;
	parent.id = "parent";

	help::section child;
	child.id = "some_section";

	parent.add_section(child);

	const help::section* found = help::find_section(parent, "nonexistent_section");

	BOOST_CHECK(found == nullptr);
}

BOOST_AUTO_TEST_CASE( test_find_section_deeply_nested )
{
	help::section root;
	root.id = "root";

	help::section level1;
	level1.id = "level1";

	help::section level2;
	level2.id = "target_deep";
	level2.title = "Deep Section";

	level1.add_section(level2);
	root.add_section(level1);

	const help::section* found = help::find_section(root, "target_deep");

	BOOST_CHECK(found != nullptr);
	BOOST_CHECK_EQUAL(found->id, "target_deep");
}

BOOST_AUTO_TEST_CASE( test_find_section_mutable )
{
	help::section parent;
	parent.id = "parent";

	help::section child;
	child.id = "mutable_section";

	parent.add_section(child);

	help::section* found = help::find_section(parent, "mutable_section");

	BOOST_CHECK(found != nullptr);
	BOOST_CHECK_EQUAL(found->id, "mutable_section");

	// Verify we can modify the found section
	found->title = "Modified Title";
	BOOST_CHECK_EQUAL(parent.sections.front().title, "Modified Title");
}

// ============================================================================
// Parse Config Tests
// ============================================================================

BOOST_AUTO_TEST_CASE( test_parse_config_empty )
{
	config cfg;

	help::section sec = help::parse_config(cfg);

	// Empty config should return empty section
	BOOST_CHECK_EQUAL(sec.id, "");
}

BOOST_AUTO_TEST_CASE( test_parse_config_with_toplevel )
{
	config cfg;

	config& toplevel = cfg.add_child("toplevel");
	toplevel["sections"] = "";
	toplevel["topics"] = "";

	help::section sec = help::parse_config(cfg);

	// Parsed section should have toplevel as ID
	BOOST_CHECK_EQUAL(sec.id, "toplevel");
}

// ============================================================================
// Topic Text Tests
// ============================================================================

BOOST_AUTO_TEST_CASE( test_topic_text_creation )
{
	std::string text = "Test content for topic text";
	auto gen = std::make_shared<help::text_topic_generator>(text);
	help::topic_text tt(gen);

	// Verify topic_text can be created
	BOOST_CHECK(true);
}

// ============================================================================
// Complex Help Structure Tests
// ============================================================================

BOOST_AUTO_TEST_CASE( test_complex_help_structure )
{
	// Create a complex help structure with nested sections and topics
	help::section root;
	root.id = "root";
	root.title = "Root Help";

	// Add topics to root
	help::topic root_topic1("Root Topic 1", "root_topic_1", "Content 1");
	help::topic root_topic2("Root Topic 2", "root_topic_2", "Content 2");
	root.topics.push_back(root_topic1);
	root.topics.push_back(root_topic2);

	// Add a subsection
	help::section units_section;
	units_section.id = "units";
	units_section.title = "Units";

	help::topic unit_topic("Elvish Archer", "unit_Elvish_Archer", "Unit description");
	units_section.topics.push_back(unit_topic);

	// Add a sub-subsection
	help::section elf_section;
	elf_section.id = "elves";
	elf_section.title = "Elves";

	help::topic elf_topic("Elvish Fighter", "unit_Elvish_Fighter", "Elf unit");
	elf_section.topics.push_back(elf_topic);

	units_section.add_section(elf_section);
	root.add_section(units_section);

	// Verify structure
	BOOST_CHECK_EQUAL(root.topics.size(), 2u);
	BOOST_CHECK_EQUAL(root.sections.size(), 1u);
	BOOST_CHECK_EQUAL(root.sections.front().topics.size(), 1u);
	BOOST_CHECK_EQUAL(root.sections.front().sections.size(), 1u);

	// Find topics at different levels
	BOOST_CHECK(help::find_topic(root, "root_topic_1") != nullptr);
	BOOST_CHECK(help::find_topic(root, "unit_Elvish_Archer") != nullptr);
	BOOST_CHECK(help::find_topic(root, "unit_Elvish_Fighter") != nullptr);

	// Find sections at different levels
	BOOST_CHECK(help::find_section(root, "units") != nullptr);
	BOOST_CHECK(help::find_section(root, "elves") != nullptr);
}

BOOST_AUTO_TEST_CASE( test_help_structure_with_hidden_elements )
{
	help::section sec;
	sec.id = "visible_section";

	// Add visible topic
	help::topic visible("Visible Topic", "visible_id", "Content");
	sec.topics.push_back(visible);

	// Add hidden topic (starting with '.')
	help::topic hidden("Hidden Topic", ".hidden_id", "Hidden content");
	sec.topics.push_back(hidden);

	// Verify both are in the section
	BOOST_CHECK_EQUAL(sec.topics.size(), 2u);

	// Verify visibility check
	BOOST_CHECK(help::is_visible_id("visible_id"));
	BOOST_CHECK(!help::is_visible_id(".hidden_id"));
}

// ============================================================================
// Error Handling Tests
// ============================================================================

BOOST_AUTO_TEST_CASE( test_parse_error_creation )
{
	std::string error_msg = "Test parse error";
	help::parse_error err(error_msg);

	BOOST_CHECK_EQUAL(err.message, error_msg);
}

// ============================================================================
// Multiple Topics and Sections Sorting Tests
// ============================================================================

BOOST_AUTO_TEST_CASE( test_topic_list_sorting )
{
	help::topic_list topics;

	help::topic t3("Charlie", "id3", "Content");
	help::topic t1("Alpha", "id1", "Content");
	help::topic t2("Beta", "id2", "Content");

	topics.push_back(t3);
	topics.push_back(t1);
	topics.push_back(t2);

	// Sort using title_less
	topics.sort(help::title_less());

	// Verify sorted order
	auto it = topics.begin();
	BOOST_CHECK_EQUAL(it->title, "Alpha");
	++it;
	BOOST_CHECK_EQUAL(it->title, "Beta");
	++it;
	BOOST_CHECK_EQUAL(it->title, "Charlie");
}

BOOST_AUTO_TEST_CASE( test_section_list_sorting )
{
	help::section_list sections;

	help::section s3;
	s3.title = "Gamma";

	help::section s1;
	s1.title = "Alpha";

	help::section s2;
	s2.title = "Beta";

	sections.push_back(s3);
	sections.push_back(s1);
	sections.push_back(s2);

	// Sort using section_less
	sections.sort(help::section_less());

	// Verify sorted order
	auto it = sections.begin();
	BOOST_CHECK_EQUAL(it->title, "Alpha");
	++it;
	BOOST_CHECK_EQUAL(it->title, "Beta");
	++it;
	BOOST_CHECK_EQUAL(it->title, "Gamma");
}

// ============================================================================
// Edge Cases and Boundary Tests
// ============================================================================

BOOST_AUTO_TEST_CASE( test_topic_with_empty_content )
{
	help::topic t("Empty Topic", "empty_id", "");

	BOOST_CHECK_EQUAL(t.title, "Empty Topic");
	BOOST_CHECK_EQUAL(t.id, "empty_id");
	BOOST_CHECK(t.text.parsed_text().empty());
}

BOOST_AUTO_TEST_CASE( test_section_with_empty_id )
{
	help::section sec;
	sec.id = "";
	sec.title = "Empty ID Section";

	BOOST_CHECK_EQUAL(sec.id, "");
	BOOST_CHECK_EQUAL(sec.title, "Empty ID Section");
}

BOOST_AUTO_TEST_CASE( test_topic_with_special_characters )
{
	help::topic t("Special Topic", "special_id", "Content with <b>bold</b> and [ref]links[/ref]");

	BOOST_CHECK_EQUAL(t.id, "special_id");
	BOOST_CHECK_EQUAL(t.title, "Special Topic");
}

BOOST_AUTO_TEST_CASE( test_topic_with_unicode )
{
	help::topic t("Unicode Topic", "unicode_id", "Content with unicode: \xe4\xb8\xad\xe6\x96\x87");

	BOOST_CHECK_EQUAL(t.id, "unicode_id");
}

BOOST_AUTO_TEST_CASE( test_find_topic_empty_section )
{
	help::section empty_sec;
	empty_sec.id = "empty";

	const help::topic* found = help::find_topic(empty_sec, "any_id");

	BOOST_CHECK(found == nullptr);
}

BOOST_AUTO_TEST_CASE( test_find_section_empty_section )
{
	help::section empty_sec;
	empty_sec.id = "empty";

	const help::section* found = help::find_section(empty_sec, "any_id");

	BOOST_CHECK(found == nullptr);
}

BOOST_AUTO_TEST_CASE( test_is_valid_id_with_special_characters )
{
	// IDs with special characters
	BOOST_CHECK(help::is_valid_id("topic_with_underscore"));
	BOOST_CHECK(help::is_valid_id("topic-with-dash"));
	BOOST_CHECK(help::is_valid_id("topic.with.dot"));
	BOOST_CHECK(help::is_valid_id("TopicWithCamelCase"));
}

BOOST_AUTO_TEST_CASE( test_is_visible_id_various_formats )
{
	// Various ID formats and their visibility
	BOOST_CHECK(help::is_visible_id("normal_topic"));
	BOOST_CHECK(help::is_visible_id("Normal_Topic"));
	BOOST_CHECK(help::is_visible_id("topic123"));
	BOOST_CHECK(!help::is_visible_id(".hidden_topic"));
	BOOST_CHECK(!help::is_visible_id("..double_hidden"));
	BOOST_CHECK(help::is_visible_id("a")); // Single character
	BOOST_CHECK(help::is_visible_id("1")); // Starts with number
}

BOOST_AUTO_TEST_SUITE_END()
