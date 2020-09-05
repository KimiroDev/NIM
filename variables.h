#ifndef VARIABLES_H_INCLUDED
#define VARIABLES_H_INCLUDED

//#define _WIN32_WINNT 0x0601 //not always necessary

// Unicode needs to be enabled so that
//WHITESPACE character can be used
#ifndef UNICODE
#define UNICODE
#define UNICODE_WAS_UNDEFINED
#endif

#include <Windows.h>

#ifdef UNICODE_WAS_UNDEFINED
#undef UNICODE
#endif

#define WHITE_SPACE 9608

#include <vector>
#include <time.h>
#include <chrono>
#include <fstream>

const int screen_width = 80, screen_height = 25;//screen dimensions in columns/rows
int cell_count = screen_height * screen_width;//number of total cells on screen
int cell_size_px_x = 8, cell_size_px_y = 12; //used to calculate over which cell the mouse hovers
int pile_turn = 1;//iterates from pile 1 to 5 and updates them in order
int pile_selected = 1, coin_selected = 1;//remembers which pile/coin are selected (by keyboard input)
int default_turn = 0;//retains the preference of who starts the rounds
int turn = 0;//indicated who's player turn is
int scene = 1;//indicates the current scene(game/options/how_to_play scene)
int nrCoins[] = {0, 1, 2, 3, 4, 5};//keeps count of the coins (on each pile)
int mi, mj;//the line and column over which the mouse hovers
int score1 = 0, score2 = 0;//keep track of score
int difficulty = 2;//for single player mode:
				   //1 = easy, 2 = medium, 3 = hard

using text = wchar_t[];
text player1 = L"Jonathan\0   ";//can be modified at runtime
text player2 = L"Alice\0     ";//can be modified at runtime
text winner = L"\0\0\0\0\0\0\0\0\0\0";//for displaying the winner player
//text over_score = {175, 175, 175, 175, 175, 175, 175, 175, 0};
//text horizontal_line = {9472, 9472, 9472, 9472, 9472, 9472, 9472, 9472, 9472, 9472, 9472, 0};

bool running = true;//the boolean used for the game loop
bool updated = false;//indicates if the turn has changed
bool score_updated = false;//checks if the score has been updated
bool pile_updated = false;//only 1 pile can be updated in a single frame
bool widgets_updated;//makes sure multiple widgets can't be open in the same frame
bool restart_game = false;//when true, the piles get filled and the game begins again
bool game_begin = true;//if true, the game has just begun; used to block players to
//manually change default turn in the middle of the game
bool game_end = false;//game has ended (one player has won)
bool game_just_ended = false;//the first frame of the end of the game
bool game_mode = true;//false = 1 player, true = 2 players

wchar_t *screen = new wchar_t[screen_width*screen_height];//console graphic buffer

std::vector<wchar_t*> names;

void resetScore()
{ score1 = score2 = 0; }

/* there are 3 scenes: howToPlay=0, game=1, options=2 */
void howToPlayScene()
{ scene = 0; }
void gameScene()
{ scene = 1; }
//optionsScene declared in options.h

size_t wstrlen(wchar_t* n)
{
    size_t i = 0;
    while(n[i])++i;
    return i;
}

size_t wstrlen(const wchar_t* n)
{
    size_t i = 0;
    while (n[i])++i;
    return i;
}

void wstrcpy(wchar_t * destination, const wchar_t *origin)
{
    size_t it = 0;
    while (origin[it] != 0)
        { destination[it] = origin[it]; ++it; }
    destination[it] = 0;
}

void wstrcpy(wchar_t* destination, const char* origin)
{
    size_t it = 0;
    while (origin[it] != 0)
    {
        destination[it] = origin[it]; ++it;
    }
    destination[it] = 0;
}

void setrunningtofalse()//end game loop
{ running = false; }

void quit()
{ exit(1); }

void players_1()
{
	game_mode = false;
	restart_game = true;
    if (default_turn == 1)
        wstrcpy(player1, "Computer");
    else wstrcpy(player2, "Computer");
}

void players_2()
{
	game_mode = true;
	restart_game = true;
    if (turn == 1)
    {
        if (names[0][0] != player1[0])
        {
            wstrcpy(player1, names[1]);
        }
        else
            wstrcpy(player1, names[0]);
    }
    else
    {
        if (names[0][0] != player2[0])
        {
            wstrcpy(player2, names[1]);
        }
        else
            wstrcpy(player2, names[0]);
    }

    turn = 0;
}

#endif // VARIABLES_H_INCLUDED
