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

BOOST_AUTO_TEST_SUITE( storyscreen )

// Test story part configuration
BOOST_AUTO_TEST_CASE( test_story_part_basic )
{
	config cfg;
	cfg["story"] = "Once upon a time...";
	cfg["background"] = "story/background.jpg";

	BOOST_CHECK_EQUAL(cfg["story"].str(), "Once upon a time...");
	BOOST_CHECK_EQUAL(cfg["background"].str(), "story/background.jpg");
}

// Test story with title
BOOST_AUTO_TEST_CASE( test_story_with_title )
{
	config cfg;
	cfg["title"] = "Chapter 1";
	cfg["story"] = "The adventure begins";

	BOOST_CHECK_EQUAL(cfg["title"].str(), "Chapter 1");
	BOOST_CHECK_EQUAL(cfg["story"].str(), "The adventure begins");
}

// Test story with music
BOOST_AUTO_TEST_CASE( test_story_with_music )
{
	config cfg;
	cfg["music"] = "story.ogg";
	cfg["story"] = "Text";

	BOOST_CHECK_EQUAL(cfg["music"].str(), "story.ogg");
}

// Test story with image
BOOST_AUTO_TEST_CASE( test_story_with_image )
{
	config cfg;
	
	config& image = cfg.add_child("image");
	image["file"] = "units/hero.png";
	image["x"] = "100";
	image["y"] = "200";

	BOOST_CHECK(cfg.has_child("image"));
	BOOST_CHECK_EQUAL(cfg.mandatory_child("image")["file"].str(), "units/hero.png");
}

// Test story with text box
BOOST_AUTO_TEST_CASE( test_story_with_textbox )
{
	config cfg;
	
	config& textbox = cfg.add_child("text");
	textbox["text"] = "Hello World";
	textbox["x"] = "50";
	textbox["y"] = "50";

	BOOST_CHECK(cfg.has_child("text"));
	BOOST_CHECK_EQUAL(cfg.mandatory_child("text")["text"].str(), "Hello World");
}

// Test multiple story parts
BOOST_AUTO_TEST_CASE( test_multiple_story_parts )
{
	config cfg;

	config& part1 = cfg.add_child("part");
	part1["story"] = "Part 1";

	config& part2 = cfg.add_child("part");
	part2["story"] = "Part 2";

	BOOST_CHECK_EQUAL(cfg.child_count("part"), 2u);
}

// Test story with sound effects
BOOST_AUTO_TEST_CASE( test_story_with_sound )
{
	config cfg;
	cfg["sound"] = "story_sound.ogg";
	cfg["story"] = "Text with sound";

	BOOST_CHECK_EQUAL(cfg["sound"].str(), "story_sound.ogg");
}

// Test story with voice track
BOOST_AUTO_TEST_CASE( test_story_with_voice )
{
	config cfg;
	cfg["voice"] = "narrator_voice.ogg";
	cfg["story"] = "Text with voice";

	BOOST_CHECK_EQUAL(cfg["voice"].str(), "narrator_voice.ogg");
}

// Test story with show_title attribute
BOOST_AUTO_TEST_CASE( test_story_show_title )
{
	config cfg;
	cfg["show_title"] = "true";
	cfg["title"] = "Story Title";
	cfg["story"] = "Story text";

	BOOST_CHECK_EQUAL(cfg["show_title"].str(), "true");
	BOOST_CHECK_EQUAL(cfg["title"].str(), "Story Title");
}

// Test story with text layout
BOOST_AUTO_TEST_CASE( test_story_text_layout )
{
	config cfg;
	cfg["text_layout"] = "top";
	cfg["story"] = "Top aligned story";

	BOOST_CHECK_EQUAL(cfg["text_layout"].str(), "top");
}

// Test story with text alignment
BOOST_AUTO_TEST_CASE( test_story_text_alignment )
{
	config cfg;
	cfg["text_alignment"] = "center";
	cfg["story"] = "Center aligned text";

	BOOST_CHECK_EQUAL(cfg["text_alignment"].str(), "center");
}

// Test story with title alignment
BOOST_AUTO_TEST_CASE( test_story_title_alignment )
{
	config cfg;
	cfg["title_alignment"] = "right";
	cfg["title"] = "Right Aligned Title";

	BOOST_CHECK_EQUAL(cfg["title_alignment"].str(), "right");
}

// Test story with title position
BOOST_AUTO_TEST_CASE( test_story_title_position )
{
	config cfg;
	cfg["title_position"] = "centered";
	cfg["title"] = "Centered Title";

	BOOST_CHECK_EQUAL(cfg["title_position"].str(), "centered");
}

// Test story with background layer
BOOST_AUTO_TEST_CASE( test_story_background_layer )
{
	config cfg;

	config& layer = cfg.add_child("background_layer");
	layer["image"] = "backgrounds/forest.jpg";
	layer["scale"] = "true";
	layer["keep_aspect_ratio"] = "true";

	BOOST_CHECK(cfg.has_child("background_layer"));
	BOOST_CHECK_EQUAL(cfg.mandatory_child("background_layer")["image"].str(), "backgrounds/forest.jpg");
}

// Test story with scale_background
BOOST_AUTO_TEST_CASE( test_story_scale_background )
{
	config cfg;
	cfg["scale_background"] = "false";
	cfg["story"] = "Story text";

	BOOST_CHECK_EQUAL(cfg["scale_background"].str(), "false");
}

