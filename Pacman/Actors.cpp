#include <stdlib.h>
#include <iostream>
#include "Actors.h"
#include "Math_Func.h"
#include "Level_Data.h"
#include "Utility.h"
#include "Constants.h"

//////////////////////////////////
// CLASS ACTOR MEMBER FUNCTIONS //
//////////////////////////////////

Actor::Actor() {

	Pos_f = new Vec_2<float>;
	Vel = new Vec_2<float>;

	Pos_i = new Vec_2<int>;
	Nxt_Tile = new Vec_2<int>;

	New_Vel = new Vec_2<float>;
	NV_Nxt_Tile = new Vec_2<int>;

	Stationary = new bool;

}

Actor::~Actor() {

	delete Pos_f;
	delete Vel;

	delete Pos_i;
	delete Nxt_Tile;

	delete New_Vel;
	delete NV_Nxt_Tile;

	delete Stationary;

}

void Actor::Update(Level* In_Level, float D_Time) {


	if (!Get_Stationary()) { 
	
		Set_Next_Tile();
		Set_NV_Nxt_Tile();

		// Actor is not Stationary

		if ((Vel->X == New_Vel->X) && (Vel->Y == New_Vel->Y)) {

			// Velocity and New Velocity are the same.


			if (!Next_Tile_Traversable(In_Level) && At_Tile_Center(D_Time)) {

				// Actor has reached center of a tile AND next tile in current direction is non-traversable
				// AND new velocity is equal to current velocity.

				// Actor position is rounded to nearest whole number which is tile center.
				// Stationary is set to true to keep actor from calculating next tiles with a velocity of zero
				// which causes the number to max out at negative 2 ^ 31;

				*Pos_f = Vec_2<float>{ roundf(Pos_f->X), roundf(Pos_f->Y) };
				Set_Vel(Vec_2<float>{0.0f, 0.0});
				*Stationary = true;
			}
			else {

				// Continue on current velocity

				New_Pos(Pos_f, Vel, D_Time);
				Set_Pos_i();
				Set_Next_Tile();
				Set_NV_Nxt_Tile();
			}
		}
		else if (((Vel->X != New_Vel->X) && (Vel->Y == New_Vel->Y))
			|| ((Vel->X == New_Vel->X) && (Vel->Y != New_Vel->Y))) {

			// This is to handle simple direction reversal where valid. The statement below requires
			// Actor be at center point of a tile. Which is a requirement for turning 90 degrees however a simple
			// reverse of direction should be done immediately.

			if (NV_Next_Tile_Traversable(In_Level)) {

				// Change direction 90 degrees

				*Pos_f = Vec_2<float>{ roundf(Pos_f->X), roundf(Pos_f->Y) };
				*Vel = *New_Vel;
				New_Pos(Pos_f, Vel, D_Time);
				Set_Pos_i();
				Set_Next_Tile();
				Set_NV_Nxt_Tile();
				
			}
			else {

				// Stop

				New_Pos(Pos_f, Vel, D_Time);
				Set_Pos_i();
				Set_Next_Tile();
				Set_NV_Nxt_Tile();

			}
		}
		else if ((Vel->X != New_Vel->X) || (Vel->Y != New_Vel->Y)) {

			if (NV_Next_Tile_Traversable(In_Level) && At_Tile_Center(D_Time)) {

				// This is to handle simple direction reversal where valid. The statement below requires
				// Actor be at center point of a tile. Which is a requirement for turning 90 degrees however a simple
				// reverse of direction should be done immediately

				*Pos_f = Vec_2<float>{ roundf(Pos_f->X), roundf(Pos_f->Y) };
				*Vel = *New_Vel;
				New_Pos(Pos_f, Vel, D_Time);
				Set_Pos_i();
				Set_Next_Tile();
				Set_NV_Nxt_Tile();
			}
			else if (!Next_Tile_Traversable(In_Level) && At_Tile_Center(D_Time)) {

				// Stop

				*Pos_f = Vec_2<float>{ roundf(Pos_f->X), roundf(Pos_f->Y) };
				Set_Vel(Vec_2<float>{0.0f, 0.0});
				*Stationary = true;
			}
			else {

				// Continue

				New_Pos(Pos_f, Vel, D_Time);
				Set_Pos_i();
				Set_Next_Tile();
				Set_NV_Nxt_Tile();

			}
		}
		else {

			// Continue

			New_Pos(Pos_f, Vel, D_Time);
			Set_Pos_i();
			Set_Next_Tile();
			Set_NV_Nxt_Tile();
		}
	}
	else if (Get_Stationary()) {

		// Actor is stationary and waiting for a new valid direction. 

		Set_NV_Nxt_Tile();

		if (In_Level->Get_Tile_Data(Pos_i->X, Pos_i->Y).Warp_Tile
			&& !Next_Tile_Traversable(In_Level) && At_Tile_Center(D_Time)) {

			// Additional code to handle "Warp tiles" where actor moves off screen.

			Set_Pos(In_Level->Get_Tile_Data(Pos_i->X, Pos_i->Y).Warp_Partner);
			*Stationary = false;
		}

		if ((Vel->X == New_Vel->X) && (Vel->Y == New_Vel->Y)) {

			// Do Nothing
		}
		else if ((Vel->X != New_Vel->X || Vel->Y != New_Vel->Y) && NV_Next_Tile_Traversable(In_Level)) {

			// If stationary and provided with a new valid direction, begin movement.

			*Vel = *New_Vel;
			New_Pos(Pos_f, Vel, D_Time);
			Set_Pos_i();
			Set_Next_Tile();
			Set_NV_Nxt_Tile();
			*Stationary = false;
		}

	}

	
}

