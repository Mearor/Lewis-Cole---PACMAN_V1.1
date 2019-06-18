#include "Game_Data.h"

Game::Game(SDL_Window* In_Window, SDL_Renderer* In_Actor_Renderer) {

	Level_Inst = new Level;
	Actor_Drawer = new Drawer(In_Window, In_Actor_Renderer);

	// Initialize Actors

	Pacman_Inst = new Pacman(Level_Inst->Get_Pacman_Start());
	Red_Inst = new Red(Level_Inst->Get_Red_Start());
	Teal_Inst = new Teal(Level_Inst->Get_Teal_Start());
	Pink_Inst = new Pink(Level_Inst->Get_Pink_Start());
	Orange_Inst = new Orange(Level_Inst->Get_Orange_Start());

	Player_Score = new int(0);
	Difficulty = new int(0);
	Ghost_Kill = new int(0);
	Player_Lives = new int(2);

	Current_State = new State(Scattering);

	State_Timer = new Timer();

	Scatter_Timer_Arr = new float[4] {7.0, 7.0f, 5.0f, 5.0f};
	Chase_Timer_Arr = new float[4]{ 20.0f, 20.0, 20.0, 99999999.0f };

	State_Timer->Set_Timer(Scatter_Timer_Arr[0]);

	Load_Textures();

	//Vec_2<float> Pacman_Start = Level_Inst->Get_Pacman_Start();
	

}

Game::~Game() {

	delete Difficulty;
	delete Player_Score;
	delete Ghost_Kill;
	delete Player_Lives;

	delete Scatter_Timer_Arr;
	delete Chase_Timer_Arr;

	delete Level_Inst;
	delete Actor_Drawer;

	delete Pacman_Inst;
	delete Red_Inst;
	delete Teal_Inst;
	delete Pink_Inst;
	delete Orange_Inst;

	delete Current_State;

	delete State_Timer;

	delete Big_Dot;
	delete Small_Dot;

	delete Pacman_Right_1;
	delete Pacman_Right_2;
	delete Pacman_Down_1;
	delete Pacman_Down_2;
	delete Pacman_Left_1;
	delete Pacman_Left_2;
	delete Pacman_Up_1;
	delete Pacman_Up_2;

	delete Ghost_Red;
	delete Ghost_Teal;
	delete Ghost_Pink;
	delete Ghost_Orange;
	delete Ghost_Vulnerable;
	delete Ghost_Flash;
	delete Ghost_Dead;

	delete Level_Background;
}

void Game::Load_Textures() {

	Big_Dot = new SDL_Surface;
	Small_Dot = new SDL_Surface;

	Pacman_Right_1 = new SDL_Surface;
	Pacman_Right_2 = new SDL_Surface;
	Pacman_Down_1 = new SDL_Surface;
	Pacman_Down_2 = new SDL_Surface;
	Pacman_Left_1 = new SDL_Surface;
	Pacman_Left_2 = new SDL_Surface;
	Pacman_Up_1 = new SDL_Surface;
	Pacman_Up_2 = new SDL_Surface;

	Ghost_Red = new SDL_Surface;
	Ghost_Teal = new SDL_Surface;
	Ghost_Pink = new SDL_Surface;
	Ghost_Orange = new SDL_Surface;
	Ghost_Vulnerable = new SDL_Surface;
	Ghost_Flash = new SDL_Surface;
	Ghost_Dead = new SDL_Surface; 

	Level_Background = new SDL_Surface;

	*Big_Dot = *IMG_Load("Big_Dot_32.png");
	Small_Dot = IMG_Load("Small_Dot_32.png");

	Pacman_Right_1 = IMG_Load("closed_32.png");
	Pacman_Right_2 = IMG_Load("open_32.png");
	Pacman_Down_1 = IMG_Load("closed_down_32.png");
	Pacman_Down_2 = IMG_Load("open_down_32.png");
	Pacman_Left_1 = IMG_Load("closed_left_32.png");
	Pacman_Left_2 = IMG_Load("open_left_32.png");
	Pacman_Up_1 = IMG_Load("closed_up_32.png");
	Pacman_Up_2 = IMG_Load("open_right_32.png");

	Ghost_Red = IMG_Load("ghost_32_red.png");
	Ghost_Teal = IMG_Load("ghost_32_cyan.png");
	Ghost_Pink = IMG_Load("ghost_32_pink.png");
	Ghost_Orange = IMG_Load("ghost_32_orange.png");
	Ghost_Vulnerable = IMG_Load("Ghost_Vulnerable_32.png");
	Ghost_Flash = IMG_Load("ghost_32.png");
	Ghost_Dead = IMG_Load("Ghost_Dead_32.png");

	Level_Background = IMG_Load("lvl1.png");


}

