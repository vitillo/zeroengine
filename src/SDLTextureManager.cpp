#include <SDL_image/SDL_image.h>

#include "SDLTextureManager.h"

using namespace ZeroEngine;

bool SDLTextureManager::Load(const char* fileName, TextureInfo* texture){

	SDL_Surface* surface = IMG_Load(fileName);
	if(surface){
		*texture = TextureInfo(surface->pixels, surface->format->palette ? surface->format->palette->colors : NULL, surface->w, surface->h, surface->format);
		return true;
	}
	return false;
}