// Getters and Setters

void Actor::Set_Pos(Vec_2<float> New_Pos) {

	*Pos_f = New_Pos;
}

void Actor::Set_Vel(Vec_2<float> New_Vel) {

	*Vel = New_Vel;
}

void Actor::Set_Pos_i() {

	*Pos_i = Vec_2<int>{ (int)round(Pos_f->X), (int)round(Pos_f->Y) };
}

void Actor::Set_Next_Tile() {

	Next_Tile(Nxt_Tile, Pos_i, Vel);
}

Vec_2<float> Actor::Get_Pos() {
	
	return* Pos_f;
}

Vec_2<float> Actor::Get_Vel() {
	
	return* Vel;
}

Vec_2<int> Actor::Get_Pos_i() {
	
	return* Pos_i;
}

Vec_2<int> Actor::Get_Next_Tile_i() {
	
	return* Nxt_Tile;
}

void Actor::Set_New_Vel(Vec_2<float> In_Vel) {
	*New_Vel = In_Vel;
}

void Actor::Set_NV_Nxt_Tile() {
	Next_Tile(NV_Nxt_Tile, Pos_i, New_Vel);
}

Vec_2<float> Actor::Get_New_Vel() {
	return *New_Vel;
}

Vec_2<int> Actor::Get_NV_Nxt_Tile() {
	return *NV_Nxt_Tile;
}

bool Actor::Get_Stationary() {
	
	return* Stationary;
}

// Debug

void Actor::Debug_Console(float D_Time) {

	system("CLS");
	Set_Cursor(0, 0);

	std::cout << "ACTOR DEBUG";

	Set_Cursor(0, 2);
	std::cout << "D_Time = " << D_Time;

	Set_Cursor(0, 3);
	std::cout << "Pos_f: X = " << Pos_f->X;

	Set_Cursor(22, 3);
	std::cout << "Pos_f: Y = " << Pos_f->Y;

	Set_Cursor(0, 4);
	std::cout << "Vel: X = " << Vel->X;

	Set_Cursor(22, 4);
	std::cout << "Vel: Y = " << Vel->Y;

	Set_Cursor(0, 5);
	std::cout << "Pos_i: X = " << Pos_i->X;

	Set_Cursor(22, 5);
	std::cout << "Pos_i: Y = " << Pos_i->Y;

	Set_Cursor(0, 6);
	std::cout << "Nxt_Tile: X = " << Nxt_Tile->X;

	Set_Cursor(22, 6);
	std::cout << "Nxt_Tile: Y = " << Nxt_Tile->Y;

	Set_Cursor(0, 7);
	std::cout << "Stationary = " << *Stationary;

	Set_Cursor(0, 8);
	std::cout << "New_Vel: X = " << New_Vel->X;

	Set_Cursor(22, 8);
	std::cout << "New_Vel: Y = " << New_Vel->Y;


	std::cout << std::endl;
}

// Following booleans are logic tests necessary for restricting movement.

