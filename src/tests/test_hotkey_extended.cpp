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
#include "hotkey/hotkey_command.hpp"
#include "hotkey/hotkey_item.hpp"

#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>

// Extended test suite for hotkey module
// Covers: hotkey binding, scope management, command execution, key combinations

BOOST_AUTO_TEST_SUITE( hotkey_extended )

// ============================================================================
// Hotkey Binding Tests (5 tests)
// ============================================================================

BOOST_AUTO_TEST_CASE( test_hotkey_keyboard_binding )
{
	config cfg;
	cfg["command"] = "undo";
	cfg["key"] = "z";
	cfg["ctrl"] = true;

	hotkey::hotkey_ptr hk = hotkey::load_from_config(cfg);

	BOOST_REQUIRE(hk);
	BOOST_CHECK_EQUAL(hk->get_command(), "undo");
	BOOST_CHECK(hk->valid());
	BOOST_CHECK(!hk->null());
	BOOST_CHECK(hk->active());
}

BOOST_AUTO_TEST_CASE( test_hotkey_mouse_binding )
{
	config cfg;
	cfg["command"] = "selecthex";
	cfg["mouse"] = 0;
	cfg["button"] = SDL_BUTTON_LEFT;

	hotkey::hotkey_ptr hk = hotkey::load_from_config(cfg);

	BOOST_REQUIRE(hk);
	BOOST_CHECK_EQUAL(hk->get_command(), "selecthex");
	BOOST_CHECK(hk->valid());
}

BOOST_AUTO_TEST_CASE( test_hotkey_mouse_double_click )
{
	config cfg;
	cfg["command"] = "selecthex";
	cfg["mouse"] = 0;
	cfg["button"] = SDL_BUTTON_LEFT;
	cfg["click"] = 2;

	hotkey::hotkey_ptr hk = hotkey::load_from_config(cfg);

	BOOST_REQUIRE(hk);
	BOOST_CHECK(hk->valid());
}

BOOST_AUTO_TEST_CASE( test_hotkey_multiple_modifiers )
{
	config cfg;
	cfg["command"] = "screenshot";
	cfg["key"] = "s";
	cfg["ctrl"] = true;
	cfg["shift"] = true;
	cfg["alt"] = true;

	hotkey::hotkey_ptr hk = hotkey::load_from_config(cfg);

	BOOST_REQUIRE(hk);
	BOOST_CHECK_EQUAL(hk->get_command(), "screenshot");
	BOOST_CHECK(hk->valid());
}

BOOST_AUTO_TEST_CASE( test_hotkey_void_invalid )
{
	hotkey::hotkey_void hk_void;

	BOOST_CHECK(!hk_void.valid());
	BOOST_CHECK_EQUAL(hk_void.get_command(), "null");
	BOOST_CHECK(hk_void.null());
	BOOST_CHECK(!hk_void.active());
}

// ============================================================================
// Hotkey State Tests (5 tests)
// ============================================================================

BOOST_AUTO_TEST_CASE( test_hotkey_default_flag )
{
	config cfg;
	cfg["command"] = "save";
	cfg["key"] = "s";
	cfg["ctrl"] = true;

	hotkey::hotkey_ptr hk = hotkey::load_from_config(cfg);

	BOOST_REQUIRE(hk);
	BOOST_CHECK(hk->is_default());

	hk->unset_default();
	BOOST_CHECK(!hk->is_default());
}

BOOST_AUTO_TEST_CASE( test_hotkey_disable_enable )
{
	config cfg;
	cfg["command"] = "test";
	cfg["key"] = "t";

	hotkey::hotkey_ptr hk = hotkey::load_from_config(cfg);

	BOOST_REQUIRE(hk);
	BOOST_CHECK(!hk->is_disabled());

	hk->disable();
	BOOST_CHECK(hk->is_disabled());

	hk->enable();
	BOOST_CHECK(!hk->is_disabled());
}

