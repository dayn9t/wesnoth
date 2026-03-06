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

#include "spirit_po/catalog.hpp"
#include "spirit_po/po_message.hpp"
#include "spirit_po/catalog_metadata.hpp"
#include "spirit_po/exceptions.hpp"

#include <sstream>
#include <string>

BOOST_AUTO_TEST_SUITE( spirit_po )

// ============================================================================
// Helper function to create a minimal valid PO file content
// ============================================================================
static std::string create_minimal_po_content() {
	return R"(
msgid ""
msgstr ""
"Content-Type: text/plain; charset=UTF-8\n"
"Plural-Forms: nplurals=2; plural=n != 1;\n"

msgid "Hello"
msgstr "Hola"

msgid "World"
msgstr "Mundo"
)";
}

static std::string create_po_with_plurals() {
	return R"(
msgid ""
msgstr ""
"Content-Type: text/plain; charset=UTF-8\n"
"Plural-Forms: nplurals=2; plural=n != 1;\n"

msgid "apple"
msgid_plural "apples"
msgstr[0] "manzana"
msgstr[1] "manzanas"
)";
}

static std::string create_po_with_context() {
	return R"(
msgid ""
msgstr ""
"Content-Type: text/plain; charset=UTF-8\n"
"Plural-Forms: nplurals=2; plural=n != 1;\n"

msgctxt "menu"
msgid "Open"
msgstr "Abrir"

msgctxt "file"
msgid "Open"
msgstr "Open File"

msgid "Open"
msgstr "General Open"
)";
}

static std::string create_po_with_plural_and_context() {
	return R"(
msgid ""
msgstr ""
"Content-Type: text/plain; charset=UTF-8\n"
"Plural-Forms: nplurals=2; plural=n != 1;\n"

msgctxt "file"
msgid "file"
msgid_plural "files"
msgstr[0] "archivo"
msgstr[1] "archivos"
)";
}

// ============================================================================
// Test 1: Basic gettext translation lookup
// ============================================================================
BOOST_AUTO_TEST_CASE( gettext_basic_lookup )
{
	std::string po_content = create_minimal_po_content();
	auto it = po_content.begin();
	auto end = po_content.end();

	spirit_po::default_catalog cat = spirit_po::default_catalog::from_iterators(it, end);

	BOOST_CHECK_EQUAL( cat.gettext("Hello"), "Hola" );
	BOOST_CHECK_EQUAL( cat.gettext("World"), "Mundo" );
}

// ============================================================================
// Test 2: gettext returns original string for untranslated messages
// ============================================================================
BOOST_AUTO_TEST_CASE( gettext_untranslated_fallback )
{
	std::string po_content = create_minimal_po_content();
	auto it = po_content.begin();
	auto end = po_content.end();

	spirit_po::default_catalog cat = spirit_po::default_catalog::from_iterators(it, end);

	// For untranslated strings, gettext returns the original msgid
	BOOST_CHECK_EQUAL( cat.gettext("Untranslated"), "Untranslated" );
	BOOST_CHECK_EQUAL( cat.gettext(""), "" );  // Empty string returns empty
}

// ============================================================================
// Test 3: gettext_str with std::string interface
// ============================================================================
BOOST_AUTO_TEST_CASE( gettext_str_interface )
{
	std::string po_content = create_minimal_po_content();
	auto it = po_content.begin();
	auto end = po_content.end();

	spirit_po::default_catalog cat = spirit_po::default_catalog::from_iterators(it, end);

	std::string msgid = "Hello";
	BOOST_CHECK_EQUAL( cat.gettext_str(msgid), "Hola" );

	std::string untranslated = "NotInCatalog";
	BOOST_CHECK_EQUAL( cat.gettext_str(untranslated), "NotInCatalog" );

	// Test with rvalue
	BOOST_CHECK_EQUAL( cat.gettext_str(std::string("World")), "Mundo" );
}

