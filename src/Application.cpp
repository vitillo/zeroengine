#include <ctime>
#include <unistd.h>
#include <iostream>

#include "Application.h"

namespace ZeroEngine{
	Application::Application(int viewportWidth, int viewportHeight, int viewportDepth, bool fullscreen) : _viewportWidth(viewportWidth), _viewportHeight(viewportHeight), _viewportDepth(viewportDepth), _fullscreen(fullscreen), _renderer(NULL), _textureManager(NULL), _frameRate(fps){}
	
	Application::~Application(){
		delete _renderer;
	}
	
	void Application::Run(){
		Initialize();
		
		double tickCounter = 0;
		int frames = 0;
		clock_t t1 = clock();
		clock_t t2;
		
		while(!HandleEvents()){
			Update();
			BeginDraw();
			Draw();
			EndDraw();

			frames++;
			t2 = clock();
			clock_t diff = t2 - t1;
			
			//Guarantee a maximum fps rate
			double sleepInteval = (double)CLOCKS_PER_SEC / fps - diff;
			if(sleepInteval > 0){
				usleep(1000000 * sleepInteval / CLOCKS_PER_SEC);
				tickCounter += sleepInteval;
			}

			//Display FPS rate
			tickCounter += diff;
			if(tickCounter > CLOCKS_PER_SEC){
				_frameRate = frames;
				tickCounter = 0;
				frames = 0;
			}	
			t1 = clock();
		}
	}
	
	void Application::BeginDraw(){
		_renderer->ClearZBuffer();
	}
	
	void Application::EndDraw(){}
}
