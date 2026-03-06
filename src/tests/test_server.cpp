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

// Include these before ban.hpp to ensure proper header ordering
#include <chrono>
#include <optional>

#include <boost/test/unit_test.hpp>

#include "server/common/simple_wml.hpp"
#include "config.hpp"
#include <memory>
#include <vector>

// ============================================================================
// simple_wml::document tests (extended)
// ============================================================================

BOOST_AUTO_TEST_SUITE(test_server_wml_document)

BOOST_AUTO_TEST_CASE(test_document_default_constructor)
{
	simple_wml::document doc;
	BOOST_CHECK_NO_THROW(doc.root());
}

BOOST_AUTO_TEST_CASE(test_document_set_attr)
{
	simple_wml::document doc;
	doc.set_attr("key", "value");

	BOOST_CHECK(doc.root().has_attr("key"));
	BOOST_CHECK_EQUAL(doc["key"].to_string(), "value");
}

BOOST_AUTO_TEST_CASE(test_document_set_attr_int)
{
	simple_wml::document doc;
	doc.root().set_attr_int("number", 42);

	BOOST_CHECK_EQUAL(doc["number"].to_int(), 42);
}

BOOST_AUTO_TEST_CASE(test_document_set_attr_dup)
{
	simple_wml::document doc;
	std::string value = "dynamic_value";
	doc.set_attr_dup("dyn_key", value.c_str());

	BOOST_CHECK_EQUAL(doc["dyn_key"].to_string(), "dynamic_value");
}

BOOST_AUTO_TEST_CASE(test_document_child_operations)
{
	simple_wml::document doc;
	simple_wml::node& child = doc.root().add_child("child");

	BOOST_CHECK(doc.root().child("child") != nullptr);
	BOOST_CHECK(doc.root().no_children() == false);
}

BOOST_AUTO_TEST_CASE(test_document_child_or_add)
{
	simple_wml::document doc;
	simple_wml::node& child = doc.root().child_or_add("new_child");

	BOOST_CHECK(doc.root().child("new_child") != nullptr);

	simple_wml::node& same_child = doc.root().child_or_add("new_child");
	BOOST_CHECK_EQUAL(&child, &same_child);
}

BOOST_AUTO_TEST_CASE(test_document_remove_child)
{
	simple_wml::document doc;
	doc.root().add_child("child1");
	doc.root().add_child("child2");

	BOOST_CHECK(doc.root().child("child1") != nullptr);

	doc.root().remove_child("child1", 0);
	BOOST_CHECK(doc.root().child("child1") == nullptr);
}

BOOST_AUTO_TEST_CASE(test_document_multiple_children)
{
	simple_wml::document doc;
	doc.root().add_child("item");
	doc.root().add_child("item");
	doc.root().add_child("item");

	const auto& children = doc.root().children("item");
	BOOST_CHECK_EQUAL(children.size(), 3);
}

BOOST_AUTO_TEST_CASE(test_document_nested_children)
{
	simple_wml::document doc;
	simple_wml::node& parent = doc.root().add_child("parent");
	parent.set_attr("name", "parent_node");

	simple_wml::node& child = parent.add_child("child");
	child.set_attr("name", "child_node");

	BOOST_CHECK_EQUAL(parent["name"].to_string(), "parent_node");

	simple_wml::node* child_ptr = parent.child("child");
	BOOST_CHECK(child_ptr != nullptr);
	BOOST_CHECK_EQUAL((*child_ptr)["name"].to_string(), "child_node");
}

BOOST_AUTO_TEST_CASE(test_document_nchildren)
{
	simple_wml::document doc;
	BOOST_CHECK_EQUAL(doc.root().nchildren(), 0);

	doc.root().add_child("a");
	doc.root().add_child("b");
	doc.root().add_child("c");

	BOOST_CHECK_EQUAL(doc.root().nchildren(), 3);
}

BOOST_AUTO_TEST_CASE(test_document_has_attr)
{
	simple_wml::document doc;
	doc.set_attr("existing", "value");

	BOOST_CHECK(doc.root().has_attr("existing"));
	BOOST_CHECK(!doc.root().has_attr("nonexistent"));
}