// ============================================================================
// Test 4: ngettext basic plural forms
// ============================================================================
BOOST_AUTO_TEST_CASE( ngettext_singular_form )
{
	std::string po_content = create_po_with_plurals();
	auto it = po_content.begin();
	auto end = po_content.end();

	spirit_po::default_catalog cat = spirit_po::default_catalog::from_iterators(it, end);

	// n=1 should return singular form (index 0)
	BOOST_CHECK_EQUAL( cat.ngettext("apple", "apples", 1), "manzana" );
}

// ============================================================================
// Test 5: ngettext plural forms (n != 1)
// ============================================================================
BOOST_AUTO_TEST_CASE( ngettext_plural_form )
{
	std::string po_content = create_po_with_plurals();
	auto it = po_content.begin();
	auto end = po_content.end();

	spirit_po::default_catalog cat = spirit_po::default_catalog::from_iterators(it, end);

	// n != 1 should return plural form (index 1)
	BOOST_CHECK_EQUAL( cat.ngettext("apple", "apples", 0), "manzanas" );
	BOOST_CHECK_EQUAL( cat.ngettext("apple", "apples", 2), "manzanas" );
	BOOST_CHECK_EQUAL( cat.ngettext("apple", "apples", 10), "manzanas" );
	BOOST_CHECK_EQUAL( cat.ngettext("apple", "apples", 100), "manzanas" );
}

// ============================================================================
// Test 6: ngettext fallback for untranslated plurals
// ============================================================================
BOOST_AUTO_TEST_CASE( ngettext_fallback )
{
	std::string po_content = create_po_with_plurals();
	auto it = po_content.begin();
	auto end = po_content.end();

	spirit_po::default_catalog cat = spirit_po::default_catalog::from_iterators(it, end);

	// For untranslated plural strings, returns appropriate form based on n
	BOOST_CHECK_EQUAL( cat.ngettext("orange", "oranges", 1), "orange" );
	BOOST_CHECK_EQUAL( cat.ngettext("orange", "oranges", 2), "oranges" );
	BOOST_CHECK_EQUAL( cat.ngettext("orange", "oranges", 0), "oranges" );
}

// ============================================================================
// Test 7: ngettext_str interface
// ============================================================================
BOOST_AUTO_TEST_CASE( ngettext_str_interface )
{
	std::string po_content = create_po_with_plurals();
	auto it = po_content.begin();
	auto end = po_content.end();

	spirit_po::default_catalog cat = spirit_po::default_catalog::from_iterators(it, end);

	BOOST_CHECK_EQUAL( cat.ngettext_str("apple", "apples", 1), "manzana" );
	BOOST_CHECK_EQUAL( cat.ngettext_str("apple", "apples", 5), "manzanas" );

	// Test with rvalue strings
	BOOST_CHECK_EQUAL( cat.ngettext_str(std::string("apple"), std::string("apples"), 1), "manzana" );
}

// ============================================================================
// Test 8: pgettext context-based translation
// ============================================================================
BOOST_AUTO_TEST_CASE( pgettext_context_lookup )
{
	std::string po_content = create_po_with_context();
	auto it = po_content.begin();
	auto end = po_content.end();

	spirit_po::default_catalog cat = spirit_po::default_catalog::from_iterators(it, end);

	// Context-specific translations
	BOOST_CHECK_EQUAL( cat.pgettext("menu", "Open"), "Abrir" );
	BOOST_CHECK_EQUAL( cat.pgettext("file", "Open"), "Open File" );

	// Non-context translation (default)
	BOOST_CHECK_EQUAL( cat.gettext("Open"), "General Open" );
}

// ============================================================================
// Test 9: pgettext fallback
// ============================================================================
BOOST_AUTO_TEST_CASE( pgettext_fallback )
{
	std::string po_content = create_po_with_context();
	auto it = po_content.begin();
	auto end = po_content.end();

	spirit_po::default_catalog cat = spirit_po::default_catalog::from_iterators(it, end);

	// Non-existent context returns msgid
	BOOST_CHECK_EQUAL( cat.pgettext("nonexistent", "Open"), "Open" );

	// Non-existent msgid in existing context returns msgid
	BOOST_CHECK_EQUAL( cat.pgettext("menu", "Close"), "Close" );
}

