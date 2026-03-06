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

#include "addon/validation.hpp"
#include "addon/info.hpp"
#include "addon/client.hpp"
#include "addon/state.hpp"
#include "addon/manager.hpp"
#include "config.hpp"

BOOST_AUTO_TEST_SUITE( addon_v2 )

// ============================================================================
// Addon Validation Tests
// ============================================================================

BOOST_AUTO_TEST_CASE( addon_filename_legal_extended )
{
	// Valid filenames
	BOOST_CHECK( addon_filename_legal("normal_file.txt") );
	BOOST_CHECK( addon_filename_legal("file-with-dash.cfg") );
	BOOST_CHECK( addon_filename_legal("file_with_underscore.lua") );
	BOOST_CHECK( addon_filename_legal("CamelCaseFile") );
	BOOST_CHECK( addon_filename_legal("123_numeric_start") );
	BOOST_CHECK( addon_filename_legal("file.many.dots.cfg") );
	BOOST_CHECK( addon_filename_legal("a") );
	BOOST_CHECK( addon_filename_legal("_.cfg") );

	// Invalid filenames - special characters
	BOOST_CHECK( !addon_filename_legal("file with space") );
	BOOST_CHECK( !addon_filename_legal("file*asterisk") );
	BOOST_CHECK( !addon_filename_legal("file<angle>bracket") );
	BOOST_CHECK( !addon_filename_legal("file|pipe") );
	BOOST_CHECK( !addon_filename_legal("file?question") );
	BOOST_CHECK( !addon_filename_legal("file\"quote") );
	BOOST_CHECK( !addon_filename_legal("file\\backslash") );

	// Invalid filenames - reserved names on Windows
	BOOST_CHECK( !addon_filename_legal("CON") );
	BOOST_CHECK( !addon_filename_legal("PRN") );
	BOOST_CHECK( !addon_filename_legal("AUX") );
	BOOST_CHECK( !addon_filename_legal("NUL") );
	BOOST_CHECK( !addon_filename_legal("COM1") );
	BOOST_CHECK( !addon_filename_legal("LPT1") );

	// Invalid filenames - path traversal
	BOOST_CHECK( !addon_filename_legal("..") );
	BOOST_CHECK( !addon_filename_legal(".") );
	BOOST_CHECK( !addon_filename_legal("../file") );
	BOOST_CHECK( !addon_filename_legal("path/../file") );
}

BOOST_AUTO_TEST_CASE( addon_name_legal_comprehensive )
{
	// Valid addon names
	BOOST_CHECK( addon_name_legal("Valid_Addon_Name") );
	BOOST_CHECK( addon_name_legal("valid-addon-name") );
	BOOST_CHECK( addon_name_legal("ValidAddon123") );
	BOOST_CHECK( addon_name_legal("123_Addon") );
	BOOST_CHECK( addon_name_legal("ALLCAPS") );
	BOOST_CHECK( addon_name_legal("lowercase") );
	BOOST_CHECK( addon_name_legal("MixedCase_With_Underscores") );
	BOOST_CHECK( addon_name_legal("MixedCase-With-Dashes") );

	// Invalid addon names - empty
	BOOST_CHECK( !addon_name_legal("") );

	// Invalid addon names - whitespace
	BOOST_CHECK( !addon_name_legal("space in name") );
	BOOST_CHECK( !addon_name_legal(" tab\tin\tname") );
	BOOST_CHECK( !addon_name_legal("newline\nin\nname") );

	// Invalid addon names - special characters
	BOOST_CHECK( !addon_name_legal("name@symbol") );
	BOOST_CHECK( !addon_name_legal("name!exclaim") );
	BOOST_CHECK( !addon_name_legal("name#hash") );
	BOOST_CHECK( !addon_name_legal("name$dollar") );
	BOOST_CHECK( !addon_name_legal("name%percent") );
	BOOST_CHECK( !addon_name_legal("name^caret") );
	BOOST_CHECK( !addon_name_legal("name&ampersand") );
	BOOST_CHECK( !addon_name_legal("name*asterisk") );
	BOOST_CHECK( !addon_name_legal("name(paren)") );
	BOOST_CHECK( !addon_name_legal("name+plus") );
	BOOST_CHECK( !addon_name_legal("name=equals") );
	BOOST_CHECK( !addon_name_legal("name[bracket]") );
	BOOST_CHECK( !addon_name_legal("name{brace}") );
	BOOST_CHECK( !addon_name_legal("name|pipe") );
	BOOST_CHECK( !addon_name_legal("name\\backslash") );
	BOOST_CHECK( !addon_name_legal("name/forwardslash") );
	BOOST_CHECK( !addon_name_legal("name:colon") );
	BOOST_CHECK( !addon_name_legal("name;semicolon") );
	BOOST_CHECK( !addon_name_legal("name\"quote") );
	BOOST_CHECK( !addon_name_legal("name'apostrophe") );
	BOOST_CHECK( !addon_name_legal("name<angle>") );
	BOOST_CHECK( !addon_name_legal("name,comma") );
	BOOST_CHECK( !addon_name_legal("name.period") );
	BOOST_CHECK( !addon_name_legal("name?question") );
	BOOST_CHECK( !addon_name_legal("name~tilde") );
	BOOST_CHECK( !addon_name_legal("name`backtick") );
}

