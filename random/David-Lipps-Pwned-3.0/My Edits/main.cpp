/*	Things I've noticed with your syntax
	Redundant whitespace is redundant, and should be eradicated with extreme passion
	E.g.
	method( input );	// Wrong
	method(input);		// Right

	fubar(one,two,three);	// Wrong
	fubar(one, two, three);	// Right

	void method() {			// Wrong
							// Wrong
		doSomething();		// Wrong
							// Wrong
	}						// Wrong

	void method() {			// Right
		doSomething();		// Right
	}						// Right

	if ( ... )	// Wrong
	if(...)		// Right

	// Wrong
	if(somethingHappened) {
		doSomething();
	}
	// Right
	if(somethingHappened)
		doSomething
	// Also Right
	if(somethingHappend)doSomething;

	// Never create getter and setter methods in your classes manually
	int getX() {return x};			// Wrong
	int setX(int _X) {x = _x;);		// Wrong
	// Instead make your variables under public: and getter/setter methods will automatically be created
	int number = class.variable;	// Right
	class.variable = 5;				// Right

	for(x = 0;x < 9;x++) {...		// Wrong
	for(x = 0;x < maxX;x++) {...	// Right

	// When defining #declares vs consts ensure you use a common standard for both conventions
	#define DIRECTION_UP 1
	or
	int const DirectionUp = 1;
	// Remember to always prefix your consts or declares with an easy to remember tag so your IDE will be more responsive
	For example don't do this:
	#define UP_DIRECTION ...
	#define DOWN_DIRECTION ...
	Do this instead:
	#define DIRECTION_UP ...
	#define DIRECTION_DOWN ...
	// That way you can just type 'DIRECTION' and see all available options

	// Ensure your variable naming conventions are logical
	// For example it's common to only capitalize the first letter of a variable if it's global or const
	const int NumberThatWontChange = 7;
	globalScope {ImportantNumber = 7;}
	int numberThatWillChange = 6;
	nonGlobalScope {importantNumber = 6;}
	// Also remember if your using a global variable your probably doing something very wrong. Globals are bad m'kay

	if(value == true)	// Wrong
	if(value)			// Right
	if(value == false)	// Wrong
	if(!value)			// Right

	private:	// Wrong
private:		// Right
	public:		// Wrong
public:			// Right

	// Comments are the DEVIL, never EVER use comments to explain what a bit of code does EVER
	// Comments are a crutch for the weak, good programmers NEVER use comments to explain code that SHOULD explain itself
	For example take a look at this bit of code:
	float gForce = algorithm(6, 2, &input, valid_ID);	// computes the required value for turbo liftoff
	That is a slap in the face of any programmer worth a damn. Instead do this:
	float gForce = turboLiftoffGForceComputation(6, 2, &input, valid_ID);

	// Any method should do only one thing, and one thing well. Ever.
	For instance this is bad and I see it in some of your methods:
	void doSomething() {
		// Do a bunch of stuff related to math
		// Do a bunch of stuff related to visuals
	}
	This should become:
	void doSomething() {
		doMaths();
		doVisuals();
	}
	void doMaths() {
		...
	}
	void doVisuals() {
		...
	}
	// This is one of the most important goals of refactoring Object Oriented code
	// Any method where I wrote: "This should be refactored out of this method for clarity" is a perpatrator of linar coding

	// Although SDL's own libraries make use of Rect.x and Rect.y in your OWN code you should really use a Point type Class or struct
	player.x = 6;		// Wrong
	player.pos.x = 6;	// Right
	This will allow another level of abstraction as identified below:
	player.x = 6;player.y = 8;	// Linear
	player.loc = Point(6, 8);	// Abstracted
	player.loc += Point(2, -4);	// Abstracted
	player.loc -= enemy.loc;	// Abstracted
*/

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>

#define SCREEN_WIDTH	1024
#define SCREEN_HEIGHT	720
#define SCREEN_BPP		32
#define FRAMERATE		30

