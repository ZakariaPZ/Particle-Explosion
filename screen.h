#pragma once
#include "SDL.h" //automatically includes in screen.cpp
namespace zak {
	class Screen {
	private:
		SDL_Window* m_window;
		SDL_Renderer* m_renderer;
		SDL_Texture* m_texture;
		Uint32* m_buffer1;
		Uint32* m_buffer2;
	public:
		static const int SCREEN_WIDTH = 800; //All screen objects will have the same 
											//same screen width and height
												//public because they're const anyways 
												//and we might need to access them 
		static const int SCREEN_HEIGHT = 600;
	public:
		Screen();
		bool init();
		bool processEvents();
		void setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue);
		void close();
		void update();
		void clear();
		void boxBlur();
	};

}