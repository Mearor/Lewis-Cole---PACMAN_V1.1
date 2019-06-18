#ifndef MATH_FUNC_H
#define MATH_FUNC_H

template <typename T>

// Using a template typename T as this software uses the Vec_2 to store floats and integers.

struct Vec_2 {

	// Data Structure to hold vector data.

	T X;
	T Y;
};

float Magnitude(Vec_2<float> In);

Vec_2<float> Normalize(Vec_2<float> In);

void Next_Tile(Vec_2 <int>* Next_Tile, Vec_2<int>* In_Pos, Vec_2<float>* In_Vel);

void New_Pos(Vec_2<float>* In_Pos, Vec_2<float>* In_Vel, float D_Time);

Vec_2<int> Render_Pos(Vec_2<float> In_Pos);

Vec_2<int> Render_Lerp(Vec_2<float> In_Pos);

int Max(int a, int b);
#endif