#define DIRECTION_UP	1000
#define DIRECTION_DOWN	1001
#define DIRECTION_LEFT	1002
#define DIRECTION_RIGHT	1003

//using namespace std

int rnd(int range) {return rand()%range;}
void seed() {srand((unsigned)time(NULL));}

const int GameBoxCountX = 8, GameBoxCountY = 8;
const int MaxBoxNumber = 9;

SDL_Surface *load_image(std::string filename) {
	SDL_Surface *loadedImage = NULL;
	SDL_Surface *optimizedImage = NULL;

	loadedImage = IMG_Load(filename.c_str());
	if(loadedImage != NULL) {
		optimizedImage = SDL_DisplayFormatAlpha(loadedImage);
		SDL_FreeSurface(loadedImage);
	}

	Uint32 colorkey = SDL_MapRGB(optimizedImage->format, 0xFF, 0xFF, 0xFF);
	SDL_SetColorKey(optimizedImage, SDL_SRCCOLORKEY, colorkey);

	return optimizedImage;
}

void apply_surface(int x, int y, SDL_Surface *source, SDL_Surface *destination) {
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;

	SDL_BlitSurface(source, NULL, destination, &offset);
}

std::string int_to_str(int number) {
	std::ostringstream convert;
	convert << number;
	return convert.str();
}

class Timer {
public:
	Timer();
	void start();
	void stop();
	void pause();
	void unpause();
	int get_tick();

	int startTick, pausedTick;
	bool isPaused, isStarted;
};

Timer::Timer() {
	startTick = 0;
	pausedTick = 0;
	isPaused = false;
	isStarted = false;
}

void Timer::start() {
	isStarted = true;
	isPaused = false;
	startTick = SDL_GetTicks();
}

void Timer::stop() {
	isStarted = false;
	isPaused = false;
}

int Timer::get_tick() {
	if(isStarted && isPaused)
		return pausedTick;
	else
		return SDL_GetTicks() - startTick;
	return false;
}

void Timer::pause() {
	if(isStarted && !isPaused) {
		isPaused = true;
		pausedTick = SDL_GetTicks() - startTick;
	}
}

void Timer::unpause() {
	if(isPaused) {
		isPaused = false;
		startTick = SDL_GetTicks() - pausedTick;
		pausedTick = 0;
	}
}

class Box {
public:
	int x, y;
	int owner, value;
};

class Cursor {
public:
	void move(int direction);
	void display_cursor();

	int x, y;
};

void Cursor::move(int direction) {
	switch(direction) {
		case DIRECTION_UP:
			y--;
		break;
		case DIRECTION_DOWN:
			y++;
		break;
		case DIRECTION_LEFT:
			x--;
		break;
		case DIRECTION_RIGHT:
			x++;
		default:
		break;
	}
}

class Game {
	SDL_Surface *screen = NULL;
	SDL_Surface *background = NULL;
	SDL_Surface *cursor_image = NULL;
	SDL_Surface *coin_one = NULL;
	SDL_Surface *coin_two = NULL;
	SDL_Surface *text = NULL;
	TTF_Font *font = NULL;
	SDL_Color textColor;
	Cursor cursor;
	Timer countdown;
	SDL_Event event;

	int turns, player;
	int seconds_remaining;
	int turn_num;
	// Even better, refactor this grid variable to be dynamically scalable. malloc it instead and create setup methods for it
	Box grid[GameBoxCountX][GameBoxCountY];

public:
	Game();
	void new_game();
	void display();
	void put_text(int,int,std::string);
	bool event_handler();
	bool runtime_logic();
	void fps_throttle();
	void clear_memory();
	void swap_player();
	void place_coin();
	void next_turn();
	bool end_game();

	Timer fps_timer;
};

Game::Game() {
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
	SDL_WM_SetCaption("PWNED", NULL);

	background = load_image("BoardBG.jpg");
	cursor_image = load_image("Cursor.png");
	coin_one = load_image("coin_one.png");
	coin_two = load_image("coin_two.png");
	font = TTF_OpenFont("comicbd.ttf", 56);
	textColor = {0, 0, 0};

	new_game();
}

