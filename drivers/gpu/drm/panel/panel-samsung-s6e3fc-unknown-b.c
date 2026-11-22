// SPDX-License-Identifier: GPL-2.0-only
/*
 * Xiaomi K7 38 0c 0b AMOLED panel.
 * Based on panel-samsung-s6e3fc-unknown.c by SCOS100.
 * Commands and timings from dsi-panel-k7-38-0c-0b-fhdp-video.dtsi.
 */

#include <linux/backlight.h>
#include <linux/delay.h>
#include <linux/gpio/consumer.h>
#include <linux/mod_devicetable.h>
#include <linux/module.h>
#include <linux/regulator/consumer.h>

#include <drm/display/drm_dsc.h>
#include <drm/display/drm_dsc_helper.h>
#include <drm/drm_mipi_dsi.h>
#include <drm/drm_modes.h>
#include <drm/drm_panel.h>
#include <drm/drm_probe_helper.h>

struct xiaomi_38_0c_b {
	struct drm_panel panel;
	struct mipi_dsi_device *dsi;
	struct drm_dsc_config dsc;
	struct regulator *supply;
	struct gpio_desc *reset_gpio;
	struct gpio_desc *en_gpio;
};

static struct xiaomi_38_0c_b *to_xiaomi_38_0c_b(struct drm_panel *panel)
{
	return container_of(panel, struct xiaomi_38_0c_b, panel);
}

static void xiaomi_38_0c_b_reset(struct xiaomi_38_0c_b *ctx)
{
	gpiod_set_value_cansleep(ctx->reset_gpio, 0);
	usleep_range(10000, 11000);
	gpiod_set_value_cansleep(ctx->reset_gpio, 1);
	usleep_range(10000, 11000);
	gpiod_set_value_cansleep(ctx->reset_gpio, 0);
	usleep_range(10000, 11000);
}

static int xiaomi_38_0c_b_on(struct xiaomi_38_0c_b *ctx)
{
	struct mipi_dsi_multi_context dsi_ctx = { .dsi = ctx->dsi };

	mipi_dsi_dcs_exit_sleep_mode_multi(&dsi_ctx);
	mipi_dsi_msleep(&dsi_ctx, 80);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xf0, 0x5a, 0x5a);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xb2, 0x01, 0x31);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xf0, 0xa5, 0xa5);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xf0, 0x5a, 0x5a);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xfc, 0x5a, 0x5a);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xe1, 0x8b);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xed, 0x00, 0x01);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xb0, 0x00, 0x06, 0xf4);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xf4, 0x1f);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xf0, 0xa5, 0xa5);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xfc, 0xa5, 0xa5);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xf0, 0x5a, 0x5a);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xfc, 0x5a, 0x5a);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xb0, 0x00, 0x11, 0xfe);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xfe, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xf0, 0xa5, 0xa5);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xfc, 0xa5, 0xa5);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x9d, 0x01);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x9e, 0x11, 0x00, 0x00, 0x89,
				     0x30, 0x80, 0x09, 0x60, 0x04, 0x38, 0x00,
				     0x28, 0x02, 0x1c, 0x02, 0x1c, 0x02, 0x00,
				     0x02, 0x0e, 0x00, 0x20, 0x03, 0xdd, 0x00,
				     0x07, 0x00, 0x0c, 0x02, 0x77, 0x02, 0x8b,
				     0x18, 0x00, 0x10, 0xf0, 0x03, 0x0c, 0x20,
				     0x00, 0x06, 0x0b, 0x0b, 0x33, 0x0e, 0x1c,
				     0x2a, 0x38, 0x46, 0x54, 0x62, 0x69, 0x70,
				     0x77, 0x79, 0x7b, 0x7d, 0x7e, 0x01, 0x02,
				     0x01, 0x00, 0x09, 0x40, 0x09, 0xbe, 0x19,
				     0xfc, 0x19, 0xfa, 0x19, 0xf8, 0x1a, 0x38,
				     0x1a, 0x78, 0x1a, 0xb6, 0x2a, 0xf6, 0x2b,
				     0x34, 0x2b, 0x74, 0x3b, 0x74, 0x6b, 0xf4,
				     0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xf0, 0x5a, 0x5a);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x60, 0x21);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xf7, 0x0b);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xf0, 0xa5, 0xa5);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xf0, 0x5a, 0x5a);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xb0, 0x00, 0x0d, 0xb2);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xb2, 0x20);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xb0, 0x00, 0x0c, 0xb2);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xb2, 0x30);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xf0, 0xa5, 0xa5);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x53, 0x28);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xf7, 0x0b);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xf0, 0x5a, 0x5a);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xcd, 0x29, 0x00, 0x08);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xf0, 0xa5, 0xa5);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xf0, 0x5a, 0x5a);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xfc, 0x5a, 0x5a);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xfe, 0xb0);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xfe, 0x30);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xf7, 0x0b);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xfc, 0xa5, 0xa5);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xf0, 0xa5, 0xa5);
	mipi_dsi_msleep(&dsi_ctx, 100);

	return dsi_ctx.accum_err;
}