// Test story with tile_background
BOOST_AUTO_TEST_CASE( test_story_tile_background )
{
	config cfg;
	cfg["tile_background"] = "true";
	cfg["story"] = "Story text";

	BOOST_CHECK_EQUAL(cfg["tile_background"].str(), "true");
}

// Test story with keep_aspect_ratio
BOOST_AUTO_TEST_CASE( test_story_keep_aspect_ratio )
{
	config cfg;
	cfg["keep_aspect_ratio"] = "false";
	cfg["story"] = "Story text";

	BOOST_CHECK_EQUAL(cfg["keep_aspect_ratio"].str(), "false");
}

// Test empty story configuration
BOOST_AUTO_TEST_CASE( test_story_empty_config )
{
	config cfg;

	BOOST_CHECK(cfg.empty());
	BOOST_CHECK(!cfg.has_attribute("story"));
	BOOST_CHECK(!cfg.has_attribute("title"));
}

// Test story with complex nested configuration
BOOST_AUTO_TEST_CASE( test_story_complex_config )
{
	config cfg;
	cfg["story"] = "Complex story text";
	cfg["title"] = "Complex Title";
	cfg["background"] = "backgrounds/castle.png";
	cfg["music"] = "background_music.ogg";
	cfg["sound"] = "sfx.ogg";
	cfg["show_title"] = "true";
	cfg["text_layout"] = "middle";
	cfg["text_alignment"] = "left";

	config& image = cfg.add_child("image");
	image["file"] = "portraits/konrad.png";
	image["x"] = "400";
	image["y"] = "300";

	BOOST_CHECK_EQUAL(cfg["story"].str(), "Complex story text");
	BOOST_CHECK_EQUAL(cfg["title"].str(), "Complex Title");
	BOOST_CHECK(cfg.has_child("image"));
	BOOST_CHECK_EQUAL(cfg.child_count("image"), 1u);
}

// Test story with multiple images
BOOST_AUTO_TEST_CASE( test_story_multiple_images )
{
	config cfg;

	config& image1 = cfg.add_child("image");
	image1["file"] = "image1.png";
	image1["x"] = "100";
	image1["y"] = "100";

	config& image2 = cfg.add_child("image");
	image2["file"] = "image2.png";
	image2["x"] = "200";
	image2["y"] = "200";

	config& image3 = cfg.add_child("image");
	image3["file"] = "image3.png";
	image3["x"] = "300";
	image3["y"] = "300";

	BOOST_CHECK_EQUAL(cfg.child_count("image"), 3u);
}

// Test story with image delay
BOOST_AUTO_TEST_CASE( test_story_image_delay )
{
	config cfg;

	config& image = cfg.add_child("image");
	image["file"] = "delayed_image.png";
	image["delay"] = "500";

	BOOST_CHECK_EQUAL(cfg.mandatory_child("image")["delay"].str(), "500");
}

// Test story with image centered
BOOST_AUTO_TEST_CASE( test_story_image_centered )
{
	config cfg;

	config& image = cfg.add_child("image");
	image["file"] = "centered_image.png";
	image["centered"] = "true";

	BOOST_CHECK_EQUAL(cfg.mandatory_child("image")["centered"].str(), "true");
}

// Test story with resize_with_background
BOOST_AUTO_TEST_CASE( test_story_image_resize_with_background )
{
	config cfg;

	config& image = cfg.add_child("image");
	image["file"] = "resizable_image.png";
	image["resize_with_background"] = "true";

	BOOST_CHECK_EQUAL(cfg.mandatory_child("image")["resize_with_background"].str(), "true");
}

// Test story with multiple background layers
BOOST_AUTO_TEST_CASE( test_story_multiple_background_layers )
{
	config cfg;

	config& layer1 = cfg.add_child("background_layer");
	layer1["image"] = "sky.jpg";
	layer1["scale"] = "true";

	config& layer2 = cfg.add_child("background_layer");
	layer2["image"] = "mountains.png";
	layer2["scale"] = "false";

	BOOST_CHECK_EQUAL(cfg.child_count("background_layer"), 2u);
}

// Test story with background layer scaling options
BOOST_AUTO_TEST_CASE( test_story_background_layer_scaling )
{
	config cfg;

	config& layer = cfg.add_child("background_layer");
	layer["image"] = "background.jpg";
	layer["scale_horizontally"] = "true";
	layer["scale_vertically"] = "false";
	layer["tile_horizontally"] = "true";
	layer["tile_vertically"] = "false";

	BOOST_CHECK_EQUAL(cfg.mandatory_child("background_layer")["scale_horizontally"].str(), "true");
	BOOST_CHECK_EQUAL(cfg.mandatory_child("background_layer")["scale_vertically"].str(), "false");
}

// Test story with base_layer flag
BOOST_AUTO_TEST_CASE( test_story_background_base_layer )
{
	config cfg;

	config& layer = cfg.add_child("background_layer");
	layer["image"] = "base.jpg";
	layer["base_layer"] = "true";

	BOOST_CHECK_EQUAL(cfg.mandatory_child("background_layer")["base_layer"].str(), "true");
}

BOOST_AUTO_TEST_SUITE_END()
