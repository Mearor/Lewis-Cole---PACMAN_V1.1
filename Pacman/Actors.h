#ifndef ACTORS_H
#define ACTORS_H

#include "Math_Func.h"
#include "Level_Data.h"
#include "Utility.h"

// This header file is used to hold all the data regarding moving entities in the game.

class Actor {

	// Actor is base class for all entities, entity specific functionality will be relegated to derived classes.

	public:
		Actor();
		~Actor();

		void Update(Level* In_Level, float D_Time);

		void Set_Pos(Vec_2<float> New_Pos);
		void Set_Vel(Vec_2<float> New_Vel);

		void Set_Pos_i();
		void Set_Next_Tile();

		Vec_2<float> Get_Pos();
		Vec_2<float> Get_Vel();

		Vec_2<int> Get_Pos_i();
		Vec_2<int> Get_Next_Tile_i();

		void Set_New_Vel(Vec_2<float> New_Vel);
		void Set_NV_Nxt_Tile();

		Vec_2<float> Get_New_Vel();
		Vec_2<int> Get_NV_Nxt_Tile();

		bool Get_Stationary();

		virtual void Debug_Console(float D_Time);

	protected:

		// Bool member functions necessary for movement logic.

		bool Next_Tile_Traversable(Level *In_Level);
		bool NV_Next_Tile_Traversable(Level *In_Level);
		bool At_Tile_Center(float D_Time);
		bool Wall_Proximity();

		Vec_2<float>* Pos_f;
		Vec_2<float>* Vel;

		Vec_2<int>* Pos_i;
		Vec_2<int>* Nxt_Tile;

		Vec_2<float>* New_Vel;
		Vec_2<int>* NV_Nxt_Tile;

		bool* Stationary;

};

class Pacman : public Actor {

	public:

		Pacman();
		Pacman(Vec_2<float> &Start_Pos);
		~Pacman();


};

class Ghost : public Actor {

	public:
		Ghost();
		~Ghost();

		virtual void On_Tick(Level* In_Level, Pacman* In_Pacman, float D_Time);

		Vec_2<float> Get_Target();

		State Get_State();
		void Set_State(State New_State);

		void Inc_Dot_Count();
		int Get_Dot_Cnt();

	protected:

		// Data Necessary for target calculation.

		Pacman* In_Pacman;

		// Target is the main driving force behind AI Pathfinding.
		Vec_2<float>* Target;
		Vec_2<int>* Start_Point;
		
		float* Vel_Modifier;

		State* Current_State;

		virtual void Scattering_Target();
		virtual void Chasing_Target();
		void Decide_New_Vel(Level* In_Level, float D_Time);
		void Decide_Vel_From_Order(Level* In_Level, Vec_2<int>* In_Pos_i);
		bool Stationary_Fix(float D_Time);

		virtual bool Exit_House();

		bool N_Valid(Level* In_Level, Vec_2<int>* In_Pos_i);
		bool E_Valid(Level* In_Level, Vec_2<int>* In_Pos_i);
		bool S_Valid(Level* In_Level, Vec_2<int>* In_Pos_i);
		bool W_Valid(Level* In_Level, Vec_2<int>* In_Pos_i);

		bool On_Junction_Tile(Level* In_Level, Vec_2<int>* In_Pos_i);

		bool Allow_Dir_Change;

		Timer* Stationary_Timer;
		int* Dot_Count;
};

class Red : public Ghost {

	public:
		Red(Vec_2<float> &Start_Pos);
		~Red();

		void On_Tick(Level* In_Level, Pacman* In_Pacman, float D_Time);
		void Scattering_Target();
		void Chasing_Target(Pacman* In_Pacman);

		bool Exit_House();

};

class Teal : public Ghost {

	public:
		Teal(Vec_2<float> &Start_Pos);
		~Teal();

		void On_Tick(Level* In_Level, Pacman* In_Pacman, float D_Time);
		void Scattering_Target();
		void Chasing_Target(Pacman* In_Pacman);

		void Set_Red_Pos(Red* In_Red);
		bool Exit_House();

	private:
		Vec_2<float>* Red_Pos;
};

class Pink : public Ghost {

	public:
		Pink(Vec_2<float> &Start_Pos);
		~Pink();

		void On_Tick(Level* In_Level, Pacman* In_Pacman, float D_Time);
		void Scattering_Target();
		void Chasing_Target(Pacman* In_Pacman);

		bool Exit_House();
};

class Orange : public Ghost {

	public:
		Orange(Vec_2<float> &Start_Pos);
		~Orange();

		void On_Tick(Level* In_Level, Pacman* In_Pacman, float D_Time);
		void Scattering_Target();
		void Chasing_Target(Pacman* In_Pacman);

		bool Exit_House();

};
#endif