bool Actor::Next_Tile_Traversable(Level *In_Level) {

	// Checks that next tile on current velocity isn't a barrier.

	if (In_Level->Get_Tile_Data(Nxt_Tile->X, Nxt_Tile->Y).Traversable) {

		return true;
	}
	else {

		return false;
	}


}

bool Actor::NV_Next_Tile_Traversable(Level * In_Level)
{
	if (In_Level->Get_Tile_Data(NV_Nxt_Tile->X, NV_Nxt_Tile->Y).Traversable) {

		return true;
	}
	else {

		return false;
	}
}

bool Actor::At_Tile_Center(float D_Time) {

	// Center point of a tile is 

	Vec_2<float>* Temp_Pos = new Vec_2<float>{ Pos_f->X,Pos_f->Y };
	
	New_Pos(Temp_Pos, Vel, D_Time);

	//if (Magnitude(Vec_2<float>{Pos_f->X - (float)Pos_i->X, Pos_f->Y - (float)Pos_i->Y}) 
	//	< Magnitude(Vec_2<float>{Pos_f->X - Temp_Pos->Y, Pos_f->Y - Temp_Pos->Y})) {

	//	//std::cout << "AT TILE CENTER";
	//	return true;
	//}
	if (Magnitude(Vec_2<float>{Pos_f->X - (float)Pos_i->X, Pos_f->Y - (float)Pos_i->Y}) < 0.25) {

		//std::cout << "AT TILE CENTER";
		return true;
	}
	else {

		return false;
	}
}

bool Actor::Wall_Proximity() {
	
	// Code needs removing, not used.

	if (Magnitude(Vec_2<float>{ Pos_f->X - Nxt_Tile->X, Pos_f->Y - Nxt_Tile->Y }) < 1.0f) {

		return true;
	}
	else {

		return false;
	}


}

///////////////////////////////////
// CLASS PACMAN MEMBER FUNCTIONS //
///////////////////////////////////

Pacman::Pacman() {


}

Pacman::Pacman(Vec_2<float> &Start_Pos) {

	*Pos_f = Start_Pos;
	Set_Pos_i();
	Set_Vel(Vec_2<float> {-1.0f*Base_Speed, 0.0f*Base_Speed});
	Set_New_Vel(Vec_2<float> {1.0f*Base_Speed, 0.0f*Base_Speed});
	Set_Next_Tile();
	Set_NV_Nxt_Tile();
	*Stationary = false;
}

Pacman::~Pacman() {

}

//////////////////////////////////
// CLASS GHOST MEMBER FUNCTIONS //
//////////////////////////////////

Ghost::Ghost() {

	In_Pacman = new Pacman;

	// Target is the main driving force behind AI Pathfinding.
	Target = new Vec_2<float>;
	Start_Point = new Vec_2<int>;

	Current_State = new State;

	Dot_Count = new int(0);

	Vel_Modifier = new float(1.0f);

	Stationary_Timer = new Timer();
	Stationary_Timer->Set_Timer(0.5f);
}

Ghost::~Ghost() {

	delete Target;
	delete Start_Point;
	delete Current_State;
	delete Dot_Count;
	delete Vel_Modifier;
}

void Ghost::On_Tick(Level* In_Level, Pacman* In_Pacman, float D_Time) {

}

Vec_2<float> Ghost::Get_Target() {

	return *Target;
}

State Ghost::Get_State() {

	return *Current_State;
}

void Ghost::Set_State(State New_State) {

	*Current_State = New_State;
}

void Ghost::Inc_Dot_Count() {

	// Ghosts exit the house when Pacman has eaten a certain amount of dots.

	*Dot_Count = *Dot_Count + 1;
}

int Ghost::Get_Dot_Cnt() {

	return *Dot_Count;
}

void Ghost::Scattering_Target() {

	// Code implementation maintained within derived classes
}

void Ghost::Chasing_Target() {

}

void Ghost::Decide_Vel_From_Order(Level* In_Level, Vec_2<int>* In_Pos_i) {

	if (N_Valid(In_Level, In_Pos_i)) {

		Set_New_Vel(Vec_2<float> {0.0f, -1.0f * Base_Speed * *Vel_Modifier});

	}
	else if (W_Valid(In_Level, In_Pos_i)) {

		Set_New_Vel(Vec_2<float> {-1.0f * Base_Speed * *Vel_Modifier, 0.0f});
	}
	else if (S_Valid(In_Level, In_Pos_i)) {

		Set_New_Vel(Vec_2<float> {0.0f, 1.0f * Base_Speed * *Vel_Modifier});
	}
	else {

		Set_New_Vel(Vec_2<float> { 1.0f * Base_Speed * *Vel_Modifier, 0.0f});
	}

}