BOOST_AUTO_TEST_CASE( addon_icon_size_validation )
{
	// Small icons should be valid
	BOOST_CHECK( !addon_icon_too_large("") );
	BOOST_CHECK( !addon_icon_too_large("x") );
	BOOST_CHECK( !addon_icon_too_large(std::string(100, 'x')) );
	BOOST_CHECK( !addon_icon_too_large(std::string(1000, 'x')) );
	BOOST_CHECK( !addon_icon_too_large(std::string(10000, 'x')) );
	BOOST_CHECK( !addon_icon_too_large(std::string(100000, 'x')) );

	// Icons exactly at the limit
	BOOST_CHECK( !addon_icon_too_large(std::string(499999, 'x')) );
	BOOST_CHECK( addon_icon_too_large(std::string(500001, 'x')) );

	// Large icons should be invalid
	BOOST_CHECK( addon_icon_too_large(std::string(600000, 'x')) );
	BOOST_CHECK( addon_icon_too_large(std::string(1000000, 'x')) );
}

BOOST_AUTO_TEST_CASE( needs_escaping_character_test )
{
	// Characters that need escaping
	BOOST_CHECK( needs_escaping('\x00') );  // null
	BOOST_CHECK( needs_escaping('\x01') );  // escape char
	BOOST_CHECK( needs_escaping('\x0D') );  // CR
	BOOST_CHECK( needs_escaping('\xFE') );  // parser code

	// Characters that don't need escaping
	BOOST_CHECK( !needs_escaping('A') );
	BOOST_CHECK( !needs_escaping('z') );
	BOOST_CHECK( !needs_escaping('0') );
	BOOST_CHECK( !needs_escaping('9') );
	BOOST_CHECK( !needs_escaping(' ') );
	BOOST_CHECK( !needs_escaping('\t') );
	BOOST_CHECK( !needs_escaping('\n') );   // LF doesn't need escaping
	BOOST_CHECK( !needs_escaping('!') );
	BOOST_CHECK( !needs_escaping('@') );
	BOOST_CHECK( !needs_escaping('#') );
	BOOST_CHECK( !needs_escaping('$') );
	BOOST_CHECK( !needs_escaping('%') );
	BOOST_CHECK( !needs_escaping('\x0A') ); // LF
	BOOST_CHECK( !needs_escaping('\xFF') );
}

BOOST_AUTO_TEST_CASE( binary_encoding_roundtrip )
{
	// Test encoding and unencoding roundtrips
	std::vector<std::string> test_strings = {
		"",                          // empty
		"plain text",                // simple text
		"with\x01escape",           // escape char
		"with\x0Dcarriage",         // CR
		"with\xFEmarker",           // parser code
		"\x01\x0D\xFE",             // all special
		"mixed\x01text\x0Dhere\xFE", // mixed special
		"unicode: \xC3\xA9",        // UTF-8 sequence
		std::string(1000, 'x'),     // long string
	};

	for (const auto& str : test_strings) {
		std::string encoded = encode_binary(str);
		std::string decoded = unencode_binary(encoded);
		BOOST_CHECK_EQUAL( decoded, str );
	}
}

