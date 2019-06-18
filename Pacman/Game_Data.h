#ifndef GAME_DATA_H
#define GAME_DATA_H

#include <conio.h>

#include "Math_Func.h"
#include "Level_Data.h"
#include "Drawer.h"
#include "Actors.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "Constants.h"
#include "Utility.h"

class Game {

	// This class provides a centralised location for running game logic and holding game objects and rendering.
	// Opted to use this as a cleaner method than programming directly in game loop.

	public:
		Game(SDL_Window* In_Window, SDL_Renderer* In_Actor_Renderer);
		~Game();

		void Load_Textures();

		bool On_Tick(float D_Time);
		bool Update_Input();
		bool Win_Condition();

		void Draw_Background();
		void Draw_Entities();
		void Draw_Pacman();
		void Draw_Red();
		void Draw_Teal();
		void Draw_Pink();
		void Draw_Orange();
		

		void Draw_UI();

		void Update_Pellets();
		void Update_States(float D_Time);
		void Update_Collision();
		void Reset();

	private:

		int* Difficulty;
		int* Player_Score;
		int* Ghost_Kill;
		int* Player_Lives;

		float* Scatter_Timer_Arr;
		float* Chase_Timer_Arr;

		Level* Level_Inst;
		Drawer* Actor_Drawer;

		Pacman* Pacman_Inst;
		Red* Red_Inst;
		Teal* Teal_Inst;
		Pink* Pink_Inst;
		Orange* Orange_Inst;

		State* Current_State;

		Timer* State_Timer;

		SDL_Surface* Big_Dot;
		SDL_Surface* Small_Dot;

		SDL_Surface* Pacman_Right_1;
		SDL_Surface* Pacman_Right_2;
		SDL_Surface* Pacman_Down_1;
		SDL_Surface* Pacman_Down_2;
		SDL_Surface* Pacman_Left_1;
		SDL_Surface* Pacman_Left_2;
		SDL_Surface* Pacman_Up_1;
		SDL_Surface* Pacman_Up_2;

		SDL_Surface* Ghost_Red;
		SDL_Surface* Ghost_Teal;
		SDL_Surface* Ghost_Pink;
		SDL_Surface* Ghost_Orange;
		SDL_Surface* Ghost_Vulnerable;
		SDL_Surface* Ghost_Flash;
		SDL_Surface* Ghost_Dead;

		SDL_Surface* Level_Background;
};

#endif