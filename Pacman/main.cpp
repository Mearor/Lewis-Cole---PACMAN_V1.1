#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "assert.h"
#include "drawer.h"
#include "Constants.h"
#include "Game_Data.h"
#include <iostream>



int main(int argc, char **argv) {
	/* initialize SDL */
	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		assert(0 && "Failed to initialize video!");
		exit(-1);
	}
	
	SDL_Window* window = SDL_CreateWindow("Pacman", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Screen_X, Screen_Y, SDL_WINDOW_OPENGL);
	SDL_Renderer* Render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); // To be acted

	if(!window)
	{
		assert(0 && "Failed to create window!");
		exit(-1);
	}

	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);

	if (TTF_Init() == -1)
	{
		assert(0 && "Failed to create ttf!");
		exit(-1);
	}

	//Drawer* drawer = Drawer::Create(window, renderer);
	Game* Game_Inst = new Game(window, Render);

	//drawer->Draw("Playfield.png");

	float lastFrame = (float) SDL_GetTicks() * 0.001f;
	SDL_Event event;
	while (SDL_PollEvent(&event) >= 0)
	{
		float currentFrame = (float) SDL_GetTicks() * 0.001f;
		float elapsedTime = currentFrame - lastFrame;

		SDL_SetRenderDrawColor(Render, 0, 0, 0, 255);
		SDL_RenderClear(Render);

		// Insert Game Loop Stuff Here
		
		if (!Game_Inst->On_Tick(elapsedTime)) {

			break;
		}

		lastFrame = currentFrame;		

		SDL_RenderPresent(Render);
		SDL_Delay(1);
	}

	TTF_Quit();
	IMG_Quit();
	SDL_Quit( );

	return 0;
}