static int xiaomi_38_0c_b_prepare(struct drm_panel *panel)
{
	struct xiaomi_38_0c_b *ctx = to_xiaomi_38_0c_b(panel);
	struct drm_dsc_picture_parameter_set pps;
	int ret;

	if (ctx->en_gpio) {
		gpiod_set_value_cansleep(ctx->en_gpio, 1);
		usleep_range(10000, 11000);
	}

	ret = regulator_enable(ctx->supply);
	if (ret)
		goto disable_enable;

	xiaomi_38_0c_b_reset(ctx);
	ret = xiaomi_38_0c_b_on(ctx);
	if (ret)
		goto disable_supply;

	drm_dsc_pps_payload_pack(&pps, &ctx->dsc);
	ret = mipi_dsi_picture_parameter_set(ctx->dsi, &pps);
	if (ret)
		goto disable_supply;

	ret = mipi_dsi_compression_mode(ctx->dsi, true);
	if (ret)
		goto disable_supply;

	return 0;

disable_supply:
	gpiod_set_value_cansleep(ctx->reset_gpio, 1);
	regulator_disable(ctx->supply);
disable_enable:
	gpiod_set_value_cansleep(ctx->en_gpio, 0);
	return ret;
}

static int xiaomi_38_0c_b_enable(struct drm_panel *panel)
{
	struct xiaomi_38_0c_b *ctx = to_xiaomi_38_0c_b(panel);
	struct mipi_dsi_multi_context dsi_ctx = { .dsi = ctx->dsi };

	mipi_dsi_dcs_set_display_on_multi(&dsi_ctx);
	mipi_dsi_msleep(&dsi_ctx, 30);

	return dsi_ctx.accum_err;
}

static int xiaomi_38_0c_b_disable(struct drm_panel *panel)
{
	struct xiaomi_38_0c_b *ctx = to_xiaomi_38_0c_b(panel);
	struct mipi_dsi_multi_context dsi_ctx = { .dsi = ctx->dsi };

	mipi_dsi_dcs_set_display_off_multi(&dsi_ctx);
	mipi_dsi_msleep(&dsi_ctx, 10);

	return dsi_ctx.accum_err;
}

static int xiaomi_38_0c_b_unprepare(struct drm_panel *panel)
{
	struct xiaomi_38_0c_b *ctx = to_xiaomi_38_0c_b(panel);
	struct mipi_dsi_multi_context dsi_ctx = { .dsi = ctx->dsi };
	int ret;

	mipi_dsi_dcs_enter_sleep_mode_multi(&dsi_ctx);
	mipi_dsi_msleep(&dsi_ctx, 150);
	gpiod_set_value_cansleep(ctx->reset_gpio, 1);
	ret = regulator_disable(ctx->supply);
	gpiod_set_value_cansleep(ctx->en_gpio, 0);

	return ret ? ret : dsi_ctx.accum_err;
}

static const struct drm_display_mode xiaomi_38_0c_b_mode = {
	.clock = (1080 + 320 + 32 + 120) * (2400 + 20 + 2 + 10) * 90 / 1000,
	.hdisplay = 1080,
	.hsync_start = 1080 + 320,
	.hsync_end = 1080 + 320 + 32,
	.htotal = 1080 + 320 + 32 + 120,
	.vdisplay = 2400,
	.vsync_start = 2400 + 20,
	.vsync_end = 2400 + 20 + 2,
	.vtotal = 2400 + 20 + 2 + 10,
	.width_mm = 67,
	.height_mm = 149,
	.type = DRM_MODE_TYPE_DRIVER,
};

static int xiaomi_38_0c_b_get_modes(struct drm_panel *panel,
				    struct drm_connector *connector)
{
	return drm_connector_helper_get_modes_fixed(connector,
						    &xiaomi_38_0c_b_mode);
}

static const struct drm_panel_funcs xiaomi_38_0c_b_panel_funcs = {
	.prepare = xiaomi_38_0c_b_prepare,
	.enable = xiaomi_38_0c_b_enable,
	.disable = xiaomi_38_0c_b_disable,
	.unprepare = xiaomi_38_0c_b_unprepare,
	.get_modes = xiaomi_38_0c_b_get_modes,
};

static int xiaomi_38_0c_b_bl_update_status(struct backlight_device *bl)
{
	struct xiaomi_38_0c_b *ctx = bl_get_data(bl);
	struct mipi_dsi_device *dsi = ctx->dsi;
	unsigned long flags = dsi->mode_flags;
	int ret;

	if (!ctx->panel.prepared)
		return 0;

	dsi->mode_flags &= ~MIPI_DSI_MODE_LPM;
	ret = mipi_dsi_dcs_set_display_brightness_large(dsi,
							backlight_get_brightness(bl));
	dsi->mode_flags = flags;

	return ret;
}

