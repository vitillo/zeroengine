#ifndef MATERIAL_H
#define MATERIAL_H

#include <SDL_image/SDL_image.h>

#include "Vertex.h"

namespace ZeroEngine {
	struct TextureInfo{
		void* Buffer;
		void* Palette;
		int Width;
		int Height;
        SDL_PixelFormat *Format;
		
		TextureInfo() : Buffer(NULL), Palette(NULL), Width(0), Height(0), Format(0){}
		TextureInfo(void* buffer, void* palette, int width, int height, SDL_PixelFormat *fmt) : Buffer(buffer), Palette(palette), Width(width), Height(height), Format(fmt) {}
		
		bool isLoaded() { return Buffer != NULL; }
        void LoadPixel(int u, int v, SDL_Color &color){
            if(Palette){
                Uint8 *texPixmem = (Uint8*) Buffer + v * Width + u * Format->BytesPerPixel;
                color = ((SDL_Color*)Palette)[*texPixmem];
            }else{
                Uint32 pixel, temp;
                
                pixel=*((Uint32*)Buffer);
                        
                /* Get Red component */
                temp=pixel&Format->Rmask; /* Isolate red component */
                temp=temp>>Format->Rshift;/* Shift it down to 8-bit */
                temp=temp<<Format->Rloss; /* Expand to a full 8-bit number */
                color.r=(Uint8)temp;
                
                /* Get Green component */
                temp=pixel&Format->Gmask; /* Isolate green component */
                temp=temp>>Format->Gshift;/* Shift it down to 8-bit */
                temp=temp<<Format->Gloss; /* Expand to a full 8-bit number */
                color.g=(Uint8)temp;
                
                /* Get Blue component */
                temp=pixel&Format->Bmask; /* Isolate blue component */
                temp=temp>>Format->Bshift;/* Shift it down to 8-bit */
                temp=temp<<Format->Bloss; /* Expand to a full 8-bit number */
                color.b=(Uint8)temp;
            }
        }
	};
	
	struct Material{
		RGBAColor Specular;
		RGBAColor Diffuse;
		TextureInfo Texture;
		int Smoothness;
		
		Material() : Specular(), Diffuse(), Texture(), Smoothness(0){}
		Material(const RGBAColor& specular, const RGBAColor& diffuse, int smoothness, const TextureInfo& texture) : Specular(specular), Diffuse(diffuse), Smoothness(smoothness), Texture(texture) {}
		Material(const RGBAColor& specular, const RGBAColor& diffuse, int smoothness) : Specular(specular), Diffuse(diffuse), Texture(), Smoothness(smoothness) {}
	};
}

#endif
