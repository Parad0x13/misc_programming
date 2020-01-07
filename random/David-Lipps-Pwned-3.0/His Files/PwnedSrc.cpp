#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>

//Program Constants
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 720;
const int SCREEN_BPP = 32;
const int FRAMERATE = 30;

SDL_Surface * load_image(std::string filename) {

    SDL_Surface * loadedImage = NULL;
    SDL_Surface * optimizedImage = NULL;

    loadedImage = IMG_Load( filename.c_str() );
    if ( loadedImage != NULL ) {

        optimizedImage = SDL_DisplayFormat( loadedImage );
        SDL_FreeSurface( loadedImage );
    }

    Uint32 colorkey = SDL_MapRGB( optimizedImage->format, 0xFF, 0xFF, 0xFF );
    SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, colorkey );

    return optimizedImage;

}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination ) {

    SDL_Rect offset;
    offset.x = x;
    offset.y = y;

    SDL_BlitSurface( source, NULL, destination, &offset );

}

std::string int_to_str(int number) {

    std::ostringstream convert;
    convert << number;
    return convert.str();

}

class Timer {

    int startTick;
    int pausedTick;
    bool paused;
    bool started;

    public:

    Timer();
    void start();
    void stop();
    void pause();
    void unpause();
    int get_tick();
    bool is_started();
    bool is_paused();

};

Timer::Timer() {

    startTick = 0;
    pausedTick = 0;
    paused = false;
    started = false;

}

void Timer::start() {

    started = true;
    paused = false;
    startTick = SDL_GetTicks();

}

void Timer::stop() {

    started = false;
    paused = false;

}

int Timer::get_tick() {

    if( started == true) {

        if (paused == true) {return pausedTick;} else {return SDL_GetTicks() - startTick;}

    }

    return 0;

}

void Timer::pause() {

    if ( (started == true) && (paused == false) ) {

        paused = true;
        pausedTick = SDL_GetTicks() - startTick;

    }

}

void Timer::unpause() {

    if ( paused == true) {

        paused = false;
        startTick = SDL_GetTicks() - pausedTick;
        pausedTick = 0;

    }

}

bool Timer::is_started() {return started;}
bool Timer::is_paused() {return paused;}

class Box {

    int x;
    int y;
    int owner;
    int value;

    public:

    int get_owner();
    int get_value();
    int get_x();
    int get_y();
    void set_xy(int, int);
    void set_value(int);
    void set_owner(int);

};

int Box::get_owner() {return owner;}
int Box::get_value() {return value;}
int Box::get_x() {return x;}
int Box::get_y() {return y;}

void Box::set_xy(int xcoord, int ycoord) {x = xcoord; y = ycoord;}
void Box::set_owner(int num) {owner = num;}
void Box::set_value(int num) {value = num;}

class Cursor {

    int x;
    int y;

    public:

    void move_up();
    void move_down();
    void move_left();
    void move_right();
    void set_xy(int,int);
    int get_x();
    int get_y();
    void display_cursor();

};

void Cursor::move_up() {y--;}
void Cursor::move_down() {y++;}
void Cursor::move_left() {x--;}
void Cursor::move_right() {x++;}

void Cursor::set_xy(int xcoord, int ycoord) {x = xcoord; y = ycoord;}

int Cursor::get_x() {return x;}
int Cursor::get_y() {return y;}

class Game {

    SDL_Surface * screen = NULL;
    SDL_Surface * background = NULL;
    SDL_Surface * cursor_image = NULL;
    SDL_Surface * coin_one = NULL;
    SDL_Surface * coin_two = NULL;
    SDL_Surface * text = NULL;
    TTF_Font * font = NULL;
    SDL_Color textColor;
    Cursor cursor;
    Timer countdown;
    SDL_Event event;

    int turns;
    int player;
    int seconds_remaining;
    int turn_num;
    Box grid[8][8];

    public:

    Timer fps_timer;

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
    int get_random_num();
    void next_turn();
    bool end_game();

};

