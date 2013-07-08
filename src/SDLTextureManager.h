#ifndef SDLTEXTUREMANAGER_H
#define SDLTEXTUREMANAGER_H

#include "TextureManager.h"

namespace ZeroEngine {
	class SDLTextureManager : public TextureManager{
	public:
		bool Load(const char* fileName, TextureInfo* texture);
	};
}

#endif
