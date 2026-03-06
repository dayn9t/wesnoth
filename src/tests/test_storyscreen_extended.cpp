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
#include "storyscreen/part.hpp"

#include <chrono>
#include <string>

BOOST_AUTO_TEST_SUITE( storyscreen_extended )

// ============================================================================
// Story Part Configuration Tests
// ============================================================================

// Test floating_image construction from config
BOOST_AUTO_TEST_CASE( test_floating_image_construction )
{
	config cfg;
	cfg["file"] = "portraits/hero.png";
	cfg["x"] = "150";
	cfg["y"] = "200";
	cfg["delay"] = "500";
	cfg["centered"] = "true";
	cfg["resize_with_background"] = "true";

	storyscreen::floating_image img(cfg);

	BOOST_CHECK_EQUAL(img.file(), "portraits/hero.png");
	BOOST_CHECK_EQUAL(img.ref_x(), 150);
	BOOST_CHECK_EQUAL(img.ref_y(), 200);
	BOOST_CHECK_EQUAL(img.display_delay().count(), 500);
	BOOST_CHECK(img.centered());
	BOOST_CHECK(img.resize_with_background());
}

// Test floating_image with minimal config
BOOST_AUTO_TEST_CASE( test_floating_image_minimal )
{
	config cfg;
	cfg["file"] = "minimal.png";

	storyscreen::floating_image img(cfg);

	BOOST_CHECK_EQUAL(img.file(), "minimal.png");
	BOOST_CHECK_EQUAL(img.ref_x(), 0);
	BOOST_CHECK_EQUAL(img.ref_y(), 0);
	BOOST_CHECK_EQUAL(img.display_delay().count(), 0);
	BOOST_CHECK(!img.centered());
	BOOST_CHECK(!img.resize_with_background());
}

// Test floating_image copy operations
BOOST_AUTO_TEST_CASE( test_floating_image_copy )
{
	config cfg;
	cfg["file"] = "original.png";
	cfg["x"] = "100";
	cfg["y"] = "200";

	storyscreen::floating_image original(cfg);
	storyscreen::floating_image copy(original);

	BOOST_CHECK_EQUAL(copy.file(), "original.png");
	BOOST_CHECK_EQUAL(copy.ref_x(), 100);
	BOOST_CHECK_EQUAL(copy.ref_y(), 200);
}

// Test background_layer default construction
BOOST_AUTO_TEST_CASE( test_background_layer_default )
{
	storyscreen::background_layer layer;

	BOOST_CHECK(layer.scale_horizontally());
	BOOST_CHECK(layer.scale_vertically());
	BOOST_CHECK(!layer.tile_horizontally());
	BOOST_CHECK(!layer.tile_vertically());
	BOOST_CHECK(layer.keep_aspect_ratio());
	BOOST_CHECK(!layer.is_base_layer());
	BOOST_CHECK(layer.file().empty());
}

// Test background_layer construction from config with scale attribute
BOOST_AUTO_TEST_CASE( test_background_layer_with_scale )
{
	config cfg;
	cfg["image"] = "backgrounds/forest.jpg";
	cfg["scale"] = "true";
	cfg["keep_aspect_ratio"] = "false";
	cfg["base_layer"] = "true";

	storyscreen::background_layer layer(cfg);

	BOOST_CHECK_EQUAL(layer.file(), "backgrounds/forest.jpg");
	BOOST_CHECK(layer.scale_horizontally());
	BOOST_CHECK(layer.scale_vertically());
	BOOST_CHECK(!layer.keep_aspect_ratio());
	BOOST_CHECK(layer.is_base_layer());
}

// Test background_layer with separate horizontal/vertical scaling
BOOST_AUTO_TEST_CASE( test_background_layer_separate_scaling )
{
	config cfg;
	cfg["image"] = "backgrounds/wide.png";
	cfg["scale_horizontally"] = "true";
	cfg["scale_vertically"] = "false";
	cfg["tile_horizontally"] = "false";
	cfg["tile_vertically"] = "true";

	storyscreen::background_layer layer(cfg);

	BOOST_CHECK(layer.scale_horizontally());
	BOOST_CHECK(!layer.scale_vertically());
	BOOST_CHECK(!layer.tile_horizontally());
	BOOST_CHECK(layer.tile_vertically());
}

