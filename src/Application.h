#ifndef Application_H
#define Application_H

#include "Renderer.h"
#include "TextureManager.h"

namespace ZeroEngine{
	
	class Application{
	public:
		Application(int viewportWidth, int viewportHeight, int viewportDepth, bool fullscreen = false);
		virtual ~Application();
		
		virtual void Initialize() = 0;
		virtual void Update() = 0;
		virtual void Draw() = 0;
		void Run();
		
	protected:
		bool _fullscreen;
		int _frameRate;
		int _viewportHeight;
		int _viewportWidth;
		int _viewportDepth;
		Renderer* _renderer;
		TextureManager* _textureManager;
		
		virtual bool HandleEvents() = 0;
		virtual void BeginDraw();
		virtual void EndDraw();		
		
		
	private:
		static const int fps = 60;
	};
	
}

#endif
