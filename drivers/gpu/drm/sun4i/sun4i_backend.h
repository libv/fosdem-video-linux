/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2015 Free Electrons
 * Copyright (C) 2015 NextThing Co
 *
 * Maxime Ripard <maxime.ripard@free-electrons.com>
 */

#ifndef _SUN4I_BACKEND_H_
#define _SUN4I_BACKEND_H_

#include <linux/clk.h>
#include <linux/list.h>
#include <linux/of.h>
#include <linux/regmap.h>
#include <linux/reset.h>

#include "sunxi_engine.h"

#define SUN4I_BACKEND_NUM_LAYERS		4
#define SUN4I_BACKEND_NUM_FRONTEND_LAYERS	1
#define SUN4I_BACKEND_NUM_YUV_PLANES		1

struct sun4i_backend {
	struct sunxi_engine	engine;
	struct sun4i_frontend	*frontend;

	struct reset_control	*reset;

	struct clk		*bus_clk;
	struct clk		*mod_clk;
	struct clk		*ram_clk;

	struct clk		*sat_clk;
	struct reset_control	*sat_reset;

	/* Protects against races in the frontend teardown */
	spinlock_t		frontend_lock;
	bool			frontend_teardown;

	const struct sun4i_backend_quirks	*quirks;
};

static inline struct sun4i_backend *
engine_to_sun4i_backend(struct sunxi_engine *engine)
{
	return container_of(engine, struct sun4i_backend, engine);
}

void sun4i_backend_layer_enable(struct sun4i_backend *backend,
				int layer, bool enable);
bool sun4i_backend_format_is_supported(uint32_t fmt, uint64_t modifier);
int sun4i_backend_update_layer_coord(struct sun4i_backend *backend,
				     int layer, struct drm_plane *plane);
void sun4i_backend_update_layer_formats(struct sun4i_backend *backend,
					int layer, struct drm_plane *plane);
void sun4i_backend_update_layer_buffer(struct sun4i_backend *backend,
				       int layer, struct drm_plane *plane);
void sun4i_backend_frontend_set(struct sun4i_backend *backend,
				int layer, uint32_t format);
int sun4i_backend_update_layer_zpos(struct sun4i_backend *backend,
				    int layer, struct drm_plane *plane);
void sun4i_backend_update_layer_alpha(struct sun4i_backend *backend,
				      int layer, struct drm_plane *plane);
void sun4i_backend_cleanup_layer(struct sun4i_backend *backend,
				 int layer);

#endif /* _SUN4I_BACKEND_H_ */