bool Ghost::Stationary_Fix(float D_Time) {

	// The AI ghosts may have a tendency to remain stationary, this is a fix to simply restart
	// movement when a ghost has been stationary for longer than a second.
	
	if (Stationary_Timer->Update(D_Time)) {

		Stationary_Timer->Set_Timer(0.5);
		return true;

	}
	else { return false; }
 }

bool Ghost::N_Valid(Level* In_Level, Vec_2<int>* In_Pos_i) {

	// Checks if NORTH is a valid movement tile, must be traversable and not the current tile actor is on.

	if (In_Level->Get_Tile_Data(In_Pos_i->X, In_Pos_i->Y - 1).Traversable) {

		if (In_Pos_i->X == Pos_i->X && In_Pos_i->Y - 1 == Pos_i->Y) {

			return false;
		}
		else {

			return true;
		}
	}
	else {

		return false;
	}

}

bool Ghost::E_Valid(Level* In_Level, Vec_2<int>* In_Pos_i) {

	// Checks if EAST is a valid movement tile, must be traversable and not the current tile actor is on.

	if (In_Level->Get_Tile_Data((In_Pos_i->X + 1), In_Pos_i->Y).Traversable) {

		if (In_Pos_i->X + 1 == Pos_i->X && In_Pos_i->Y == Pos_i->Y) {

			return false;
		}
		else {

			return true;
		}
	}
	else {

		return false;
	}
}

bool Ghost::S_Valid(Level* In_Level, Vec_2<int>* In_Pos_i) {

	// Checks if SOUTH is a valid movement tile, must be traversable and not the current tile actor is on.

	if (In_Level->Get_Tile_Data(In_Pos_i->X, (In_Pos_i->Y + 1)).Traversable) {

		if (In_Pos_i->X == Pos_i->X && In_Pos_i->Y+1 == Pos_i->Y
			|| (In_Level->Get_Tile_Data(In_Pos_i->X, (In_Pos_i->Y + 1)).Ghost_Only && (Current_State == !Dead))) {

			return false;
		}
		else {

			return true;
		}
	}
	else {

		return false;
	}
}

bool Ghost::W_Valid(Level* In_Level, Vec_2<int>* In_Pos_i) {

	// Checks if WEST is a valid movement tile, must be traversable and not the current tile actor is on.

	if (In_Level->Get_Tile_Data((In_Pos_i->X - 1), In_Pos_i->Y).Traversable) {

		if ((In_Pos_i->X - 1) == Pos_i->X && In_Pos_i->Y == Pos_i->Y) {

			return false;
		}
		else {

			return true;
		}
	}
	else {

		return false;
	}
}

bool Ghost::On_Junction_Tile(Level* In_Level, Vec_2<int>* In_Pos_i) {

	// Checks if specified counts as a junction, 3 adjacent tiles.

	int Valid_Dir = 0;

	if (N_Valid(In_Level, In_Pos_i)) { Valid_Dir++; };
	if (E_Valid(In_Level, In_Pos_i)) { Valid_Dir++; };
	if (S_Valid(In_Level, In_Pos_i)) { Valid_Dir++; };
	if (W_Valid(In_Level, In_Pos_i)) { Valid_Dir++; };

	if (Valid_Dir > 2) {

		return true;
	}
	else {

		return false;
	}


}

