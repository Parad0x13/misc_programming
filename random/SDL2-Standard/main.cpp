/*****
	SDL2 Tutorials being followed from: http://lazyfoo.net/tutorials/SDL/index.php#Hello
*****/

#include <iostream>
#include "SDL.h"

#define WIDTH	640
#define HEIGHT	480

using namespace std;

SDL_Window* window;
SDL_Surface* screenSurface;
SDL_Surface* square;

void init_SDL() {
	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	screenSurface = SDL_GetWindowSurface(window);
}

void close_SDL() {
	SDL_FreeSurface(square);
	square = NULL;

	SDL_DestroyWindow(window);
	window = NULL;

	SDL_Quit();
}

void loadMedia() {
	square = SDL_LoadBMP("resources/images/Square.bmp");
	if(square == NULL)
		cout << "Unable to load resource" << endl;
}

int main(int argc, char* args[]) {
	init_SDL();
	loadMedia();

	SDL_Event event;
	bool quit = false;
	while(!quit) {
		while(SDL_PollEvent(&event) != 0) {
			if(event.type == SDL_QUIT)
				quit = true;
		}

		SDL_BlitSurface(square, NULL, screenSurface, NULL);
		SDL_UpdateWindowSurface(window);
	}

	close_SDL();

	return 0;
}