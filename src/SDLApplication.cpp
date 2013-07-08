#include <iostream>

#include "SDLSoftwareRenderer.h"
#include "SDLTextureManager.h"
#include "SDLApplication.h"

namespace ZeroEngine{
	SDLApplication::SDLApplication(int viewportWidth, int viewportHeight, int viewportDepth, bool fullscreen) : Application(viewportWidth, viewportHeight, viewportDepth, fullscreen), _viewport(NULL){
		Uint32 initflags = SDL_INIT_VIDEO; 
		Uint32 videoflags = SDL_HWSURFACE | SDL_DOUBLEBUF | (fullscreen ? SDL_FULLSCREEN : 0);
		
		if ( SDL_Init(initflags) < 0 ) {
			fprintf(stderr, "Couldn't initialize SDL: %s\n",
					SDL_GetError());
			exit(SDLINITIALIZE_ERROR);
		}
		
		_viewport=SDL_SetVideoMode(viewportWidth, viewportHeight, viewportDepth, videoflags);
        if (_viewport == NULL) {
			fprintf(stderr, "Couldn't set video mode: %s\n", SDL_GetError());
			SDL_Quit();
			exit(SDLVIDEOMODE_ERROR);
		}
		
		SDL_WM_GrabInput(SDL_GRAB_ON);
		SDL_ShowCursor(SDL_DISABLE);
		_renderer = new SDLSoftwareRenderer(_viewport, viewportWidth, viewportHeight, viewportDepth);
		_textureManager = new SDLTextureManager();
	}
	
	SDLApplication::~SDLApplication(){
		SDL_Quit();
	}
	
	void SDLApplication::BeginDraw(){
		SDL_FillRect(_viewport, NULL, 0);
		Application::BeginDraw();
	}
	
	bool SDLApplication::HandleEvents(){
        SDL_Event event;
		HandleRepeatingKeys();
		while ( SDL_PollEvent(&event) ) {
			switch (event.type) {
				case SDL_QUIT:
					return true;
					break;
				default:
					return HandleSDLEvent(event);
					break;
			}
		}
		
		return false;	
	}
	
	void SDLApplication::EndDraw(){
		SDL_Flip(_viewport);
		Application::EndDraw();
	}
}
