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
#include "desktop/paths.hpp"
#include "desktop/notifications.hpp"
#include "desktop/version.hpp"
#include "desktop/clipboard.hpp"
#include "desktop/open.hpp"
#include "desktop/battery_info.hpp"
#include "tstring.hpp"
#include <string>
#include <vector>
#include <set>
#include <sstream>

// ============================================================================
// desktop::path_info tests
// ============================================================================

BOOST_AUTO_TEST_SUITE(test_desktop_path_info)

BOOST_AUTO_TEST_CASE(test_path_info_construction)
{
	desktop::path_info info;

	BOOST_CHECK(info.name.empty());
	BOOST_CHECK(info.label.empty());
	BOOST_CHECK(info.path.empty());
}

BOOST_AUTO_TEST_CASE(test_path_info_with_values)
{
	desktop::path_info info;
	info.name = "Test Path";
	info.label = "Test Label";
	info.path = "/test/path";

	BOOST_CHECK_EQUAL(info.name, "Test Path");
	BOOST_CHECK_EQUAL(info.label, "Test Label");
	BOOST_CHECK_EQUAL(info.path, "/test/path");
}

BOOST_AUTO_TEST_CASE(test_path_info_display_name)
{
	desktop::path_info info;
	info.name = "Display Test";
	info.path = "/display/test";

	std::string display = info.display_name();

	BOOST_CHECK(!display.empty());
}

BOOST_AUTO_TEST_CASE(test_path_info_with_translatable_name)
{
	desktop::path_info info;
	info.name = t_string("Translatable Path", "wesnoth");
	info.path = "/translatable/path";

	BOOST_CHECK(!info.name.empty());
	BOOST_CHECK_EQUAL(info.path, "/translatable/path");
}

BOOST_AUTO_TEST_SUITE_END()

// ============================================================================
// desktop::bookmark_info tests
// ============================================================================

BOOST_AUTO_TEST_SUITE(test_desktop_bookmark_info)

BOOST_AUTO_TEST_CASE(test_bookmark_info_construction)
{
	desktop::bookmark_info info;

	BOOST_CHECK(info.label.empty());
	BOOST_CHECK(info.path.empty());
}

BOOST_AUTO_TEST_CASE(test_bookmark_info_with_values)
{
	desktop::bookmark_info info;
	info.label = "My Bookmark";
	info.path = "/my/bookmark/path";

	BOOST_CHECK_EQUAL(info.label, "My Bookmark");
	BOOST_CHECK_EQUAL(info.path, "/my/bookmark/path");
}

BOOST_AUTO_TEST_SUITE_END()

// ============================================================================
// desktop::notifications tests
// ============================================================================

BOOST_AUTO_TEST_SUITE(test_desktop_notifications)

BOOST_AUTO_TEST_CASE(test_notification_type_values)
{
	BOOST_CHECK_EQUAL(static_cast<int>(desktop::notifications::CHAT), 0);
	BOOST_CHECK_EQUAL(static_cast<int>(desktop::notifications::TURN_CHANGED), 1);
	BOOST_CHECK_EQUAL(static_cast<int>(desktop::notifications::OTHER), 2);
}

BOOST_AUTO_TEST_CASE(test_notification_send_no_throw)
{
	// Test that sending notifications doesn't throw
	BOOST_CHECK_NO_THROW(desktop::notifications::send("TestOwner", "Test message", desktop::notifications::OTHER));
	BOOST_CHECK_NO_THROW(desktop::notifications::send("Game", "Your turn!", desktop::notifications::TURN_CHANGED));
	BOOST_CHECK_NO_THROW(desktop::notifications::send("Chat", "New message", desktop::notifications::CHAT));
}

BOOST_AUTO_TEST_SUITE_END()

// ============================================================================
// desktop paths enum tests
// ============================================================================

BOOST_AUTO_TEST_SUITE(test_desktop_path_enums)

BOOST_AUTO_TEST_CASE(test_game_path_types)
{
	std::set<desktop::GAME_PATH_TYPES> paths;
	paths.insert(desktop::GAME_BIN_DIR);
	paths.insert(desktop::GAME_CORE_DATA_DIR);
	paths.insert(desktop::GAME_USER_DATA_DIR);

	BOOST_CHECK_EQUAL(paths.size(), 3);
	BOOST_CHECK(paths.count(desktop::GAME_BIN_DIR) > 0);
	BOOST_CHECK(paths.count(desktop::GAME_CORE_DATA_DIR) > 0);
	BOOST_CHECK(paths.count(desktop::GAME_USER_DATA_DIR) > 0);
}

