/*
	Copyright (c) 2022 ByteBit/xtreme8000

	This file is part of CavEX.

	CavEX is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	CavEX is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with CavEX.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <gccore.h>
#include <string.h>

#include "gfx.h"
#include "gui_util.h"

int gutil_control_icon(int x, enum gutil_control_icon icon, char* str) {
	gfx_bind_texture(TEXTURE_GUI);
	int scale = 32;
	int text_scale = 10;

	gutil_texquad(x, gfx_height() - scale * 8 / 5, (448 + (icon % 2) * 32) / 2,
				  (icon / 2) * 32, 32 / 2, 32, scale, scale);
	gutil_text(x + scale + text_scale / 2,
			   gfx_height() - scale * 8 / 5 + (scale - text_scale) / 2, str,
			   text_scale);
	return scale + text_scale + gutil_font_width(str, text_scale);
}

void gutil_texquad_col(int x, int y, int tx, int ty, int sx, int sy, int width,
					   int height, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	GX_Begin(GX_QUADS, GX_VTXFMT2, 4);
	GX_Position3s16(x, y, -2);
	GX_Color4u8(r, g, b, a);
	GX_TexCoord2u16(tx, ty);
	GX_Position3s16(x + width, y, -2);
	GX_Color4u8(r, g, b, a);
	GX_TexCoord2u16(tx + sx, ty);
	GX_Position3s16(x + width, y + height, -2);
	GX_Color4u8(r, g, b, a);
	GX_TexCoord2u16(tx + sx, ty + sy);
	GX_Position3s16(x, y + height, -2);
	GX_Color4u8(r, g, b, a);
	GX_TexCoord2u16(tx, ty + sy);
	GX_End();
}

void gutil_texquad(int x, int y, int tx, int ty, int sx, int sy, int width,
				   int height) {
	gutil_texquad_col(x, y, tx, ty, sx, sy, width, height, 0xFF, 0xFF, 0xFF,
					  0xFF);
}

void gutil_texquad_rt(int x, int y, int tx, int ty, int sx, int sy, int width,
					  int height) {
	GX_Begin(GX_QUADS, GX_VTXFMT2, 4);
	GX_Position3s16(x, y, -2);
	GX_Color4u8(0xFF, 0xFF, 0xFF, 0xFF);
	GX_TexCoord2u16(tx, ty + sy);
	GX_Position3s16(x + width, y, -2);
	GX_Color4u8(0xFF, 0xFF, 0xFF, 0xFF);
	GX_TexCoord2u16(tx, ty);
	GX_Position3s16(x + width, y + height, -2);
	GX_Color4u8(0xFF, 0xFF, 0xFF, 0xFF);
	GX_TexCoord2u16(tx + sx, ty);
	GX_Position3s16(x, y + height, -2);
	GX_Color4u8(0xFF, 0xFF, 0xFF, 0xFF);
	GX_TexCoord2u16(tx + sx, ty + sy);
	GX_End();
}

void gutil_bg() {
	gfx_bind_texture(TEXTURE_TERRAIN);

	int scale = 16 * 4;
	int cx = (gfx_width() + scale - 1) / scale;
	int cy = (gfx_height() + scale - 1) / scale;

	GX_Begin(GX_QUADS, GX_VTXFMT2, 4 * cx * cy);
	for(int y = 0; y < cy; y++) {
		for(int x = 0; x < cx; x++) {
			GX_Position3s16(x * scale, y * scale, -9);
			GX_Color4u8(0x40, 0x40, 0x40, 0xFF);
			GX_TexCoord2u16(39, 3);
			GX_Position3s16(x * scale + scale, y * scale, -9);
			GX_Color4u8(0x40, 0x40, 0x40, 0xFF);
			GX_TexCoord2u16(55, 3);
			GX_Position3s16(x * scale + scale, y * scale + scale, -9);
			GX_Color4u8(0x40, 0x40, 0x40, 0xFF);
			GX_TexCoord2u16(55, 19);
			GX_Position3s16(x * scale, y * scale + scale, -9);
			GX_Color4u8(0x40, 0x40, 0x40, 0xFF);
			GX_TexCoord2u16(39, 19);
		}
	}
	GX_End();
}

static const uint8_t font_char_width[256] = {
	8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
	8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4, 1, 4, 5, 5, 5, 5, 2, 4, 4, 4, 5,
	1, 5, 1, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 1, 1, 4, 5, 4, 5, 6, 5,
	5, 5, 5, 5, 5, 5, 5, 3, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 5, 3, 5, 3, 5, 5, 2, 5, 5, 5, 5, 5, 4, 5, 5, 1, 5, 4, 2, 5,
	5, 5, 5, 5, 5, 5, 4, 5, 5, 5, 5, 5, 5, 4, 1, 4, 6, 5,
};

int gutil_font_width(char* str, int scale) {
	int x = 0;

	while(*str) {
		x += (font_char_width[(int)*str] + 1) * scale / 8;
		str++;
	}

	return x;
}

void gutil_text(int x, int y, char* str, int scale) {
	gfx_bind_texture(TEXTURE_FONT);
	GX_Begin(GX_QUADS, GX_VTXFMT1, strlen(str) * 4 * 2);

	while(*str) {
		uint8_t tex_x = *str % 16 * 8;
		uint8_t tex_y = *str / 16 * 8;
		uint8_t width = (font_char_width[(int)*str] + 1) * scale / 8;

		GX_Position3s16(x + scale / 8, y + scale / 8, -2);
		GX_Color4u8(0x80, 0x80, 0x80, 0xFF);
		GX_TexCoord2u8(tex_x, tex_y);
		GX_Position3s16(x + scale + scale / 8, y + scale / 8, -2);
		GX_Color4u8(0x80, 0x80, 0x80, 0xFF);
		GX_TexCoord2u8(tex_x + 8, tex_y);
		GX_Position3s16(x + scale + scale / 8, y + scale + scale / 8, -2);
		GX_Color4u8(0x80, 0x80, 0x80, 0xFF);
		GX_TexCoord2u8(tex_x + 8, tex_y + 8);
		GX_Position3s16(x + scale / 8, y + scale + scale / 8, -2);
		GX_Color4u8(0x80, 0x80, 0x80, 0xFF);
		GX_TexCoord2u8(tex_x, tex_y + 8);

		GX_Position3s16(x, y, -1);
		GX_Color4u8(0xFF, 0xFF, 0xFF, 0xFF);
		GX_TexCoord2u8(tex_x, tex_y);
		GX_Position3s16(x + scale, y, -1);
		GX_Color4u8(0xFF, 0xFF, 0xFF, 0xFF);
		GX_TexCoord2u8(tex_x + 8, tex_y);
		GX_Position3s16(x + scale, y + scale, -1);
		GX_Color4u8(0xFF, 0xFF, 0xFF, 0xFF);
		GX_TexCoord2u8(tex_x + 8, tex_y + 8);
		GX_Position3s16(x, y + scale, -1);
		GX_Color4u8(0xFF, 0xFF, 0xFF, 0xFF);
		GX_TexCoord2u8(tex_x, tex_y + 8);

		x += width;
		str++;
	}

	GX_End();
}