// ============================================================================
// Addon Info Tests
// ============================================================================

BOOST_AUTO_TEST_CASE( addon_info_type_parsing )
{
	// Test all addon types
	BOOST_CHECK_EQUAL( get_addon_type("unknown"), ADDON_UNKNOWN );
	BOOST_CHECK_EQUAL( get_addon_type("core"), ADDON_CORE );
	BOOST_CHECK_EQUAL( get_addon_type("campaign"), ADDON_SP_CAMPAIGN );
	BOOST_CHECK_EQUAL( get_addon_type("scenario"), ADDON_SP_SCENARIO );
	BOOST_CHECK_EQUAL( get_addon_type("campaign_sp_mp"), ADDON_SP_MP_CAMPAIGN );
	BOOST_CHECK_EQUAL( get_addon_type("campaign_mp"), ADDON_MP_CAMPAIGN );
	BOOST_CHECK_EQUAL( get_addon_type("scenario_mp"), ADDON_MP_SCENARIO );
	BOOST_CHECK_EQUAL( get_addon_type("map_pack"), ADDON_MP_MAPS );
	BOOST_CHECK_EQUAL( get_addon_type("era"), ADDON_MP_ERA );
	BOOST_CHECK_EQUAL( get_addon_type("faction"), ADDON_MP_FACTION );
	BOOST_CHECK_EQUAL( get_addon_type("mod_mp"), ADDON_MOD );
	BOOST_CHECK_EQUAL( get_addon_type("media"), ADDON_MEDIA );
	BOOST_CHECK_EQUAL( get_addon_type("theme"), ADDON_THEME );
	BOOST_CHECK_EQUAL( get_addon_type("other"), ADDON_OTHER );

	// Invalid type
	BOOST_CHECK_EQUAL( get_addon_type("invalid_type"), ADDON_UNKNOWN );
	BOOST_CHECK_EQUAL( get_addon_type(""), ADDON_UNKNOWN );
}

BOOST_AUTO_TEST_CASE( addon_info_type_string_conversion )
{
	// Test all addon type to string conversions
	BOOST_CHECK_EQUAL( get_addon_type_string(ADDON_UNKNOWN), "unknown" );
	BOOST_CHECK_EQUAL( get_addon_type_string(ADDON_CORE), "core" );
	BOOST_CHECK_EQUAL( get_addon_type_string(ADDON_SP_CAMPAIGN), "campaign" );
	BOOST_CHECK_EQUAL( get_addon_type_string(ADDON_SP_SCENARIO), "scenario" );
	BOOST_CHECK_EQUAL( get_addon_type_string(ADDON_SP_MP_CAMPAIGN), "campaign_sp_mp" );
	BOOST_CHECK_EQUAL( get_addon_type_string(ADDON_MP_CAMPAIGN), "campaign_mp" );
	BOOST_CHECK_EQUAL( get_addon_type_string(ADDON_MP_SCENARIO), "scenario_mp" );
	BOOST_CHECK_EQUAL( get_addon_type_string(ADDON_MP_MAPS), "map_pack" );
	BOOST_CHECK_EQUAL( get_addon_type_string(ADDON_MP_ERA), "era" );
	BOOST_CHECK_EQUAL( get_addon_type_string(ADDON_MP_FACTION), "faction" );
	BOOST_CHECK_EQUAL( get_addon_type_string(ADDON_MOD), "mod_mp" );
	BOOST_CHECK_EQUAL( get_addon_type_string(ADDON_MEDIA), "media" );
	BOOST_CHECK_EQUAL( get_addon_type_string(ADDON_THEME), "theme" );
	BOOST_CHECK_EQUAL( get_addon_type_string(ADDON_OTHER), "other" );
}

