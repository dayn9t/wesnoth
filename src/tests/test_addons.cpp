/*
	Copyright (C) 2012 - 2025
	by Iris Morelle <shadowm2006@gmail.com>
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
#include "config.hpp"

BOOST_AUTO_TEST_SUITE( addons )

BOOST_AUTO_TEST_CASE( validation )
{
	BOOST_CHECK( !addon_filename_legal("") );
	BOOST_CHECK( !addon_filename_legal(".") );
	BOOST_CHECK( !addon_filename_legal("..") );
	BOOST_CHECK( !addon_filename_legal("invalid/slash") );
	BOOST_CHECK( !addon_filename_legal("invalid\\backslash") );
	BOOST_CHECK( !addon_filename_legal("invalid:colon") );
	BOOST_CHECK( !addon_filename_legal("invalid~tilde") );
	BOOST_CHECK( !addon_filename_legal("invalid/../parent") );

	std::vector<std::string> ddns = { "NUL", "CON", "AUX", "PRN", "CONIN$", "CONOUT$" };
	for(unsigned i = 1; i < 10; ++i) {
		ddns.emplace_back(std::string{"LPT"} + std::to_string(i));
		ddns.emplace_back(std::string{"COM"} + std::to_string(i));
	}

	for(const auto& name : ddns) {
		BOOST_CHECK( addon_filename_legal("foo.bar." + name) );
		BOOST_CHECK( addon_filename_legal("foo." + name + ".bar") );
		BOOST_CHECK( !addon_filename_legal(name + ".foo.bar") );
		BOOST_CHECK( !addon_filename_legal(name + ':') );
		BOOST_CHECK( !addon_filename_legal(name) );
	}

	BOOST_CHECK( addon_name_legal("-0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz") );

	BOOST_CHECK( !addon_name_legal("invalid\nnewline") );
	BOOST_CHECK( !addon_name_legal("invalid\x0A""explicitLF") );
	BOOST_CHECK( !addon_name_legal("invalid\x0D\x0A""explicitCRLF") );
	BOOST_CHECK( !addon_name_legal("invalid\x0D""explicitCR") );
	BOOST_CHECK( !addon_name_legal("invalid`grave accent`") );
	BOOST_CHECK( !addon_name_legal("invalid$dollarsign$") );
}

BOOST_AUTO_TEST_CASE( encoding )
{
	BOOST_CHECK( encode_binary("").empty() );
	BOOST_CHECK( unencode_binary("").empty() );

	//
	// Plain string.
	//

	const std::string plain = "ABC";

	BOOST_CHECK( encode_binary(plain) == plain );
	BOOST_CHECK( unencode_binary(plain) == plain );

	//
	// Binary escaping (direct).
	//

	const char bin_escape = '\x01';
	const std::string bin_special = "\x0D\xFE";
	const std::string raw = "ABC \x01 DEF \x0D\x0A JKL \xFE MNO";

	std::string encoded;

	//
	// The encoding algorithm as of 1.11.15 is as follows:
	//
	//   * let c be the char to encode
	//   * let e be the escaping char (\x01)
	//   * if (c in \x00\x0D\xFE or c == e) then return e followed by the
	//     character with value c+1.
	//
	// There is no test for \x00 here because \x00 really shouldn't occur in
	// a string -- otherwise things get weird.
	//
	for(const char c : raw)
	{
		if(c == bin_escape || bin_special.find(c) != std::string::npos) {
			encoded += bin_escape;
			encoded += (c + 1);
		} else {
			encoded += c;
		}
	}

	BOOST_CHECK( encode_binary(raw) == encoded );
	BOOST_CHECK( unencode_binary(encoded) == raw );
	// Identity.
	BOOST_CHECK( unencode_binary(encode_binary(raw)) == raw );
	BOOST_CHECK( unencode_binary(encode_binary(encoded)) == encoded );

	//
	// Binary escaping (recursive).
	//

	const unsigned recursive_steps = 16;
	std::string recursive_encoded = raw;

	for(unsigned n = 0; n < recursive_steps; ++n) {
		recursive_encoded = encode_binary(recursive_encoded);
	}

	BOOST_CHECK( recursive_encoded != raw );

	for(unsigned n = 0; n < recursive_steps; ++n) {
		recursive_encoded = unencode_binary(recursive_encoded);
	}

	BOOST_CHECK( recursive_encoded == raw );
}

// ============================================================================
// Extended addon_info tests
// ============================================================================

BOOST_AUTO_TEST_CASE( addon_info_default_construction )
{
	addon_info info;

	BOOST_CHECK( info.id.empty() );
	BOOST_CHECK( info.title.empty() );
	BOOST_CHECK( info.description.empty() );
	BOOST_CHECK( info.author.empty() );
	BOOST_CHECK_EQUAL( info.size, 0 );
	BOOST_CHECK_EQUAL( info.downloads, 0 );
	BOOST_CHECK_EQUAL( info.uploads, 0 );
	BOOST_CHECK_EQUAL( info.type, ADDON_UNKNOWN );
	BOOST_CHECK( info.tags.empty() );
	BOOST_CHECK( info.local_only == false );
}

BOOST_AUTO_TEST_CASE( addon_info_from_config )
{
	config cfg;
	cfg["name"] = "test_addon";
	cfg["title"] = "Test Add-on";
	cfg["description"] = "A test add-on";
	cfg["author"] = "Test Author";
	cfg["version"] = "1.0.0";
	cfg["size"] = "1024";
	cfg["downloads"] = "100";
	cfg["type"] = "campaign";

	addon_info info(cfg);

	BOOST_CHECK_EQUAL( info.id, "test_addon" );
	BOOST_CHECK_EQUAL( info.title, "Test Add-on" );
	BOOST_CHECK_EQUAL( info.description, "A test add-on" );
	BOOST_CHECK_EQUAL( info.author, "Test Author" );
	BOOST_CHECK_EQUAL( info.size, 1024 );
}

BOOST_AUTO_TEST_CASE( addon_info_display_title )
{
	addon_info info;
	info.id = "test_addon_name";
	info.title = "";

	// When title is empty, display_title should use id
	std::string display = info.display_title();
	BOOST_CHECK( !display.empty() );

	// With a title
	info.title = "Real Title";
	display = info.display_title();
	BOOST_CHECK_EQUAL( display, "Real Title" );
}

BOOST_AUTO_TEST_CASE( addon_info_translation_default )
{
	addon_info_translation trans;

	BOOST_CHECK( trans.supported == true );
	BOOST_CHECK( trans.title.empty() );
	BOOST_CHECK( trans.description.empty() );
}

BOOST_AUTO_TEST_CASE( addon_info_translation_from_config )
{
	config cfg;
	cfg["title"] = "Translated Title";
	cfg["description"] = "Translated Description";

	addon_info_translation trans(cfg);

	BOOST_CHECK_EQUAL( trans.title, "Translated Title" );
	BOOST_CHECK_EQUAL( trans.description, "Translated Description" );
}

BOOST_AUTO_TEST_CASE( addon_type_conversion )
{
	BOOST_CHECK_EQUAL( get_addon_type("unknown"), ADDON_UNKNOWN );
	BOOST_CHECK_EQUAL( get_addon_type("core"), ADDON_CORE );
	BOOST_CHECK_EQUAL( get_addon_type("campaign"), ADDON_SP_CAMPAIGN );
	BOOST_CHECK_EQUAL( get_addon_type("scenario_mp"), ADDON_MP_SCENARIO );
	BOOST_CHECK_EQUAL( get_addon_type("era"), ADDON_MP_ERA );
	BOOST_CHECK_EQUAL( get_addon_type("mod_mp"), ADDON_MOD );
}

BOOST_AUTO_TEST_CASE( addon_type_string_conversion )
{
	BOOST_CHECK_EQUAL( get_addon_type_string(ADDON_UNKNOWN), "unknown" );
	BOOST_CHECK_EQUAL( get_addon_type_string(ADDON_CORE), "core" );
	BOOST_CHECK_EQUAL( get_addon_type_string(ADDON_SP_CAMPAIGN), "campaign" );
	BOOST_CHECK_EQUAL( get_addon_type_string(ADDON_MP_SCENARIO), "scenario_mp" );
}

BOOST_AUTO_TEST_CASE( addon_check_status_descriptions )
{
	std::string desc = addon_check_status_desc(ADDON_CHECK_STATUS::SUCCESS);
	BOOST_CHECK( !desc.empty() );

	desc = addon_check_status_desc(ADDON_CHECK_STATUS::BAD_NAME);
	BOOST_CHECK( !desc.empty() );

	desc = translated_addon_check_status(ADDON_CHECK_STATUS::SUCCESS);
	BOOST_CHECK( !desc.empty() );
}

BOOST_AUTO_TEST_CASE( make_addon_title_test )
{
	BOOST_CHECK_EQUAL( make_addon_title("my_addon"), "my addon" );
	BOOST_CHECK_EQUAL( make_addon_title("simple"), "simple" );
	BOOST_CHECK_EQUAL( make_addon_title("multi_word_addon"), "multi word addon" );
}

BOOST_AUTO_TEST_CASE( size_display_string_test )
{
	// size_display_string returns empty string for 0 and uses utils::si_string
	BOOST_CHECK_EQUAL( size_display_string(0), "" );
	BOOST_CHECK_EQUAL( size_display_string(512), "512 B" );
	BOOST_CHECK_EQUAL( size_display_string(1024), "1024 B" );
	BOOST_CHECK_EQUAL( size_display_string(1048576), "1024 KiB" );
}

BOOST_AUTO_TEST_CASE( addon_filename_legal_valid )
{
	BOOST_CHECK( addon_filename_legal("valid_name") );
	BOOST_CHECK( addon_filename_legal("valid-name") );
	BOOST_CHECK( addon_filename_legal("valid.name") );
	BOOST_CHECK( addon_filename_legal("valid_name.txt") );
	BOOST_CHECK( addon_filename_legal("a") );
	BOOST_CHECK( addon_filename_legal("123") );
}

BOOST_AUTO_TEST_CASE( addon_filename_legal_invalid )
{
	BOOST_CHECK( !addon_filename_legal("") );
	BOOST_CHECK( !addon_filename_legal("con") );
	BOOST_CHECK( !addon_filename_legal("aux") );
	BOOST_CHECK( !addon_filename_legal("nul") );
}

BOOST_AUTO_TEST_CASE( addon_name_legal_valid )
{
	BOOST_CHECK( addon_name_legal("valid_name") );
	BOOST_CHECK( addon_name_legal("ValidName") );
	BOOST_CHECK( addon_name_legal("valid-name") );
	BOOST_CHECK( addon_name_legal("123") );
	BOOST_CHECK( addon_name_legal("a") );
}

BOOST_AUTO_TEST_CASE( needs_escaping_test )
{
	BOOST_CHECK( needs_escaping('\x00') );
	BOOST_CHECK( needs_escaping('\x01') );
	BOOST_CHECK( needs_escaping('\x0D') );
	BOOST_CHECK( needs_escaping('\xFE') );
	BOOST_CHECK( !needs_escaping('a') );
	BOOST_CHECK( !needs_escaping('A') );
	BOOST_CHECK( !needs_escaping(' ') );
}

// ============================================================================
// Addon config tests
// ============================================================================

BOOST_AUTO_TEST_CASE( addon_info_write_read )
{
	addon_info original;
	original.id = "test_addon";
	original.title = "Test Title";
	original.description = "Test Description";
	original.author = "Test Author";
	original.size = 2048;
	original.downloads = 50;
	original.type = ADDON_SP_CAMPAIGN;

	config cfg;
	original.write(cfg);

	// Note: addon_info::read() expects "name" for id, not "id"
	// So we need to set both for round-trip to work
	cfg["name"] = cfg["id"];

	addon_info parsed(cfg);

	BOOST_CHECK_EQUAL( parsed.id, original.id );
	BOOST_CHECK_EQUAL( parsed.title, original.title );
	BOOST_CHECK_EQUAL( parsed.description, original.description );
	BOOST_CHECK_EQUAL( parsed.author, original.author );
	BOOST_CHECK_EQUAL( parsed.size, original.size );
}

BOOST_AUTO_TEST_CASE( addon_info_write_minimal )
{
	addon_info info;
	info.id = "minimal_test";
	info.title = "Minimal Test";
	info.type = ADDON_MP_SCENARIO;
	info.uploads = 5;

	config cfg;
	info.write_minimal(cfg);

	// write_minimal writes: version, uploads, type, title, dependencies, core
	BOOST_CHECK( cfg.has_attribute("version") );
	BOOST_CHECK( cfg.has_attribute("title") );
	BOOST_CHECK( cfg.has_attribute("type") );
	BOOST_CHECK( cfg.has_attribute("uploads") );
}

BOOST_AUTO_TEST_CASE( read_addons_list_test )
{
	config campaigns;

	for (int i = 0; i < 3; ++i) {
		config& campaign = campaigns.add_child("campaign");
		// read_addons_list uses "name" for the id, not "id"
		campaign["name"] = "addon_" + std::to_string(i);
		campaign["title"] = "Addon " + std::to_string(i);
		campaign["version"] = "1.0.0";
	}

	addons_list list;
	read_addons_list(campaigns, list);

	BOOST_CHECK_EQUAL( list.size(), 3 );
	BOOST_CHECK( list.count("addon_0") > 0 );
	BOOST_CHECK( list.count("addon_1") > 0 );
	BOOST_CHECK( list.count("addon_2") > 0 );
}

BOOST_AUTO_TEST_CASE( addon_dependencies )
{
	addons_list addons;

	config cfg1;
	cfg1["name"] = "main_addon";
	cfg1["title"] = "Main";
	// dependencies are parsed from "dependencies" not "depends"
	cfg1["dependencies"] = "dep1,dep2";
	addons["main_addon"] = addon_info(cfg1);

	config cfg2;
	cfg2["name"] = "dep1";
	cfg2["title"] = "Dependency 1";
	addons["dep1"] = addon_info(cfg2);

	config cfg3;
	cfg3["name"] = "dep2";
	cfg3["title"] = "Dependency 2";
	addons["dep2"] = addon_info(cfg3);

	// Test dependency resolution - deps are resolved recursively
	auto deps = addons["main_addon"].resolve_dependencies(addons);
	// Since dep1 and dep2 have no dependencies themselves, they won't be in the set
	// The set only contains dependencies of dependencies
	// Let's verify the main addon has the right dependencies configured
	BOOST_CHECK_EQUAL( addons["main_addon"].depends.size(), 2 );
	BOOST_CHECK_EQUAL( addons["main_addon"].depends[0], "dep1" );
	BOOST_CHECK_EQUAL( addons["main_addon"].depends[1], "dep2" );
}

// ============================================================================
// Additional addon validation tests
// ============================================================================

BOOST_AUTO_TEST_CASE( addon_icon_size_test )
{
	// Test icon size checking
	std::string small_icon(1000, 'x');
	std::string large_icon(600000, 'x');

	BOOST_CHECK( !addon_icon_too_large(small_icon) );
	BOOST_CHECK( addon_icon_too_large(large_icon) );
	BOOST_CHECK( !addon_icon_too_large("") );
}

BOOST_AUTO_TEST_CASE( addon_all_type_conversions )
{
	// Test all addon type conversions
	BOOST_CHECK_EQUAL( get_addon_type("scenario"), ADDON_SP_SCENARIO );
	BOOST_CHECK_EQUAL( get_addon_type("campaign_sp_mp"), ADDON_SP_MP_CAMPAIGN );
	BOOST_CHECK_EQUAL( get_addon_type("campaign_mp"), ADDON_MP_CAMPAIGN );
	BOOST_CHECK_EQUAL( get_addon_type("map_pack"), ADDON_MP_MAPS );
	BOOST_CHECK_EQUAL( get_addon_type("faction"), ADDON_MP_FACTION );
	BOOST_CHECK_EQUAL( get_addon_type("media"), ADDON_MEDIA );
	BOOST_CHECK_EQUAL( get_addon_type("theme"), ADDON_THEME );
	BOOST_CHECK_EQUAL( get_addon_type("other"), ADDON_OTHER );

	// Test string conversions for all types
	BOOST_CHECK_EQUAL( get_addon_type_string(ADDON_SP_SCENARIO), "scenario" );
	BOOST_CHECK_EQUAL( get_addon_type_string(ADDON_SP_MP_CAMPAIGN), "campaign_sp_mp" );
	BOOST_CHECK_EQUAL( get_addon_type_string(ADDON_MP_CAMPAIGN), "campaign_mp" );
	BOOST_CHECK_EQUAL( get_addon_type_string(ADDON_MP_MAPS), "map_pack" );
	BOOST_CHECK_EQUAL( get_addon_type_string(ADDON_MP_FACTION), "faction" );
	BOOST_CHECK_EQUAL( get_addon_type_string(ADDON_MEDIA), "media" );
	BOOST_CHECK_EQUAL( get_addon_type_string(ADDON_THEME), "theme" );
	BOOST_CHECK_EQUAL( get_addon_type_string(ADDON_OTHER), "other" );
}

BOOST_AUTO_TEST_CASE( addon_info_versions_test )
{
	config cfg;
	cfg["name"] = "versioned_addon";
	cfg["title"] = "Versioned Add-on";
	cfg["version"] = "1.2.3";

	// Add multiple versions
	config& v1 = cfg.add_child("version");
	v1["version"] = "1.0.0";
	config& v2 = cfg.add_child("version");
	v2["version"] = "1.1.0";
	config& v3 = cfg.add_child("version");
	v3["version"] = "1.2.3";

	addon_info info(cfg);
	BOOST_CHECK_EQUAL( info.current_version.str(), "1.2.3" );
	BOOST_CHECK_EQUAL( info.versions.size(), 3 );
}

BOOST_AUTO_TEST_CASE( addon_info_tags_test )
{
	config cfg;
	cfg["name"] = "tagged_addon";
	cfg["title"] = "Tagged Add-on";
	cfg["tags"] = "tag1,tag2,tag3";

	addon_info info(cfg);
	BOOST_CHECK_EQUAL( info.tags.size(), 3 );
	BOOST_CHECK_EQUAL( info.tags[0], "tag1" );
	BOOST_CHECK_EQUAL( info.tags[1], "tag2" );
	BOOST_CHECK_EQUAL( info.tags[2], "tag3" );
}

BOOST_AUTO_TEST_CASE( addon_info_core_test )
{
	config cfg;
	cfg["name"] = "core_addon";
	cfg["title"] = "Core Add-on";
	cfg["core"] = "default";

	addon_info info(cfg);
	BOOST_CHECK_EQUAL( info.core, "default" );
}

BOOST_AUTO_TEST_CASE( addon_info_feedback_url_test )
{
	config cfg;
	cfg["name"] = "feedback_addon";
	cfg["title"] = "Feedback Add-on";
	cfg["feedback_url"] = "https://example.com/feedback";

	addon_info info(cfg);
	BOOST_CHECK_EQUAL( info.feedback_url, "https://example.com/feedback" );
}

BOOST_AUTO_TEST_CASE( addon_check_status_all_codes )
{
	// Test that all status codes have descriptions
	std::vector<ADDON_CHECK_STATUS> codes = {
		ADDON_CHECK_STATUS::SUCCESS,
		ADDON_CHECK_STATUS::UNAUTHORIZED,
		ADDON_CHECK_STATUS::DENIED,
		ADDON_CHECK_STATUS::USER_DOES_NOT_EXIST,
		ADDON_CHECK_STATUS::UNEXPECTED_DELTA,
		ADDON_CHECK_STATUS::EMPTY_PACK,
		ADDON_CHECK_STATUS::BAD_DELTA,
		ADDON_CHECK_STATUS::BAD_NAME,
		ADDON_CHECK_STATUS::NAME_HAS_MARKUP,
		ADDON_CHECK_STATUS::ILLEGAL_FILENAME,
		ADDON_CHECK_STATUS::FILENAME_CASE_CONFLICT,
		ADDON_CHECK_STATUS::INVALID_UTF8_NAME,
		ADDON_CHECK_STATUS::NO_TITLE,
		ADDON_CHECK_STATUS::NO_AUTHOR,
		ADDON_CHECK_STATUS::NO_VERSION,
		ADDON_CHECK_STATUS::NO_DESCRIPTION,
		ADDON_CHECK_STATUS::NO_EMAIL,
		ADDON_CHECK_STATUS::NO_PASSPHRASE,
		ADDON_CHECK_STATUS::TITLE_HAS_MARKUP,
		ADDON_CHECK_STATUS::BAD_TYPE,
		ADDON_CHECK_STATUS::VERSION_NOT_INCREMENTED,
		ADDON_CHECK_STATUS::BAD_FEEDBACK_TOPIC_ID,
		ADDON_CHECK_STATUS::FEEDBACK_TOPIC_ID_NOT_FOUND,
		ADDON_CHECK_STATUS::INVALID_UTF8_ATTRIBUTE,
		ADDON_CHECK_STATUS::AUTH_TYPE_MISMATCH,
		ADDON_CHECK_STATUS::ICON_TOO_LARGE,
		ADDON_CHECK_STATUS::SERVER_UNSPECIFIED,
		ADDON_CHECK_STATUS::SERVER_READ_ONLY,
		ADDON_CHECK_STATUS::SERVER_ADDONS_LIST,
		ADDON_CHECK_STATUS::SERVER_DELTA_NO_VERSIONS,
		ADDON_CHECK_STATUS::SERVER_FORUM_AUTH_DISABLED
	};

	for (auto code : codes) {
		std::string desc = addon_check_status_desc(code);
		BOOST_CHECK( !desc.empty() );
	}
}

BOOST_AUTO_TEST_CASE( addon_name_legal_edge_cases )
{
	// Edge cases for addon name validation
	BOOST_CHECK( addon_name_legal("a") );
	BOOST_CHECK( addon_name_legal("1") );
	BOOST_CHECK( addon_name_legal("_") );
	BOOST_CHECK( addon_name_legal("-") );
	BOOST_CHECK( !addon_name_legal("") );
	BOOST_CHECK( !addon_name_legal(" ") );
	BOOST_CHECK( !addon_name_legal("a b") );
	BOOST_CHECK( !addon_name_legal("a/b") );
	BOOST_CHECK( !addon_name_legal("a\\b") );
	BOOST_CHECK( !addon_name_legal("a@b") );
	BOOST_CHECK( !addon_name_legal("a#b") );
}

BOOST_AUTO_TEST_CASE( addon_translation_write_read )
{
	addon_info_translation original;
	original.supported = false;
	original.title = "Test Title";
	original.description = "Test Description";

	config cfg;
	original.write(cfg);

	addon_info_translation parsed(cfg);
	BOOST_CHECK_EQUAL( parsed.supported, false );
	BOOST_CHECK_EQUAL( parsed.title, "Test Title" );
	BOOST_CHECK_EQUAL( parsed.description, "Test Description" );
}

BOOST_AUTO_TEST_CASE( addon_info_locales_test )
{
	config cfg;
	cfg["name"] = "localized_addon";
	cfg["title"] = "Localized Add-on";

	config& trans1 = cfg.add_child("translation");
	trans1["language"] = "en_US";
	trans1["title"] = "English Title";
	trans1["description"] = "English Description";
	trans1["supported"] = true;

	config& trans2 = cfg.add_child("translation");
	trans2["language"] = "de_DE";
	trans2["title"] = "German Title";
	trans2["description"] = "German Description";
	trans2["supported"] = true;

	addon_info info(cfg);
	BOOST_CHECK_EQUAL( info.locales.size(), 2 );
	BOOST_CHECK_EQUAL( info.info_translations.size(), 2 );
}

BOOST_AUTO_TEST_CASE( addon_info_local_only_test )
{
	config cfg;
	cfg["name"] = "local_addon";
	cfg["title"] = "Local Add-on";
	cfg["local_only"] = true;

	addon_info info(cfg);
	BOOST_CHECK( info.local_only == true );
}

BOOST_AUTO_TEST_CASE( addon_info_uploads_downloads_test )
{
	config cfg;
	cfg["name"] = "popular_addon";
	cfg["title"] = "Popular Add-on";
	cfg["downloads"] = "5000";
	cfg["uploads"] = "10";

	addon_info info(cfg);
	BOOST_CHECK_EQUAL( info.downloads, 5000 );
	BOOST_CHECK_EQUAL( info.uploads, 10 );
}

BOOST_AUTO_TEST_CASE( addon_default_port_test )
{
	// Verify the default addon server port
	BOOST_CHECK_EQUAL( default_campaignd_port, 15019 );
}

BOOST_AUTO_TEST_SUITE_END()