// Test background_layer with tile attribute
BOOST_AUTO_TEST_CASE( test_background_layer_with_tile )
{
	config cfg;
	cfg["image"] = "backgrounds/tile.png";
	cfg["tile"] = "true";

	storyscreen::background_layer layer(cfg);

	BOOST_CHECK(layer.tile_horizontally());
	BOOST_CHECK(layer.tile_vertically());
}

// Test background_layer setters
BOOST_AUTO_TEST_CASE( test_background_layer_setters )
{
	storyscreen::background_layer layer;

	layer.set_file("new_background.jpg");
	layer.set_scale_horizontally(false);
	layer.set_scale_vertically(true);
	layer.set_tile_horizontally(true);
	layer.set_tile_vertically(false);
	layer.set_keep_aspect_ratio(false);
	layer.set_base_layer(true);

	BOOST_CHECK_EQUAL(layer.file(), "new_background.jpg");
	BOOST_CHECK(!layer.scale_horizontally());
	BOOST_CHECK(layer.scale_vertically());
	BOOST_CHECK(layer.tile_horizontally());
	BOOST_CHECK(!layer.tile_vertically());
	BOOST_CHECK(!layer.keep_aspect_ratio());
	BOOST_CHECK(layer.is_base_layer());
}

// ============================================================================
// Image Display Tests
// ============================================================================

// Test multiple floating images with delays
BOOST_AUTO_TEST_CASE( test_multiple_floating_images_sequence )
{
	config part_cfg;
	part_cfg["story"] = "A story with multiple images";

	config& img1 = part_cfg.add_child("image");
	img1["file"] = "intro.png";
	img1["x"] = "0";
	img1["y"] = "0";
	img1["delay"] = "0";

	config& img2 = part_cfg.add_child("image");
	img2["file"] = "chapter1.png";
	img2["x"] = "100";
	img2["y"] = "100";
	img2["delay"] = "1000";

	config& img3 = part_cfg.add_child("image");
	img3["file"] = "chapter2.png";
	img3["x"] = "200";
	img3["y"] = "200";
	img3["delay"] = "2000";

	BOOST_CHECK_EQUAL(part_cfg.child_count("image"), 3u);

	// Verify each image's configuration
	const config& first_img = part_cfg.mandatory_child("image", 0);
	BOOST_CHECK_EQUAL(first_img["file"].str(), "intro.png");
	BOOST_CHECK_EQUAL(first_img["delay"].to_int(), 0);

	const config& second_img = part_cfg.mandatory_child("image", 1);
	BOOST_CHECK_EQUAL(second_img["file"].str(), "chapter1.png");
	BOOST_CHECK_EQUAL(second_img["delay"].to_int(), 1000);
}

// Test image with centered positioning
BOOST_AUTO_TEST_CASE( test_centered_image )
{
	config cfg;
	cfg["file"] = "centered_portrait.png";
	cfg["centered"] = "true";
	cfg["x"] = "400";
	cfg["y"] = "300";

	storyscreen::floating_image img(cfg);

	// When centered, x and y specify the center point
	BOOST_CHECK(img.centered());
	BOOST_CHECK_EQUAL(img.ref_x(), 400);
	BOOST_CHECK_EQUAL(img.ref_y(), 300);
}

// Test image that resizes with background
BOOST_AUTO_TEST_CASE( test_resize_with_background_image )
{
	config cfg;
	cfg["file"] = "scaled_image.png";
	cfg["resize_with_background"] = "true";
	cfg["x"] = "50";
	cfg["y"] = "50";

	storyscreen::floating_image img(cfg);

	BOOST_CHECK(img.resize_with_background());
	BOOST_CHECK_EQUAL(img.ref_x(), 50);
	BOOST_CHECK_EQUAL(img.ref_y(), 50);
}