BOOST_AUTO_TEST_CASE( addon_info_config_roundtrip )
{
	addon_info original;
	original.id = "test_addon_v2";
	original.title = "Test Add-on V2";
	original.description = "A comprehensive test add-on";
	original.author = "Test Author";
	original.icon = "icons/test.png";
	original.size = 5000;
	original.downloads = 1000;
	original.uploads = 10;
	original.type = ADDON_SP_CAMPAIGN;
	original.tags = {"campaign", "singleplayer", "test"};
	original.core = "default";
	original.depends = {"core_dependency", "utility_pack"};
	original.feedback_url = "https://example.com/feedback";

	config cfg;
	original.write(cfg);

	// Set name attribute for reading (the read function uses "name" for id)
	cfg["name"] = cfg["id"];

	addon_info parsed(cfg);

	BOOST_CHECK_EQUAL( parsed.id, original.id );
	BOOST_CHECK_EQUAL( parsed.title, original.title );
	BOOST_CHECK_EQUAL( parsed.description, original.description );
	BOOST_CHECK_EQUAL( parsed.author, original.author );
	BOOST_CHECK_EQUAL( parsed.icon, original.icon );
	BOOST_CHECK_EQUAL( parsed.size, original.size );
	BOOST_CHECK_EQUAL( parsed.downloads, original.downloads );
	BOOST_CHECK_EQUAL( parsed.uploads, original.uploads );
	BOOST_CHECK_EQUAL( parsed.type, original.type );
	BOOST_CHECK_EQUAL( parsed.core, original.core );
	BOOST_CHECK_EQUAL( parsed.feedback_url, original.feedback_url );
}

BOOST_AUTO_TEST_CASE( addon_info_display_title_fallback )
{
	addon_info info;
	info.id = "addon_with_underscores";
	info.title = "";

	// When title is empty, display_title should convert underscores to spaces
	std::string display = info.display_title();
	BOOST_CHECK( display.find('_') == std::string::npos );
	BOOST_CHECK( display.find(' ') != std::string::npos );

	// With a proper title
	info.title = "Proper Title";
	display = info.display_title();
	BOOST_CHECK_EQUAL( display, "Proper Title" );
}

BOOST_AUTO_TEST_CASE( addon_info_versions_management )
{
	config cfg;
	cfg["name"] = "versioned_addon";
	cfg["title"] = "Versioned Add-on";
	cfg["version"] = "2.0.0";

	// Add version history
	config& v1 = cfg.add_child("version");
	v1["version"] = "1.0.0";
	config& v2 = cfg.add_child("version");
	v2["version"] = "1.5.0";
	config& v3 = cfg.add_child("version");
	v3["version"] = "2.0.0";

	addon_info info(cfg);

	BOOST_CHECK_EQUAL( info.current_version.str(), "2.0.0" );
	BOOST_CHECK_EQUAL( info.versions.size(), 3 );
}

// ============================================================================
// Addon Status Tests
// ============================================================================

BOOST_AUTO_TEST_CASE( addon_status_enum_values )
{
	// Test that addon status values are in expected order
	BOOST_CHECK( ADDON_NONE < ADDON_INSTALLED );
	BOOST_CHECK( ADDON_INSTALLED < ADDON_INSTALLED_UPGRADABLE );
	BOOST_CHECK( ADDON_INSTALLED_UPGRADABLE < ADDON_INSTALLED_OUTDATED );
	BOOST_CHECK( ADDON_INSTALLED_OUTDATED < ADDON_INSTALLED_LOCAL_ONLY );
	BOOST_CHECK( ADDON_INSTALLED_LOCAL_ONLY < ADDON_INSTALLED_BROKEN );
	BOOST_CHECK( ADDON_INSTALLED_BROKEN < ADDON_NOT_TRACKED );
}

BOOST_AUTO_TEST_CASE( is_installed_addon_status_check )
{
	// Test is_installed_addon_status function
	BOOST_CHECK( is_installed_addon_status(ADDON_INSTALLED) );
	BOOST_CHECK( is_installed_addon_status(ADDON_INSTALLED_UPGRADABLE) );
	BOOST_CHECK( is_installed_addon_status(ADDON_INSTALLED_OUTDATED) );
	BOOST_CHECK( is_installed_addon_status(ADDON_INSTALLED_LOCAL_ONLY) );
	BOOST_CHECK( is_installed_addon_status(ADDON_INSTALLED_BROKEN) );
	BOOST_CHECK( is_installed_addon_status(ADDON_NOT_TRACKED) );

	// ADDON_NONE should not be considered installed
	BOOST_CHECK( !is_installed_addon_status(ADDON_NONE) );
}

