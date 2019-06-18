#include "Drawer.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <iostream>

Drawer::Drawer(SDL_Window* aWindow, SDL_Renderer* aRenderer)
: myWindow(aWindow)
, myRenderer(aRenderer)
{

	// Loading the textures as SDL_Surfaces into memory preemptively to see if it
	// cuts rendering time.

}

Drawer::~Drawer(void)
{

	delete myWindow;
	delete myRenderer;
	delete world;

}

bool Drawer::Init()
{
	if (!myWindow)
		return false;

	return true;
}

void Drawer::Draw(const char* anImage, int aCellX, int aCellY)
{
	world = IMG_Load( anImage ) ;

	if (!world)
		return;

	SDL_Texture* optimizedSurface = SDL_CreateTextureFromSurface(myRenderer, world);

    SDL_Rect sizeRect;
    sizeRect.x = 0 ;
    sizeRect.y = 0 ;
    sizeRect.w = world->w ;
    sizeRect.h = world->h ;

    SDL_Rect posRect ;
    posRect.x = aCellX;
    posRect.y = aCellY;
	posRect.w = sizeRect.w;
	posRect.h = sizeRect.h;

	SDL_RenderCopy(myRenderer, optimizedSurface, &sizeRect, &posRect);

	SDL_DestroyTexture(optimizedSurface);
	SDL_FreeSurface(world);

}

void Drawer::Draw_From_File(SDL_Surface* In_Surface, int aCellX, int aCellY) {

	if (!In_Surface) {
		
		std::cout << "ERROR";

		return;
	}

	SDL_Texture* optimizedSurface = SDL_CreateTextureFromSurface(myRenderer, In_Surface);

	SDL_Rect sizeRect;
	sizeRect.x = 0;
	sizeRect.y = 0;
	sizeRect.w = In_Surface->w;
	sizeRect.h = In_Surface->h;

	SDL_Rect posRect;
	posRect.x = aCellX;
	posRect.y = aCellY;
	posRect.w = sizeRect.w;
	posRect.h = sizeRect.h;

	SDL_RenderCopy(myRenderer, optimizedSurface, &sizeRect, &posRect);

	SDL_DestroyTexture(optimizedSurface);
}


void Drawer::DrawText(const char* aText, const char* aFontFile, int aX, int aY) {

	TTF_Font* font=TTF_OpenFont(aFontFile, 24);

	SDL_Color fg={255,0,0,255};
	SDL_Surface* surface = TTF_RenderText_Solid(font, aText, fg);

	SDL_Texture* optimizedSurface = SDL_CreateTextureFromSurface(myRenderer, surface);

    SDL_Rect sizeRect;
    sizeRect.x = 0 ;
    sizeRect.y = 0 ;
    sizeRect.w = surface->w ;
    sizeRect.h = surface->h ;

    SDL_Rect posRect ;
    posRect.x = aX;
    posRect.y = aY;
	posRect.w = sizeRect.w;
	posRect.h = sizeRect.h;

	SDL_RenderCopy(myRenderer, optimizedSurface, &sizeRect, &posRect);
	SDL_DestroyTexture(optimizedSurface);
	SDL_FreeSurface(surface);
	TTF_CloseFont(font);
}