void Ghost::Decide_New_Vel(Level* In_Level, float D_Time) {

	float Shortest = 100;

	if (Vel->X == New_Vel->X && Vel->Y == New_Vel->Y) { Allow_Dir_Change = true; }
	else { Allow_Dir_Change = false; }

	if (At_Tile_Center(D_Time) && Allow_Dir_Change) {

		// Picks the valid direction that would put Ghost closest to it's target.

		int Direction = 0;

			// Check North

		if (Magnitude(Vec_2<float> {(float)Nxt_Tile->X - Target->X, ((float)Nxt_Tile->Y - 1) - Target->Y }) < Shortest && N_Valid(In_Level, Nxt_Tile)) {

			Shortest = Magnitude(Vec_2<float> {(float)Nxt_Tile->X - Target->X, ((float)Nxt_Tile->Y - 1) - Target->Y });

			Direction = 0;
		}

			// Check East

		if (Magnitude(Vec_2<float> {((float)Nxt_Tile->X + 1) - Target->X, (float)Nxt_Tile->Y - Target->Y }) < Shortest && E_Valid(In_Level, Nxt_Tile)) {

			Shortest = Magnitude(Vec_2<float> {((float)Nxt_Tile->X + 1) - Target->X, (float)Nxt_Tile->Y - Target->Y });

			Direction = 1;
		}

			// Check South

		if (Magnitude(Vec_2<float> {(float)Nxt_Tile->X - Target->X, ((float)Nxt_Tile->Y + 1) - Target->Y }) < Shortest && S_Valid(In_Level, Nxt_Tile)) {

			Shortest = Magnitude(Vec_2<float> {(float)Nxt_Tile->X - Target->X, ((float)Nxt_Tile->Y + 1) - Target->Y });

			Direction = 2;
		}

			// Check West

		if (Magnitude(Vec_2<float> {((float)Nxt_Tile->X - 1) - Target->X, (float)Nxt_Tile->Y - Target->Y }) < Shortest && W_Valid(In_Level, Nxt_Tile)) {

			Shortest = Magnitude(Vec_2<float> {((float)Nxt_Tile->X - 1) - Target->X, (float)Nxt_Tile->Y - Target->Y });

			Direction = 3;

		}

		switch (Direction) {

			case 0: {
				Set_New_Vel(Vec_2<float> {0.0f, -1.0f * Base_Speed * *Vel_Modifier});
				break;
			}
			case 1: {
				Set_New_Vel(Vec_2<float> { 1.0f * Base_Speed * *Vel_Modifier, 0.0f});
				break;
			}
			case 2: {
				Set_New_Vel(Vec_2<float> {0.0f, 1.0f * Base_Speed * *Vel_Modifier});
				break;
			}
			case 3: {
				Set_New_Vel(Vec_2<float> {-1.0f * Base_Speed * *Vel_Modifier, 0.0f});
				break;
			}
		}
	}

	if (Nxt_Tile->X < -1) {

		// Fix for issue regarding ghosts becoming stuck in the offscreen tunnels. With Next Tile values being negative 2 ^ 31.
		// Simply identifies the issue and replaces it where necessary.

		*Stationary = true;
		Decide_Vel_From_Order(In_Level, Pos_i);
	}

	//if (Pos_i->X != Start_Point->X && Pos_i->Y != Start_Point->Y)


}

bool Ghost::Exit_House()
{

	// Defines when Pacman may leave the house.

	return false;
}

////////////////////////////////
// CLASS RED MEMBER FUNCTIONS //
////////////////////////////////

Red::Red(Vec_2<float>& Start_Pos) {

	*Pos_f = Start_Pos;
	Set_Pos_i();
	Set_Vel(Vec_2<float> {0.0f*Base_Speed, 0.0f*Base_Speed});
	Set_New_Vel(Vec_2<float> {0.0f*Base_Speed, 0.0f*Base_Speed});
	*Stationary = true;
	*Current_State = Waiting;
	*Start_Point = Vec_2<int>{ (int)Start_Pos.X, (int)Start_Pos.Y };
}

Red::~Red() {

}

