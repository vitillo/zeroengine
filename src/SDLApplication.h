#ifndef SDLAPPLICATION_H
#define SDLAPPLICATION_H

#include "SDL.h"

#include "Application.h"

namespace ZeroEngine{
	class SDLApplication : public Application{
	public:
		SDLApplication(int viewportWidth, int viewportHeight, int viewportDepth, bool fullscreen = false);
		~SDLApplication();
		
		
	protected:
		void BeginDraw();
		bool HandleEvents();
		void EndDraw();
		virtual bool HandleSDLEvent(SDL_Event) = 0;
		virtual void HandleRepeatingKeys() = 0;
		
		
	private:
		enum ExitError{
			SDLINITIALIZE_ERROR = 1,
			SDLVIDEOMODE_ERROR
		};
		
		SDL_Surface* _viewport;
	};
}

#endif
