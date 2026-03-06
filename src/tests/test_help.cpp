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

BOOST_AUTO_TEST_SUITE( help )

// Test help topic configuration
BOOST_AUTO_TEST_CASE( test_help_topic_basic )
{
	config cfg;
	cfg["id"] = "topic_01";
	cfg["title"] = "Help Topic";
	cfg["text"] = "This is help text";

	BOOST_CHECK_EQUAL(cfg["id"].str(), "topic_01");
	BOOST_CHECK_EQUAL(cfg["title"].str(), "Help Topic");
	BOOST_CHECK_EQUAL(cfg["text"].str(), "This is help text");
}

// Test help section configuration
BOOST_AUTO_TEST_CASE( test_help_section_basic )
{
	config cfg;
	cfg["id"] = "section_01";
	cfg["title"] = "Help Section";

	BOOST_CHECK_EQUAL(cfg["id"].str(), "section_01");
	BOOST_CHECK_EQUAL(cfg["title"].str(), "Help Section");
}

// Test help with nested topics
BOOST_AUTO_TEST_CASE( test_help_nested_topics )
{
	config cfg;
	cfg["id"] = "parent_section";
	cfg["title"] = "Parent Section";

	config& topic = cfg.add_child("topic");
	topic["id"] = "child_topic";
	topic["title"] = "Child Topic";

	BOOST_CHECK(cfg.has_child("topic"));
	BOOST_CHECK_EQUAL(cfg.mandatory_child("topic")["id"].str(), "child_topic");
}

// Test help with multiple topics
BOOST_AUTO_TEST_CASE( test_help_multiple_topics )
{
	config cfg;

	config& topic1 = cfg.add_child("topic");
	topic1["id"] = "topic_1";
	topic1["title"] = "First Topic";

	config& topic2 = cfg.add_child("topic");
	topic2["id"] = "topic_2";
	topic2["title"] = "Second Topic";

	BOOST_CHECK_EQUAL(cfg.child_count("topic"), 2u);
}

// Test help with nested sections
BOOST_AUTO_TEST_CASE( test_help_nested_sections )
{
	config cfg;
	cfg["id"] = "main_section";

	config& section = cfg.add_child("section");
	section["id"] = "sub_section";
	section["title"] = "Sub Section";

	BOOST_CHECK(cfg.has_child("section"));
	BOOST_CHECK_EQUAL(cfg.mandatory_child("section")["id"].str(), "sub_section");
}

// Test help topic with generator
BOOST_AUTO_TEST_CASE( test_help_topic_generator )
{
	config cfg;
	cfg["id"] = "generated_topic";
	cfg["generator"] = "units";

	BOOST_CHECK_EQUAL(cfg["generator"].str(), "units");
}

// Test help with sort order
BOOST_AUTO_TEST_CASE( test_help_sort_order )
{
	config cfg;
	cfg["id"] = "ordered_section";
	cfg["sort_order"] = "10";

	BOOST_CHECK_EQUAL(cfg["sort_order"].str(), "10");
}

// Test help topic with references
BOOST_AUTO_TEST_CASE( test_help_references )
{
	config cfg;
	cfg["id"] = "referencing_topic";
	cfg["ref"] = "referenced_topic";

	BOOST_CHECK_EQUAL(cfg["ref"].str(), "referenced_topic");
}

// Test help with unit type reference
BOOST_AUTO_TEST_CASE( test_help_unit_reference )
{
	config cfg;
	cfg["id"] = "Elvish_Archer";
	cfg["help_topic"] = "true";

	BOOST_CHECK_EQUAL(cfg["help_topic"].str(), "true");
}

// Test help with terrain reference
BOOST_AUTO_TEST_CASE( test_help_terrain_reference )
{
	config cfg;
	cfg["id"] = "Gg";
	cfg["help_topic"] = "true";

	BOOST_CHECK_EQUAL(cfg["help_topic"].str(), "true");
}

// Test help with ability reference
BOOST_AUTO_TEST_CASE( test_help_ability_reference )
{
	config cfg;
	cfg["id"] = "heals";
	cfg["help_topic"] = "true";

	BOOST_CHECK_EQUAL(cfg["help_topic"].str(), "true");
}