void Red::On_Tick(Level* In_Level, Pacman* In_Pacman, float D_Time) {

	switch (*Current_State) {
	
		case (Waiting): {

			// Wait to exit the house.

			// Due to nxt_tile showing excessive negative values as a result of zero velocity,
			// Manually setting the values here is a fix for the issue.

			Nxt_Tile->X = 0;
			Nxt_Tile->Y = 0;

			if (Exit_House()) {

				*Vel_Modifier = 0.9375f;

				Set_Pos(Vec_2<float>{ 15.5f, 14.0f });
				Scattering_Target();
				Set_Vel(Vec_2<float> {1.0f * Base_Speed * *Vel_Modifier, 0.0f});
				Set_New_Vel(Vec_2<float> {1.0f * Base_Speed * *Vel_Modifier, 0.0f});
				Set_Next_Tile();
				Set_NV_Nxt_Tile();

				*Stationary = false;
				Update(In_Level, D_Time);

				*Current_State = Scattering;
			}

			break;
		}
		case (Scattering): {
			
			*Vel_Modifier = 0.9375f;

			Scattering_Target();

			Update(In_Level, D_Time);
			Decide_New_Vel(In_Level, D_Time);

			break;
		}
		case (Chasing): {

			*Vel_Modifier = 0.9375f;

			Chasing_Target(In_Pacman);

			Update(In_Level, D_Time);
			Decide_New_Vel(In_Level, D_Time);

			break;
		}
		case (Frightened): {

			*Vel_Modifier = 0.625f;

			*Target = Vec_2<float>{ (In_Pacman->Get_Pos().X - Pos_f->X)*-1, (In_Pacman->Get_Pos().Y - Pos_f->Y)*-1 };

			Update(In_Level, D_Time);
			Decide_New_Vel(In_Level, D_Time);

			break;
		}
		case (Dead): {

			*Vel_Modifier = 3.0f;

			*Target = Vec_2<float>{ 15.5f, 14.0f };
			Update(In_Level, D_Time);
			Decide_New_Vel(In_Level, D_Time);

			if (Magnitude(Vec_2<float>{Pos_f->X - Target->X, Pos_f->Y - Target->Y }) < 1.0f) {

				Scattering_Target();
				Decide_New_Vel(In_Level, D_Time);
				Update(In_Level, D_Time);

				*Current_State = Waiting;
			}

			break;
		}
	}
}

void Red::Scattering_Target() {

	*Target = Vec_2<float>{ 29.0f, 0.0f };
}

void Red::Chasing_Target(Pacman* In_Pacman) {

	*Target = In_Pacman->Get_Pos();
}

bool Red::Exit_House()
{
	// Exit House is always true for red.

	return true;
}

/////////////////////////////////
// CLASS Teal MEMBER FUNCTIONS //
/////////////////////////////////

Teal::Teal(Vec_2<float>& Start_Pos) {

	Red_Pos = new Vec_2<float>;
	*Pos_f = Start_Pos;
	Set_Pos_i();
	Set_Vel(Vec_2<float> {0.0f*Base_Speed, 0.0f*Base_Speed});
	Set_New_Vel(Vec_2<float> {0.0f*Base_Speed, 0.0f*Base_Speed});
	*Stationary = true;
	*Current_State = Waiting;
	*Start_Point = Vec_2<int>{ (int)Start_Pos.X, (int)Start_Pos.Y };

}

Teal::~Teal() {

	delete Red_Pos;
}

void Teal::On_Tick(Level * In_Level, Pacman * In_Pacman, float D_Time) {


	switch (*Current_State) {

		case (Waiting): {

			// Wait to exit the house.

			// Due to nxt_tile showing excessive negative values as a result of zero velocity,
			// Manually setting the values here is a fix for the issue.

			Nxt_Tile->X = 0;
			Nxt_Tile->Y = 0;

			if (Exit_House()) {

				*Vel_Modifier = 0.9375f;

				Set_Pos(Vec_2<float>{ 15.5f, 14.0f });
				Scattering_Target();
				Set_Vel(Vec_2<float> {1.0f * Base_Speed * *Vel_Modifier, 0.0f});
				Set_New_Vel(Vec_2<float> {1.0f * Base_Speed * *Vel_Modifier, 0.0f});
				Set_Next_Tile();
				Set_NV_Nxt_Tile();

				*Stationary = false;
				Update(In_Level, D_Time);

				*Current_State = Scattering;
			}

			break;
		}
		case (Scattering): {

			*Vel_Modifier = 0.9375f;

			Scattering_Target();

			Update(In_Level, D_Time);
			Decide_New_Vel(In_Level, D_Time);

			break;
		}
		case (Chasing): {

			*Vel_Modifier = 0.9375f;

			Chasing_Target(In_Pacman);

			Update(In_Level, D_Time);
			Decide_New_Vel(In_Level, D_Time);

			break;
		}
		case (Frightened): {

			*Vel_Modifier = 0.625f;

			*Target = Vec_2<float>{ (In_Pacman->Get_Pos().X - Pos_f->X)*-1, (In_Pacman->Get_Pos().Y - Pos_f->Y)*-1 };

			Update(In_Level, D_Time);
			Decide_New_Vel(In_Level, D_Time);

			break;
		}
		case (Dead): {

			*Vel_Modifier = 3.0f;

			*Target = Vec_2<float>{ 15.5f, 14.0f };
			Update(In_Level, D_Time);
			Decide_New_Vel(In_Level, D_Time);

			if (Magnitude(Vec_2<float>{Pos_f->X - Target->X, Pos_f->Y - Target->Y }) < 1.0f) {

				Scattering_Target();
				Decide_New_Vel(In_Level, D_Time);
				Update(In_Level, D_Time);

				*Current_State = Waiting;
			}

			break;
		}
	}
}

