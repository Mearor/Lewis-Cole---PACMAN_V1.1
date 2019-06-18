#ifndef DRAWER_H
#define DRAWER_H

#include "Level_Data.h"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Surface;

class Drawer
{
public:
	Drawer(SDL_Window* aWindow, SDL_Renderer* aRenderer);
	//static Drawer* Create(SDL_Window* aWindow, SDL_Renderer* aRenderer);
	~Drawer(void);

	void Draw(const char* anImage, int aCellX = 0, int aCellY = 0);
	void Draw_From_File(SDL_Surface* In_Surface, int aCellX = 0, int aCellY = 0);
	void DrawText(const char* aText, const char* aFontFile, int aX, int aY);
	//void Draw_Pellets(const char* anImage, Level* In_Level);


private:

	//SDL_Surface Get_Surface(int In_Surface);

	bool Init();
	
	SDL_Window* myWindow;
	SDL_Renderer* myRenderer;
	SDL_Surface* world;



};

#endif // DRAWER_H