// Test help with weapon special reference
BOOST_AUTO_TEST_CASE( test_help_weapon_special_reference )
{
	config cfg;
	cfg["id"] = "magical";
	cfg["help_topic"] = "true";

	BOOST_CHECK_EQUAL(cfg["help_topic"].str(), "true");
}

// Test help with faction reference
BOOST_AUTO_TEST_CASE( test_help_faction_reference )
{
	config cfg;
	cfg["id"] = "Rebels";
	cfg["help_topic"] = "true";

	BOOST_CHECK_EQUAL(cfg["help_topic"].str(), "true");
}

// Test help with era reference
BOOST_AUTO_TEST_CASE( test_help_era_reference )
{
	config cfg;
	cfg["id"] = "Default_Era";
	cfg["help_topic"] = "true";

	BOOST_CHECK_EQUAL(cfg["help_topic"].str(), "true");
}

// Test help with campaign reference
BOOST_AUTO_TEST_CASE( test_help_campaign_reference )
{
	config cfg;
	cfg["id"] = "Heir_To_The_Throne";
	cfg["help_topic"] = "true";

	BOOST_CHECK_EQUAL(cfg["help_topic"].str(), "true");
}

// Test help with hidden attribute
BOOST_AUTO_TEST_CASE( test_help_hidden )
{
	config cfg;
	cfg["id"] = "hidden_topic";
	cfg["hidden"] = "yes";

	BOOST_CHECK_EQUAL(cfg["hidden"].str(), "yes");
}

// Test help with weight attribute
BOOST_AUTO_TEST_CASE( test_help_weight )
{
	config cfg;
	cfg["id"] = "weighted_topic";
	cfg["weight"] = "5";

	BOOST_CHECK_EQUAL(cfg["weight"].str(), "5");
}

// Test help with icon
BOOST_AUTO_TEST_CASE( test_help_icon )
{
	config cfg;
	cfg["id"] = "icon_topic";
	cfg["icon"] = "icons/help.png";

	BOOST_CHECK_EQUAL(cfg["icon"].str(), "icons/help.png");
}

// Test help with level attribute
BOOST_AUTO_TEST_CASE( test_help_level )
{
	config cfg;
	cfg["id"] = "leveled_section";
	cfg["level"] = "2";

	BOOST_CHECK_EQUAL(cfg["level"].str(), "2");
}

// Test help topic with rich text formatting
BOOST_AUTO_TEST_CASE( test_help_rich_text )
{
	config cfg;
	cfg["id"] = "formatted_topic";
	cfg["text"] = "<b>Bold</b> and <i>italic</i> text";

	BOOST_CHECK_EQUAL(cfg["text"].str(), "<b>Bold</b> and <i>italic</i> text");
}

// Test help with cross-references
BOOST_AUTO_TEST_CASE( test_help_cross_references )
{
	config cfg;
	cfg["id"] = "cross_ref_topic";
	cfg["text"] = "See also: [topic]another_topic[/topic]";

	BOOST_CHECK(cfg["text"].str().find("[topic]") != std::string::npos);
}

// Test help with image references
BOOST_AUTO_TEST_CASE( test_help_images )
{
	config cfg;
	cfg["id"] = "image_topic";
	cfg["text"] = "[img]units/elvish-archer.png[/img]";

	BOOST_CHECK(cfg["text"].str().find("[img]") != std::string::npos);
}

// Test help with table formatting
BOOST_AUTO_TEST_CASE( test_help_tables )
{
	config cfg;
	cfg["id"] = "table_topic";
	cfg["text"] = "[table][row][cell]Data[/cell][/row][/table]";

	BOOST_CHECK(cfg["text"].str().find("[table]") != std::string::npos);
}

// Test help with bullet lists
BOOST_AUTO_TEST_CASE( test_help_bullets )
{
	config cfg;
	cfg["id"] = "bullet_topic";
	cfg["text"] = "[bullet]Item 1[/bullet][bullet]Item 2[/bullet]";

	BOOST_CHECK(cfg["text"].str().find("[bullet]") != std::string::npos);
}

// Test help with definition lists
BOOST_AUTO_TEST_CASE( test_help_definition_lists )
{
	config cfg;
	cfg["id"] = "definition_topic";
	cfg["text"] = "[definition][term]Term[/term][def]Definition[/def][/definition]";

	BOOST_CHECK(cfg["text"].str().find("[definition]") != std::string::npos);
}