BOOST_AUTO_TEST_CASE( addon_tracking_info_default_values )
{
	addon_tracking_info info;

	BOOST_CHECK_EQUAL( info.state, ADDON_NONE );
	BOOST_CHECK_EQUAL( info.can_publish, false );
	BOOST_CHECK_EQUAL( info.in_version_control, false );
	BOOST_CHECK( info.installed_version == version_info() );
	BOOST_CHECK( info.remote_version == version_info() );
}

BOOST_AUTO_TEST_CASE( addon_status_filter_values )
{
	// Test status filter enum values
	BOOST_CHECK_EQUAL( FILTER_ALL, 0 );
	BOOST_CHECK_EQUAL( FILTER_INSTALLED, 1 );
	BOOST_CHECK_EQUAL( FILTER_UPGRADABLE, 2 );
	BOOST_CHECK_EQUAL( FILTER_PUBLISHABLE, 3 );
	BOOST_CHECK_EQUAL( FILTER_NOT_INSTALLED, 4 );
	BOOST_CHECK_EQUAL( FILTER_COUNT, 5 );
}

// ============================================================================
// Addon Check Status Tests
// ============================================================================

BOOST_AUTO_TEST_CASE( addon_check_status_descriptions_exist )
{
	// General errors
	BOOST_CHECK( !addon_check_status_desc(ADDON_CHECK_STATUS::SUCCESS).empty() );
	BOOST_CHECK( !addon_check_status_desc(ADDON_CHECK_STATUS::UNAUTHORIZED).empty() );
	BOOST_CHECK( !addon_check_status_desc(ADDON_CHECK_STATUS::DENIED).empty() );
	BOOST_CHECK( !addon_check_status_desc(ADDON_CHECK_STATUS::USER_DOES_NOT_EXIST).empty() );
	BOOST_CHECK( !addon_check_status_desc(ADDON_CHECK_STATUS::UNEXPECTED_DELTA).empty() );

	// Structure errors
	BOOST_CHECK( !addon_check_status_desc(ADDON_CHECK_STATUS::EMPTY_PACK).empty() );
	BOOST_CHECK( !addon_check_status_desc(ADDON_CHECK_STATUS::BAD_DELTA).empty() );
	BOOST_CHECK( !addon_check_status_desc(ADDON_CHECK_STATUS::BAD_NAME).empty() );
	BOOST_CHECK( !addon_check_status_desc(ADDON_CHECK_STATUS::NAME_HAS_MARKUP).empty() );
	BOOST_CHECK( !addon_check_status_desc(ADDON_CHECK_STATUS::ILLEGAL_FILENAME).empty() );
	BOOST_CHECK( !addon_check_status_desc(ADDON_CHECK_STATUS::FILENAME_CASE_CONFLICT).empty() );
	BOOST_CHECK( !addon_check_status_desc(ADDON_CHECK_STATUS::INVALID_UTF8_NAME).empty() );

	// PBL errors
	BOOST_CHECK( !addon_check_status_desc(ADDON_CHECK_STATUS::NO_TITLE).empty() );
	BOOST_CHECK( !addon_check_status_desc(ADDON_CHECK_STATUS::NO_AUTHOR).empty() );
	BOOST_CHECK( !addon_check_status_desc(ADDON_CHECK_STATUS::NO_VERSION).empty() );
	BOOST_CHECK( !addon_check_status_desc(ADDON_CHECK_STATUS::NO_DESCRIPTION).empty() );
	BOOST_CHECK( !addon_check_status_desc(ADDON_CHECK_STATUS::NO_EMAIL).empty() );
	BOOST_CHECK( !addon_check_status_desc(ADDON_CHECK_STATUS::NO_PASSPHRASE).empty() );
	BOOST_CHECK( !addon_check_status_desc(ADDON_CHECK_STATUS::TITLE_HAS_MARKUP).empty() );
	BOOST_CHECK( !addon_check_status_desc(ADDON_CHECK_STATUS::BAD_TYPE).empty() );
	BOOST_CHECK( !addon_check_status_desc(ADDON_CHECK_STATUS::VERSION_NOT_INCREMENTED).empty() );
	BOOST_CHECK( !addon_check_status_desc(ADDON_CHECK_STATUS::BAD_FEEDBACK_TOPIC_ID).empty() );
	BOOST_CHECK( !addon_check_status_desc(ADDON_CHECK_STATUS::FEEDBACK_TOPIC_ID_NOT_FOUND).empty() );
	BOOST_CHECK( !addon_check_status_desc(ADDON_CHECK_STATUS::AUTH_TYPE_MISMATCH).empty() );
	BOOST_CHECK( !addon_check_status_desc(ADDON_CHECK_STATUS::ICON_TOO_LARGE).empty() );

	// Server errors
	BOOST_CHECK( !addon_check_status_desc(ADDON_CHECK_STATUS::SERVER_UNSPECIFIED).empty() );
	BOOST_CHECK( !addon_check_status_desc(ADDON_CHECK_STATUS::SERVER_READ_ONLY).empty() );
	BOOST_CHECK( !addon_check_status_desc(ADDON_CHECK_STATUS::SERVER_ADDONS_LIST).empty() );
	BOOST_CHECK( !addon_check_status_desc(ADDON_CHECK_STATUS::SERVER_DELTA_NO_VERSIONS).empty() );
	BOOST_CHECK( !addon_check_status_desc(ADDON_CHECK_STATUS::SERVER_FORUM_AUTH_DISABLED).empty() );
}