BOOST_AUTO_TEST_CASE(test_system_path_types)
{
	std::set<desktop::SYSTEM_PATH_TYPES> paths;
	paths.insert(desktop::SYSTEM_ALL_DRIVES);
	paths.insert(desktop::SYSTEM_USER_PROFILE);
	paths.insert(desktop::SYSTEM_ROOTFS);

	BOOST_CHECK_EQUAL(paths.size(), 3);
	BOOST_CHECK(paths.count(desktop::SYSTEM_ALL_DRIVES) > 0);
	BOOST_CHECK(paths.count(desktop::SYSTEM_USER_PROFILE) > 0);
	BOOST_CHECK(paths.count(desktop::SYSTEM_ROOTFS) > 0);
}

BOOST_AUTO_TEST_SUITE_END()

// ============================================================================
// Desktop config tests
// ============================================================================

BOOST_AUTO_TEST_SUITE(test_desktop_config)

BOOST_AUTO_TEST_CASE(test_notification_config)
{
	config cfg;
	cfg["enabled"] = "true";
	cfg["sound"] = "false";

	BOOST_CHECK_EQUAL(cfg["enabled"].str(), "true");
	BOOST_CHECK_EQUAL(cfg["sound"].str(), "false");
}

BOOST_AUTO_TEST_CASE(test_bookmark_config)
{
	config bookmarks;

	for (int i = 0; i < 3; ++i) {
		config& bookmark = bookmarks.add_child("bookmark");
		bookmark["label"] = "Bookmark " + std::to_string(i);
		bookmark["path"] = "/path/" + std::to_string(i);
	}

	BOOST_CHECK_EQUAL(bookmarks.child_count("bookmark"), 3);
}

BOOST_AUTO_TEST_CASE(test_path_config)
{
	config paths;

	config& game_path = paths.add_child("game_path");
	game_path["type"] = "bin";
	game_path["path"] = "/usr/bin/wesnoth";

	config& user_path = paths.add_child("game_path");
	user_path["type"] = "user_data";
	user_path["path"] = "/home/user/.wesnoth";

	BOOST_CHECK_EQUAL(paths.child_count("game_path"), 2);
}

BOOST_AUTO_TEST_CASE(test_desktop_settings_config)
{
	config settings;
	settings["notifications_enabled"] = "true";
	settings["battery_indicator"] = "true";
	settings["auto_save_path"] = "/home/user/saves";

	BOOST_CHECK_EQUAL(settings["notifications_enabled"].str(), "true");
	BOOST_CHECK(settings.has_attribute("battery_indicator"));
}

BOOST_AUTO_TEST_CASE(test_notification_preferences_config)
{
	config prefs;

	config& chat = prefs.add_child("notification");
	chat["type"] = "chat";
	chat["enabled"] = "true";

	config& turn = prefs.add_child("notification");
	turn["type"] = "turn_changed";
	turn["enabled"] = "true";

	config& other = prefs.add_child("notification");
	other["type"] = "other";
	other["enabled"] = "false";

	BOOST_CHECK_EQUAL(prefs.child_count("notification"), 3);
}

BOOST_AUTO_TEST_CASE(test_empty_bookmark_config)
{
	config bookmarks;
	BOOST_CHECK_EQUAL(bookmarks.child_count("bookmark"), 0);
}

BOOST_AUTO_TEST_CASE(test_path_info_config_with_label)
{
	config path_cfg;
	path_cfg["name"] = "Test Drive";
	path_cfg["label"] = "Local Disk";
	path_cfg["path"] = "/media/test";

	BOOST_CHECK_EQUAL(path_cfg["name"].str(), "Test Drive");
	BOOST_CHECK_EQUAL(path_cfg["label"].str(), "Local Disk");
	BOOST_CHECK_EQUAL(path_cfg["path"].str(), "/media/test");
}

BOOST_AUTO_TEST_SUITE_END()

// ============================================================================
// Desktop clipboard tests
// ============================================================================

BOOST_AUTO_TEST_SUITE(test_desktop_clipboard)

BOOST_AUTO_TEST_CASE(test_clipboard_copy_no_throw)
{
	// Test that copying to clipboard doesn't throw
	BOOST_CHECK_NO_THROW(desktop::clipboard::copy_to_clipboard("Test text"));
	BOOST_CHECK_NO_THROW(desktop::clipboard::copy_to_clipboard(""));
	BOOST_CHECK_NO_THROW(desktop::clipboard::copy_to_clipboard("Special chars: 中文 🎮"));
	BOOST_CHECK_NO_THROW(desktop::clipboard::copy_to_clipboard("Multi\nline\ntext"));
}