BOOST_AUTO_TEST_CASE( test_hotkey_clear_binding )
{
	config cfg;
	cfg["command"] = "undo";
	cfg["key"] = "z";
	cfg["ctrl"] = true;

	hotkey::hotkey_ptr hk = hotkey::load_from_config(cfg);

	BOOST_REQUIRE(hk);
	BOOST_CHECK_EQUAL(hk->get_command(), "undo");
	BOOST_CHECK(hk->active());

	hk->clear();
	BOOST_CHECK_EQUAL(hk->get_command(), "null");
	BOOST_CHECK(hk->null());
	BOOST_CHECK(!hk->active());
}

BOOST_AUTO_TEST_CASE( test_hotkey_set_command )
{
	hotkey::hotkey_keyboard hk;
	hk.set_keycode(SDLK_a);
	hk.set_text("a");

	BOOST_CHECK_EQUAL(hk.get_command(), "null");

	hk.set_command("attack");
	BOOST_CHECK_EQUAL(hk.get_command(), "attack");
	BOOST_CHECK(hk.active());

	hk.set_command("recruit");
	BOOST_CHECK_EQUAL(hk.get_command(), "recruit");
}

BOOST_AUTO_TEST_CASE( test_hotkey_hidden_default )
{
	hotkey::hotkey_keyboard hk;
	BOOST_CHECK(!hk.hidden());
}

// ============================================================================
// Scope Tests (4 tests)
// ============================================================================

BOOST_AUTO_TEST_CASE( test_hotkey_scope_enum_values )
{
	BOOST_CHECK_EQUAL(hotkey::SCOPE_MAIN_MENU, 0);
	BOOST_CHECK_EQUAL(hotkey::SCOPE_GAME, 1);
	BOOST_CHECK_EQUAL(hotkey::SCOPE_EDITOR, 2);
	BOOST_CHECK_EQUAL(hotkey::SCOPE_COUNT, 3);
}

BOOST_AUTO_TEST_CASE( test_hotkey_scope_constants )
{
	BOOST_CHECK_EQUAL(hotkey::scope_game, 1u << hotkey::SCOPE_GAME);
	BOOST_CHECK_EQUAL(hotkey::scope_editor, 1u << hotkey::SCOPE_EDITOR);
	BOOST_CHECK_EQUAL(hotkey::scope_main, 1u << hotkey::SCOPE_MAIN_MENU);
}

BOOST_AUTO_TEST_CASE( test_hotkey_scope_bitset_operations )
{
	hotkey::hk_scopes scopes = hotkey::scope_game | hotkey::scope_editor;

	BOOST_CHECK(scopes.test(hotkey::SCOPE_GAME));
	BOOST_CHECK(scopes.test(hotkey::SCOPE_EDITOR));
	BOOST_CHECK(!scopes.test(hotkey::SCOPE_MAIN_MENU));
}

BOOST_AUTO_TEST_CASE( test_hotkey_scope_combined )
{
	hotkey::hk_scopes all_scopes = hotkey::scope_game | hotkey::scope_editor | hotkey::scope_main;

	BOOST_CHECK(all_scopes.all());
	BOOST_CHECK_EQUAL(all_scopes.count(), hotkey::SCOPE_COUNT);
}

// ============================================================================
// Hotkey Name Tests (6 tests)
// ============================================================================

BOOST_AUTO_TEST_CASE( test_hotkey_get_name_simple )
{
	hotkey::hotkey_keyboard hk;
	hk.set_keycode(SDLK_a);
	hk.set_text("a");
	hk.set_command("test");

	std::string name = hk.get_name();
	BOOST_CHECK_EQUAL(name, "a");
}

BOOST_AUTO_TEST_CASE( test_hotkey_get_name_with_ctrl )
{
	hotkey::hotkey_keyboard hk;
	hk.set_keycode(SDLK_s);
	hk.set_text("s");
	hk.set_mods(KMOD_CTRL);
	hk.set_command("save");

	std::string name = hk.get_name();
	BOOST_CHECK_EQUAL(name, "ctrl+s");
}

BOOST_AUTO_TEST_CASE( test_hotkey_get_name_with_shift )
{
	hotkey::hotkey_keyboard hk;
	hk.set_keycode(SDLK_a);
	hk.set_text("a");
	hk.set_mods(KMOD_SHIFT);
	hk.set_command("attack");

	std::string name = hk.get_name();
	BOOST_CHECK_EQUAL(name, "shift+a");
}