bool Game::On_Tick(float D_Time) {

	// Main Gameplay Loop

	if (Win_Condition()) {


		return false;
	}

	if (!Update_Input()) {

		return false;
	}

	if (*Player_Lives < 0) {

		return false;
	}
	// On Tick for all Actors

	Pacman_Inst->Update(Level_Inst,D_Time);
	Red_Inst->On_Tick(Level_Inst, Pacman_Inst, D_Time);
	Teal_Inst->Set_Red_Pos(Red_Inst);
	Teal_Inst->On_Tick(Level_Inst, Pacman_Inst, D_Time);
	Pink_Inst->On_Tick(Level_Inst, Pacman_Inst, D_Time);
	Orange_Inst->On_Tick(Level_Inst, Pacman_Inst, D_Time);

	Orange_Inst->Debug_Console(D_Time);
	
	Update_Pellets();
	Update_States(D_Time);
	Update_Collision();

	Draw_Background();
	Draw_Entities();
	Draw_UI();

	/*Vec_2<int> Ent_Pixel = Render_Pos(Red_Inst->Get_Target());

	Actor_Drawer->Draw_From_File(Ghost_Flash, Ent_Pixel.X, Ent_Pixel.Y);*/

	return true;
}

void Game::Draw_Background() {

	Actor_Drawer->Draw_From_File(Level_Background);

	for (int i = 0; i < Level_Inst->Get_Width(); i++) {
		for (int j = 0; j < Level_Inst->Get_Length(); j++) {

			if (Level_Inst->Get_Tile_Data(i, j).Has_Pellet) {

				if (Level_Inst->Get_Tile_Data(i, j).Large_Pellet) {

					Vec_2<int> Pixel_Pos = Render_Pos(Vec_2<float>{(float)i, (float)j});

					Actor_Drawer->Draw_From_File(Big_Dot, Pixel_Pos.X, Pixel_Pos.Y);

				}
				else {

					Vec_2<int> Pixel_Pos = Render_Pos(Vec_2<float>{(float)i, (float)j});

					Actor_Drawer->Draw_From_File(Small_Dot, Pixel_Pos.X, Pixel_Pos.Y);

				}
			}
		}
	}
}

void Game::Draw_Entities() {

	// Revise render lerp drawing is a bit off.
	
	Draw_Pacman();
	Draw_Red();
	Draw_Teal();
	Draw_Pink();
	Draw_Orange();
}

void Game::Draw_Pacman() {

	Vec_2<int> Ent_Pixel = Render_Pos(Pacman_Inst->Get_Pos());

	if (Normalize(Pacman_Inst->Get_Vel()).Y == 1) {

		// DOWN
		Actor_Drawer->Draw_From_File(Pacman_Down_1, Ent_Pixel.X, Ent_Pixel.Y);
	}
	else if (Normalize(Pacman_Inst->Get_Vel()).Y == -1) {

		// UP
		Actor_Drawer->Draw_From_File(Pacman_Up_1, Ent_Pixel.X, Ent_Pixel.Y);
	}
	else if (Normalize(Pacman_Inst->Get_Vel()).X == 1) {

		// Right
		Actor_Drawer->Draw_From_File(Pacman_Right_1, Ent_Pixel.X, Ent_Pixel.Y);
	}
	else {

		// Left
		Actor_Drawer->Draw_From_File(Pacman_Left_1, Ent_Pixel.X, Ent_Pixel.Y);
	}
}