void Teal::Scattering_Target() {

	*Target = Vec_2<float>{ 28.0f, 35.0f };
}

void Teal::Chasing_Target(Pacman * In_Pacman) {

	// Pos_2_Ahead is two tiles INFRONT of pacman.

	Vec_2 <float> Pos_2_Ahead = { In_Pacman->Get_Pos().X + (Normalize(In_Pacman->Get_Vel()).X * 2), In_Pacman->Get_Pos().Y + (Normalize(In_Pacman->Get_Vel()).Y * 2) };

	// Target is infront of Pacman

	*Target = Vec_2<float>{ (Pos_2_Ahead.X - Red_Pos->X) * 2, (Pos_2_Ahead.X - Red_Pos->X) * 2 };
}

void Teal::Set_Red_Pos(Red* In_Red) {

	 *Red_Pos = In_Red->Get_Pos();
}

bool Teal::Exit_House() {
	
	if (*Dot_Count > 30) {

		return true;
	}
	else {

		return false; 
	}

}

/////////////////////////////////
// CLASS Pink MEMBER FUNCTIONS //
/////////////////////////////////

Pink::Pink(Vec_2<float>& Start_Pos) {

	*Pos_f = Start_Pos;
	Set_Pos_i();
	Set_Vel(Vec_2<float> {0.0f*Base_Speed, 0.0f*Base_Speed});
	Set_New_Vel(Vec_2<float> {0.0f*Base_Speed, 0.0f*Base_Speed});
	*Stationary = true;
	*Current_State = Waiting;
	*Start_Point = Vec_2<int>{ (int)Start_Pos.X, (int)Start_Pos.Y };
}

Pink::~Pink() {

}

void Pink::On_Tick(Level * In_Level, Pacman * In_Pacman, float D_Time) {


	switch (*Current_State) {

		case (Waiting): {

			// Wait to exit the house.

			// Due to nxt_tile showing excessive negative values as a result of zero velocity,
			// Manually setting the values here is a fix for the issue.

			Nxt_Tile->X = 0;
			Nxt_Tile->Y = 0;

			if (Exit_House()) {

				*Vel_Modifier = 0.9375f;

				Set_Pos(Vec_2<float>{ 15.5f, 14.0f });
				Scattering_Target();
				Set_Vel(Vec_2<float> {1.0f * Base_Speed * *Vel_Modifier, 0.0f});
				Set_New_Vel(Vec_2<float> {1.0f * Base_Speed * *Vel_Modifier, 0.0f});
				Set_Next_Tile();
				Set_NV_Nxt_Tile();

				*Stationary = false;
				Update(In_Level, D_Time);

				*Current_State = Scattering;
			}

			break;
		}
		case (Scattering): {

			*Vel_Modifier = 0.9375f;

			Scattering_Target();

			Update(In_Level, D_Time);
			Decide_New_Vel(In_Level, D_Time);

			break;
		}
		case (Chasing): {

			*Vel_Modifier = 0.9375f;

			Chasing_Target(In_Pacman);

			Update(In_Level, D_Time);
			Decide_New_Vel(In_Level, D_Time);

			break;
		}
		case (Frightened): {

			*Vel_Modifier = 0.625f;

			*Target = Vec_2<float>{ (In_Pacman->Get_Pos().X - Pos_f->X)*-1, (In_Pacman->Get_Pos().Y - Pos_f->Y)*-1 };

			Update(In_Level, D_Time);
			Decide_New_Vel(In_Level, D_Time);

			break;
		}
		case (Dead): {

			*Vel_Modifier = 3.0f;

			*Target = Vec_2<float>{ 15.5f, 14.0f };
			Update(In_Level, D_Time);
			Decide_New_Vel(In_Level, D_Time);

			if (Magnitude(Vec_2<float>{Pos_f->X - Target->X, Pos_f->Y - Target->Y }) < 1.0f) {

				Scattering_Target();
				Decide_New_Vel(In_Level, D_Time);
				Update(In_Level, D_Time);

				*Current_State = Waiting;
			}

			break;
		}
	}
}

