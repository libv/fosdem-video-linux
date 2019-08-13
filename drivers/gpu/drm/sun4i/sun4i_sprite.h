// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (c) 2019 Luc Verhaegen <libv@skynet.be>
 */

#ifndef _SUN4I_SPRITE_H_
#define _SUN4I_SPRITE_H_ 1

struct drm_plane *sun4i_sprite_plane_init(struct drm_device *drm,
					  struct sun4i_backend *backend,
					  int id, int zpos_start);

#endif /* _SUN4I_SPRITE_H_ */
