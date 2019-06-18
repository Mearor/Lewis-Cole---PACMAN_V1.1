#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

static std::string Level_File_Path = "Level_Data.txt";

// Strings for easy reference when drawing

static char* Big_Dot = "Big_Dot_32.png";
static char* Small_Dot = "Small_Dot_32";

static char* Pacman_Right_1 = "closed_32.png";
static char* Pacman_Right_2 = "open_32.png";
static char* Pacman_Down_1 = "closed_down_32.png";
static char* Pacman_Down_2 = "open_down_32.png";
static char* Pacman_Left_1 = "closed_left_32.png";
static char* Pacman_Left_2 = "open_left_32.png";
static char* Pacman_Up_1 = "closed_up_32.png";
static char* Pacman_Up_2 = "open_right_32.png";

static char* Ghost_Red = "ghost_32_red.png";
static char* Ghost_Teal = "ghost_32_cyan.png";
static char* Ghost_Pink = "ghost_32_pink.png";
static char* Ghost_Orange = "ghost_32_orange.png";
static char* Ghost_Vulnerable = "Ghost_Vulnerable_32.png";
static char* Ghost_Flash = "ghost_32.png";
static char* Ghost_Dead = "Ghost_Dead_32.png";

const float Base_Speed = 5.0f;
const float Dead_Modifier = 1.5f;

const int Screen_X = 336;
const int Screen_Y = 432;
const int Tile_Size = 12;


#endif