void Pink::Scattering_Target() {

	*Target = Vec_2<float>{ 4.0f, 0.0f };
}

void Pink::Chasing_Target(Pacman * In_Pacman) {

	*Target = Vec_2<float>{ In_Pacman->Get_Pos().X + (Normalize(In_Pacman->Get_Vel()).X * 2), In_Pacman->Get_Pos().Y + (Normalize(In_Pacman->Get_Vel()).Y * 2) };
}

bool Pink::Exit_House() {

	if (*Dot_Count > 10) {

		return true;
	}
	else {

		return false;
	}
}

///////////////////////////////////
// CLASS Orange MEMBER FUNCTIONS //
///////////////////////////////////

Orange::Orange(Vec_2<float>& Start_Pos) {

	*Pos_f = Start_Pos;
	Set_Pos_i();
	Set_Vel(Vec_2<float> {1.0f*Base_Speed, 0.0f*Base_Speed});
	Set_New_Vel(Vec_2<float> {1.0f*Base_Speed, 0.0f*Base_Speed});
	*Stationary = true;
	*Current_State = Waiting;
	*Start_Point = Vec_2<int>{ (int)Start_Pos.X, (int)Start_Pos.Y };
}

Orange::~Orange() {

}

void Orange::On_Tick(Level * In_Level, Pacman * In_Pacman, float D_Time) {


	switch (*Current_State) {

		case (Waiting): {

			// Wait to exit the house.

			// Due to nxt_tile showing excessive negative values as a result of zero velocity,
			// Manually setting the values here is a fix for the issue.

			Nxt_Tile->X = 0;
			Nxt_Tile->Y = 0;

			if (Exit_House()) {

				*Vel_Modifier = 0.9375f;

				Set_Pos(Vec_2<float>{ 15.5f, 14.0f });
				Scattering_Target();
				Set_Vel(Vec_2<float> {1.0f * Base_Speed * *Vel_Modifier, 0.0f});
				Set_New_Vel(Vec_2<float> {1.0f * Base_Speed * *Vel_Modifier, 0.0f});
				Set_Next_Tile();
				Set_NV_Nxt_Tile();

				*Stationary = false;
				Update(In_Level, D_Time);

				*Current_State = Scattering;
			}

			break;
		}
		case (Scattering): {

			*Vel_Modifier = 0.9375f;

			Scattering_Target();

			Update(In_Level, D_Time);
			Decide_New_Vel(In_Level, D_Time);

			break;
		}
		case (Chasing): {

			*Vel_Modifier = 0.9375f;

			Chasing_Target(In_Pacman);

			Update(In_Level, D_Time);
			Decide_New_Vel(In_Level, D_Time);

			break;
		}
		case (Frightened): {

			*Vel_Modifier = 0.625f;

			*Target = Vec_2<float>{ (In_Pacman->Get_Pos().X - Pos_f->X)*-1, (In_Pacman->Get_Pos().Y - Pos_f->Y)*-1 };

			Update(In_Level, D_Time);
			Decide_New_Vel(In_Level, D_Time);

			break;
		}
		case (Dead): {

			*Vel_Modifier = 3.0f;

			*Target = Vec_2<float>{ 15.5f, 14.0f };
			Update(In_Level, D_Time);
			Decide_New_Vel(In_Level, D_Time);

			if (Magnitude(Vec_2<float>{Pos_f->X - Target->X, Pos_f->Y - Target->Y }) < 1.0f) {

				Scattering_Target();
				Decide_New_Vel(In_Level, D_Time);
				Update(In_Level, D_Time);

				*Current_State = Waiting;
			}

			break;
		}
	}
}

void Orange::Scattering_Target() {

	*Target = Vec_2<float>{ 2.0f, 35.0f };
}

void Orange::Chasing_Target(Pacman * In_Pacman) {

	if (Magnitude(Vec_2<float> {Get_Pos().X - In_Pacman->Get_Pos().X, Get_Pos().Y - In_Pacman->Get_Pos().Y}) < 8) {

		*Target = Vec_2<float>{ 2.0f, 35.0f };
	}
	else {

		*Target = In_Pacman->Get_Pos();
	}
}

bool Orange::Exit_House() {

	if (*Dot_Count > 60) {

		return true;
	}
	else {

		return false;
	}
}