BOOST_AUTO_TEST_CASE(test_document_swap)
{
	simple_wml::document doc1;
	simple_wml::document doc2;

	doc1.set_attr("key1", "value1");
	doc2.set_attr("key2", "value2");

	swap(doc1, doc2);

	BOOST_CHECK_EQUAL(doc1["key2"].to_string(), "value2");
	BOOST_CHECK_EQUAL(doc2["key1"].to_string(), "value1");
}

BOOST_AUTO_TEST_CASE(test_document_clear)
{
	simple_wml::document doc;
	doc.set_attr("key", "value");
	doc.root().add_child("child");

	doc.clear();

	// After clear, root should be empty
	BOOST_CHECK(doc.root().no_children());
}

BOOST_AUTO_TEST_CASE(test_document_output)
{
	simple_wml::document doc;
	doc.set_attr("test", "value");

	const char* output = doc.output();
	BOOST_CHECK(output != nullptr);
	BOOST_CHECK(strlen(output) > 0);
}

BOOST_AUTO_TEST_SUITE_END()

// ============================================================================
// simple_wml::node tests (extended)
// ============================================================================

BOOST_AUTO_TEST_SUITE(test_server_wml_node)

BOOST_AUTO_TEST_CASE(test_node_attribute_access)
{
	simple_wml::document doc;
	simple_wml::node& root = doc.root();

	root.set_attr("attr1", "value1");
	root.set_attr("attr2", "value2");

	BOOST_CHECK_EQUAL(root["attr1"].to_string(), "value1");
	BOOST_CHECK_EQUAL(root["attr2"].to_string(), "value2");
}

BOOST_AUTO_TEST_CASE(test_node_attr_method)
{
	simple_wml::document doc;
	simple_wml::node& root = doc.root();

	root.set_attr("test", "test_value");

	BOOST_CHECK_EQUAL(root.attr("test").to_string(), "test_value");
}

BOOST_AUTO_TEST_CASE(test_node_set_attr_esc)
{
	simple_wml::document doc;
	simple_wml::node& root = doc.root();

	simple_wml::string_span value("value_with_special");
	root.set_attr_esc("escaped", value);

	BOOST_CHECK(root.has_attr("escaped"));
}