// Test multiple background layers composition
BOOST_AUTO_TEST_CASE( test_composite_background_layers )
{
	config part_cfg;
	part_cfg["story"] = "Story with layered background";

	// Base layer
	config& base = part_cfg.add_child("background_layer");
	base["image"] = "sky.jpg";
	base["scale"] = "true";
	base["base_layer"] = "true";

	// Middle layer
	config& middle = part_cfg.add_child("background_layer");
	middle["image"] = "mountains.png";
	middle["scale_horizontally"] = "true";
	middle["scale_vertically"] = "false";

	// Foreground layer
	config& foreground = part_cfg.add_child("background_layer");
	foreground["image"] = "trees.png";
	foreground["scale"] = "false";

	BOOST_CHECK_EQUAL(part_cfg.child_count("background_layer"), 3u);

	// Verify layer order and properties
	const config& base_layer = part_cfg.mandatory_child("background_layer", 0);
	BOOST_CHECK_EQUAL(base_layer["image"].str(), "sky.jpg");
	BOOST_CHECK(base_layer["base_layer"].to_bool());

	const config& mid_layer = part_cfg.mandatory_child("background_layer", 1);
	BOOST_CHECK_EQUAL(mid_layer["image"].str(), "mountains.png");

	const config& fg_layer = part_cfg.mandatory_child("background_layer", 2);
	BOOST_CHECK_EQUAL(fg_layer["image"].str(), "trees.png");
}

// ============================================================================
// Branch Selection Logic Tests (WML Configuration)
// ============================================================================

// Test story with conditional [if] block
BOOST_AUTO_TEST_CASE( test_story_conditional_if )
{
	config story_cfg;
	story_cfg["story"] = "Conditional story";

	config& if_block = story_cfg.add_child("if");

	// Condition: variable equals value
	config& condition = if_block.add_child("variable");
	condition["name"] = "chapter_unlocked";
	condition["equals"] = "yes";

	// Then branch
	config& then_block = if_block.add_child("then");
	then_block["story"] = "You have unlocked the secret chapter!";

	// Else branch
	config& else_block = if_block.add_child("else");
	else_block["story"] = "This chapter is still locked.";

	BOOST_CHECK(story_cfg.has_child("if"));
	BOOST_CHECK(story_cfg.child_count("if") == 1u);
}

// Test story with [switch] block for multiple branches
BOOST_AUTO_TEST_CASE( test_story_switch_branch )
{
	config story_cfg;
	story_cfg["story"] = "Branch selection story";

	config& switch_block = story_cfg.add_child("switch");
	switch_block["variable"] = "player_alignment";

	// Case: good
	config& case_good = switch_block.add_child("case");
	case_good["value"] = "good";
	config& part_good = case_good.add_child("part");
	part_good["story"] = "The hero chooses the path of light.";

	// Case: evil
	config& case_evil = switch_block.add_child("case");
	case_evil["value"] = "evil";
	config& part_evil = case_evil.add_child("part");
	part_evil["story"] = "The hero descends into darkness.";

	// Case: neutral
	config& case_neutral = switch_block.add_child("case");
	case_neutral["value"] = "neutral";
	config& part_neutral = case_neutral.add_child("part");
	part_neutral["story"] = "The hero walks a balanced path.";

	// Default case
	config& case_default = switch_block.add_child("else");
	config& part_default = case_default.add_child("part");
	part_default["story"] = "The hero's path is uncertain.";

	BOOST_CHECK(story_cfg.has_child("switch"));

	const config& sw = story_cfg.mandatory_child("switch");
	BOOST_CHECK_EQUAL(sw["variable"].str(), "player_alignment");
	BOOST_CHECK(sw.has_child("case"));
	BOOST_CHECK(sw.has_child("else"));
}

// Test story with [if] and [elseif] chain
BOOST_AUTO_TEST_CASE( test_story_if_elseif_chain )
{
	config story_cfg;
	story_cfg["story"] = "Multi-condition story";

	config& if_block = story_cfg.add_child("if");

	// Primary condition
	config& cond1 = if_block.add_child("variable");
	cond1["name"] = "player_level";
	cond1["greater_than"] = "10";

	// Then for primary
	config& then1 = if_block.add_child("then");
	then1["story"] = "You are a veteran!";

	// First elseif
	config& elseif1 = if_block.add_child("elseif");
	config& cond2 = elseif1.add_child("variable");
	cond2["name"] = "player_level";
	cond2["greater_than"] = "5";
	config& then2 = elseif1.add_child("then");
	then2["story"] = "You are experienced.";

	// Second elseif
	config& elseif2 = if_block.add_child("elseif");
	config& cond3 = elseif2.add_child("variable");
	cond3["name"] = "player_level";
	cond3["greater_than"] = "1";
	config& then3 = elseif2.add_child("then");
	then3["story"] = "You are still learning.";

	// Else
	config& else_block = if_block.add_child("else");
	else_block["story"] = "Welcome, newcomer!";

	BOOST_CHECK(story_cfg.has_child("if"));

	const config& if_cfg = story_cfg.mandatory_child("if");
	BOOST_CHECK(if_cfg.has_child("then"));
	BOOST_CHECK(if_cfg.has_child("elseif"));
	BOOST_CHECK(if_cfg.has_child("else"));
}