Game::Game() {

    SDL_Init( SDL_INIT_EVERYTHING); TTF_Init();
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
    SDL_WM_SetCaption( "PWNED", NULL );

    background = load_image( "BoardBG.jpg" );
    cursor_image = load_image( "Cursor.gif" );
    coin_one = load_image( "coin_one.gif" );
    coin_two = load_image( "coin_two.gif" );
    font = TTF_OpenFont( "comicbd.ttf", 56 );
    textColor = { 0, 0, 0 };

    new_game();

}

void Game::new_game() {

    turns = 0;
    player = 1;
    turn_num = get_random_num();
    cursor.set_xy(0,0);

    //Init grid boxes
    for (int ycoord=0; ycoord < 8; ycoord++ ) {

        for (int xcoord=0; xcoord < 8; xcoord++) {

            grid[xcoord][ycoord].set_owner(0);
            grid[xcoord][ycoord].set_xy(xcoord, ycoord);
            grid[xcoord][ycoord].set_value(0);

        }
    }

    countdown.start();

}

void Game::display() {

    //Clears the screen
    SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );
    apply_surface(0,0,background,screen);

    if (player == 1) {

        textColor = { 200, 0, 0 };
        put_text(87,80,"one");
        textColor = { 0, 0, 0 };

    } else {

        textColor = { 0, 200, 0 };
        put_text(87,80,"two");
        textColor = { 0, 0, 0 };

    }

    put_text(95,240, int_to_str(seconds_remaining));
    put_text(110,470, int_to_str(turn_num));


    //Displays coins
    for (int ycoord=0; ycoord < 8; ycoord++ ) {

        for (int xcoord=0; xcoord < 8; xcoord++) {

            if (grid[xcoord][ycoord].get_owner() == 1) {

                apply_surface(grid[xcoord][ycoord].get_x() * 85 + 300, grid[xcoord][ycoord].get_y() * 80 + 35, coin_one, screen );

                } else if (grid[xcoord][ycoord].get_owner() == 2) {

                    apply_surface(grid[xcoord][ycoord].get_x() * 85 + 300, grid[xcoord][ycoord].get_y() * 80 + 35, coin_two, screen );

                }

            if (grid[xcoord][ycoord].get_value() != 0) {

            }
        }
    }

    apply_surface(cursor.get_x() * 85 + 300, cursor.get_y() * 80 + 35, cursor_image, screen );

    //Displays grid numbers
    for (int ycoord=0; ycoord < 8; ycoord++ ) {

        for (int xcoord=0; xcoord < 8; xcoord++) {

            if ( grid[xcoord][ycoord].get_value() != 0 ) {

                put_text(grid[xcoord][ycoord].get_x() * 85 + 327, grid[xcoord][ycoord].get_y() * 80 + 35, int_to_str(grid[xcoord][ycoord].get_value()));

            }
        }
    }

    SDL_Flip( screen );

}

void Game::put_text(int x, int y, std::string str) {

    text = TTF_RenderText_Solid( font, str.c_str(), textColor );
    apply_surface(x,y,text,screen);

}

bool Game::event_handler() {

    while ( SDL_PollEvent( &event ) ) {

        if ( event.type == SDL_KEYDOWN) {

            if ( event.key.keysym.sym == SDLK_ESCAPE ) { return true;}
            switch( event.key.keysym.sym ) {

                case SDLK_UP: if (cursor.get_y() > 0) {cursor.move_up();} break;
                case SDLK_DOWN: if (cursor.get_y() < 7) {cursor.move_down();}  break;
                case SDLK_LEFT: if (cursor.get_x() > 0) {cursor.move_left();} break;
                case SDLK_RIGHT: if (cursor.get_x() < 7) {cursor.move_right();} break;
                case SDLK_RETURN: place_coin(); break;

            }

        }

        if ( event.type == SDL_QUIT ) {return true;}

    }

    return false;

}

void Game::fps_throttle() {

    if( ( fps_timer.get_tick() < 1000 / FRAMERATE ) ) { SDL_Delay( ( 1000 / FRAMERATE ) - fps_timer.get_tick() ); }

}

void Game::clear_memory() {

    SDL_FreeSurface( background );
    SDL_FreeSurface( text );
    SDL_FreeSurface( coin_one );
    SDL_FreeSurface( coin_two );
    SDL_FreeSurface( cursor_image );

    TTF_Quit();
    SDL_Quit();

}

