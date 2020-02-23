#include "screen.h"
#include <iostream>
using namespace std;

namespace zak {

	Screen::Screen() {
		m_window = NULL;
		m_renderer = NULL;
		m_texture = NULL;
		m_buffer1 = NULL;
		m_buffer2 = NULL;
	}
	bool Screen::init() {
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			return false; //If it fails, output false from the main subroutine/function
		}

		m_window = SDL_CreateWindow("Particle Fire Explosion",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH, SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN); //Use API tutorials to figure things like this out 

		if (m_window == NULL) { //if window can't be created
			SDL_Quit(); //we initialized SDL but couldn't run the window, so we must quit it now
			return false;
		}

		m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC);
		//SDL_RENDERER_PRESENTVSYNC makes sure the rendering is synced with the screen refresh rate
					//so that we don't draw something (refresh) while rendering - prevents shearing
		m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);//this is what the renderer will render - it will contain our pixel information

		if (m_renderer == NULL) {
			SDL_DestroyWindow(m_window);
			SDL_Quit(); //could create window and initialize SDL - must quit and	
							//destroy window if renderer fails 
			return false;
		}

		if (m_texture == NULL) {
			SDL_DestroyRenderer(m_renderer);
			SDL_DestroyWindow(m_window);
			SDL_Quit();

			return false;
		}

		m_buffer1 = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT]; //Uint32 is a type declared by SDL which is equivalent to an int (32 bits) on most systems - SDL created this to ensure the int will be 32 bits 
					//since some systems don't use 32 bits 
				//The pixel RGBA8888 - Red, blue, green and alpga - each of those uses 1 byte, which is 8 bits, so we needa
					//32 bit int to hold pixel info
				//notice we've made heright*width nunber of Units32's - we need to allocate enough memory for each 32 bit pixel 
					//on the screen, which of of size width (bits) * height (bits)
		m_buffer2 = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];
		memset(m_buffer1, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32)); //turns the screen white - (maximizes value in bytes per pixel - 255 value?)
		//a value of 0 makes the screen black
		memset(m_buffer2, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32)); //turns the screen white - (maximizes value in bytes per pixel - 255 value?)

		return true;
	}

	bool Screen::processEvents() {
		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) { //quitting event 
				return false;
			}
		}
		return true;
	}

	void Screen::close() {
		delete[]m_buffer1; //remember we must manually delete anything with the "new" operator (which works with pointers)
		delete[]m_buffer2;
		SDL_DestroyRenderer(m_renderer);
		SDL_DestroyTexture(m_texture);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
	}


	void Screen::setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue) {
		Uint32 color = 0;
		if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) {
			return;
		}
		else {
			color += red;
			color <<= 8;
			color += green;
			color <<= 8;
			color += blue;
			color <<= 8;
			color += 0xFF; //alpha value
			
			//RGBA

			m_buffer1[(y * SCREEN_WIDTH) + x] = color;
		}
	}

	void Screen::update() {
		SDL_UpdateTexture(m_texture, NULL, m_buffer1, SCREEN_WIDTH * sizeof(Uint32));
		SDL_RenderClear(m_renderer);
		SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
		SDL_RenderPresent(m_renderer);
	}

	void Screen::boxBlur() {
		Uint32* temp = m_buffer1;
		m_buffer1 = m_buffer2;
		m_buffer2 = temp;

		for (int y = 0; y < SCREEN_HEIGHT; y++) {
			for (int x = 0; x < SCREEN_WIDTH; x++) {

				//Below iterates of 3x3 grid of pixels

				int redTotal = 0;
				int greenTotal = 0;
				int blueTotal = 0;

				for (int row = -1; row <= 1; row++) {
					for (int col = -1; col <= 1; col++) {
						int currentX = x + col;
						int currentY = y + row;

						if (currentX >= 0 && currentX < SCREEN_WIDTH && currentY >= 0 && currentY < SCREEN_HEIGHT) {
							Uint32 color = m_buffer2[currentY * SCREEN_WIDTH + currentX]; //buffer contains pixel color info 

							Uint8 red = color >> 24; //get red out of color (since red is on the left by 24 bits or 3 bytes)
							Uint8 green = color >> 16;
							Uint8 blue = color >> 8;

							redTotal += red;
							greenTotal += green;
							blueTotal += blue;
							
						}
					}
				}
				Uint8 red = redTotal / 9;
				Uint8 green = greenTotal / 9;
				Uint8 blue = blueTotal / 9; 

				setPixel(x, y, red, green, blue);
			}
		}
	}
}