void Game::new_game() {
	turns = 0;
	player = 1;
	// Refactor this static value '9' out of the method
	turn_num = rand()%MaxBoxNumber;
	cursor.x = 0;
	cursor.y = 0;

	for(int y=0;y < GameBoxCountX;y++) {
		for(int x=0;x < GameBoxCountY;x++) {
			grid[x][y].owner = 0;
			grid[x][y].x = x;
			grid[x][y].y = y;
			grid[x][y].value = 0;
		}
	}
	countdown.start();
}

void Game::display() {
	// Clear the screen using SDL. This should be refactored out of this method for clarity
	SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
	apply_surface(0, 0, background, screen);

	if(player == 1) {
		textColor = {200, 0, 0};
		put_text(87, 80, "one");
		textColor = {0, 0, 0};
	} else if(player == 2) {
		textColor = {0, 200, 0};
		put_text(87, 80, "two");
		textColor = {0, 0, 0};
	}
	// Future player checks will be conducted here

	put_text(95, 240, int_to_str(seconds_remaining));
	put_text(110, 470, int_to_str(turn_num));

	// This should be refactored out of this method for clarity
	int boardXStart = 300, boardYStart = 35;
	for(int y=0;y < GameBoxCountY;y++ ) {
		for(int x=0;x < GameBoxCountX;x++) {
			if(grid[x][y].owner == 1)
				apply_surface(grid[x][y].x * 85 + boardXStart + coin_one->w/4.0f, grid[x][y].y * 80 + boardYStart + coin_one->h/4.0f, coin_one, screen);
			else if(grid[x][y].owner == 2)
				apply_surface(grid[x][y].x * 85 + boardXStart + coin_two->w/4.0f, grid[x][y].y * 80 + boardYStart + coin_two->h/4.0f, coin_two, screen);

			if(grid[x][y].value != 0) {
				//
			}
		}
	}

	//if(player == 1)
	//	apply_surface(cursor.x * 85 + 300, cursor.y * 80 + 35, coin_one, screen);
	//else if(player == 2)
	//	apply_surface(cursor.x * 85 + 300, cursor.y * 80 + 35, coin_two, screen);

	// This should be refactored out of this method for clarity
	for(int y=0;y < GameBoxCountY;y++) {
		for(int x=0; x < GameBoxCountX; x++) {
			if(grid[x][y].value != 0 )
				put_text(grid[x][y].x * 85 + 327, grid[x][y].y * 80 + 35, int_to_str(grid[x][y].value));
		}
	}

	apply_surface(cursor.x * 85 + 300, cursor.y * 80 + 35, cursor_image, screen);

	SDL_Flip(screen);
}

void Game::put_text(int x, int y, std::string str) {
	//text = TTF_RenderText_Solid(font, str.c_str(), textColor);
	text = TTF_RenderText_Blended(font, str.c_str(), textColor);
	apply_surface(x, y, text, screen);
}

bool Game::event_handler() {
	while(SDL_PollEvent(&event)) {
		if(event.type == SDL_KEYDOWN) {
			int key = event.key.keysym.sym;
			if(key == SDLK_ESCAPE)
				return true;
			if(key == SDLK_UP || key == SDLK_w)
				if(cursor.y > 0)cursor.move(DIRECTION_UP);
			if(key == SDLK_DOWN || key == SDLK_s)
				if(cursor.y < GameBoxCountY-1)cursor.move(DIRECTION_DOWN);
			if(key == SDLK_LEFT || key == SDLK_a)
				if(cursor.x > 0)cursor.move(DIRECTION_LEFT);
			if(key == SDLK_RIGHT || key == SDLK_d)
				if(cursor.x < GameBoxCountX-1)cursor.move(DIRECTION_RIGHT);
			if(key == SDLK_RETURN)
				place_coin();
		}
		if(event.type == SDL_QUIT)return true;
	}
	return false;
}

