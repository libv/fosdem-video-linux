// SPDX-License-Identifier: GPL-2.0+

/*
 * Copyright (c) 2019 Luc Verhaegen <libv@skynet.be>
 *
 * EDID block for a mode close to standard 720, but adjusted so a tfp401
 * module can reliably capture it.
 */

/* EDID */
#define VERSION 1
#define REVISION 3

/* Display */
#define CLOCK 74500 /* kHz */
#define XPIX 1280
#define YPIX 720
#define XY_RATIO XY_RATIO_16_9
#define XBLANK 372
#define YBLANK 32
#define XOFFSET 110
#define XPULSE 40
#define YOFFSET 5
#define YPULSE 5
#define DPI 72
#define VFREQ 60 /* Hz */
#define TIMING_NAME "720p TFP401"
/* No ESTABLISHED_TIMINGx_BITS */
#define HSYNC_POL 1
#define VSYNC_POL 1

#include "edid.S"