// ============================================================================
// Test 10: pgettext_str interface
// ============================================================================
BOOST_AUTO_TEST_CASE( pgettext_str_interface )
{
	std::string po_content = create_po_with_context();
	auto it = po_content.begin();
	auto end = po_content.end();

	spirit_po::default_catalog cat = spirit_po::default_catalog::from_iterators(it, end);

	BOOST_CHECK_EQUAL( cat.pgettext_str("menu", "Open"), "Abrir" );
	BOOST_CHECK_EQUAL( cat.pgettext_str("file", "Open"), "Open File" );

	// Test with rvalue msgid
	BOOST_CHECK_EQUAL( cat.pgettext_str("menu", std::string("Open")), "Abrir" );
}

// ============================================================================
// Test 11: npgettext plural with context
// ============================================================================
BOOST_AUTO_TEST_CASE( npgettext_plural_with_context )
{
	std::string po_content = create_po_with_plural_and_context();
	auto it = po_content.begin();
	auto end = po_content.end();

	spirit_po::default_catalog cat = spirit_po::default_catalog::from_iterators(it, end);

	// Singular form (n=1)
	BOOST_CHECK_EQUAL( cat.npgettext("file", "file", "files", 1), "archivo" );

	// Plural forms (n != 1)
	BOOST_CHECK_EQUAL( cat.npgettext("file", "file", "files", 2), "archivos" );
	BOOST_CHECK_EQUAL( cat.npgettext("file", "file", "files", 0), "archivos" );
	BOOST_CHECK_EQUAL( cat.npgettext("file", "file", "files", 10), "archivos" );
}

// ============================================================================
// Test 12: npgettext fallback
// ============================================================================
BOOST_AUTO_TEST_CASE( npgettext_fallback )
{
	std::string po_content = create_po_with_plural_and_context();
	auto it = po_content.begin();
	auto end = po_content.end();

	spirit_po::default_catalog cat = spirit_po::default_catalog::from_iterators(it, end);

	// Non-existent context returns appropriate fallback
	BOOST_CHECK_EQUAL( cat.npgettext("nonexistent", "doc", "docs", 1), "doc" );
	BOOST_CHECK_EQUAL( cat.npgettext("nonexistent", "doc", "docs", 2), "docs" );
}

// ============================================================================
// Test 13: npgettext_str interface
// ============================================================================
BOOST_AUTO_TEST_CASE( npgettext_str_interface )
{
	std::string po_content = create_po_with_plural_and_context();
	auto it = po_content.begin();
	auto end = po_content.end();

	spirit_po::default_catalog cat = spirit_po::default_catalog::from_iterators(it, end);

	BOOST_CHECK_EQUAL( cat.npgettext_str("file", "file", "files", 1), "archivo" );
	BOOST_CHECK_EQUAL( cat.npgettext_str("file", "file", "files", 5), "archivos" );
}

// ============================================================================
// Test 14: po_message default construction
// ============================================================================
BOOST_AUTO_TEST_CASE( po_message_default_construction )
{
	spirit_po::po_message msg;

	BOOST_CHECK( !msg.context );
	BOOST_CHECK( msg.id.empty() );
	BOOST_CHECK( msg.id_plural().empty() );
	BOOST_CHECK( msg.strings().empty() );
	BOOST_CHECK( !msg.is_plural() );
	BOOST_CHECK_EQUAL( msg.line_no, 0 );
}

// ============================================================================
// Test 15: po_message is_plural detection
// ============================================================================
BOOST_AUTO_TEST_CASE( po_message_plural_detection )
{
	spirit_po::po_message msg;

	// No id_plural means not plural
	BOOST_CHECK( !msg.is_plural() );

	// Set id_plural makes it plural
	msg.id_plural() = "apples";
	BOOST_CHECK( msg.is_plural() );

	// Empty id_plural means not plural
	msg.id_plural().clear();
	BOOST_CHECK( !msg.is_plural() );
}

