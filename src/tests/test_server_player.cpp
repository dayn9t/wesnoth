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
#include "server/wesnothd/player.hpp"
#include "server/common/simple_wml.hpp"

BOOST_AUTO_TEST_SUITE( server_player )

BOOST_AUTO_TEST_CASE( test_player_construction )
{
	simple_wml::document doc;
	simple_wml::node& cfg = doc.root();

	wesnothd::player p("test_player", cfg, 1, false, "1.16.0", "test_source", 12345);

	BOOST_CHECK_EQUAL(p.name(), "test_player");
	BOOST_CHECK_EQUAL(p.version(), "1.16.0");
	BOOST_CHECK_EQUAL(p.source(), "test_source");
	BOOST_CHECK_EQUAL(p.registered(), false);
	BOOST_CHECK_EQUAL(p.get_login_id(), 12345ULL);
}

BOOST_AUTO_TEST_CASE( test_player_registered_status )
{
	simple_wml::document doc;
	simple_wml::node& cfg = doc.root();

	wesnothd::player p("alice", cfg, 2, false, "1.16.0", "test", 100);

	BOOST_CHECK_EQUAL(p.registered(), false);

	p.mark_registered(true);
	BOOST_CHECK_EQUAL(p.registered(), true);

	p.mark_registered(false);
	BOOST_CHECK_EQUAL(p.registered(), false);
}

BOOST_AUTO_TEST_CASE( test_player_moderator_flag )
{
	simple_wml::document doc;
	simple_wml::node& cfg = doc.root();

	wesnothd::player p("mod_player", cfg, 3, false, "1.16.0", "test", 200, 4, std::chrono::seconds{10}, true);

	BOOST_CHECK(p.is_moderator());

	p.set_moderator(false);
	BOOST_CHECK(!p.is_moderator());

	p.set_moderator(true);
	BOOST_CHECK(p.is_moderator());
}

BOOST_AUTO_TEST_CASE( test_player_name_and_version )
{
	simple_wml::document doc;
	simple_wml::node& cfg = doc.root();

	wesnothd::player p("player_name", cfg, 5, true, "1.17.0", "source_addr", 300);

	BOOST_CHECK_EQUAL(p.name(), "player_name");
	BOOST_CHECK_EQUAL(p.version(), "1.17.0");
	BOOST_CHECK_EQUAL(p.source(), "source_addr");
}

BOOST_AUTO_TEST_CASE( test_player_status_transitions )
{
	simple_wml::document doc;
	simple_wml::node& cfg = doc.root();

	wesnothd::player p("status_test", cfg, 6, false, "1.16.0", "test", 400);

	// Test status transitions
	p.set_status(wesnothd::player::LOBBY);
	BOOST_CHECK_EQUAL(cfg["status"].to_string(), "lobby");

	p.set_status(wesnothd::player::PLAYING);
	BOOST_CHECK_EQUAL(cfg["status"].to_string(), "playing");

	p.set_status(wesnothd::player::OBSERVING);
	BOOST_CHECK_EQUAL(cfg["status"].to_string(), "observing");
}

BOOST_AUTO_TEST_CASE( test_player_mark_available )
{
	simple_wml::document doc;
	simple_wml::node& cfg = doc.root();

	wesnothd::player p("available_test", cfg, 7, false, "1.16.0", "test", 500);

	// Mark as in lobby
	p.mark_available(0, "");
	BOOST_CHECK_EQUAL(cfg["available"].to_string(), "yes");
	BOOST_CHECK_EQUAL(cfg["game_id"].to_string(), "0");

	// Mark as in game
	p.mark_available(42, "TestGame");
	BOOST_CHECK_EQUAL(cfg["available"].to_string(), "no");
	BOOST_CHECK_EQUAL(cfg["game_id"].to_string(), "42");
	BOOST_CHECK_EQUAL(cfg["location"].to_string(), "TestGame");
}

BOOST_AUTO_TEST_CASE( test_player_queue_management )
{
	simple_wml::document doc;
	simple_wml::node& cfg = doc.root();

	wesnothd::player p("queue_test", cfg, 8, false, "1.16.0", "test", 600);

	// Test adding to queues
	p.add_queue(1);
	p.add_queue(2);
	p.add_queue(3);

	const auto& queues = p.get_queues();
	BOOST_CHECK_EQUAL(queues.size(), 3);
	BOOST_CHECK(queues.count(1));
	BOOST_CHECK(queues.count(2));
	BOOST_CHECK(queues.count(3));

	// Test removing from queue
	p.remove_from_queue(2);
	BOOST_CHECK_EQUAL(queues.size(), 2);
	BOOST_CHECK(!queues.count(2));

	// Test clearing queues
	p.clear_queues();
	BOOST_CHECK(queues.empty());
}

BOOST_AUTO_TEST_CASE( test_player_message_flooding )
{
	simple_wml::document doc;
	simple_wml::node& cfg = doc.root();

	// Create player with low flood threshold for testing
	wesnothd::player p("flood_test", cfg, 9, false, "1.16.0", "test", 700, 2, std::chrono::seconds{60});

	// First check should not trigger flooding
	BOOST_CHECK(!p.is_message_flooding());

	// Second check should not trigger flooding yet
	BOOST_CHECK(!p.is_message_flooding());
}

BOOST_AUTO_TEST_CASE( test_player_config_address )
{
	simple_wml::document doc;
	simple_wml::node& cfg = doc.root();

	wesnothd::player p("config_test", cfg, 10, false, "1.16.0", "test", 800);

	BOOST_CHECK_EQUAL(p.config_address(), &cfg);
}

BOOST_AUTO_TEST_CASE( test_player_default_construction_values )
{
	simple_wml::document doc;
	simple_wml::node& cfg = doc.root();

	// Create player with default parameters
	wesnothd::player p("default_test", cfg, 11, false, "1.16.0", "test_source", 900);

	BOOST_CHECK_EQUAL(p.registered(), false);
	BOOST_CHECK(!p.is_moderator());
	BOOST_CHECK_EQUAL(p.get_login_id(), 900ULL);
}

BOOST_AUTO_TEST_CASE( test_player_registered_player_construction )
{
	simple_wml::document doc;
	simple_wml::node& cfg = doc.root();

	// Create a registered player
	wesnothd::player p("registered_player", cfg, 12, true, "1.16.0", "test", 1000);

	BOOST_CHECK_EQUAL(p.registered(), true);
	BOOST_CHECK_EQUAL(cfg["registered"].to_string(), "yes");
}

BOOST_AUTO_TEST_SUITE_END()