void Game::Draw_Red() {

	Vec_2<int> Ent_Pixel = Render_Pos(Red_Inst->Get_Pos());

	if (Red_Inst->Get_State() == Frightened) {

		if (State_Timer->Get_Cur_Time() < 2.0f) {

			Actor_Drawer->Draw_From_File(Ghost_Flash, Ent_Pixel.X, Ent_Pixel.Y);
		}
		else {

			Actor_Drawer->Draw_From_File(Ghost_Vulnerable, Ent_Pixel.X, Ent_Pixel.Y);
		}
	}
	else if (Red_Inst->Get_State() == Dead) {

			Actor_Drawer->Draw_From_File(Ghost_Dead, Ent_Pixel.X, Ent_Pixel.Y);
	}
	else {

			Actor_Drawer->Draw_From_File(Ghost_Red, Ent_Pixel.X, Ent_Pixel.Y);
	}
}

void Game::Draw_Teal() {


	Vec_2<int> Ent_Pixel = Render_Pos(Teal_Inst->Get_Pos());

	if (Teal_Inst->Get_State() == Frightened) {

		if (State_Timer->Get_Cur_Time() < 2.0f) {

			Actor_Drawer->Draw_From_File(Ghost_Flash, Ent_Pixel.X, Ent_Pixel.Y);
		}
		else {

			Actor_Drawer->Draw_From_File(Ghost_Vulnerable, Ent_Pixel.X, Ent_Pixel.Y);
		}
	}
	else if (Teal_Inst->Get_State() == Dead) {

		Actor_Drawer->Draw_From_File(Ghost_Dead, Ent_Pixel.X, Ent_Pixel.Y);
	}
	else {

		Actor_Drawer->Draw_From_File(Ghost_Teal, Ent_Pixel.X, Ent_Pixel.Y);
	}
}

void Game::Draw_Pink() {


	Vec_2<int> Ent_Pixel = Render_Pos(Pink_Inst->Get_Pos());

	if (Pink_Inst->Get_State() == Frightened) {

		if (State_Timer->Get_Cur_Time() < 2.0f) {

			Actor_Drawer->Draw_From_File(Ghost_Flash, Ent_Pixel.X, Ent_Pixel.Y);
		}
		else {

			Actor_Drawer->Draw_From_File(Ghost_Vulnerable, Ent_Pixel.X, Ent_Pixel.Y);
		}
	}
	else if (Pink_Inst->Get_State() == Dead) {

		Actor_Drawer->Draw_From_File(Ghost_Dead, Ent_Pixel.X, Ent_Pixel.Y);
	}
	else {

		Actor_Drawer->Draw_From_File(Ghost_Pink, Ent_Pixel.X, Ent_Pixel.Y);
	}
}

void Game::Draw_Orange() {

	Vec_2<int> Ent_Pixel = Render_Pos(Orange_Inst->Get_Pos());

	if (Orange_Inst->Get_State() == Frightened) {

		if (State_Timer->Get_Cur_Time() < 2.0f) {

			Actor_Drawer->Draw_From_File(Ghost_Flash, Ent_Pixel.X, Ent_Pixel.Y);
		}
		else {

			Actor_Drawer->Draw_From_File(Ghost_Vulnerable, Ent_Pixel.X, Ent_Pixel.Y);
		}
	}
	else if (Orange_Inst->Get_State() == Dead) {

		Actor_Drawer->Draw_From_File(Ghost_Dead, Ent_Pixel.X, Ent_Pixel.Y);
	}
	else {

		Actor_Drawer->Draw_From_File(Ghost_Orange, Ent_Pixel.X, Ent_Pixel.Y);
	}
}

void Game::Draw_UI() {

	std::string Score_Output = std::to_string(*Player_Score);

	Actor_Drawer->DrawText(Score_Output.c_str(), "freefont-ttf\\sfd\\FreeMono.ttf", 0, 0);

	for (int i = 0; i < *Player_Lives; i++) {

		// Height = 410

		Actor_Drawer->Draw_From_File(Pacman_Right_1, 18 * i, 410);
	}
}

