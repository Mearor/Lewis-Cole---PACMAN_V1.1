#ifndef LEVEL_DATA_H
#define LEVEL_DATA_H

#include <string>
#include <iostream>
#include "Math_Func.h"

struct Tile {

	// Data structure for individual tiles of a Pacman level

	bool Traversable = true;
	bool Has_Pellet = false;
	bool Large_Pellet = false;
	bool Ghost_Only = false;

	// If there is a tile offscreen, it must allow Pacman to appear on the other side of the level.
	// Warp tile is a flag for when additional functionality need to be called to move Pacman from an offscreen
	// to it's warp partner.

	bool Warp_Tile = false;
	Vec_2 <float> Warp_Partner;
	
};

class Level {

	// Contains functionality to generate a map from text file in a 2D array.
	// Additional functionality includes getting tile data and clearing a tile.

	public:
		Level();
		~Level();

		Tile Get_Tile_Data(int Array_X, int Array_Y);
		void Clear_Tile(int Array_X, int Array_Y);

		void Read_Level(std::string File_Path);

		void Debug(int Data_Type);

		int Get_Pellet_Cnt();
		void Decr_Pellet_Cnt(int X, int Y);

		int Get_Width();
		int Get_Length();

		Vec_2 <float> Get_Pacman_Start();

		Vec_2 <float> Get_Red_Start();
		Vec_2 <float> Get_Teal_Start();
		Vec_2 <float> Get_Pink_Start();
		Vec_2 <float> Get_Orange_Start();

	private:

		int* Pellet_Cnt;
		int* Length;
		int* Width;

		Tile** Tile_Array;

		Vec_2 <float>* Pacman_Start;

		Vec_2 <float>* Red_Start;
		Vec_2 <float>* Teal_Start;
		Vec_2 <float>* Pink_Start;
		Vec_2 <float>* Orange_Start;
};

#endif