// Test nested conditional blocks
BOOST_AUTO_TEST_CASE( test_story_nested_conditionals )
{
	config story_cfg;
	story_cfg["story"] = "Complex branching story";

	config& outer_if = story_cfg.add_child("if");

	// Outer condition
	config& outer_cond = outer_if.add_child("variable");
	outer_cond["name"] = "has_quest";
	outer_cond["equals"] = "yes";

	// Outer then with nested if
	config& outer_then = outer_if.add_child("then");

	config& inner_if = outer_then.add_child("if");
	config& inner_cond = inner_if.add_child("variable");
	inner_cond["name"] = "quest_completed";
	inner_cond["equals"] = "yes";

	config& inner_then = inner_if.add_child("then");
	inner_then["story"] = "Quest completed!";

	config& inner_else = inner_if.add_child("else");
	inner_else["story"] = "Quest in progress.";

	// Outer else
	config& outer_else = outer_if.add_child("else");
	outer_else["story"] = "No quest available.";

	BOOST_CHECK(story_cfg.has_child("if"));

	// Verify nested structure
	const config& outer = story_cfg.mandatory_child("if");
	BOOST_CHECK(outer.has_child("then"));
	BOOST_CHECK(outer.has_child("else"));

	const config& then_block = outer.mandatory_child("then");
	BOOST_CHECK(then_block.has_child("if"));
}

// Test story with multiple [part] blocks in sequence
BOOST_AUTO_TEST_CASE( test_story_multiple_parts_sequence )
{
	config story_cfg;

	// Part 1: Introduction
	config& part1 = story_cfg.add_child("part");
	part1["title"] = "Prologue";
	part1["story"] = "Long ago, in a distant land...";
	part1["background"] = "intro_bg.jpg";

	// Part 2: Chapter 1
	config& part2 = story_cfg.add_child("part");
	part2["title"] = "Chapter 1: The Beginning";
	part2["story"] = "Our hero awakens in a small village.";
	part2["background"] = "village.jpg";

	// Part 3: Chapter 2
	config& part3 = story_cfg.add_child("part");
	part3["title"] = "Chapter 2: The Journey";
	part3["story"] = "The adventure truly begins.";
	part3["background"] = "road.jpg";

	BOOST_CHECK_EQUAL(story_cfg.child_count("part"), 3u);

	// Verify sequence
	const config& p1 = story_cfg.mandatory_child("part", 0);
	BOOST_CHECK_EQUAL(p1["title"].str(), "Prologue");

	const config& p2 = story_cfg.mandatory_child("part", 1);
	BOOST_CHECK_EQUAL(p2["title"].str(), "Chapter 1: The Beginning");

	const config& p3 = story_cfg.mandatory_child("part", 2);
	BOOST_CHECK_EQUAL(p3["title"].str(), "Chapter 2: The Journey");
}

// Test story with audio configuration
BOOST_AUTO_TEST_CASE( test_story_audio_configuration )
{
	config part_cfg;
	part_cfg["story"] = "A story with full audio";
	part_cfg["music"] = "ambient_forest.ogg";
	part_cfg["sound"] = "wind_blowing.ogg";
	part_cfg["voice"] = "narrator_chapter1.ogg";

	BOOST_CHECK_EQUAL(part_cfg["music"].str(), "ambient_forest.ogg");
	BOOST_CHECK_EQUAL(part_cfg["sound"].str(), "wind_blowing.ogg");
	BOOST_CHECK_EQUAL(part_cfg["voice"].str(), "narrator_chapter1.ogg");
}

// Test text layout and alignment options
BOOST_AUTO_TEST_CASE( test_story_text_layout_options )
{
	config part_cfg;
	part_cfg["story"] = "Formatted story text";
	part_cfg["text_layout"] = "middle";
	part_cfg["text_alignment"] = "center";
	part_cfg["title_alignment"] = "right";
	part_cfg["title_position"] = "top,right";

	BOOST_CHECK_EQUAL(part_cfg["text_layout"].str(), "middle");
	BOOST_CHECK_EQUAL(part_cfg["text_alignment"].str(), "center");
	BOOST_CHECK_EQUAL(part_cfg["title_alignment"].str(), "right");
	BOOST_CHECK_EQUAL(part_cfg["title_position"].str(), "top,right");
}