bool Game::Update_Input() {

	const Uint8 *keystate = SDL_GetKeyboardState(NULL);

	if (keystate[SDL_SCANCODE_UP]) { Pacman_Inst->Set_New_Vel(Vec_2<float> { 0.0f, -1.0f * Base_Speed }); }
	else if (keystate[SDL_SCANCODE_DOWN]) { Pacman_Inst->Set_New_Vel(Vec_2<float> { 0.0f, 1.0f * Base_Speed }); }
	else if (keystate[SDL_SCANCODE_RIGHT]) { Pacman_Inst->Set_New_Vel(Vec_2<float> { 1.0f * Base_Speed, 0.0f }); }
	else if (keystate[SDL_SCANCODE_LEFT]) { Pacman_Inst->Set_New_Vel(Vec_2<float> { -1.0f * Base_Speed, 0.0f }); }

	if (keystate[SDL_SCANCODE_ESCAPE])
		return false;

	return true;
}

bool Game::Win_Condition() {

	if (Level_Inst->Get_Pellet_Cnt() == 0) {

		return true;
	}
	else {

		return false;
	}

}

void Game::Update_Pellets() {

	if (Magnitude(Vec_2<float> { Pacman_Inst->Get_Pos().X - (float)Pacman_Inst->Get_Pos_i().X,
		Pacman_Inst->Get_Pos().Y - (float)Pacman_Inst->Get_Pos_i().Y }) < 0.25) {

		if (Level_Inst->Get_Tile_Data(Pacman_Inst->Get_Pos_i().X, Pacman_Inst->Get_Pos_i().Y).Has_Pellet) {

			if (Level_Inst->Get_Tile_Data(Pacman_Inst->Get_Pos_i().X, Pacman_Inst->Get_Pos_i().Y).Large_Pellet) {

				Level_Inst->Decr_Pellet_Cnt(Pacman_Inst->Get_Pos_i().X, Pacman_Inst->Get_Pos_i().Y);

				*Player_Score = *Player_Score + 50;

				if (Red_Inst->Get_State() != Waiting) { Red_Inst->Set_State(Frightened); }
				if (Teal_Inst->Get_State() != Waiting) { Teal_Inst->Set_State(Frightened); }
				if (Pink_Inst->Get_State() != Waiting) { Pink_Inst->Set_State(Frightened); }
				if (Orange_Inst->Get_State() != Waiting) { Orange_Inst->Set_State(Frightened); }

				*Current_State = Frightened;
				State_Timer->Set_Timer(6.0f);

			}
			else {

				Level_Inst->Decr_Pellet_Cnt(Pacman_Inst->Get_Pos_i().X, Pacman_Inst->Get_Pos_i().Y);

				*Player_Score = *Player_Score + 10;
			}

			Pink_Inst->Inc_Dot_Count();
			Teal_Inst->Inc_Dot_Count();
			Orange_Inst->Inc_Dot_Count();
		}
	}
}

void Game::Update_States(float D_Time) {

	// The ghosts in pacman typically follow specific behaviours that last a certain amount of time before swapping.
	// this function ensures that the swap

	if (State_Timer->Update(D_Time)) {

		switch (*Current_State) {

		case (Scattering): {

			*Current_State = Chasing;

			if (Red_Inst->Get_State() != Waiting) { Red_Inst->Set_State(Chasing); }
			if (Teal_Inst->Get_State() != Waiting) { Teal_Inst->Set_State(Chasing); }
			if (Pink_Inst->Get_State() != Waiting) { Pink_Inst->Set_State(Chasing); }
			if (Orange_Inst->Get_State() != Waiting) { Orange_Inst->Set_State(Chasing); }

			State_Timer->Set_Timer(Chase_Timer_Arr[*Difficulty]);

			break;
		}
		case (Chasing): {

			*Difficulty = *Difficulty + 1;
			*Current_State = Scattering;

			if (Red_Inst->Get_State() != Waiting) { Red_Inst->Set_State(Scattering); }
			if (Teal_Inst->Get_State() != Waiting) { Teal_Inst->Set_State(Scattering); }
			if (Pink_Inst->Get_State() != Waiting) { Pink_Inst->Set_State(Scattering); }
			if (Orange_Inst->Get_State() != Waiting) { Orange_Inst->Set_State(Scattering); }

			State_Timer->Set_Timer(Scatter_Timer_Arr[*Difficulty]);

			break;
		}
		case (Frightened): {

			*Current_State = Scattering;

			*Ghost_Kill = 0;

			if (Red_Inst->Get_State() != Dead) { Red_Inst->Set_State(Scattering); }
			if (Teal_Inst->Get_State() != Dead) { Teal_Inst->Set_State(Scattering); }
			if (Pink_Inst->Get_State() != Dead) { Pink_Inst->Set_State(Scattering); }
			if (Orange_Inst->Get_State() != Dead) { Orange_Inst->Set_State(Scattering); }

			State_Timer->Set_Timer(Scatter_Timer_Arr[*Difficulty]);

			break;
		}
		}
	}
}

