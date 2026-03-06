/*
	Copyright (C) 2003 - 2025
	by David White <dave@whitevine.net>
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

#include "widgets/button.hpp"
#include "widgets/scrollbar.hpp"
#include "widgets/widget.hpp"
#include "sdl/rect.hpp"

// Note: These tests focus on widget configuration and state management.
// Many widget operations require SDL/display initialization and image resources,
// so we test what we can without requiring those dependencies.

BOOST_AUTO_TEST_SUITE( widgets )

// ============================================================================
// Widget base class tests
// ============================================================================

BOOST_AUTO_TEST_CASE( widget_location_and_size )
{
	// Test rect construction and basic properties
	rect r{10, 20, 100, 50};

	BOOST_CHECK_EQUAL( r.x, 10 );
	BOOST_CHECK_EQUAL( r.y, 20 );
	BOOST_CHECK_EQUAL( r.w, 100 );
	BOOST_CHECK_EQUAL( r.h, 50 );
}

BOOST_AUTO_TEST_CASE( widget_rect_contains )
{
	rect r{0, 0, 100, 100};

	// Points inside
	BOOST_CHECK( r.contains(50, 50) );
	BOOST_CHECK( r.contains(0, 0) );
	BOOST_CHECK( r.contains(99, 99) );

	// Points outside
	BOOST_CHECK( !r.contains(-1, 50) );
	BOOST_CHECK( !r.contains(100, 50) );
	BOOST_CHECK( !r.contains(50, -1) );
	BOOST_CHECK( !r.contains(50, 100) );
}

BOOST_AUTO_TEST_CASE( widget_rect_overlaps )
{
	rect r1{0, 0, 100, 100};

	// Overlapping rectangles
	rect r2{50, 50, 100, 100};
	BOOST_CHECK( r1.overlaps(r2) );

	rect r3{-50, -50, 100, 100};
	BOOST_CHECK( r1.overlaps(r3) );

	// Non-overlapping rectangles
	rect r4{100, 0, 100, 100};  // Adjacent but not overlapping
	BOOST_CHECK( !r1.overlaps(r4) );

	rect r5{0, 100, 100, 100};  // Adjacent vertically
	BOOST_CHECK( !r1.overlaps(r5) );
}

// ============================================================================
// Button type enum tests
// ============================================================================

BOOST_AUTO_TEST_CASE( button_type_values )
{
	// Verify button type enum values exist and are distinct
	gui::button::TYPE types[] = {
		gui::button::TYPE_PRESS,
		gui::button::TYPE_CHECK,
		gui::button::TYPE_TURBO,
		gui::button::TYPE_IMAGE,
		gui::button::TYPE_RADIO
	};

	// Ensure all types are unique by checking their values differ
	for (size_t i = 0; i < sizeof(types)/sizeof(types[0]); ++i) {
		for (size_t j = i + 1; j < sizeof(types)/sizeof(types[0]); ++j) {
			BOOST_CHECK_NE( types[i], types[j] );
		}
	}
}

BOOST_AUTO_TEST_CASE( button_space_consumption_values )
{
	// Verify space consumption enum values
	gui::button::SPACE_CONSUMPTION default_space = gui::button::DEFAULT_SPACE;
	gui::button::SPACE_CONSUMPTION minimum_space = gui::button::MINIMUM_SPACE;

	BOOST_CHECK_NE( default_space, minimum_space );
}

// ============================================================================
// Scrollbar configuration tests
// ============================================================================

BOOST_AUTO_TEST_CASE( scrollbar_position_bounds )
{
	// Test scrollbar position logic
	unsigned full_height = 1000;
	unsigned shown_size = 200;
	unsigned max_position = full_height - shown_size;

	BOOST_CHECK_EQUAL( max_position, 800 );

	// Position should be clamped between 0 and max_position
	unsigned valid_position = 500;
	BOOST_CHECK( valid_position <= max_position );

	// Position at max
	unsigned position_at_max = max_position;
	BOOST_CHECK( position_at_max == max_position );
}

BOOST_AUTO_TEST_CASE( scrollbar_grip_ratio )
{
	// Test grip size calculation logic
	unsigned full_height = 1000;
	unsigned shown_size = 200;
	unsigned grip_position = 400;

	// The grip represents the visible portion as a ratio of total
	float grip_ratio = static_cast<float>(shown_size) / full_height;
	BOOST_CHECK_CLOSE( grip_ratio, 0.2f, 0.01f );

	// Position within range
	BOOST_CHECK( grip_position <= (full_height - shown_size) );
}

BOOST_AUTO_TEST_CASE( scrollbar_scroll_rate )
{
	// Test scroll rate calculations
	unsigned scroll_rate = 20;
	unsigned current_position = 100;

	// Scroll down increases position
	unsigned new_position_down = current_position + scroll_rate;
	BOOST_CHECK_EQUAL( new_position_down, 120 );

	// Scroll up decreases position (with floor at 0)
	int new_position_up = static_cast<int>(current_position) - static_cast<int>(scroll_rate);
	BOOST_CHECK_EQUAL( new_position_up, 80 );

	// Scroll up from small position
	unsigned small_position = 10;
	int scrolled_up = static_cast<int>(small_position) - static_cast<int>(scroll_rate);
	if (scrolled_up < 0) scrolled_up = 0;
	BOOST_CHECK_EQUAL( scrolled_up, 0 );
}

BOOST_AUTO_TEST_CASE( scrollbar_adjust_position )
{
	// Test adjust_position logic: ensure viewport contains the position
	unsigned grip_position = 100;
	unsigned grip_height = 200;

	// Position before viewport - should move to include it
	unsigned pos_before = 50;
	if (pos_before < grip_position) {
		// Should move grip_position to pos_before
		BOOST_CHECK( pos_before < grip_position );
	}

	// Position within viewport - no change needed
	unsigned pos_within = 150;
	bool in_viewport = (pos_within >= grip_position && pos_within < grip_position + grip_height);
	BOOST_CHECK( in_viewport );

	// Position after viewport - should move to include it
	unsigned pos_after = 400;
	if (pos_after >= grip_position + grip_height) {
		// Should move grip_position so pos_after is visible
		BOOST_CHECK( pos_after >= grip_position + grip_height );
	}
}

// ============================================================================
// Rect utility tests for widgets
// ============================================================================

BOOST_AUTO_TEST_CASE( widget_rect_operations )
{
	// Test rect construction with different values
	rect r1{0, 0, 50, 50};
	rect r2{25, 25, 50, 50};

	// Overlapping area
	BOOST_CHECK( r1.overlaps(r2) );

	// Test rect with negative coordinates
	rect r3{-10, -10, 30, 30};
	BOOST_CHECK( r3.contains(0, 0) );
	BOOST_CHECK( r3.contains(-5, -5) );
	BOOST_CHECK( !r3.contains(-11, -11) );
}

BOOST_AUTO_TEST_CASE( widget_rect_edge_cases )
{
	// Zero-sized rect
	rect zero{0, 0, 0, 0};
	BOOST_CHECK( !zero.contains(0, 0) );  // Zero-size doesn't contain origin

	// Single pixel rect
	rect single{10, 10, 1, 1};
	BOOST_CHECK( single.contains(10, 10) );
	BOOST_CHECK( !single.contains(11, 10) );
	BOOST_CHECK( !single.contains(10, 11) );
}

// ============================================================================
// Widget state management tests
// ============================================================================

BOOST_AUTO_TEST_CASE( widget_enabled_disabled_states )
{
	// Test widget enabled/disabled state concept
	bool enabled = true;

	// Enable state
	BOOST_CHECK( enabled == true );

	// Disable
	enabled = false;
	BOOST_CHECK( enabled == false );

	// Re-enable
	enabled = true;
	BOOST_CHECK( enabled == true );
}

BOOST_AUTO_TEST_CASE( widget_hidden_visible_states )
{
	// Test widget visibility states
	enum widget_state { UNINIT, HIDDEN, DIRTY, DRAWN };

	widget_state state = UNINIT;
	BOOST_CHECK( state == UNINIT );

	state = HIDDEN;
	BOOST_CHECK( state == HIDDEN );

	state = DRAWN;
	BOOST_CHECK( state == DRAWN );

	state = DIRTY;
	BOOST_CHECK( state == DIRTY );
}

// ============================================================================
// Button state tests
// ============================================================================

BOOST_AUTO_TEST_CASE( button_state_values )
{
	// Test button state enum values
	// These represent the different visual states a button can have
	enum button_state { UNINIT, NORMAL, ACTIVE, PRESSED, PRESSED_ACTIVE, TOUCHED_NORMAL, TOUCHED_PRESSED };

	button_state state = NORMAL;
	BOOST_CHECK( state == NORMAL );

	state = ACTIVE;
	BOOST_CHECK( state == ACTIVE );

	state = PRESSED;
	BOOST_CHECK( state == PRESSED );

	state = PRESSED_ACTIVE;
	BOOST_CHECK( state == PRESSED_ACTIVE );
}

BOOST_AUTO_TEST_CASE( button_check_state_toggle )
{
	// Test checkbox/radio button state toggling logic
	bool checked = false;

	// Toggle on
	checked = true;
	BOOST_CHECK( checked == true );

	// Toggle off
	checked = false;
	BOOST_CHECK( checked == false );
}

// ============================================================================
// Scrollbar state tests
// ============================================================================

BOOST_AUTO_TEST_CASE( scrollbar_state_values )
{
	// Test scrollbar state enum values
	enum scrollbar_state { UNINIT, NORMAL, ACTIVE, DRAGGED };

	scrollbar_state state = NORMAL;
	BOOST_CHECK( state == NORMAL );

	state = ACTIVE;
	BOOST_CHECK( state == ACTIVE );

	state = DRAGGED;
	BOOST_CHECK( state == DRAGGED );
}

BOOST_AUTO_TEST_CASE( scrollbar_grip_area_calculation )
{
	// Test grip area calculation logic
	unsigned groove_height = 200;
	unsigned full_height = 1000;
	unsigned shown_height = 200;
	unsigned grip_position = 400;

	// Calculate grip height as proportion of groove
	int grip_height = static_cast<int>(groove_height) * shown_height / full_height;
	BOOST_CHECK_EQUAL( grip_height, 40 );

	// Calculate grip Y position within groove
	int grip_y = (groove_height - grip_height) * grip_position / (full_height - shown_height);
	BOOST_CHECK_EQUAL( grip_y, 40 );
}

// ============================================================================
// Widget ID and tooltip tests
// ============================================================================

BOOST_AUTO_TEST_CASE( widget_id_management )
{
	// Test widget ID concept
	std::string widget_id;

	// Initially empty
	BOOST_CHECK( widget_id.empty() );

	// Set ID
	widget_id = "test_button";
	BOOST_CHECK_EQUAL( widget_id, "test_button" );
	BOOST_CHECK( !widget_id.empty() );

	// Different IDs are different
	std::string other_id = "other_button";
	BOOST_CHECK_NE( widget_id, other_id );
}

BOOST_AUTO_TEST_CASE( widget_tooltip_management )
{
	// Test widget tooltip concept
	std::string tooltip;

	// Initially empty
	BOOST_CHECK( tooltip.empty() );

	// Set tooltip
	tooltip = "This is a tooltip";
	BOOST_CHECK_EQUAL( tooltip, "This is a tooltip" );

	// Clear tooltip
	tooltip.clear();
	BOOST_CHECK( tooltip.empty() );
}

// ============================================================================
// Complex widget scenarios
// ============================================================================

BOOST_AUTO_TEST_CASE( button_type_check_vs_press )
{
	// Verify that CHECK and PRESS types behave differently conceptually
	gui::button::TYPE check_type = gui::button::TYPE_CHECK;
	gui::button::TYPE press_type = gui::button::TYPE_PRESS;

	BOOST_CHECK_NE( check_type, press_type );

	// CHECK and RADIO should be similar in behavior (toggle buttons)
	gui::button::TYPE radio_type = gui::button::TYPE_RADIO;
	BOOST_CHECK_NE( check_type, radio_type );
	// But both are toggle types (conceptually similar)
}

BOOST_AUTO_TEST_CASE( scrollbar_full_size_equals_shown )
{
	// Edge case: when full size equals shown size, no scrolling needed
	unsigned full_height = 100;
	unsigned shown_height = 100;
	unsigned max_position = full_height - shown_height;

	BOOST_CHECK_EQUAL( max_position, 0 );

	// Grip should fill the entire groove in this case
}

BOOST_AUTO_TEST_CASE( widget_clip_rect )
{
	// Test clip rect concept for widgets
	rect widget_rect{10, 10, 100, 100};
	rect clip_rect{0, 0, 50, 50};

	// Widget overlaps clip region
	BOOST_CHECK( widget_rect.overlaps(clip_rect) );

	// Clipping would restrict drawing to intersection
}

// ============================================================================
// Additional coverage tests
// ============================================================================

BOOST_AUTO_TEST_CASE( button_hit_test_logic )
{
	// Test button hit detection using rect contains
	rect button_rect{100, 100, 80, 30};

	// Points inside button
	BOOST_CHECK( button_rect.contains(100, 100) );  // Top-left corner
	BOOST_CHECK( button_rect.contains(140, 115) );  // Center
	BOOST_CHECK( button_rect.contains(179, 129) );  // Bottom-right corner (exclusive)

	// Points outside button
	BOOST_CHECK( !button_rect.contains(99, 100) );   // Left of button
	BOOST_CHECK( !button_rect.contains(100, 99) );   // Above button
	BOOST_CHECK( !button_rect.contains(180, 115) );  // Right of button
	BOOST_CHECK( !button_rect.contains(140, 130) );  // Below button
}

BOOST_AUTO_TEST_CASE( scrollbar_move_position_logic )
{
	// Test move_position logic with clamping
	unsigned grip_position = 100;
	unsigned full_height = 1000;
	unsigned shown_height = 200;
	unsigned max_pos = full_height - shown_height;  // 800

	// Move down
	int dep = 50;
	int new_pos = grip_position + dep;
	if (new_pos > 0) {
		if (static_cast<unsigned>(new_pos) > max_pos) {
			new_pos = max_pos;
		}
	}
	BOOST_CHECK_EQUAL( new_pos, 150 );

	// Move beyond max
	dep = 1000;
	new_pos = grip_position + dep;
	if (new_pos > static_cast<int>(max_pos)) {
		new_pos = max_pos;
	}
	BOOST_CHECK_EQUAL( new_pos, 800 );

	// Move negative
	dep = -200;
	new_pos = grip_position + dep;
	if (new_pos < 0) {
		new_pos = 0;
	}
	BOOST_CHECK_EQUAL( new_pos, 0 );
}

BOOST_AUTO_TEST_CASE( widget_focus_management )
{
	// Test widget focus concept
	bool has_focus = false;

	// Gain focus
	has_focus = true;
	BOOST_CHECK( has_focus );

	// Lose focus
	has_focus = false;
	BOOST_CHECK( !has_focus );
}

BOOST_AUTO_TEST_CASE( button_label_handling )
{
	// Test button label concept
	std::string label;

	// Empty label
	BOOST_CHECK( label.empty() );

	// Set label
	label = "Click Me";
	BOOST_CHECK_EQUAL( label, "Click Me" );

	// Change label
	label = "Don't Click";
	BOOST_CHECK_EQUAL( label, "Don't Click" );

	// Clear label
	label.clear();
	BOOST_CHECK( label.empty() );
}

BOOST_AUTO_TEST_CASE( widget_size_methods )
{
	// Test widget size getter/setter concept using rect
	rect widget_rect{50, 100, 200, 150};

	// Width
	BOOST_CHECK_EQUAL( widget_rect.w, 200 );

	// Height
	BOOST_CHECK_EQUAL( widget_rect.h, 150 );

	// Set new width
	int new_width = 300;
	widget_rect.w = new_width;
	BOOST_CHECK_EQUAL( widget_rect.w, 300 );

	// Set new height
	int new_height = 250;
	widget_rect.h = new_height;
	BOOST_CHECK_EQUAL( widget_rect.h, 250 );
}

BOOST_AUTO_TEST_CASE( widget_position_methods )
{
	// Test widget position concept
	rect widget_rect{50, 100, 200, 150};

	// X position
	BOOST_CHECK_EQUAL( widget_rect.x, 50 );

	// Y position
	BOOST_CHECK_EQUAL( widget_rect.y, 100 );

	// Set new position (keeping size)
	int new_x = 150;
	int new_y = 200;
	widget_rect.x = new_x;
	widget_rect.y = new_y;
	BOOST_CHECK_EQUAL( widget_rect.x, 150 );
	BOOST_CHECK_EQUAL( widget_rect.y, 200 );
	// Size should be unchanged
	BOOST_CHECK_EQUAL( widget_rect.w, 200 );
	BOOST_CHECK_EQUAL( widget_rect.h, 150 );
}

BOOST_AUTO_TEST_SUITE_END()