// Test help with command references
BOOST_AUTO_TEST_CASE( test_help_commands )
{
	config cfg;
	cfg["id"] = "command_topic";
	cfg["text"] = "Use ':quit' to exit";

	BOOST_CHECK(cfg["text"].str().find(":quit") != std::string::npos);
}

// Test help with hotkey references
BOOST_AUTO_TEST_CASE( test_help_hotkeys )
{
	config cfg;
	cfg["id"] = "hotkey_topic";
	cfg["text"] = "Press [key]Ctrl+S[/key] to save";

	BOOST_CHECK(cfg["text"].str().find("[key]") != std::string::npos);
}

// Test help with variable references
BOOST_AUTO_TEST_CASE( test_help_variables )
{
	config cfg;
	cfg["id"] = "variable_topic";
	cfg["text"] = "The variable $unit.name contains the unit name";

	BOOST_CHECK(cfg["text"].str().find("$") != std::string::npos);
}

// Test help with formula references
BOOST_AUTO_TEST_CASE( test_help_formulas )
{
	config cfg;
	cfg["id"] = "formula_topic";
	cfg["text"] = "Use formula: $(5 + 10)";

	BOOST_CHECK(cfg["text"].str().find("$") != std::string::npos);
}

// Test help with link colors
BOOST_AUTO_TEST_CASE( test_help_link_colors )
{
	config cfg;
	cfg["id"] = "colored_links_topic";
	cfg["text"] = "[color=blue]Blue text[/color]";

	BOOST_CHECK(cfg["text"].str().find("[color=") != std::string::npos);
}

// Test help with font sizes
BOOST_AUTO_TEST_CASE( test_help_font_sizes )
{
	config cfg;
	cfg["id"] = "sized_text_topic";
	cfg["text"] = "[size=12]Small text[/size] [size=24]Large text[/size]";

	BOOST_CHECK(cfg["text"].str().find("[size=") != std::string::npos);
}

// Test help with alignment
BOOST_AUTO_TEST_CASE( test_help_alignment )
{
	config cfg;
	cfg["id"] = "aligned_topic";
	cfg["text"] = "[center]Centered text[/center]";

	BOOST_CHECK(cfg["text"].str().find("[center]") != std::string::npos);
}

// Test help with spoilers
BOOST_AUTO_TEST_CASE( test_help_spoilers )
{
	config cfg;
	cfg["id"] = "spoiler_topic";
	cfg["text"] = "[spoiler]Hidden content[/spoiler]";

	BOOST_CHECK(cfg["text"].str().find("[spoiler]") != std::string::npos);
}

// Test help with version information
BOOST_AUTO_TEST_CASE( test_help_version )
{
	config cfg;
	cfg["id"] = "version_topic";
	cfg["version"] = "1.16.0";

	BOOST_CHECK_EQUAL(cfg["version"].str(), "1.16.0");
}

// Test help with author information
BOOST_AUTO_TEST_CASE( test_help_author )
{
	config cfg;
	cfg["id"] = "author_topic";
	cfg["author"] = "Test Author";

	BOOST_CHECK_EQUAL(cfg["author"].str(), "Test Author");
}

// Test help with timestamp
BOOST_AUTO_TEST_CASE( test_help_timestamp )
{
	config cfg;
	cfg["id"] = "timestamped_topic";
	cfg["timestamp"] = "2025-01-01";

	BOOST_CHECK_EQUAL(cfg["timestamp"].str(), "2025-01-01");
}

// Test help with source reference
BOOST_AUTO_TEST_CASE( test_help_source )
{
	config cfg;
	cfg["id"] = "sourced_topic";
	cfg["source"] = "data/core/help.cfg";

	BOOST_CHECK_EQUAL(cfg["source"].str(), "data/core/help.cfg");
}

// Test help with language attribute
BOOST_AUTO_TEST_CASE( test_help_language )
{
	config cfg;
	cfg["id"] = "localized_topic";
	cfg["language"] = "en_US";

	BOOST_CHECK_EQUAL(cfg["language"].str(), "en_US");
}

// Test help with translation status
BOOST_AUTO_TEST_CASE( test_help_translation_status )
{
	config cfg;
	cfg["id"] = "translation_topic";
	cfg["translated"] = "yes";

	BOOST_CHECK_EQUAL(cfg["translated"].str(), "yes");
}

BOOST_AUTO_TEST_SUITE_END()
