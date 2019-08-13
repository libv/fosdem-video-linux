// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (c) 2019 Luc Verhaegen <libv@skynet.be>
 */
#include <linux/regmap.h>

#include <drm/drmP.h>
#include <drm/drm_plane_helper.h>
#include <drm/drm_atomic_helper.h>
#include <drm/drm_atomic_state_helper.h>
#include <drm/drm_gem_framebuffer_helper.h>

#include "sunxi_engine.h"
#include "sun4i_backend_regs.h"
#include "sun4i_backend.h"
#include "sun4i_sprite.h"

#define SUN4I_BE_SPRITE_COUNT 32

struct sun4i_sprite {
	struct drm_plane plane;
	struct sun4i_backend *backend;
	struct drm_device *drm;

	int id;
};

static inline struct sun4i_sprite *
sun4i_sprite_from_drm_plane(struct drm_plane *plane)
{
	return container_of(plane, struct sun4i_sprite, plane);
}

static int
sun4i_sprite_atomic_check(struct drm_plane *plane,
			  struct drm_plane_state *state)
{
	struct sun4i_sprite *sprite = sun4i_sprite_from_drm_plane(plane);

	DRM_DEBUG_DRIVER("%s(%d);\n", __func__, sprite->id);

	/* verify that we are not scaling */
	if (((state->crtc_w << 16) != state->src_w) ||
	    ((state->crtc_h << 16) != state->src_h)) {
		DRM_ERROR("%s(%d.%d): scaling is not allowed.\n",
			  __func__, sprite->backend->engine.id, sprite->id);
		return -EINVAL;
	}

	/* todo: pluck the physical limits from my 2013 code */

	return 0;
}

static void
sun4i_sprite_atomic_update(struct drm_plane *plane,
			   struct drm_plane_state *plane_state_old)
{
	struct sun4i_sprite *sprite = sun4i_sprite_from_drm_plane(plane);

	DRM_DEBUG_DRIVER("%s(%d);\n", __func__, sprite->id);
}

static void
sun4i_sprite_atomic_disable(struct drm_plane *plane,
			    struct drm_plane_state *old_state)
{
	struct sun4i_sprite *sprite = sun4i_sprite_from_drm_plane(plane);

	DRM_DEBUG_DRIVER("%s(%d);\n", __func__, sprite->id);
}

static const struct drm_plane_helper_funcs sun4i_sprite_helper_funcs = {
	.prepare_fb = drm_gem_fb_prepare_fb,
	.atomic_check = sun4i_sprite_atomic_check,
	.atomic_disable = sun4i_sprite_atomic_disable,
	.atomic_update = sun4i_sprite_atomic_update,
};

static const struct drm_plane_funcs sun4i_sprite_drm_plane_funcs = {
	.atomic_destroy_state = drm_atomic_helper_plane_destroy_state,
	.atomic_duplicate_state	= drm_atomic_helper_plane_duplicate_state,

	.destroy = drm_plane_cleanup,

	.disable_plane = drm_atomic_helper_disable_plane,
	.reset = drm_atomic_helper_plane_reset,
	.update_plane = drm_atomic_helper_update_plane,
};

/*
 * Lock down format for now, to ease multi-sprite logic, as this one
 * format must fit all.
 */
static const uint32_t sun4i_sprite_drm_formats[] = {
	DRM_FORMAT_ARGB8888,
	//DRM_FORMAT_BGRA8888,
};

struct drm_plane *
sun4i_sprite_plane_init(struct drm_device *drm,
			struct sun4i_backend *backend, int id,
			int zpos_start)
{
	struct sun4i_sprite *sprite;
	int zpos_end = zpos_start + SUN4I_BE_SPRITE_COUNT - 1;
	int ret;

	if (id >= SUN4I_BE_SPRITE_COUNT)
		return ERR_PTR(-ENODEV);

	sprite = devm_kzalloc(drm->dev, sizeof(struct sun4i_sprite),
			      GFP_KERNEL);
	if (!sprite)
		return ERR_PTR(-ENOMEM);

	sprite->backend = backend;
	sprite->drm = drm;
	sprite->id = id;

	ret = drm_universal_plane_init(drm, &sprite->plane, 0,
				       &sun4i_sprite_drm_plane_funcs,
				       sun4i_sprite_drm_formats,
				       ARRAY_SIZE(sun4i_sprite_drm_formats),
				       NULL, DRM_PLANE_TYPE_OVERLAY,
				       "Sprite-%d", id);
	if (ret) {
		DRM_DEV_ERROR(drm->dev, "%s(%d,%d): drm_universal_plane_init()"
			      " failed\n", __func__, backend->engine.id, id);
		return ERR_PTR(ret);
	}

	drm_plane_helper_add(&sprite->plane, &sun4i_sprite_helper_funcs);

	/* disable global alpha for now */
	//drm_plane_create_alpha_property(&sprite->plane);
	drm_plane_create_zpos_property(&sprite->plane, zpos_start + id,
				       zpos_start, zpos_end);

	return &sprite->plane;
}