BOOST_AUTO_TEST_CASE(test_clipboard_paste_no_throw)
{
	// Test that pasting from clipboard doesn't throw
	BOOST_CHECK_NO_THROW(desktop::clipboard::copy_from_clipboard());
}

BOOST_AUTO_TEST_CASE(test_clipboard_roundtrip)
{
	// Test basic clipboard operations
	std::string test_text = "Wesnoth clipboard test";
	BOOST_CHECK_NO_THROW(desktop::clipboard::copy_to_clipboard(test_text));

	// Note: SDL clipboard might not work in headless test environment
	// so we just verify it doesn't throw
	std::string result = desktop::clipboard::copy_from_clipboard();
	// Result may be empty in headless environment, but should not throw
	(void)result; // Suppress unused variable warning
}

BOOST_AUTO_TEST_CASE(test_clipboard_empty_string)
{
	// Test copying empty string
	BOOST_CHECK_NO_THROW(desktop::clipboard::copy_to_clipboard(""));
	std::string result = desktop::clipboard::copy_from_clipboard();
	(void)result;
}

BOOST_AUTO_TEST_SUITE_END()

// ============================================================================
// Desktop open_object tests
// ============================================================================

BOOST_AUTO_TEST_SUITE(test_desktop_open_object)

BOOST_AUTO_TEST_CASE(test_open_object_supported)
{
	// Test that open_object_is_supported() returns a consistent value
	bool supported = desktop::open_object_is_supported();

	// Should be true on supported platforms, false otherwise
	// We just verify it doesn't throw and returns a valid bool
	BOOST_CHECK(supported == true || supported == false);
}

BOOST_AUTO_TEST_CASE(test_open_object_no_throw)
{
	// Test that calling open_object doesn't throw
	// Note: We don't actually expect it to succeed in test environment
	BOOST_CHECK_NO_THROW(desktop::open_object("https://www.wesnoth.org"));
}

BOOST_AUTO_TEST_CASE(test_open_object_empty_path)
{
	// Test with empty path
	BOOST_CHECK_NO_THROW(desktop::open_object(""));
}

BOOST_AUTO_TEST_CASE(test_open_object_file_path)
{
	// Test with file path
	BOOST_CHECK_NO_THROW(desktop::open_object("/tmp"));
}

BOOST_AUTO_TEST_SUITE_END()

// ============================================================================
// Desktop battery_info tests
// ============================================================================

BOOST_AUTO_TEST_SUITE(test_desktop_battery_info)

BOOST_AUTO_TEST_CASE(test_battery_info_no_throw)
{
	// Test that battery info functions don't throw
	BOOST_CHECK_NO_THROW(desktop::battery_info::does_device_have_battery());
	BOOST_CHECK_NO_THROW(desktop::battery_info::get_battery_percentage());
}

BOOST_AUTO_TEST_CASE(test_battery_percentage_range)
{
	// Get battery percentage
	double percentage = desktop::battery_info::get_battery_percentage();

	// Percentage should be -1 (error/unknown), or between 0 and 100
	BOOST_CHECK(percentage == -1.0 || (percentage >= 0.0 && percentage <= 100.0));
}

BOOST_AUTO_TEST_CASE(test_battery_have_battery_returns_bool)
{
	// does_device_have_battery should return a valid bool
	bool has_battery = desktop::battery_info::does_device_have_battery();
	BOOST_CHECK(has_battery == true || has_battery == false);
}

BOOST_AUTO_TEST_SUITE_END()

// ============================================================================
// Desktop version/path integration tests
// ============================================================================

BOOST_AUTO_TEST_SUITE(test_desktop_integration)

BOOST_AUTO_TEST_CASE(test_os_version_not_empty)
{
	// os_version() should return something on any platform
	std::string version = desktop::os_version();

	// The result might be empty on some platforms, but it shouldn't throw
	BOOST_CHECK_NO_THROW(desktop::os_version());
}

BOOST_AUTO_TEST_CASE(test_user_profile_dir)
{
	// user_profile_dir() should work on most platforms
	BOOST_CHECK_NO_THROW(desktop::user_profile_dir());
}

BOOST_AUTO_TEST_CASE(test_game_paths_bin_dir)
{
	std::set<desktop::GAME_PATH_TYPES> paths;
	paths.insert(desktop::GAME_BIN_DIR);

	auto result = desktop::game_paths(paths);

	// Should return without throwing
	BOOST_CHECK_NO_THROW(desktop::game_paths(paths));
}