BOOST_AUTO_TEST_CASE(test_node_add_child_at)
{
	simple_wml::document doc;
	simple_wml::node& root = doc.root();

	root.add_child("item");
	root.add_child_at("inserted", 0);

	const auto& children = root.children("item");
	BOOST_CHECK_EQUAL(children.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_node_output_size)
{
	simple_wml::document doc;
	simple_wml::node& root = doc.root();

	root.set_attr("key", "value");

	int size = root.output_size();
	BOOST_CHECK(size > 0);
}

BOOST_AUTO_TEST_CASE(test_node_is_dirty)
{
	simple_wml::document doc;
	simple_wml::node& root = doc.root();

	BOOST_CHECK(root.is_dirty());

	root.set_attr("key", "value");

	BOOST_CHECK(root.is_dirty());
}

BOOST_AUTO_TEST_CASE(test_node_one_child)
{
	simple_wml::document doc;
	simple_wml::node& root = doc.root();

	BOOST_CHECK(root.no_children());
	BOOST_CHECK(!root.one_child());

	root.add_child("single");

	BOOST_CHECK(!root.no_children());
	BOOST_CHECK(root.one_child());

	root.add_child("another");

	BOOST_CHECK(!root.one_child());
}

BOOST_AUTO_TEST_CASE(test_node_copy_into)
{
	simple_wml::document doc1;
	simple_wml::document doc2;

	simple_wml::node& root1 = doc1.root();
	root1.set_attr("source", "original");
	root1.add_child("child");

	simple_wml::node& root2 = doc2.root();
	root1.copy_into(root2);

	BOOST_CHECK(root2.has_attr("source"));
	BOOST_CHECK(root2.child("child") != nullptr);
}

BOOST_AUTO_TEST_CASE(test_node_apply_diff)
{
	simple_wml::document doc1;
	simple_wml::document doc2;

	simple_wml::node& root1 = doc1.root();
	root1.set_attr("base", "value");

	simple_wml::node& root2 = doc2.root();
	// Create proper diff format with insert child
	simple_wml::node& insert_node = root2.add_child("insert");
	insert_node.set_attr("base", "new_value");

	// Apply diff from doc2 to doc1
	root1.apply_diff(root2);

	BOOST_CHECK_EQUAL(root1["base"].to_string(), "new_value");
}

BOOST_AUTO_TEST_SUITE_END()

// ============================================================================
// simple_wml::string_span tests (extended)
// ============================================================================

BOOST_AUTO_TEST_SUITE(test_server_string_span)

BOOST_AUTO_TEST_CASE(test_string_span_default_constructor)
{
	simple_wml::string_span span;
	BOOST_CHECK(span.empty());
	BOOST_CHECK(span.is_null());
	BOOST_CHECK_EQUAL(span.size(), 0);
}

BOOST_AUTO_TEST_CASE(test_string_span_c_str_constructor)
{
	const char* str = "hello";
	simple_wml::string_span span(str);

	BOOST_CHECK(!span.empty());
	BOOST_CHECK(!span.is_null());
	BOOST_CHECK_EQUAL(span.size(), 5);
	BOOST_CHECK_EQUAL(span.begin(), str);
}

BOOST_AUTO_TEST_CASE(test_string_span_size_constructor)
{
	const char* str = "world";
	simple_wml::string_span span(str, 5);

	BOOST_CHECK_EQUAL(span.size(), 5);
	BOOST_CHECK(span == "world");
}

BOOST_AUTO_TEST_CASE(test_string_span_iterator_constructor)
{
	const char* str = "test123";
	simple_wml::string_span span(str, str + 7);

	BOOST_CHECK_EQUAL(span.size(), 7);
	BOOST_CHECK(span == "test123");
}

BOOST_AUTO_TEST_CASE(test_string_span_equality_operators)
{
	simple_wml::string_span span1("hello");
	simple_wml::string_span span2("hello");
	simple_wml::string_span span3("world");

	BOOST_CHECK(span1 == span2);
	BOOST_CHECK(span1 != span3);
	BOOST_CHECK(span1 == "hello");
	BOOST_CHECK(span1 != "world");
	BOOST_CHECK(span1 == std::string("hello"));
	BOOST_CHECK(span1 != std::string("world"));
}

BOOST_AUTO_TEST_CASE(test_string_span_less_than_operator)
{
	simple_wml::string_span span1("abc");
	simple_wml::string_span span2("abd");
	simple_wml::string_span span3("abcd");

	BOOST_CHECK(span1 < span2);
	BOOST_CHECK(span1 < span3);
	BOOST_CHECK(!(span2 < span1));
}

BOOST_AUTO_TEST_CASE(test_string_span_to_string)
{
	simple_wml::string_span span("test_value");
	BOOST_CHECK_EQUAL(span.to_string(), "test_value");
}

BOOST_AUTO_TEST_CASE(test_string_span_to_int)
{
	simple_wml::string_span span1("12345");
	BOOST_CHECK_EQUAL(span1.to_int(), 12345);

	simple_wml::string_span span2("-42");
	BOOST_CHECK_EQUAL(span2.to_int(), -42);

	simple_wml::string_span span3("0");
	BOOST_CHECK_EQUAL(span3.to_int(), 0);
}

BOOST_AUTO_TEST_CASE(test_string_span_to_bool)
{
	simple_wml::string_span span1("true");
	BOOST_CHECK_EQUAL(span1.to_bool(), true);

	simple_wml::string_span span2("yes");
	BOOST_CHECK_EQUAL(span2.to_bool(), true);

	simple_wml::string_span span3("1");
	BOOST_CHECK_EQUAL(span3.to_bool(), true);

	simple_wml::string_span span4("false");
	BOOST_CHECK_EQUAL(span4.to_bool(), false);

	simple_wml::string_span span5("0");
	BOOST_CHECK_EQUAL(span5.to_bool(), false);

	simple_wml::string_span span6("");
	BOOST_CHECK_EQUAL(span6.to_bool(false), false);
	BOOST_CHECK_EQUAL(span6.to_bool(true), true);
}

BOOST_AUTO_TEST_CASE(test_string_span_begin_end)
{
	const char* str = "hello";
	simple_wml::string_span span(str);

	BOOST_CHECK_EQUAL(span.begin(), str);
	BOOST_CHECK_EQUAL(span.end(), str + 5);
}

BOOST_AUTO_TEST_CASE(test_string_span_string_view_conversion)
{
	simple_wml::string_span span("test");
	std::string_view view = span;

	BOOST_CHECK_EQUAL(view, "test");
	BOOST_CHECK_EQUAL(view.size(), 4);
}

BOOST_AUTO_TEST_CASE(test_string_span_duplicate)
{
	simple_wml::string_span span("duplicate");
	char* dup = span.duplicate();

	BOOST_CHECK(dup != nullptr);
	BOOST_CHECK_EQUAL(std::string(dup), "duplicate");

	delete[] dup;
}

BOOST_AUTO_TEST_SUITE_END()

// ============================================================================
// Server-specific data structure tests
// ============================================================================

BOOST_AUTO_TEST_SUITE(test_server_data_structures)

BOOST_AUTO_TEST_CASE(test_ban_data_in_document)
{
	simple_wml::document doc;
	simple_wml::node& ban_node = doc.root().add_child("ban");

	ban_node.set_attr("ip", "192.168.1.1");
	ban_node.set_attr("reason", "Cheating");
	ban_node.set_attr("who_banned", "Admin");
	ban_node.set_attr("duration", "24h");

	BOOST_CHECK(ban_node.has_attr("ip"));
	BOOST_CHECK(ban_node.has_attr("reason"));
	BOOST_CHECK_EQUAL(ban_node["ip"].to_string(), "192.168.1.1");
	BOOST_CHECK_EQUAL(ban_node["reason"].to_string(), "Cheating");
}

BOOST_AUTO_TEST_CASE(test_player_data_in_document)
{
	simple_wml::document doc;
	simple_wml::node& player = doc.root().add_child("player");

	player.set_attr("name", "TestPlayer");
	player.set_attr("version", "1.16.0");
	player.set_attr("source", "192.168.1.100");
	player.set_attr_int("id", 12345);

	BOOST_CHECK_EQUAL(player["name"].to_string(), "TestPlayer");
	BOOST_CHECK_EQUAL(player["version"].to_string(), "1.16.0");
	BOOST_CHECK_EQUAL(player["id"].to_int(), 12345);
}

BOOST_AUTO_TEST_CASE(test_game_room_data_in_document)
{
	simple_wml::document doc;
	simple_wml::node& room = doc.root().add_child("gameroom");

	room.set_attr("name", "Test Room");
	room.set_attr_int("id", 1);
	room.set_attr_int("max_players", 8);

	simple_wml::node& players = room.add_child("players");
	players.set_attr_int("count", 3);

	simple_wml::node& settings = room.add_child("settings");
	settings.set_attr("map", "test_map");
	settings.set_attr("era", "default");

	BOOST_CHECK_EQUAL(room["name"].to_string(), "Test Room");
	BOOST_CHECK_EQUAL(room["max_players"].to_int(), 8);

	simple_wml::node* players_ptr = room.child("players");
	BOOST_CHECK(players_ptr != nullptr);
	BOOST_CHECK_EQUAL((*players_ptr)["count"].to_int(), 3);

	simple_wml::node* settings_ptr = room.child("settings");
	BOOST_CHECK(settings_ptr != nullptr);
	BOOST_CHECK_EQUAL((*settings_ptr)["map"].to_string(), "test_map");
}

BOOST_AUTO_TEST_CASE(test_server_config_structure)
{
	simple_wml::document doc;
	simple_wml::node& config = doc.root().add_child("server_config");

	config.set_attr("name", "Official Server");
	config.set_attr_int("port", 15000);
	config.set_attr("host", "server.wesnoth.org");

	simple_wml::node& options = config.add_child("options");
	options.set_attr("allow_registration", "yes");
	options.set_attr("require_version", "1.16.0");

	BOOST_CHECK_EQUAL(config["port"].to_int(), 15000);

	simple_wml::node* options_ptr = config.child("options");
	BOOST_CHECK(options_ptr != nullptr);
}

BOOST_AUTO_TEST_CASE(test_message_queue_structure)
{
	simple_wml::document doc;
	simple_wml::node& messages = doc.root().add_child("messages");

	for (int i = 0; i < 5; ++i) {
		simple_wml::node& msg = messages.add_child("message");
		msg.set_attr_dup("from", ("user" + std::to_string(i)).c_str());
		msg.set_attr_dup("content", ("Test message " + std::to_string(i)).c_str());
		msg.set_attr_int("timestamp", 1000 + i);
	}

	const auto& msg_list = messages.children("message");
	BOOST_CHECK_EQUAL(msg_list.size(), 5);
}

BOOST_AUTO_TEST_CASE(test_multiple_document_instances)
{
	std::vector<std::unique_ptr<simple_wml::document>> docs;

	for (int i = 0; i < 10; ++i) {
		auto d = std::make_unique<simple_wml::document>();
		d->set_attr_dup("doc_id", std::to_string(i).c_str());
		d->root().add_child("data");
		docs.push_back(std::move(d));
	}

	BOOST_CHECK_EQUAL(docs.size(), 10);

	for (int i = 0; i < 10; ++i) {
		BOOST_CHECK_EQUAL((*docs[i])["doc_id"].to_string(), std::to_string(i));
	}
}

BOOST_AUTO_TEST_SUITE_END()

// ============================================================================
// Ban system tests
// ============================================================================

#include "server/wesnothd/ban.hpp"

BOOST_AUTO_TEST_SUITE(test_server_ban_system)

BOOST_AUTO_TEST_CASE(test_parse_ip_basic)
{
	// Test basic IP parsing
	wesnothd::ip_mask result = wesnothd::parse_ip("192.168.1.1");
	BOOST_CHECK_EQUAL(result.first, 0xC0A80101U); // 192.168.1.1 in hex
	BOOST_CHECK_EQUAL(result.second, 0xFFFFFFFFU); // Full mask
}

BOOST_AUTO_TEST_CASE(test_parse_ip_with_wildcard)
{
	// Test IP with wildcard
	wesnothd::ip_mask result = wesnothd::parse_ip("192.168.1.*");
	BOOST_CHECK_EQUAL(result.second, 0xFFFFFF00U); // /24 mask
}

BOOST_AUTO_TEST_CASE(test_parse_ip_subnet)
{
	// Test subnet mask parsing
	wesnothd::ip_mask result = wesnothd::parse_ip("10.0.0.0");
	BOOST_CHECK_EQUAL(result.first, 0x0A000000U); // 10.0.0.0 in hex
}

BOOST_AUTO_TEST_CASE(test_banned_construction_basic)
{
	// Test basic banned construction - note: basic constructor doesn't store IP text
	wesnothd::banned ban("192.168.1.1");
	// Basic constructor only sets ip_ and mask_, not ip_text_
	BOOST_CHECK_EQUAL(ban.get_int_ip(), 0xC0A80101U);
	BOOST_CHECK_EQUAL(ban.mask(), 0xFFFFFFFFU);
}

BOOST_AUTO_TEST_CASE(test_banned_construction_full)
{
	// Test full banned construction with all parameters
	auto end_time = std::chrono::system_clock::now() + std::chrono::hours(24);
	wesnothd::banned ban("192.168.1.1", end_time, "Test reason", "Admin", "test_group", "test_nick");

	BOOST_CHECK_EQUAL(ban.get_ip(), "192.168.1.1");
	BOOST_CHECK_EQUAL(ban.get_reason(), "Test reason");
	BOOST_CHECK_EQUAL(ban.get_who_banned(), "Admin");
	BOOST_CHECK_EQUAL(ban.get_group(), "test_group");
	BOOST_CHECK_EQUAL(ban.get_nick(), "test_nick");
}

BOOST_AUTO_TEST_CASE(test_banned_match_ip)
{
	// Test IP matching
	wesnothd::banned ban("192.168.1.1");
	wesnothd::ip_mask test_ip = wesnothd::parse_ip("192.168.1.1");

	BOOST_CHECK(ban.match_ip(test_ip));
}

BOOST_AUTO_TEST_CASE(test_banned_match_ip_negative)
{
	// Test IP non-matching
	wesnothd::banned ban("192.168.1.1");
	wesnothd::ip_mask test_ip = wesnothd::parse_ip("192.168.1.2");

	BOOST_CHECK(!ban.match_ip(test_ip));
}

BOOST_AUTO_TEST_CASE(test_banned_match_group)
{
	auto end_time = std::chrono::system_clock::now() + std::chrono::hours(24);
	wesnothd::banned ban("192.168.1.1", end_time, "Test", "Admin", "test_group");

	BOOST_CHECK(ban.match_group("test_group"));
	BOOST_CHECK(!ban.match_group("other_group"));
}

BOOST_AUTO_TEST_CASE(test_banned_get_remaining_time)
{
	// Test ban with finite time
	auto end_time = std::chrono::system_clock::now() + std::chrono::hours(24);
	wesnothd::banned ban("192.168.1.1", end_time, "Test reason", "Admin");

	auto remaining = ban.get_remaining_ban_time();
	BOOST_CHECK(remaining.has_value());
	BOOST_CHECK(remaining->count() > 0);
}

BOOST_AUTO_TEST_CASE(test_banned_permanent_ban)
{
	// Test permanent ban (no end time)
	wesnothd::banned ban("192.168.1.1", utils::nullopt, "Permanent ban", "Admin");

	auto remaining = ban.get_remaining_ban_time();
	BOOST_CHECK(!remaining.has_value());
	BOOST_CHECK_EQUAL(ban.get_human_end_time(), "permanent");
}

BOOST_AUTO_TEST_CASE(test_banned_config_read_write)
{
	// Test reading/writing from config
	config cfg;
	cfg["ip"] = "10.0.0.1";
	cfg["reason"] = "Config test";
	cfg["who_banned"] = "TestAdmin";
	cfg["group"] = "testers";
	cfg["nick"] = "testuser";

	wesnothd::banned ban(cfg);

	BOOST_CHECK_EQUAL(ban.get_ip(), "10.0.0.1");
	BOOST_CHECK_EQUAL(ban.get_reason(), "Config test");
	BOOST_CHECK_EQUAL(ban.get_who_banned(), "TestAdmin");
	BOOST_CHECK_EQUAL(ban.get_group(), "testers");
	BOOST_CHECK_EQUAL(ban.get_nick(), "testuser");
}

BOOST_AUTO_TEST_CASE(test_ban_manager_construction)
{
	// Test ban manager construction
	wesnothd::ban_manager manager;
	BOOST_CHECK(!manager.get_ban_help().empty());
}

BOOST_AUTO_TEST_CASE(test_ban_manager_parse_time)
{
	wesnothd::ban_manager manager;
	auto start_time = std::chrono::system_clock::now();

	// Test parsing "permanent"
	auto [success1, end_time1] = manager.parse_time("permanent", start_time);
	BOOST_CHECK(success1);
	BOOST_CHECK(!end_time1.has_value());

	// Test parsing "0"
	auto [success2, end_time2] = manager.parse_time("0", start_time);
	BOOST_CHECK(success2);
	BOOST_CHECK(!end_time2.has_value());
}

BOOST_AUTO_TEST_CASE(test_ban_manager_parse_time_hours)
{
	wesnothd::ban_manager manager;
	auto start_time = std::chrono::system_clock::now();

	// Test parsing hours
	auto [success, end_time] = manager.parse_time("2h", start_time);
	BOOST_CHECK(success);
	BOOST_CHECK(end_time.has_value());
	// Should be approximately 2 hours later
	auto diff = *end_time - start_time;
	BOOST_CHECK(diff >= std::chrono::hours(2) - std::chrono::seconds(1));
	BOOST_CHECK(diff <= std::chrono::hours(2) + std::chrono::seconds(1));
}

BOOST_AUTO_TEST_CASE(test_ban_manager_parse_time_complex)
{
	wesnothd::ban_manager manager;
	auto start_time = std::chrono::system_clock::now();

	// Test parsing complex duration
	auto [success, end_time] = manager.parse_time("1h30m", start_time);
	BOOST_CHECK(success);
	BOOST_CHECK(end_time.has_value());

	auto diff = *end_time - start_time;
	// Should be approximately 90 minutes
	BOOST_CHECK(diff >= std::chrono::minutes(90) - std::chrono::seconds(1));
	BOOST_CHECK(diff <= std::chrono::minutes(90) + std::chrono::seconds(1));
}

BOOST_AUTO_TEST_CASE(test_banned_comparison)
{
	auto end_time1 = std::chrono::system_clock::now() + std::chrono::hours(1);
	auto end_time2 = std::chrono::system_clock::now() + std::chrono::hours(2);

	wesnothd::banned ban1("192.168.1.1", end_time1, "Shorter", "Admin");
	wesnothd::banned ban2("192.168.1.2", end_time2, "Longer", "Admin");

	// ban2 has later end time, so ban1 > ban2 should be false
	BOOST_CHECK(!(ban1 > ban2));
	BOOST_CHECK(ban2 > ban1);
}

BOOST_AUTO_TEST_SUITE_END()