BOOST_AUTO_TEST_CASE( addon_check_status_translated )
{
	// Test that translated versions are available
	std::string translated = translated_addon_check_status(ADDON_CHECK_STATUS::SUCCESS);
	BOOST_CHECK( !translated.empty() );

	translated = translated_addon_check_status(ADDON_CHECK_STATUS::UNAUTHORIZED);
	BOOST_CHECK( !translated.empty() );

	translated = translated_addon_check_status(ADDON_CHECK_STATUS::BAD_NAME);
	BOOST_CHECK( !translated.empty() );
}

// ============================================================================
// Addon Dependency Tests
// ============================================================================

BOOST_AUTO_TEST_CASE( addon_dependencies_parsing )
{
	config cfg;
	cfg["name"] = "main_addon";
	cfg["title"] = "Main Add-on";
	cfg["dependencies"] = "dep1, dep2, dep3";

	addon_info info(cfg);

	BOOST_CHECK_EQUAL( info.depends.size(), 3 );
	BOOST_CHECK_EQUAL( info.depends[0], "dep1" );
	BOOST_CHECK_EQUAL( info.depends[1], "dep2" );
	BOOST_CHECK_EQUAL( info.depends[2], "dep3" );
}

BOOST_AUTO_TEST_CASE( addon_dependencies_empty )
{
	config cfg;
	cfg["name"] = "standalone_addon";
	cfg["title"] = "Standalone Add-on";

	addon_info info(cfg);

	BOOST_CHECK( info.depends.empty() );
}

BOOST_AUTO_TEST_CASE( addon_dependency_resolution )
{
	addons_list addons;

	// Create main addon with dependencies
	config cfg1;
	cfg1["name"] = "main";
	cfg1["title"] = "Main";
	cfg1["dependencies"] = "dep1,dep2";
	addons["main"] = addon_info(cfg1);

	// Create dependency addons (no dependencies themselves)
	config cfg2;
	cfg2["name"] = "dep1";
	cfg2["title"] = "Dependency 1";
	addons["dep1"] = addon_info(cfg2);

	config cfg3;
	cfg3["name"] = "dep2";
	cfg3["title"] = "Dependency 2";
	addons["dep2"] = addon_info(cfg3);

	// Resolve dependencies
	std::set<std::string> resolved = addons["main"].resolve_dependencies(addons);

	// Dependencies themselves have no dependencies, so set should be empty
	// (resolve_dependencies returns dependencies of dependencies)
	BOOST_CHECK( resolved.empty() );

	// Verify the depends vector is correct
	BOOST_CHECK_EQUAL( addons["main"].depends.size(), 2 );
}