void Game::fps_throttle() {
	if((fps_timer.get_tick() < 1000 / FRAMERATE))
		SDL_Delay((1000 / FRAMERATE) - fps_timer.get_tick());
}

void Game::clear_memory() {
	SDL_FreeSurface(background);
	SDL_FreeSurface(text);
	SDL_FreeSurface(coin_one);
	SDL_FreeSurface(coin_two);
	SDL_FreeSurface(cursor_image);

	TTF_Quit();
	SDL_Quit();
}

void Game::swap_player() {
	if(player == 1)player = 2;
	else if(player == 2)player = 1;
	// Future player checks will be conducted here
}

void Game::place_coin() {
	if(grid[cursor.x][cursor.y].value == 0) {
		countdown.stop();
		grid[cursor.x][cursor.y].value = (turn_num);
		grid[cursor.x][cursor.y].owner = player;

		//Flips coin owner
		if(cursor.y != 0 ) {
			if((grid[cursor.x][cursor.y - 1].value <= turn_num) && (grid[cursor.x][cursor.y - 1].value != 0))
				grid[cursor.x][cursor.y - 1].owner = player;
		}
		if(cursor.y != 7) {
			if((grid[cursor.x][cursor.y + 1].value <= turn_num) && (grid[cursor.x][cursor.y + 1].value != 0))
				grid[cursor.x][cursor.y + 1].owner = player;
		}
		if(cursor.x != 0) {
			if((grid[cursor.x - 1][cursor.y].value <= turn_num) && (grid[cursor.x - 1][cursor.y].value != 0))
				grid[cursor.x - 1][cursor.y].owner = player;
		}
		if(cursor.x != 7) {
			if((grid[cursor.x + 1][cursor.y].value <= turn_num) && (grid[cursor.x + 1][cursor.y].value != 0))
				grid[cursor.x + 1][cursor.y].owner = player;
		}
		if((cursor.x != 7) && (cursor.y != 0)) {
			if((grid[cursor.x + 1][cursor.y - 1].value <= turn_num) && (grid[cursor.x + 1][cursor.y - 1].value != 0))
				grid[cursor.x + 1][cursor.y - 1].owner = player;
		}
		if((cursor.x != 7) && (cursor.y != 7)) {
			if((grid[cursor.x + 1][cursor.y + 1].value <= turn_num) && (grid[cursor.x + 1][cursor.y + 1].value != 0) )
				grid[cursor.x + 1][cursor.y + 1].owner = player;
		}
		if((cursor.x != 0) && (cursor.y != 7)) {
			if((grid[cursor.x - 1][cursor.y + 1].value <= turn_num) && (grid[cursor.x - 1][cursor.y + 1].value != 0))
				grid[cursor.x - 1][cursor.y + 1].owner = player;
		}
		if((cursor.x != 0) && (cursor.y != 0)) {
			if((grid[cursor.x - 1][cursor.y - 1].value <= turn_num) && (grid[cursor.x - 1][cursor.y - 1].value != 0))
				grid[cursor.x - 1][cursor.y - 1].owner = player;
		}
		turns++;
		next_turn();
	}
}

void Game::next_turn() {
	swap_player();
	// Refactor this static value '9' out of the method
	turn_num = rand()%MaxBoxNumber;
	countdown.start();
}

bool Game::runtime_logic() {
	if(turns == 64)return end_game();

	// Refactor the 30000 out of being a hardcoded value
	if(countdown.get_tick() < 30000)
		seconds_remaining = 30 - int(countdown.get_tick() / 1000);
	else {
		countdown.stop();
		next_turn();
	}

	return event_handler();
}

bool Game::end_game() {
	new_game();
	return false;
}

int main(int argc, char * args[]) {
	seed();
	Game game;

	bool quit = false;
	while(!quit) {
		game.fps_timer.start();
		quit = game.runtime_logic();
		game.display();
		game.fps_throttle();
	}

	game.clear_memory();
	return false;
}