BOOST_AUTO_TEST_CASE( test_hotkey_get_name_with_multiple_modifiers )
{
	hotkey::hotkey_keyboard hk;
	hk.set_keycode(SDLK_q);
	hk.set_text("q");
	hk.set_mods(KMOD_CTRL | KMOD_SHIFT);
	hk.set_command("quit");

	std::string name = hk.get_name();
	BOOST_CHECK_EQUAL(name, "ctrl+shift+q");
}

BOOST_AUTO_TEST_CASE( test_hotkey_save_to_config )
{
	hotkey::hotkey_keyboard hk;
	hk.set_keycode(SDLK_z);
	hk.set_text("z");
	hk.set_mods(KMOD_CTRL);
	hk.set_command("undo");
	hk.unset_default();

	config cfg;
	hk.save(cfg);

	BOOST_CHECK_EQUAL(cfg["command"].str(), "undo");
	BOOST_CHECK_EQUAL(cfg["key"].str(), "z");
	BOOST_CHECK(cfg["ctrl"].to_bool());
	BOOST_CHECK(!cfg["shift"].to_bool());
	BOOST_CHECK(!cfg["alt"].to_bool());
}

BOOST_AUTO_TEST_CASE( test_hotkey_mouse_save_to_config )
{
	hotkey::hotkey_mouse hk;
	hk.set_button(SDL_BUTTON_RIGHT);
	hk.set_clicks(1);
	hk.set_command("context");

	config cfg;
	hk.save(cfg);

	BOOST_CHECK_EQUAL(cfg["command"].str(), "context");
	BOOST_CHECK_EQUAL(cfg["button"].to_int(), SDL_BUTTON_RIGHT);
}

// ============================================================================
// Config Serialization Tests (3 tests)
// ============================================================================

BOOST_AUTO_TEST_CASE( test_hotkey_load_from_empty_config )
{
	config cfg;
	hotkey::hotkey_ptr hk = hotkey::load_from_config(cfg);

	BOOST_REQUIRE(hk);
	BOOST_CHECK(!hk->valid());
}

BOOST_AUTO_TEST_CASE( test_hotkey_load_with_disabled_flag )
{
	config cfg;
	cfg["command"] = "test";
	cfg["key"] = "t";
	cfg["disabled"] = true;

	hotkey::hotkey_ptr hk = hotkey::load_from_config(cfg);

	BOOST_REQUIRE(hk);
	BOOST_CHECK(hk->is_disabled());
}

BOOST_AUTO_TEST_CASE( test_hotkey_load_without_disabled_flag )
{
	config cfg;
	cfg["command"] = "test";
	cfg["key"] = "t";

	hotkey::hotkey_ptr hk = hotkey::load_from_config(cfg);

	BOOST_REQUIRE(hk);
	BOOST_CHECK(!hk->is_disabled());
}

// ============================================================================
// HOTKEY_COMMAND Enum Tests (2 tests)
// ============================================================================

BOOST_AUTO_TEST_CASE( test_hotkey_command_values )
{
	// Verify some key hotkey command values exist and are unique
	BOOST_CHECK(hotkey::HOTKEY_CYCLE_UNITS != hotkey::HOTKEY_UNDO);
	BOOST_CHECK(hotkey::HOTKEY_UNDO != hotkey::HOTKEY_REDO);
	BOOST_CHECK(hotkey::HOTKEY_SAVE_GAME != hotkey::HOTKEY_LOAD_GAME);
	BOOST_CHECK(hotkey::HOTKEY_RECRUIT != hotkey::HOTKEY_RECALL);
}

BOOST_AUTO_TEST_CASE( test_hotkey_null_command_value )
{
	// HOTKEY_NULL should be the last command in the enum
	// Used as terminator for iterating
	BOOST_CHECK(hotkey::HOTKEY_NULL > hotkey::HOTKEY_CYCLE_UNITS);
	BOOST_CHECK(hotkey::HOTKEY_NULL > hotkey::HOTKEY_ZOOM_IN);
	BOOST_CHECK(hotkey::HOTKEY_NULL > hotkey::HOTKEY_REPLAY_EXIT);
}

