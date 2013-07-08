#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include "Material.h"

namespace ZeroEngine {
	class TextureManager{
	public:
		virtual bool Load(const char* fileName, TextureInfo* texture) = 0;
	};
}

#endif