BOOST_AUTO_TEST_CASE( addon_transitive_dependencies )
{
	addons_list addons;

	// Create addon with transitive dependencies
	config cfg1;
	cfg1["name"] = "top";
	cfg1["title"] = "Top";
	cfg1["dependencies"] = "middle";
	addons["top"] = addon_info(cfg1);

	config cfg2;
	cfg2["name"] = "middle";
	cfg2["title"] = "Middle";
	cfg2["dependencies"] = "bottom";
	addons["middle"] = addon_info(cfg2);

	config cfg3;
	cfg3["name"] = "bottom";
	cfg3["title"] = "Bottom";
	addons["bottom"] = addon_info(cfg3);

	// Resolve transitive dependencies
	std::set<std::string> resolved = addons["top"].resolve_dependencies(addons);

	// Should contain "bottom" since middle depends on it
	BOOST_CHECK( resolved.count("bottom") > 0 );
}

// ============================================================================
// Addon Translation Tests
// ============================================================================

BOOST_AUTO_TEST_CASE( addon_info_translation_default_state )
{
	addon_info_translation trans;

	BOOST_CHECK_EQUAL( trans.supported, true );
	BOOST_CHECK( trans.title.empty() );
	BOOST_CHECK( trans.description.empty() );
}

BOOST_AUTO_TEST_CASE( addon_info_translation_config_roundtrip )
{
	addon_info_translation original;
	original.supported = false;
	original.title = "Translated Title";
	original.description = "Translated Description";

	config cfg;
	original.write(cfg);

	addon_info_translation parsed(cfg);

	BOOST_CHECK_EQUAL( parsed.supported, original.supported );
	BOOST_CHECK_EQUAL( parsed.title, original.title );
	BOOST_CHECK_EQUAL( parsed.description, original.description );
}

BOOST_AUTO_TEST_CASE( addon_info_multiple_translations )
{
	config cfg;
	cfg["name"] = "localized_addon";
	cfg["title"] = "Base Title";

	// Add translations for multiple languages
	config& trans_en = cfg.add_child("translation");
	trans_en["language"] = "en_US";
	trans_en["title"] = "English Title";
	trans_en["description"] = "English Description";
	trans_en["supported"] = true;

	config& trans_de = cfg.add_child("translation");
	trans_de["language"] = "de_DE";
	trans_de["title"] = "German Title";
	trans_de["description"] = "German Description";
	trans_de["supported"] = true;

	addon_info info(cfg);

	BOOST_CHECK_EQUAL( info.locales.size(), 2 );
	BOOST_CHECK_EQUAL( info.info_translations.size(), 2 );
}

// ============================================================================
// Addon Size Display Tests
// ============================================================================

BOOST_AUTO_TEST_CASE( size_display_string_formats )
{
	// Test various size formats
	BOOST_CHECK_EQUAL( size_display_string(0), "" );

	// Bytes
	BOOST_CHECK_EQUAL( size_display_string(512), "512 B" );
	BOOST_CHECK_EQUAL( size_display_string(1000), "1000 B" );
	BOOST_CHECK_EQUAL( size_display_string(1023), "1023 B" );

	// Kilobytes
	BOOST_CHECK_EQUAL( size_display_string(1024), "1024 B" );  // Actually 1 KiB
	BOOST_CHECK_EQUAL( size_display_string(1048576), "1024 KiB" );  // 1 MiB

	// Megabytes
	BOOST_CHECK_EQUAL( size_display_string(1048576 * 10), "10240 KiB" );  // 10 MiB
}

// ============================================================================
// Addon Title Generation Tests
// ============================================================================

BOOST_AUTO_TEST_CASE( make_addon_title_conversion )
{
	// Test underscore to space conversion
	BOOST_CHECK_EQUAL( make_addon_title("simple"), "simple" );
	BOOST_CHECK_EQUAL( make_addon_title("two_words"), "two words" );
	BOOST_CHECK_EQUAL( make_addon_title("three_word_title"), "three word title" );
	BOOST_CHECK_EQUAL( make_addon_title("CamelCase_Name"), "CamelCase Name" );
	BOOST_CHECK_EQUAL( make_addon_title("A_B_C_D"), "A B C D" );
}

// ============================================================================
// Addon Hash Tests
// ============================================================================

BOOST_AUTO_TEST_CASE( file_hash_from_contents )
{
	config file;
	file["name"] = "test.txt";
	file["contents"] = "Hello, World!";

	std::string hash = file_hash(file);
	BOOST_CHECK( !hash.empty() );
	// Base64 encoded MD5 should be around 24 characters
	BOOST_CHECK_EQUAL( hash.length(), 24 );
}