void Game::Update_Collision() {

	if (Magnitude(Vec_2<float>{Pacman_Inst->Get_Pos().X - Red_Inst->Get_Pos().X, Pacman_Inst->Get_Pos().Y - Red_Inst->Get_Pos().Y}) < 0.25f) {

		if (Red_Inst->Get_State() == Frightened) {

			*Ghost_Kill = *Ghost_Kill + 1;

			*Player_Score = *Player_Score + *Ghost_Kill * 200;

			//SDL_Delay(1000);

			Red_Inst->Set_State(Dead);
		}
		else if (Red_Inst->Get_State() == Dead) {
			
			// Do Nothing;
		}
		else {
			
			Reset();
		}
	}

	if (Magnitude(Vec_2<float>{Pacman_Inst->Get_Pos().X - Teal_Inst->Get_Pos().X, Pacman_Inst->Get_Pos().Y - Teal_Inst->Get_Pos().Y}) < 0.25f) {

		if (Teal_Inst->Get_State() == Frightened) {

			*Ghost_Kill = *Ghost_Kill + 1;

			*Player_Score = *Player_Score + *Ghost_Kill * 200;

			//SDL_Delay(1000);

			Teal_Inst->Set_State(Dead);
		}
		else if (Teal_Inst->Get_State() == Dead) {

			// Do Nothing;
		}
		else {

			Reset();
		}
	}

	if (Magnitude(Vec_2<float>{Pacman_Inst->Get_Pos().X - Pink_Inst->Get_Pos().X, Pacman_Inst->Get_Pos().Y - Pink_Inst->Get_Pos().Y}) < 0.25f) {

		if (Pink_Inst->Get_State() == Frightened) {

			*Ghost_Kill = *Ghost_Kill + 1;

			*Player_Score = *Player_Score + *Ghost_Kill * 200;

			//SDL_Delay(1000);

			Pink_Inst->Set_State(Dead);
		}
		else if (Pink_Inst->Get_State() == Dead) {

			// Do Nothing;
		}
		else {

			Reset();
		}
	}

	if (Magnitude(Vec_2<float>{Pacman_Inst->Get_Pos().X - Orange_Inst->Get_Pos().X, Pacman_Inst->Get_Pos().Y - Orange_Inst->Get_Pos().Y}) < 0.25f) {

		if (Orange_Inst->Get_State() == Frightened) {

			*Ghost_Kill = *Ghost_Kill + 1;

			*Player_Score = *Player_Score + *Ghost_Kill * 200;

			//SDL_Delay(1000);

			Orange_Inst->Set_State(Dead);
		}
		else if (Orange_Inst->Get_State() == Dead) {

			// Do Nothing;
		}
		else {

			Reset();
		}
	}
}

void Game::Reset() {

	// Sequence to be called when player collides with a non-vulnerable ghost.

	delete Pacman_Inst;
	delete Red_Inst;
	delete Pink_Inst;
	delete Orange_Inst;

	*Player_Lives = *Player_Lives - 1;

	SDL_Delay(500);

	State_Timer->Set_Timer(Scatter_Timer_Arr[*Difficulty]);

	Pacman_Inst = new Pacman(Level_Inst->Get_Pacman_Start());
	Red_Inst = new Red(Level_Inst->Get_Red_Start());
	Teal_Inst = new Teal(Level_Inst->Get_Teal_Start());
	Pink_Inst = new Pink(Level_Inst->Get_Pink_Start());
	Orange_Inst = new Orange(Level_Inst->Get_Orange_Start());

	*Current_State = Scattering;
}

