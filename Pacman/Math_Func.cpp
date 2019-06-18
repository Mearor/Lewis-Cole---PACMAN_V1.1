#include <math.h>
#include "Math_Func.h"
#include "Constants.h"

float Magnitude(Vec_2<float> In) {

	// Magnitude of In Vector

	return sqrtf(powf(In.X, 2.0f) + powf(In.Y, 2.0f));

}

Vec_2<float> Normalize(Vec_2<float> In) {

	// Returns a Normalized Copy of In vector

	return Vec_2<float>{ In.X / Magnitude(In), In.Y / Magnitude(In) };
}

void Next_Tile(Vec_2 <int>* Next_Tile, Vec_2<int>* In_Pos, Vec_2<float>* In_Vel) {

	// Calculates the next tile based on input velocity, necessary for navigation system

	Vec_2<float> Norm_Vel = Normalize(*In_Vel);

	*Next_Tile = { ((int)Norm_Vel.X + In_Pos->X), ((int)Norm_Vel.Y + In_Pos->Y) };
};

void New_Pos(Vec_2<float>* In_Pos, Vec_2<float>* In_Vel, float D_Time) {

	// Calculates new position based on Vec_2

	In_Pos->X = In_Pos->X + (In_Vel->X * D_Time);
	In_Pos->Y = In_Pos->Y + (In_Vel->Y * D_Time);
}

Vec_2<int> Render_Pos(Vec_2<float> In_Pos) {

	// Float to pixel,

	int X = ((In_Pos.X - 2) * Tile_Size) - 3 ;
	int Y = ((In_Pos.Y) * Tile_Size) - 3;

	return Vec_2<int> { X, Y };
}
