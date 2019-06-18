#ifndef UTILITY_H
#define UTILITY_H

enum State { Waiting, Scattering, Chasing, Frightened, Dead };

void Set_Cursor(int X, int Y);


class Timer {

	// Timer class created as a way of handling aspects of gameplay 

	public:
		Timer();
		~Timer();

		void Set_Timer(float In_Start);
		bool Update(float On_Tick);

		float Get_Cur_Time();

	private:
		float* Cur_Time;

};
#endif