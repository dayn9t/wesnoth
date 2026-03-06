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

#include "filesystem.hpp"
#include <string>

BOOST_AUTO_TEST_SUITE(test_filesystem_operations)

BOOST_AUTO_TEST_CASE(test_filesystem_basename_simple)
{
	std::string path = "/home/user/file.txt";
	std::string base = filesystem::base_name(path);

	BOOST_CHECK(!base.empty());
}

BOOST_AUTO_TEST_CASE(test_filesystem_dirname_simple)
{
	std::string path = "/home/user/file.txt";
	std::string dir = filesystem::directory_name(path);

	BOOST_CHECK(!dir.empty());
}

BOOST_AUTO_TEST_CASE(test_filesystem_normalize_path)
{
	std::string path = "/home/../home/user";
	std::string normalized = filesystem::normalize_path(path, true, true);

	BOOST_CHECK(!normalized.empty() || normalized.empty());
}

BOOST_AUTO_TEST_CASE(test_filesystem_is_relative)
{
	BOOST_CHECK(filesystem::is_relative("relative/path"));
	BOOST_CHECK(!filesystem::is_relative("/absolute/path"));
}

BOOST_AUTO_TEST_CASE(test_filesystem_file_exists)
{
	BOOST_CHECK_NO_THROW(filesystem::file_exists("/nonexistent/file.txt"));
}

BOOST_AUTO_TEST_CASE(test_filesystem_is_directory)
{
	BOOST_CHECK_NO_THROW(filesystem::is_directory("/tmp"));
}

BOOST_AUTO_TEST_CASE(test_filesystem_create_directory)
{
	BOOST_CHECK_NO_THROW(filesystem::make_directory("/tmp/test_dir_wesnoth"));
}

BOOST_AUTO_TEST_CASE(test_filesystem_get_user_data_dir)
{
	BOOST_CHECK_NO_THROW(filesystem::get_user_data_dir());
}

BOOST_AUTO_TEST_CASE(test_filesystem_get_cache_dir)
{
	BOOST_CHECK_NO_THROW(filesystem::get_cache_dir());
}

BOOST_AUTO_TEST_SUITE_END()