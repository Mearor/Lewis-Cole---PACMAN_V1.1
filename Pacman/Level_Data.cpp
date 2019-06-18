#include "Level_Data.h"
#include "Constants.h"
#include <iostream>
#include <fstream>
#include <conio.h>

Level::Level()
{
	Read_Level(Level_File_Path);
	//Debug(3);
	//std::cout << *Pellet_Cnt;
}

Level::~Level()
{

	delete Pellet_Cnt;
	delete Length;
	delete Width;

	delete Tile_Array;

	delete Pacman_Start;

	delete Red_Start;
	delete Teal_Start;
	delete Pink_Start;
	delete Orange_Start;
}

void Level::Read_Level(std::string File_Path) {

	// Read Level takes data from a txt file and stores it in a multidimensional array,
	// Producing a simple navigation graph for the game.


	std::ifstream File;

	File.open(File_Path);

	if (!File) {
		std::cerr << "Failure to Open File: " << File_Path << std::endl;
		exit(1);
	}
	else {

		std::string Str;

		bool Copying = false;

		Length = new int(0);
		Width = new int(0);

		while (std::getline(File, Str)) {

			// Get size of file necessary for Dynamic Array Allocation

			// Dynamic Allocation used just in case experimenting with larger
			// level sizes. Vector not necessary as the data is fixed once loaded.

			if (Str == "le") {

				Copying = false;

			}
			else if (Copying) {

				*Length = *Length + 1;


				if (Str.length() > *Width) {
					*Width = Str.length();

					//std::cout << *Width << std::endl;
				}


			}
			else if (Str == "ls") {

				Copying = true;

			}

		}

		File.clear();
		File.seekg(0, std::ios::beg);

		std::cout << *Length << std::endl;
		std::cout << *Width << std::endl;

		// Array Allocation

		Tile_Array = new Tile*[*Length];
		for (int i = 0; i < *Length; ++i)
			Tile_Array[i] = new Tile[*Width];

		int i = 0;
		int Ghosts_Assigned = 0;

		Pellet_Cnt = new int(0);

		while (std::getline(File, Str)) {

			if (Str == "le") {

				Copying = false;

			}
			else if (Copying) {

				for (int j = 0; j < Str.length(); ++j) {

					// Following if statements determine appropriate values for each value in the Tile Structure

					if (Str[j] == '.' || Str[j] == '*') {
						Tile_Array[i][j].Has_Pellet = true;

						if (Str[j] == '*') {

							Tile_Array[i][j].Large_Pellet = true;
						}

						*Pellet_Cnt = *Pellet_Cnt + 1;
					}

					if (Str[j] == '@' || Str[j] == '#') {
						Tile_Array[i][j].Traversable = false;
					}
					if (Str[j] == '-') {
						Tile_Array[i][j].Traversable = false;
					}
					if (Str[j] == '?') {


						Pacman_Start = new Vec_2<float>{ (float)j + 0.5f, (float)i};
						//Tile_Array[i][j].Has_Pellet = true;
						//*Pellet_Cnt = *Pellet_Cnt + 1;
					}
					if (Str[j] == '+') {

						Tile_Array[i][j].Warp_Tile = true;

						Tile_Array[i][j].Warp_Partner = Vec_2<float>{ (float)Str.length() - (float)j,(float)i };
					}
					if (Str[j] == '&') {

						switch (Ghosts_Assigned) {
						case 0: Red_Start = new Vec_2<float>{ (float)j + 0.5f, (float)i };
						case 1: Teal_Start = new Vec_2<float>{ (float)j + 0.5f, (float)i };
						case 2: Pink_Start = new Vec_2<float>{ (float)j + 0.5f, (float)i };
						case 3: Orange_Start = new Vec_2<float>{ (float)j + 0.5f, (float)i };
						}

						Ghosts_Assigned++;
					}

				}

				// std::cout << "line: " << i << " = " << Str << std::endl;
				++i;
			}
			else if (Str == "ls") {

				Copying = true;

			}

		}

		//std::cout << "Finished reading data, closing file: " << File_Path << std::endl;

		File.close();
	}
}

Tile Level::Get_Tile_Data(int Array_X, int Array_Y)
{
	// Getter for all data from a specific tile.

	return Tile_Array[Array_Y][Array_X];
}

void Level::Clear_Tile(int Array_X, int Array_Y)
{
	// Changes "Has_Pellet" and "Large_Pellet" to false. To be used when Pacman travels over the pellet.

	Tile_Array[Array_X][Array_Y].Has_Pellet = false;
	Tile_Array[Array_X][Array_Y].Large_Pellet = false;

}

void Level::Debug(int Data_Type)
{
	// Couts level data depending on input number

	switch (Data_Type) {

		case (0): 
			for (int i = 0; i < *Length; ++i) {

				std::cout << std::endl;
				for (int j = 0; j < *Width; ++j) {

					std::cout << Tile_Array[i][j].Ghost_Only;
				}
			}
			break;
		case (1):
			for (int i = 0; i < *Length; ++i) {

				std::cout << std::endl;
				for (int j = 0; j < *Width; ++j) {

					std::cout << Tile_Array[i][j].Has_Pellet;
				}
			}
			break;
		case (2):
			for (int i = 0; i < *Length; ++i) {

				std::cout << std::endl;
				for (int j = 0; j < *Width; ++j) {

					std::cout << Tile_Array[i][j].Large_Pellet;
				}
			}
			break;
		case (3):
			for (int i = 0; i < *Length; ++i) {

				std::cout << std::endl;
				for (int j = 0; j < *Width; ++j) {

					std::cout << Tile_Array[i][j].Traversable;
				}
			}
			break;
		case (4):
			for (int i = 0; i < *Length; ++i) {

				std::cout << std::endl;
				for (int j = 0; j < *Width; ++j) {

					std::cout << Tile_Array[i][j].Warp_Tile;
				}
			}
			break;
	}
}

int Level::Get_Pellet_Cnt()
{
	return *Pellet_Cnt;
}

void Level::Decr_Pellet_Cnt(int X, int Y) {

	// Allows Pellet Count to be decremented externally.

	*Pellet_Cnt = *Pellet_Cnt - 1;
	Tile_Array[Y][X].Has_Pellet = false;

}

int Level::Get_Width()
{
	return *Width;
}

int Level::Get_Length()
{
	return *Length;
}

// Start Points

Vec_2<float> Level::Get_Pacman_Start()
{
	return *Pacman_Start;
}

Vec_2<float> Level::Get_Red_Start()
{
	return *Red_Start;;
}

Vec_2<float> Level::Get_Teal_Start()
{
	return *Teal_Start;
}

Vec_2<float> Level::Get_Pink_Start()
{
	return *Pink_Start;;
}

Vec_2<float> Level::Get_Orange_Start()
{
	return *Orange_Start;
}


