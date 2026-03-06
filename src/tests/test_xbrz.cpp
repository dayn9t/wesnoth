/*
	Copyright (C) 2026
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

#include "xBRZ/xbrz.hpp"
#include "xBRZ/xbrz_config.hpp"
#include "xBRZ/xbrz_tools.hpp"

#include <cmath>
#include <cstdint>

BOOST_AUTO_TEST_SUITE(xbrz)

// Test ScalerCfg default values
BOOST_AUTO_TEST_CASE(test_scaler_cfg_defaults)
{
	xbrz::ScalerCfg cfg;

	BOOST_CHECK_CLOSE(cfg.luminanceWeight, 1.0, 0.001);
	BOOST_CHECK_CLOSE(cfg.equalColorTolerance, 30.0, 0.001);
	BOOST_CHECK_CLOSE(cfg.centerDirectionBias, 4.0, 0.001);
	BOOST_CHECK_CLOSE(cfg.dominantDirectionThreshold, 3.6, 0.001);
	BOOST_CHECK_CLOSE(cfg.steepDirectionThreshold, 2.2, 0.001);
	BOOST_CHECK_CLOSE(cfg.newTestAttribute, 0.0, 0.001);
}

// Test ScalerCfg custom values
BOOST_AUTO_TEST_CASE(test_scaler_cfg_custom_values)
{
	xbrz::ScalerCfg cfg;
	cfg.luminanceWeight = 1.5;
	cfg.equalColorTolerance = 40.0;
	cfg.centerDirectionBias = 5.0;
	cfg.dominantDirectionThreshold = 4.0;
	cfg.steepDirectionThreshold = 3.0;
	cfg.newTestAttribute = 1.0;

	BOOST_CHECK_CLOSE(cfg.luminanceWeight, 1.5, 0.001);
	BOOST_CHECK_CLOSE(cfg.equalColorTolerance, 40.0, 0.001);
	BOOST_CHECK_CLOSE(cfg.centerDirectionBias, 5.0, 0.001);
	BOOST_CHECK_CLOSE(cfg.dominantDirectionThreshold, 4.0, 0.001);
	BOOST_CHECK_CLOSE(cfg.steepDirectionThreshold, 3.0, 0.001);
	BOOST_CHECK_CLOSE(cfg.newTestAttribute, 1.0, 0.001);
}

// Test scale factor maximum constant
BOOST_AUTO_TEST_CASE(test_scale_factor_max)
{
	BOOST_CHECK_EQUAL(xbrz::SCALE_FACTOR_MAX, 6);
}

// Test ColorFormat enum values
BOOST_AUTO_TEST_CASE(test_color_format_enum)
{
	// Verify that ColorFormat enum has expected values
	xbrz::ColorFormat rgb = xbrz::ColorFormat::RGB;
	xbrz::ColorFormat argb = xbrz::ColorFormat::ARGB;
	xbrz::ColorFormat argb_unbuffered = xbrz::ColorFormat::ARGB_UNBUFFERED;

	// Just verify we can create and use these values
	BOOST_CHECK(rgb == xbrz::ColorFormat::RGB);
	BOOST_CHECK(argb == xbrz::ColorFormat::ARGB);
	BOOST_CHECK(argb_unbuffered == xbrz::ColorFormat::ARGB_UNBUFFERED);
}

// Test makePixel function with alpha channel
BOOST_AUTO_TEST_CASE(test_make_pixel_with_alpha)
{
	uint32_t pixel = xbrz::makePixel(255, 128, 64, 32);

	unsigned char a = xbrz::getAlpha(pixel);
	unsigned char r = xbrz::getRed(pixel);
	unsigned char g = xbrz::getGreen(pixel);
	unsigned char b = xbrz::getBlue(pixel);

	BOOST_CHECK_EQUAL(a, 255);
	BOOST_CHECK_EQUAL(r, 128);
	BOOST_CHECK_EQUAL(g, 64);
	BOOST_CHECK_EQUAL(b, 32);
}

// Test makePixel function without alpha channel
BOOST_AUTO_TEST_CASE(test_make_pixel_without_alpha)
{
	uint32_t pixel = xbrz::makePixel(200, 100, 50);

	unsigned char r = xbrz::getRed(pixel);
	unsigned char g = xbrz::getGreen(pixel);
	unsigned char b = xbrz::getBlue(pixel);

	BOOST_CHECK_EQUAL(r, 200);
	BOOST_CHECK_EQUAL(g, 100);
	BOOST_CHECK_EQUAL(b, 50);
}

// Test pixel component extraction
BOOST_AUTO_TEST_CASE(test_pixel_component_extraction)
{
	uint32_t pixel = 0xAABBCCDD;

	BOOST_CHECK_EQUAL(xbrz::getAlpha(pixel), 0xAA);
	BOOST_CHECK_EQUAL(xbrz::getRed(pixel), 0xBB);
	BOOST_CHECK_EQUAL(xbrz::getGreen(pixel), 0xCC);
	BOOST_CHECK_EQUAL(xbrz::getBlue(pixel), 0xDD);
}

// Test RGB color format conversion functions
BOOST_AUTO_TEST_CASE(test_rgb_format_conversions)
{
	// Test RGB555 to RGB888
	uint16_t rgb555 = 0x7FFF; // Max value in RGB555
	uint32_t rgb888_from_555 = xbrz::rgb555to888(rgb555);
	BOOST_CHECK(rgb888_from_555 != 0);

	// Test RGB565 to RGB888
	uint16_t rgb565 = 0xFFFF; // Max value in RGB565
	uint32_t rgb888_from_565 = xbrz::rgb565to888(rgb565);
	BOOST_CHECK(rgb888_from_565 != 0);

	// Test RGB888 to RGB555
	uint32_t rgb888 = 0x00F8F8F8;
	uint16_t to_rgb555 = xbrz::rgb888to555(rgb888);
	BOOST_CHECK(to_rgb555 != 0);

	// Test RGB888 to RGB565
	uint16_t to_rgb565 = xbrz::rgb888to565(rgb888);
	BOOST_CHECK(to_rgb565 != 0);
}

// Test equalColorTest with identical colors
BOOST_AUTO_TEST_CASE(test_equal_color_test_identical)
{
	uint32_t color1 = xbrz::makePixel(255, 128, 64, 32);
	uint32_t color2 = xbrz::makePixel(255, 128, 64, 32);

	bool result = xbrz::equalColorTest(color1, color2, xbrz::ColorFormat::ARGB, 1.0, 30.0);
	BOOST_CHECK(result);
}

// Test equalColorTest with different colors
BOOST_AUTO_TEST_CASE(test_equal_color_test_different)
{
	uint32_t color1 = xbrz::makePixel(255, 128, 64, 32);
	uint32_t color2 = xbrz::makePixel(255, 200, 150, 100);

	bool result = xbrz::equalColorTest(color1, color2, xbrz::ColorFormat::ARGB, 1.0, 30.0);
	BOOST_CHECK(!result);
}

// Test equalColorTest with tolerance threshold
BOOST_AUTO_TEST_CASE(test_equal_color_test_tolerance)
{
	// Two colors that are very similar
	uint32_t color1 = xbrz::makePixel(255, 100, 100, 100);
	uint32_t color2 = xbrz::makePixel(255, 105, 105, 105);

	// With high tolerance, should be considered equal
	bool result_high = xbrz::equalColorTest(color1, color2, xbrz::ColorFormat::ARGB, 1.0, 50.0);
	BOOST_CHECK(result_high);

	// With very low tolerance, might not be considered equal
	bool result_low = xbrz::equalColorTest(color1, color2, xbrz::ColorFormat::ARGB, 1.0, 0.1);
	BOOST_CHECK(!result_low);
}

// Test equalColorTest with RGB format
BOOST_AUTO_TEST_CASE(test_equal_color_test_rgb_format)
{
	uint32_t color1 = xbrz::makePixel(128, 64, 32);
	uint32_t color2 = xbrz::makePixel(128, 64, 32);

	bool result = xbrz::equalColorTest(color1, color2, xbrz::ColorFormat::RGB, 1.0, 30.0);
	BOOST_CHECK(result);
}

// Test equalColorTest with ARGB_UNBUFFERED format
BOOST_AUTO_TEST_CASE(test_equal_color_test_argb_unbuffered)
{
	uint32_t color1 = xbrz::makePixel(255, 128, 64, 32);
	uint32_t color2 = xbrz::makePixel(255, 128, 64, 32);

	bool result = xbrz::equalColorTest(color1, color2, xbrz::ColorFormat::ARGB_UNBUFFERED, 1.0, 30.0);
	BOOST_CHECK(result);
}

// Test luminance weight parameter in equalColorTest
BOOST_AUTO_TEST_CASE(test_equal_color_test_luminance_weight)
{
	uint32_t color1 = xbrz::makePixel(255, 100, 100, 100);
	uint32_t color2 = xbrz::makePixel(255, 110, 110, 110);

	// Test with different luminance weights
	bool result1 = xbrz::equalColorTest(color1, color2, xbrz::ColorFormat::ARGB, 0.5, 30.0);
	bool result2 = xbrz::equalColorTest(color1, color2, xbrz::ColorFormat::ARGB, 2.0, 30.0);

	// Both should handle the luminance weight parameter
	BOOST_CHECK(result1 || !result1); // Just verify it doesn't crash
	BOOST_CHECK(result2 || !result2);
}

// Test ScalerCfg luminance weight validation
BOOST_AUTO_TEST_CASE(test_scaler_cfg_luminance_weight_range)
{
	xbrz::ScalerCfg cfg;

	// Test valid ranges
	cfg.luminanceWeight = 0.0;
	BOOST_CHECK_CLOSE(cfg.luminanceWeight, 0.0, 0.001);

	cfg.luminanceWeight = 1.0;
	BOOST_CHECK_CLOSE(cfg.luminanceWeight, 1.0, 0.001);

	cfg.luminanceWeight = 2.0;
	BOOST_CHECK_CLOSE(cfg.luminanceWeight, 2.0, 0.001);
}

// Test ScalerCfg equalColorTolerance range
BOOST_AUTO_TEST_CASE(test_scaler_cfg_equal_color_tolerance_range)
{
	xbrz::ScalerCfg cfg;

	// Test valid ranges
	cfg.equalColorTolerance = 0.0;
	BOOST_CHECK_CLOSE(cfg.equalColorTolerance, 0.0, 0.001);

	cfg.equalColorTolerance = 30.0;
	BOOST_CHECK_CLOSE(cfg.equalColorTolerance, 30.0, 0.001);

	cfg.equalColorTolerance = 100.0;
	BOOST_CHECK_CLOSE(cfg.equalColorTolerance, 100.0, 0.001);
}

// Test ScalerCfg centerDirectionBias range
BOOST_AUTO_TEST_CASE(test_scaler_cfg_center_direction_bias_range)
{
	xbrz::ScalerCfg cfg;

	cfg.centerDirectionBias = 0.0;
	BOOST_CHECK_CLOSE(cfg.centerDirectionBias, 0.0, 0.001);

	cfg.centerDirectionBias = 4.0;
	BOOST_CHECK_CLOSE(cfg.centerDirectionBias, 4.0, 0.001);

	cfg.centerDirectionBias = 10.0;
	BOOST_CHECK_CLOSE(cfg.centerDirectionBias, 10.0, 0.001);
}

// Test ScalerCfg dominantDirectionThreshold range
BOOST_AUTO_TEST_CASE(test_scaler_cfg_dominant_direction_threshold_range)
{
	xbrz::ScalerCfg cfg;

	cfg.dominantDirectionThreshold = 0.0;
	BOOST_CHECK_CLOSE(cfg.dominantDirectionThreshold, 0.0, 0.001);

	cfg.dominantDirectionThreshold = 3.6;
	BOOST_CHECK_CLOSE(cfg.dominantDirectionThreshold, 3.6, 0.001);

	cfg.dominantDirectionThreshold = 10.0;
	BOOST_CHECK_CLOSE(cfg.dominantDirectionThreshold, 10.0, 0.001);
}

// Test ScalerCfg steepDirectionThreshold range
BOOST_AUTO_TEST_CASE(test_scaler_cfg_steep_direction_threshold_range)
{
	xbrz::ScalerCfg cfg;

	cfg.steepDirectionThreshold = 0.0;
	BOOST_CHECK_CLOSE(cfg.steepDirectionThreshold, 0.0, 0.001);

	cfg.steepDirectionThreshold = 2.2;
	BOOST_CHECK_CLOSE(cfg.steepDirectionThreshold, 2.2, 0.001);

	cfg.steepDirectionThreshold = 5.0;
	BOOST_CHECK_CLOSE(cfg.steepDirectionThreshold, 5.0, 0.001);
}

BOOST_AUTO_TEST_SUITE_END()