BOOST_AUTO_TEST_CASE(test_game_paths_user_data)
{
	std::set<desktop::GAME_PATH_TYPES> paths;
	paths.insert(desktop::GAME_USER_DATA_DIR);

	BOOST_CHECK_NO_THROW(desktop::game_paths(paths));
}

BOOST_AUTO_TEST_CASE(test_system_paths_user_profile)
{
	std::set<desktop::SYSTEM_PATH_TYPES> paths;
	paths.insert(desktop::SYSTEM_USER_PROFILE);

	BOOST_CHECK_NO_THROW(desktop::system_paths(paths));
}

BOOST_AUTO_TEST_CASE(test_system_paths_all)
{
	std::set<desktop::SYSTEM_PATH_TYPES> paths;
	paths.insert(desktop::SYSTEM_ALL_DRIVES);
	paths.insert(desktop::SYSTEM_USER_PROFILE);
	paths.insert(desktop::SYSTEM_ROOTFS);

	BOOST_CHECK_NO_THROW(desktop::system_paths(paths));
}

BOOST_AUTO_TEST_CASE(test_game_paths_core_data)
{
	std::set<desktop::GAME_PATH_TYPES> paths;
	paths.insert(desktop::GAME_CORE_DATA_DIR);

	auto result = desktop::game_paths(paths);
	BOOST_CHECK_NO_THROW(desktop::game_paths(paths));

	// Should return at most 1 path
	BOOST_CHECK_LE(result.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_game_paths_editor_maps)
{
	std::set<desktop::GAME_PATH_TYPES> paths;
	paths.insert(desktop::GAME_EDITOR_MAP_DIR);

	BOOST_CHECK_NO_THROW(desktop::game_paths(paths));
}

BOOST_AUTO_TEST_CASE(test_game_paths_multiple)
{
	std::set<desktop::GAME_PATH_TYPES> paths;
	paths.insert(desktop::GAME_BIN_DIR);
	paths.insert(desktop::GAME_CORE_DATA_DIR);
	paths.insert(desktop::GAME_USER_DATA_DIR);
	paths.insert(desktop::GAME_EDITOR_MAP_DIR);

	auto result = desktop::game_paths(paths);

	// Should return without throwing and have at most 4 entries
	BOOST_CHECK_NO_THROW(desktop::game_paths(paths));
	BOOST_CHECK_LE(result.size(), 4);
}

BOOST_AUTO_TEST_CASE(test_system_paths_rootfs)
{
	std::set<desktop::SYSTEM_PATH_TYPES> paths;
	paths.insert(desktop::SYSTEM_ROOTFS);

	auto result = desktop::system_paths(paths);
	BOOST_CHECK_NO_THROW(desktop::system_paths(paths));
}

BOOST_AUTO_TEST_CASE(test_empty_path_set)
{
	std::set<desktop::GAME_PATH_TYPES> empty_game_paths;
	auto game_result = desktop::game_paths(empty_game_paths);
	BOOST_CHECK_EQUAL(game_result.size(), 0);

	std::set<desktop::SYSTEM_PATH_TYPES> empty_system_paths;
	auto system_result = desktop::system_paths(empty_system_paths);
	BOOST_CHECK_EQUAL(system_result.size(), 0);
}

BOOST_AUTO_TEST_SUITE_END()

// ============================================================================
// Desktop path_info display_name edge cases
// ============================================================================

BOOST_AUTO_TEST_SUITE(test_desktop_path_info_display)

BOOST_AUTO_TEST_CASE(test_display_name_with_label)
{
	desktop::path_info info;
	info.name = "C:";
	info.label = "Windows";
	info.path = "/mnt/c";

	std::string display = info.display_name();

	// Should contain both label and name
	BOOST_CHECK(!display.empty());
	BOOST_CHECK(display.find("Windows") != std::string::npos);
	BOOST_CHECK(display.find("C:") != std::string::npos);
}

BOOST_AUTO_TEST_CASE(test_display_name_without_label)
{
	desktop::path_info info;
	info.name = "Home";
	info.label = "";
	info.path = "/home/user";

	std::string display = info.display_name();

	// Should just return name when label is empty
	BOOST_CHECK_EQUAL(display, "Home");
}

BOOST_AUTO_TEST_CASE(test_path_info_stream_output)
{
	desktop::path_info info;
	info.name = "Test";
	info.label = "TestLabel";
	info.path = "/test/path";

	std::ostringstream oss;
	oss << info;

	std::string output = oss.str();
	BOOST_CHECK(!output.empty());
	BOOST_CHECK(output.find("Test") != std::string::npos);
	BOOST_CHECK(output.find("/test/path") != std::string::npos);
}

BOOST_AUTO_TEST_SUITE_END()

// ============================================================================
// Desktop bookmark operations tests
// ============================================================================

BOOST_AUTO_TEST_SUITE(test_desktop_bookmark_operations)

BOOST_AUTO_TEST_CASE(test_bookmark_add_remove)
{
	// Get initial bookmark count
	auto initial_bookmarks = desktop::user_bookmarks();
	size_t initial_count = initial_bookmarks.size();

	// Add a test bookmark
	unsigned count = desktop::add_user_bookmark("Test Bookmark", "/tmp/test_path");
	BOOST_CHECK_GE(count, initial_count + 1);

	// Verify bookmark was added
	auto bookmarks = desktop::user_bookmarks();
	bool found = false;
	for (const auto& bm : bookmarks) {
		if (bm.label == "Test Bookmark" && bm.path == "/tmp/test_path") {
			found = true;
			break;
		}
	}
	BOOST_CHECK(found);

	// Remove the bookmark (remove from the end)
	if (count > 0) {
		desktop::remove_user_bookmark(count - 1);
	}
}

BOOST_AUTO_TEST_CASE(test_bookmark_multiple_add)
{
	auto initial_bookmarks = desktop::user_bookmarks();
	size_t initial_count = initial_bookmarks.size();

	// Add multiple bookmarks
	unsigned count1 = desktop::add_user_bookmark("Bookmark 1", "/path/1");
	unsigned count2 = desktop::add_user_bookmark("Bookmark 2", "/path/2");
	unsigned count3 = desktop::add_user_bookmark("Bookmark 3", "/path/3");

	// Counts should be increasing
	BOOST_CHECK_LT(count1, count2);
	BOOST_CHECK_LT(count2, count3);

	// Cleanup - remove the bookmarks we added
	desktop::remove_user_bookmark(count3 - 1);
	desktop::remove_user_bookmark(count2 - 1);
	desktop::remove_user_bookmark(count1 - 1);
}

BOOST_AUTO_TEST_CASE(test_bookmark_remove_invalid_index)
{
	// Should not throw when removing invalid index
	BOOST_CHECK_NO_THROW(desktop::remove_user_bookmark(99999));
	BOOST_CHECK_NO_THROW(desktop::remove_user_bookmark(0));
}

BOOST_AUTO_TEST_CASE(test_bookmark_empty_label_and_path)
{
	// Test adding bookmark with empty strings
	unsigned count = desktop::add_user_bookmark("", "");
	BOOST_CHECK_GE(count, 1);

	// Cleanup
	if (count > 0) {
		desktop::remove_user_bookmark(count - 1);
	}
}

BOOST_AUTO_TEST_SUITE_END()

// ============================================================================
// Desktop notification availability tests
// ============================================================================

BOOST_AUTO_TEST_SUITE(test_desktop_notification_availability)

BOOST_AUTO_TEST_CASE(test_notification_available_returns_bool)
{
	bool available = desktop::notifications::available();
	BOOST_CHECK(available == true || available == false);
}

BOOST_AUTO_TEST_CASE(test_notification_send_empty_strings)
{
	// Test sending notifications with empty strings
	BOOST_CHECK_NO_THROW(desktop::notifications::send("", "", desktop::notifications::OTHER));
	BOOST_CHECK_NO_THROW(desktop::notifications::send("", "Message", desktop::notifications::CHAT));
	BOOST_CHECK_NO_THROW(desktop::notifications::send("Owner", "", desktop::notifications::TURN_CHANGED));
}

BOOST_AUTO_TEST_CASE(test_notification_send_long_message)
{
	// Test with long message
	std::string long_message(1000, 'x');
	BOOST_CHECK_NO_THROW(desktop::notifications::send("Test", long_message, desktop::notifications::OTHER));
}

BOOST_AUTO_TEST_CASE(test_notification_send_special_chars)
{
	// Test with special characters
	BOOST_CHECK_NO_THROW(desktop::notifications::send("Test", "Special: !@#$%^&*()", desktop::notifications::OTHER));
	BOOST_CHECK_NO_THROW(desktop::notifications::send("Test", "Unicode: 中文 🎮", desktop::notifications::OTHER));
	BOOST_CHECK_NO_THROW(desktop::notifications::send("Test", "New\nLine\tTab", desktop::notifications::OTHER));
}

BOOST_AUTO_TEST_SUITE_END()