void Game::swap_player() {

    if (player == 1) {player = 2;} else { player = 1;}

}

int Game::get_random_num() {

    srand((unsigned)time(0));
    return (rand()%9)+1;

}

void Game::place_coin() {

    if ( grid[cursor.get_x()][cursor.get_y()].get_value() == 0) {

        countdown.stop();
        grid[cursor.get_x()][cursor.get_y()].set_value(turn_num);
        grid[cursor.get_x()][cursor.get_y()].set_owner(player);

        //Flips coin owner
        if ( cursor.get_y() != 0 ) {if ( (grid[cursor.get_x()][cursor.get_y() - 1].get_value() <= turn_num) && (grid[cursor.get_x()][cursor.get_y() - 1].get_value() != 0) ) {grid[cursor.get_x()][cursor.get_y() - 1].set_owner(player);}}
        if ( cursor.get_y() != 7 ) {if ( (grid[cursor.get_x()][cursor.get_y() + 1].get_value() <= turn_num) && (grid[cursor.get_x()][cursor.get_y() + 1].get_value() != 0) ) {grid[cursor.get_x()][cursor.get_y() + 1].set_owner(player);}}
        if ( cursor.get_x() != 0 ) {if ( (grid[cursor.get_x() - 1][cursor.get_y()].get_value() <= turn_num) && (grid[cursor.get_x() - 1][cursor.get_y()].get_value() != 0) ) {grid[cursor.get_x() - 1][cursor.get_y()].set_owner(player);}}
        if ( cursor.get_x() != 7 ) {if ( (grid[cursor.get_x() + 1][cursor.get_y()].get_value() <= turn_num) && (grid[cursor.get_x() + 1][cursor.get_y()].get_value() != 0) ) {grid[cursor.get_x() + 1][cursor.get_y()].set_owner(player);}}
        if ( (cursor.get_x() != 7) && (cursor.get_y() != 0) ) {if ( (grid[cursor.get_x() + 1][cursor.get_y() - 1].get_value() <= turn_num) && (grid[cursor.get_x() + 1][cursor.get_y() - 1].get_value() != 0) ) {grid[cursor.get_x() + 1][cursor.get_y() - 1].set_owner(player);}}
        if ( (cursor.get_x() != 7) && (cursor.get_y() != 7) ) {if ( (grid[cursor.get_x() + 1][cursor.get_y() + 1].get_value() <= turn_num) && (grid[cursor.get_x() + 1][cursor.get_y() + 1].get_value() != 0) ) {grid[cursor.get_x() + 1][cursor.get_y() + 1].set_owner(player);}}
        if ( (cursor.get_x() != 0) && (cursor.get_y() != 7) ) {if ( (grid[cursor.get_x() - 1][cursor.get_y() + 1].get_value() <= turn_num) && (grid[cursor.get_x() - 1][cursor.get_y() + 1].get_value() != 0) ) {grid[cursor.get_x() - 1][cursor.get_y() + 1].set_owner(player);}}
        if ( (cursor.get_x() != 0) && (cursor.get_y() != 0) ) {if ( (grid[cursor.get_x() - 1][cursor.get_y() - 1].get_value() <= turn_num) && (grid[cursor.get_x() - 1][cursor.get_y() - 1].get_value() != 0) ) {grid[cursor.get_x() - 1][cursor.get_y() - 1].set_owner(player);}}

        turns++;
        next_turn();

    }


}

void Game::next_turn() {

    swap_player();
    turn_num = get_random_num();
    cursor.set_xy(0,0);
    countdown.start();

}

bool Game::runtime_logic() {

    if (turns == 64) {return end_game();}

    if ( countdown.get_tick() < 30000 ) {

        seconds_remaining = 30 - int(countdown.get_tick() / 1000);

    } else {countdown.stop(); next_turn();}

    return event_handler();

}

bool Game::end_game() {

    new_game();
    return false;

}

int main ( int argc, char * args[] ) {

    Game game;

    bool quit = false;
    while (quit == false) {

        game.fps_timer.start();

        quit = game.runtime_logic();

        game.display();

        game.fps_throttle();

    }

    game.clear_memory();

    return 0;
}