// ============================================================================
// Test 16: catalog_metadata default construction
// ============================================================================
BOOST_AUTO_TEST_CASE( catalog_metadata_default_construction )
{
	spirit_po::catalog_metadata meta;

	BOOST_CHECK( meta.project_id.empty() );
	BOOST_CHECK( meta.language.empty() );
	BOOST_CHECK( meta.language_team.empty() );
	BOOST_CHECK( meta.last_translator.empty() );
	BOOST_CHECK_EQUAL( meta.num_plural_forms, 0 );
	BOOST_CHECK( meta.plural_forms_function_string.empty() );
	BOOST_CHECK( meta.charset.empty() );
}

// ============================================================================
// Test 17: catalog_metadata parse_header
// ============================================================================
BOOST_AUTO_TEST_CASE( catalog_metadata_parse_header )
{
	spirit_po::catalog_metadata meta;

	std::string header =
		"Project-Id-Version: Test Project\n"
		"Language: es\n"
		"Language-Team: Spanish Team\n"
		"Last-Translator: John Doe\n"
		"Content-Type: text/plain; charset=UTF-8\n"
		"Plural-Forms: nplurals=2; plural=n != 1;\n";

	std::string error = meta.parse_header(header);

	BOOST_CHECK( error.empty() );
	BOOST_CHECK_EQUAL( meta.project_id, "Test Project" );
	BOOST_CHECK_EQUAL( meta.language, "es" );
	BOOST_CHECK_EQUAL( meta.language_team, "Spanish Team" );
	BOOST_CHECK_EQUAL( meta.last_translator, "John Doe" );
	BOOST_CHECK_EQUAL( meta.num_plural_forms, 2 );
	BOOST_CHECK_EQUAL( meta.plural_forms_function_string, "n != 1" );
	BOOST_CHECK_EQUAL( meta.charset, "UTF-8" );
}

// ============================================================================
// Test 18: catalog_metadata default plural forms
// ============================================================================
BOOST_AUTO_TEST_CASE( catalog_metadata_default_plural_forms )
{
	spirit_po::catalog_metadata meta;

	// Header without Plural-Forms should use defaults
	std::string header = "Project-Id-Version: Test\n";

	std::string error = meta.parse_header(header);

	BOOST_CHECK( error.empty() );
	BOOST_CHECK_EQUAL( meta.num_plural_forms, 2 );
	BOOST_CHECK_EQUAL( meta.plural_forms_function_string, "n != 1" );
}

// ============================================================================
// Test 19: catalog size
// ============================================================================
BOOST_AUTO_TEST_CASE( catalog_size )
{
	std::string po_content = create_minimal_po_content();
	auto it = po_content.begin();
	auto end = po_content.end();

	spirit_po::default_catalog cat = spirit_po::default_catalog::from_iterators(it, end);

	// Size should be 2 (Hello and World), excluding the header entry
	BOOST_CHECK_EQUAL( cat.size(), 2 );
}

// ============================================================================
// Test 20: catalog get_metadata
// ============================================================================
BOOST_AUTO_TEST_CASE( catalog_metadata_access )
{
	std::string po_content = create_minimal_po_content();
	auto it = po_content.begin();
	auto end = po_content.end();

	spirit_po::default_catalog cat = spirit_po::default_catalog::from_iterators(it, end);

	const spirit_po::catalog_metadata& meta = cat.get_metadata();

	BOOST_CHECK_EQUAL( meta.num_plural_forms, 2 );
	BOOST_CHECK_EQUAL( meta.plural_forms_function_string, "n != 1" );
	BOOST_CHECK_EQUAL( meta.charset, "UTF-8" );
}

// ============================================================================
// Test 21: catalog from_istream
// ============================================================================
BOOST_AUTO_TEST_CASE( catalog_from_istream )
{
	std::string po_content = create_minimal_po_content();
	std::istringstream iss(po_content);

	spirit_po::default_catalog cat = spirit_po::default_catalog::from_istream(iss);

	BOOST_CHECK_EQUAL( cat.gettext("Hello"), "Hola" );
	BOOST_CHECK_EQUAL( cat.gettext("World"), "Mundo" );
}