static const struct backlight_ops xiaomi_38_0c_b_bl_ops = {
	.update_status = xiaomi_38_0c_b_bl_update_status,
};

static struct backlight_device *
xiaomi_38_0c_b_create_backlight(struct xiaomi_38_0c_b *ctx)
{
	struct device *dev = &ctx->dsi->dev;
	const struct backlight_properties props = {
		.type = BACKLIGHT_RAW,
		.brightness = 2047,
		.max_brightness = 2047,
	};

	return devm_backlight_device_register(dev, dev_name(dev), dev, ctx,
					      &xiaomi_38_0c_b_bl_ops, &props);
}

static int xiaomi_38_0c_b_probe(struct mipi_dsi_device *dsi)
{
	struct device *dev = &dsi->dev;
	struct xiaomi_38_0c_b *ctx;
	int ret;

	ctx = devm_drm_panel_alloc(dev, struct xiaomi_38_0c_b, panel,
				   &xiaomi_38_0c_b_panel_funcs,
				   DRM_MODE_CONNECTOR_DSI);
	if (IS_ERR(ctx))
		return PTR_ERR(ctx);

	ctx->supply = devm_regulator_get(dev, "vddio");
	if (IS_ERR(ctx->supply))
		return dev_err_probe(dev, PTR_ERR(ctx->supply),
				     "Failed to get vddio regulator\n");

	ctx->reset_gpio = devm_gpiod_get(dev, "reset", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->reset_gpio))
		return dev_err_probe(dev, PTR_ERR(ctx->reset_gpio),
				     "Failed to get reset GPIO\n");

	ctx->en_gpio = devm_gpiod_get_optional(dev, "enable", GPIOD_OUT_LOW);
	if (IS_ERR(ctx->en_gpio))
		return dev_err_probe(dev, PTR_ERR(ctx->en_gpio),
				     "Failed to get enable GPIO\n");

	ctx->dsi = dsi;
	mipi_dsi_set_drvdata(dsi, ctx);
	dsi->lanes = 4;
	dsi->format = MIPI_DSI_FMT_RGB888;
	dsi->mode_flags = MIPI_DSI_MODE_VIDEO | MIPI_DSI_MODE_VIDEO_BURST |
			  MIPI_DSI_CLOCK_NON_CONTINUOUS | MIPI_DSI_MODE_LPM;
	dsi->dsc = &ctx->dsc;
	ctx->dsc.dsc_version_major = 1;
	ctx->dsc.dsc_version_minor = 1;
	ctx->dsc.slice_height = 20;
	ctx->dsc.slice_width = 540;
	ctx->dsc.slice_count = 2;
	ctx->dsc.bits_per_component = 8;
	ctx->dsc.bits_per_pixel = 8 << 4;
	ctx->dsc.block_pred_enable = true;

	ctx->panel.prepare_prev_first = true;
	ctx->panel.backlight = xiaomi_38_0c_b_create_backlight(ctx);
	if (IS_ERR(ctx->panel.backlight))
		return dev_err_probe(dev, PTR_ERR(ctx->panel.backlight),
				     "Failed to create backlight\n");

	drm_panel_add(&ctx->panel);
	ret = mipi_dsi_attach(dsi);
	if (ret) {
		drm_panel_remove(&ctx->panel);
		return dev_err_probe(dev, ret, "Failed to attach to DSI host\n");
	}

	return 0;
}

static void xiaomi_38_0c_b_remove(struct mipi_dsi_device *dsi)
{
	struct xiaomi_38_0c_b *ctx = mipi_dsi_get_drvdata(dsi);
	int ret;

	ret = mipi_dsi_detach(dsi);
	if (ret)
		dev_err(&dsi->dev, "Failed to detach from DSI host: %d\n", ret);

	drm_panel_remove(&ctx->panel);
}

static const struct of_device_id xiaomi_38_0c_b_of_match[] = {
	{ .compatible = "xiaomi,k7-38-0c-0b" },
	{ }
};
MODULE_DEVICE_TABLE(of, xiaomi_38_0c_b_of_match);

static struct mipi_dsi_driver xiaomi_38_0c_b_driver = {
	.probe = xiaomi_38_0c_b_probe,
	.remove = xiaomi_38_0c_b_remove,
	.driver = {
		.name = "panel-xiaomi-k7-38-0c-0b",
		.of_match_table = xiaomi_38_0c_b_of_match,
	},
};
module_mipi_dsi_driver(xiaomi_38_0c_b_driver);

MODULE_DESCRIPTION("DRM driver for Xiaomi K7 38 0c 0b AMOLED panel");
MODULE_LICENSE("GPL");