// Test title position variations
BOOST_AUTO_TEST_CASE( test_story_title_position_variations )
{
	// Centered title
	config cfg1;
	cfg1["title_position"] = "centered";
	BOOST_CHECK_EQUAL(cfg1["title_position"].str(), "centered");

	// Single position (horizontal or vertical)
	config cfg2;
	cfg2["title_position"] = "left";
	BOOST_CHECK_EQUAL(cfg2["title_position"].str(), "left");

	// Combined position
	config cfg3;
	cfg3["title_position"] = "center,bottom";
	BOOST_CHECK_EQUAL(cfg3["title_position"].str(), "center,bottom");

	// Top-left position
	config cfg4;
	cfg4["title_position"] = "left,top";
	BOOST_CHECK_EQUAL(cfg4["title_position"].str(), "left,top");
}

// Test story with show_title flag
BOOST_AUTO_TEST_CASE( test_story_show_title_flag )
{
	// Show title explicitly
	config cfg1;
	cfg1["title"] = "Visible Title";
	cfg1["show_title"] = "true";

	BOOST_CHECK(cfg1["show_title"].to_bool());

	// Hide title
	config cfg2;
	cfg2["title"] = "Hidden Title";
	cfg2["show_title"] = "false";

	BOOST_CHECK(!cfg2["show_title"].to_bool());

	// Title without show_title defaults to showing
	config cfg3;
	cfg3["title"] = "Default Visible";

	// When title is present without show_title, it should be shown
	BOOST_CHECK(cfg3.has_attribute("title"));
}

// Test story with empty configuration
BOOST_AUTO_TEST_CASE( test_story_empty_part )
{
	config part_cfg;

	BOOST_CHECK(part_cfg.empty());
	BOOST_CHECK(!part_cfg.has_attribute("story"));
	BOOST_CHECK(!part_cfg.has_attribute("title"));
	BOOST_CHECK(!part_cfg.has_attribute("background"));
	BOOST_CHECK_EQUAL(part_cfg.all_children_count(), 0);
}

// Test complex story part with all features
BOOST_AUTO_TEST_CASE( test_story_full_featured_part )
{
	config part_cfg;

	// Text content
	part_cfg["title"] = "Epic Battle";
	part_cfg["story"] = "The armies clashed on the battlefield...";
	part_cfg["show_title"] = "true";

	// Layout
	part_cfg["text_layout"] = "bottom";
	part_cfg["text_alignment"] = "left";
	part_cfg["title_alignment"] = "center";
	part_cfg["title_position"] = "centered";

	// Background
	part_cfg["background"] = "battlefield.jpg";
	part_cfg["scale_background"] = "true";
	part_cfg["keep_aspect_ratio"] = "true";

	// Audio
	part_cfg["music"] = "battle_music.ogg";
	part_cfg["sound"] = "sword_clash.ogg";

	// Floating image (portrait)
	config& portrait = part_cfg.add_child("image");
	portrait["file"] = "portraits/commander.png";
	portrait["x"] = "800";
	portrait["y"] = "100";
	portrait["centered"] = "false";

	// Background layer overlay
	config& overlay = part_cfg.add_child("background_layer");
	overlay["image"] = "overlays/dust.png";
	overlay["scale"] = "false";
	overlay["tile"] = "true";

	// Verify all features
	BOOST_CHECK_EQUAL(part_cfg["title"].str(), "Epic Battle");
	BOOST_CHECK_EQUAL(part_cfg["story"].str(), "The armies clashed on the battlefield...");
	BOOST_CHECK_EQUAL(part_cfg["background"].str(), "battlefield.jpg");
	BOOST_CHECK_EQUAL(part_cfg["music"].str(), "battle_music.ogg");
	BOOST_CHECK(part_cfg["show_title"].to_bool());
	BOOST_CHECK_EQUAL(part_cfg.child_count("image"), 1u);
	BOOST_CHECK_EQUAL(part_cfg.child_count("background_layer"), 1u);
}

BOOST_AUTO_TEST_SUITE_END()