// ============================================================================
// Test 22: form_context_index static method
// ============================================================================
BOOST_AUTO_TEST_CASE( form_context_index )
{
	std::string index = spirit_po::default_catalog::form_context_index("menu", "Open");

	// The context index should contain EOT character (ASCII 4) between context and id
	BOOST_CHECK_EQUAL( index.length(), std::string("menu").length() + 1 + std::string("Open").length() );

	// Check that context and id are present
	BOOST_CHECK( index.find("menu") != std::string::npos );
	BOOST_CHECK( index.find("Open") != std::string::npos );
}

// ============================================================================
// Test 23: gettext_line_no
// ============================================================================
BOOST_AUTO_TEST_CASE( gettext_line_no )
{
	std::string po_content = create_minimal_po_content();
	auto it = po_content.begin();
	auto end = po_content.end();

	spirit_po::default_catalog cat = spirit_po::default_catalog::from_iterators(it, end);

	// Line number for existing messages should be > 0
	// For non-existing messages, should be 0
	std::size_t line_no = cat.gettext_line_no("NonExistent");
	BOOST_CHECK_EQUAL( line_no, 0 );
}

// ============================================================================
// Test 24: pgettext_line_no
// ============================================================================
BOOST_AUTO_TEST_CASE( pgettext_line_no )
{
	std::string po_content = create_po_with_context();
	auto it = po_content.begin();
	auto end = po_content.end();

	spirit_po::default_catalog cat = spirit_po::default_catalog::from_iterators(it, end);

	// For non-existing context+msgid combination, should be 0
	std::size_t line_no = cat.pgettext_line_no("nonexistent", "Open");
	BOOST_CHECK_EQUAL( line_no, 0 );
}

// ============================================================================
// Test 25: Complex PO file with multiple features
// ============================================================================
BOOST_AUTO_TEST_CASE( complex_po_file )
{
	std::string po_content = R"(
msgid ""
msgstr ""
"Project-Id-Version: Test\n"
"Language: de\n"
"Content-Type: text/plain; charset=UTF-8\n"
"Plural-Forms: nplurals=2; plural=n != 1;\n"

msgid "Hello, World!"
msgstr "Hallo, Welt!"

msgid "Goodbye"
msgstr "Auf Wiedersehen"

msgctxt "button"
msgid "Save"
msgstr "Speichern"

msgctxt "button"
msgid "Cancel"
msgstr "Abbrechen"

msgid "item"
msgid_plural "items"
msgstr[0] "Element"
msgstr[1] "Elemente"

msgctxt "inventory"
msgid "sword"
msgid_plural "swords"
msgstr[0] "Schwert"
msgstr[1] "Schwerter"
)";

	auto it = po_content.begin();
	auto end = po_content.end();

	spirit_po::default_catalog cat = spirit_po::default_catalog::from_iterators(it, end);

	// Test basic translations
	BOOST_CHECK_EQUAL( cat.gettext("Hello, World!"), "Hallo, Welt!" );
	BOOST_CHECK_EQUAL( cat.gettext("Goodbye"), "Auf Wiedersehen" );

	// Test context translations
	BOOST_CHECK_EQUAL( cat.pgettext("button", "Save"), "Speichern" );
	BOOST_CHECK_EQUAL( cat.pgettext("button", "Cancel"), "Abbrechen" );

	// Test plural translations
	BOOST_CHECK_EQUAL( cat.ngettext("item", "items", 1), "Element" );
	BOOST_CHECK_EQUAL( cat.ngettext("item", "items", 5), "Elemente" );

	// Test plural with context
	BOOST_CHECK_EQUAL( cat.npgettext("inventory", "sword", "swords", 1), "Schwert" );
	BOOST_CHECK_EQUAL( cat.npgettext("inventory", "sword", "swords", 3), "Schwerter" );

	// Test metadata
	BOOST_CHECK_EQUAL( cat.get_metadata().language, "de" );
	BOOST_CHECK_EQUAL( cat.size(), 6 );  // 2 simple + 2 context + 1 plural + 1 context plural
}