BOOST_AUTO_TEST_CASE( file_hash_from_attribute )
{
	config file;
	file["name"] = "test.txt";
	file["contents"] = "content";
	file["hash"] = "precomputed_hash";

	// Should use the precomputed hash if available
	BOOST_CHECK_EQUAL( file_hash(file), "precomputed_hash" );
}

BOOST_AUTO_TEST_CASE( comp_file_hash_identical )
{
	config file_a;
	file_a["name"] = "test.txt";
	file_a["contents"] = "Same content";

	config file_b;
	file_b["name"] = "test.txt";
	file_b["contents"] = "Same content";

	BOOST_CHECK( comp_file_hash(file_a, file_b) );
}

BOOST_AUTO_TEST_CASE( comp_file_hash_different_name )
{
	config file_a;
	file_a["name"] = "test_a.txt";
	file_a["contents"] = "Same content";

	config file_b;
	file_b["name"] = "test_b.txt";
	file_b["contents"] = "Same content";

	BOOST_CHECK( !comp_file_hash(file_a, file_b) );
}

BOOST_AUTO_TEST_CASE( comp_file_hash_different_content )
{
	config file_a;
	file_a["name"] = "test.txt";
	file_a["contents"] = "Content A";

	config file_b;
	file_b["name"] = "test.txt";
	file_b["contents"] = "Content B";

	BOOST_CHECK( !comp_file_hash(file_a, file_b) );
}

// ============================================================================
// Addon Names Legal Check Tests
// ============================================================================

BOOST_AUTO_TEST_CASE( check_names_legal_with_valid_structure )
{
	// Create a valid addon directory structure
	config root;
	config& dir = root.add_child("dir");
	dir["name"] = "valid_addon";

	config& file = dir.add_child("file");
	file["name"] = "main.cfg";
	file["contents"] = "test content";

	config& subdir = dir.add_child("dir");
	subdir["name"] = "subdirectory";

	config& subfile = subdir.add_child("file");
	subfile["name"] = "utils.cfg";
	subfile["contents"] = "more content";

	BOOST_CHECK( check_names_legal(root) );
}

BOOST_AUTO_TEST_CASE( check_names_legal_with_invalid_filename )
{
	config root;
	config& dir = root.add_child("dir");
	dir["name"] = "valid_addon";

	// Add a file with invalid name (contains space)
	config& file = dir.add_child("file");
	file["name"] = "invalid file.cfg";
	file["contents"] = "content";

	BOOST_CHECK( !check_names_legal(root) );

	// Also test with badlist collection
	std::vector<std::string> badlist;
	BOOST_CHECK( !check_names_legal(root, &badlist) );
	BOOST_CHECK( !badlist.empty() );
}

BOOST_AUTO_TEST_CASE( check_case_insensitive_duplicates_no_conflict )
{
	config root;
	config& dir = root.add_child("dir");
	dir["name"] = "addon";

	config& file1 = dir.add_child("file");
	file1["name"] = "main.cfg";

	config& file2 = dir.add_child("file");
	file2["name"] = "utils.cfg";

	BOOST_CHECK( check_case_insensitive_duplicates(root) );
}

BOOST_AUTO_TEST_CASE( check_case_insensitive_duplicates_with_conflict )
{
	config root;
	config& dir = root.add_child("dir");
	dir["name"] = "addon";

	config& file1 = dir.add_child("file");
	file1["name"] = "main.cfg";

	config& file2 = dir.add_child("file");
	file2["name"] = "MAIN.cfg";  // Case conflict

	BOOST_CHECK( !check_case_insensitive_duplicates(root) );

	// Test with badlist
	std::vector<std::string> badlist;
	BOOST_CHECK( !check_case_insensitive_duplicates(root, &badlist) );
	BOOST_CHECK( !badlist.empty() );
}

// ============================================================================
// Addon Default Port Test
// ============================================================================

BOOST_AUTO_TEST_CASE( default_campaignd_port_value )
{
	BOOST_CHECK_EQUAL( default_campaignd_port, 15019 );
}

BOOST_AUTO_TEST_SUITE_END()
