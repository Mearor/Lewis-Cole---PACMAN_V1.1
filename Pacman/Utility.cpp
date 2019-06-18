#include "Utility.h"
#include <Windows.h>

void Set_Cursor(int X, int Y) {

	HANDLE Output = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD Pos = { X , Y };
	SetConsoleCursorPosition(Output, Pos);
}

Timer::Timer() {

	Cur_Time = new float;
}

Timer::~Timer() {

	delete Cur_Time;
}

void Timer::Set_Timer(float In_Start) {

	*Cur_Time = In_Start;
}

bool Timer::Update(float On_Tick) {

	*Cur_Time = *Cur_Time - On_Tick;
	
	if (*Cur_Time < 0.0f) {

		return true;
	}
	else {

		return false;
	}
}

float Timer::Get_Cur_Time()
{
	return *Cur_Time;
}
