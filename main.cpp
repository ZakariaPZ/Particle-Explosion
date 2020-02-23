#include <iostream>
#include <SDL.h>
#include <math.h>
#include "screen.h"
#include <stdlib.h>
#include <time.h>
#include "Swarm.h"
#include "particleExp.h"
#include <windows.h>
using namespace std;
using namespace zak;

int main(int argc, char *argv[]) {


	PlaySound(TEXT("Aude.wav"), NULL, SND_ASYNC);

	srand(time(NULL));

	Screen screen;

	if (screen.init() == false) {
		cout << "Error initialising SDL" << endl;
		return 1;
	}


	Swarm swarm;
	
	while (true) {
		

		

		int elapsed = SDL_GetTicks();

	
		swarm.update(elapsed);

		unsigned char green = 128 + 128 * sin(elapsed * 0.0001); //unsigned char can only have up to 255 
				//so we don't have to worry about going past 255

		unsigned char red = 128 + 128 * sin(elapsed * 0.0002);
		unsigned char blue = 128 + 128 * sin(elapsed * 0.0003);


		const Particle* const pParticles = swarm.getParticles();

		for (int i = 0; i < swarm.NPARTICLES; i++) {
			Particle particle = pParticles[i];

			int x = (particle.m_x + 1) * screen.SCREEN_WIDTH / 2; 
			int y = particle.m_y  * screen.SCREEN_WIDTH / 2 + (screen.SCREEN_HEIGHT / 2);

			screen.setPixel(x, y, red, green, blue);
		}
		

		screen.boxBlur();

		//Draw the screen
		screen.update();


		if (!screen.processEvents()) {
			break;
		}
		//update particles
		//draw particles
		//check for messages/events (like clicking a window, or presses a button)
	}



	screen.close();
	
	return 0;
}