// ============================================================================
// Test 26: Fuzzy messages are ignored
// ============================================================================
BOOST_AUTO_TEST_CASE( fuzzy_messages_ignored )
{
	std::string po_content = R"(
msgid ""
msgstr ""
"Content-Type: text/plain; charset=UTF-8\n"
"Plural-Forms: nplurals=2; plural=n != 1;\n"

#, fuzzy
msgid "Fuzzy Message"
msgstr "This should be ignored"

msgid "Clear Message"
msgstr "This should be included"
)";

	auto it = po_content.begin();
	auto end = po_content.end();

	spirit_po::default_catalog cat = spirit_po::default_catalog::from_iterators(it, end);

	// Fuzzy messages should not be in catalog
	BOOST_CHECK_EQUAL( cat.gettext("Fuzzy Message"), "Fuzzy Message" );

	// Non-fuzzy messages should be translated
	BOOST_CHECK_EQUAL( cat.gettext("Clear Message"), "This should be included" );
}

// ============================================================================
// Test 27: Untranslated entries (empty msgstr) are ignored
// ============================================================================
BOOST_AUTO_TEST_CASE( untranslated_entries_ignored )
{
	std::string po_content = R"(
msgid ""
msgstr ""
"Content-Type: text/plain; charset=UTF-8\n"
"Plural-Forms: nplurals=2; plural=n != 1;\n"

msgid "Translated"
msgstr "Translated Value"

msgid "Untranslated"
msgstr ""
)";

	auto it = po_content.begin();
	auto end = po_content.end();

	spirit_po::default_catalog cat = spirit_po::default_catalog::from_iterators(it, end);

	// Translated entry should work
	BOOST_CHECK_EQUAL( cat.gettext("Translated"), "Translated Value" );

	// Untranslated entry should fall back to msgid
	BOOST_CHECK_EQUAL( cat.gettext("Untranslated"), "Untranslated" );
}

// ============================================================================
// Test 28: Multiline strings
// ============================================================================
BOOST_AUTO_TEST_CASE( multiline_strings )
{
	std::string po_content = R"(
msgid ""
msgstr ""
"Content-Type: text/plain; charset=UTF-8\n"
"Plural-Forms: nplurals=2; plural=n != 1;\n"

msgid "Multi-line"
" message"
msgstr "Multi-line"
" translation"
)";

	auto it = po_content.begin();
	auto end = po_content.end();

	spirit_po::default_catalog cat = spirit_po::default_catalog::from_iterators(it, end);

	BOOST_CHECK_EQUAL( cat.gettext("Multi-line message"), "Multi-line translation" );
}

// ============================================================================
// Test 29: Escaped characters
// ============================================================================
BOOST_AUTO_TEST_CASE( escaped_characters )
{
	std::string po_content = R"(
msgid ""
msgstr ""
"Content-Type: text/plain; charset=UTF-8\n"
"Plural-Forms: nplurals=2; plural=n != 1;\n"

msgid "Line1\nLine2"
msgstr "Zeile1\nZeile2"

msgid "Tab\there"
msgstr "Tab\thier"
)";

	auto it = po_content.begin();
	auto end = po_content.end();

	spirit_po::default_catalog cat = spirit_po::default_catalog::from_iterators(it, end);

	// Note: The raw string literal R"(...)" doesn't process \n as newline,
	// so the msgid is literally "Line1\nLine2" with backslash-n
	BOOST_CHECK_EQUAL( cat.gettext("Line1\nLine2"), "Zeile1\nZeile2" );
	BOOST_CHECK_EQUAL( cat.gettext("Tab\there"), "Tab\thier" );
}

// ============================================================================
// Test 30: catalog_metadata check_compatibility
// ============================================================================
BOOST_AUTO_TEST_CASE( metadata_compatibility )
{
	spirit_po::catalog_metadata meta1;
	meta1.num_plural_forms = 2;

	spirit_po::catalog_metadata meta2;
	meta2.num_plural_forms = 2;

	spirit_po::catalog_metadata meta3;
	meta3.num_plural_forms = 3;

	// Same number of plural forms should be compatible
	BOOST_CHECK( meta1.check_compatibility(meta2).empty() );

	// Different number of plural forms should not be compatible
	BOOST_CHECK( !meta1.check_compatibility(meta3).empty() );
}

BOOST_AUTO_TEST_SUITE_END()
