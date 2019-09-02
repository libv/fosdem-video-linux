// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (c) 2019 Luc Verhaegen <libv@skynet.be>
 */

#ifndef _SUN4I_SPRITE_H_
#define _SUN4I_SPRITE_H_ 1

struct drm_plane *sun4i_sprite_plane_init(struct drm_device *drm,
					  struct sun4i_backend *backend,
					  int id, int zpos_start);

int sun4i_sprites_crtc_atomic_check(struct sunxi_engine *engine,
				    struct drm_crtc_state *crtc_state);

void sun4i_sprites_crtc_commit(struct drm_crtc *drm_crtc,
			       struct drm_crtc_state *state_old);

#endif /* _SUN4I_SPRITE_H_ */
