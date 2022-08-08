#ifndef SDLHELPERS_H
#define SDLHELPERS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>

static const char *font_paths[] = {"C:/windows/fonts/", "/usr/share/fonts/TTF/", "./"};
static const short font_path_count = 3;

static SDL_Rect getRect(Sint16 x, Sint16 y, Uint16 w, Uint16 h) {
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	return rect;
}

static SDL_Color getColor(Uint8 r, Uint8 g, Uint8 b) {
	SDL_Color color;
	color.r = r;
	color.g = g;
	color.b = b;
	return color;
}

static TTF_Font *openFont(std::vector<std::string> fonts, int size) {
	TTF_Font *font = NULL;
	for (int x = 0; x < fonts.size(); x++) {
		for (int y = 0; y < font_path_count; y++) {
			std::string val(font_paths[y]);
			val.append(fonts[x]);
			font = TTF_OpenFont(val.c_str(), size);
			if (font)
				return font;
		}
	}
	return NULL;
}

#endif