// ============================================================================
// Key Combination Tests (3 tests)
// ============================================================================

BOOST_AUTO_TEST_CASE( test_hotkey_ctrl_only )
{
	config cfg;
	cfg["command"] = "copy";
	cfg["key"] = "c";
	cfg["ctrl"] = true;

	hotkey::hotkey_ptr hk = hotkey::load_from_config(cfg);

	BOOST_REQUIRE(hk);
	BOOST_CHECK(hk->valid());

	config saved;
	hk->save(saved);
	BOOST_CHECK(saved["ctrl"].to_bool());
	BOOST_CHECK(!saved["shift"].to_bool());
	BOOST_CHECK(!saved["alt"].to_bool());
	BOOST_CHECK(!saved["cmd"].to_bool());
}

BOOST_AUTO_TEST_CASE( test_hotkey_alt_modifier )
{
	config cfg;
	cfg["command"] = "menu";
	cfg["key"] = "m";
	cfg["alt"] = true;

	hotkey::hotkey_ptr hk = hotkey::load_from_config(cfg);

	BOOST_REQUIRE(hk);
	BOOST_CHECK(hk->valid());

	config saved;
	hk->save(saved);
	BOOST_CHECK(!saved["ctrl"].to_bool());
	BOOST_CHECK(saved["alt"].to_bool());
}

BOOST_AUTO_TEST_CASE( test_hotkey_cmd_gui_modifier )
{
	config cfg;
	cfg["command"] = "preferences";
	cfg["key"] = "p";
	cfg["cmd"] = true;

	hotkey::hotkey_ptr hk = hotkey::load_from_config(cfg);

	BOOST_REQUIRE(hk);
	BOOST_CHECK(hk->valid());

	config saved;
	hk->save(saved);
	BOOST_CHECK(saved["cmd"].to_bool());
}

// ============================================================================
// Keyboard Hotkey Specific Tests (2 tests)
// ============================================================================

BOOST_AUTO_TEST_CASE( test_hotkey_keyboard_valid )
{
	hotkey::hotkey_keyboard hk;

	BOOST_CHECK(!hk.valid()); // No keycode set

	hk.set_keycode(SDLK_a);
	BOOST_CHECK(!hk.valid()); // No text set

	hk.set_text("a");
	BOOST_CHECK(hk.valid()); // Both keycode and text set
}

BOOST_AUTO_TEST_CASE( test_hotkey_keyboard_text_lowercase )
{
	hotkey::hotkey_keyboard hk;
	hk.set_text("A");

	// The set_text method should lowercase the text
	BOOST_CHECK_EQUAL(hk.get_name(), "a");
}

// ============================================================================
// Mouse Hotkey Specific Tests (4 tests)
// ============================================================================

BOOST_AUTO_TEST_CASE( test_hotkey_mouse_valid )
{
	hotkey::hotkey_mouse hk;

	BOOST_CHECK(!hk.valid()); // No button set

	hk.set_button(SDL_BUTTON_LEFT);
	BOOST_CHECK(hk.valid());
}

BOOST_AUTO_TEST_CASE( test_hotkey_mouse_button_values )
{
	hotkey::hotkey_mouse hk;

	hk.set_button(SDL_BUTTON_LEFT);
	BOOST_CHECK(hk.valid());

	hk.set_button(SDL_BUTTON_RIGHT);
	BOOST_CHECK(hk.valid());

	hk.set_button(SDL_BUTTON_MIDDLE);
	BOOST_CHECK(hk.valid());

	hk.set_button(0);
	BOOST_CHECK(!hk.valid());
}

BOOST_AUTO_TEST_CASE( test_hotkey_mouse_get_name )
{
	hotkey::hotkey_mouse hk;
	hk.set_button(SDL_BUTTON_LEFT);
	hk.set_command("select");

	BOOST_CHECK_EQUAL(hk.get_name(), "left mouse");
}

BOOST_AUTO_TEST_CASE( test_hotkey_mouse_clicks )
{
	hotkey::hotkey_mouse hk;
	hk.set_button(SDL_BUTTON_LEFT);
	hk.set_clicks(2);
	hk.set_command("doubleclick");

	// Multiple clicks should be reflected in the name
	std::string name = hk.get_name();
	BOOST_CHECK(name.find("clicks") != std::string::npos);
	BOOST_CHECK(name.find("2") != std::string::npos);
}

// ============================================================================
// Category Tests (1 test)
// ============================================================================

BOOST_AUTO_TEST_CASE( test_hotkey_category_enum_values )
{
	// Verify category enum values are sequential and unique
	BOOST_CHECK_EQUAL(hotkey::HKCAT_GENERAL, 0);
	BOOST_CHECK_EQUAL(hotkey::HKCAT_SAVING, 1);
	BOOST_CHECK_EQUAL(hotkey::HKCAT_MAP, 2);
	BOOST_CHECK_EQUAL(hotkey::HKCAT_UNITS, 3);
	BOOST_CHECK_EQUAL(hotkey::HKCAT_CHAT, 4);
	BOOST_CHECK_EQUAL(hotkey::HKCAT_REPLAY, 5);
	BOOST_CHECK_EQUAL(hotkey::HKCAT_WHITEBOARD, 6);
	BOOST_CHECK_EQUAL(hotkey::HKCAT_SCENARIO, 7);
	BOOST_CHECK_EQUAL(hotkey::HKCAT_PALETTE, 8);
	BOOST_CHECK_EQUAL(hotkey::HKCAT_TOOLS, 9);
	BOOST_CHECK_EQUAL(hotkey::HKCAT_CLIPBOARD, 10);
	BOOST_CHECK_EQUAL(hotkey::HKCAT_DEBUG, 11);
	BOOST_CHECK_EQUAL(hotkey::HKCAT_CUSTOM, 12);
	BOOST_CHECK_EQUAL(hotkey::HKCAT_PLACEHOLDER, 13);
}

// ============================================================================
// Edge Case Tests (3 tests)
// ============================================================================

BOOST_AUTO_TEST_CASE( test_hotkey_empty_command )
{
	config cfg;
	cfg["command"] = "";
	cfg["key"] = "x";

	hotkey::hotkey_ptr hk = hotkey::load_from_config(cfg);

	BOOST_REQUIRE(hk);
	BOOST_CHECK_EQUAL(hk->get_command(), "");
}

BOOST_AUTO_TEST_CASE( test_hotkey_bindings_equal_nullptr )
{
	config cfg;
	cfg["command"] = "test";
	cfg["key"] = "t";

	hotkey::hotkey_ptr hk = hotkey::load_from_config(cfg);

	BOOST_REQUIRE(hk);
	BOOST_CHECK(!hk->bindings_equal(nullptr));
}

BOOST_AUTO_TEST_CASE( test_hotkey_void_hidden )
{
	hotkey::hotkey_void hk;
	BOOST_CHECK(!hk.hidden());
}

// ============================================================================
// Hotkey List Operations Tests (2 tests)
// ============================================================================

BOOST_AUTO_TEST_CASE( test_hotkey_clear_all_hotkeys )
{
	// Clear all hotkeys - this is a global operation
	hotkey::clear_hotkeys();
	BOOST_CHECK(hotkey::get_hotkeys().empty());
}

BOOST_AUTO_TEST_CASE( test_hotkey_add_and_check )
{
	// Clear first to have a clean state
	hotkey::clear_hotkeys();

	config cfg;
	cfg["command"] = "test_unique_cmd_xyz";
	cfg["key"] = "x";

	hotkey::hotkey_ptr hk = hotkey::load_from_config(cfg);
	BOOST_REQUIRE(hk);

	hotkey::add_hotkey(hk);
	BOOST_CHECK(hotkey::has_hotkey_item("test_unique_cmd_xyz"));

	// Cleanup
	hotkey::clear_hotkeys();
}

BOOST_AUTO_TEST_SUITE_END()
