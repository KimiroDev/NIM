#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <chrono>
#include "variables.h"
#include "button.h"
#include "pile.h"
#include "bot.h"

//when start_game is true the piles are filled up again
void setrestart_gametotrue()
{
    if(scene==1)restart_game=true;
}

void changeDefaultTurn()
{
    turn = 1 - turn;
    default_turn = turn;
}

Button new_game_hidden(0, 11, 24, 69, "", setrestart_gametotrue);

std::vector<Pile> piles;

bool thinking = false;
bool moving = false;
auto start = std::chrono::high_resolution_clock::now();
auto stop = start;
int to_pile(0), to_coin(0);

void game()
{
    //display
	for (size_t i = 0; i < 5; i++)
	{
		piles[i].draw(screen);
	}

    /* check at which state the game is */
    if (sum() <= 1 || cols() == 0)
    {
        if (game_end == false)
            game_just_ended = true;
        else game_just_ended = false;
        game_end = true;
    }
    else game_end = game_just_ended = false;

    if (pile_updated)
        game_begin = false;

    pile_updated = false;


    if (!game_end)// game still running
    {
		if (game_mode == true || turn == 0)
		{
			for (size_t i = 0; i < 5; ++i)
				nrCoins[i + 1] = piles[i].update(mi, mj);
            if (sum() == 0)
                game_end = game_just_ended = true;
		}
		else 
		{
			if (!thinking)
			{
				thinking = true;
				start = std::chrono::high_resolution_clock::now();
				srand(time(0));
                to_pile = rand() % 5 + 1;
                play(to_pile, to_coin);
                if (to_coin == 0) to_coin = 1;
                if (to_coin > nrCoins[to_pile]+1) {
                    throw(to_coin * 100 + nrCoins[to_pile]);
                }
			}
			else
			{
				if (!moving)
				{
					stop = std::chrono::high_resolution_clock::now();
					auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
					if (duration.count() > 0 * game_begin)
					{
						moving = true;
						start = std::chrono::high_resolution_clock::now();
					}
				}
				else
				{
					stop = std::chrono::high_resolution_clock::now();
					auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
					if (duration.count() > 200000)
					{
                        if (to_pile > pile_selected)
                        {
                            ++pile_selected;
                            while (!nrCoins[pile_selected] && pile_selected < 5)
                                ++pile_selected;
                        }
                        else if (to_pile < pile_selected)
                        {
                            --pile_selected;
                            while (!nrCoins[pile_selected] && pile_selected > 1)
                                --pile_selected;
                        }
						else if (to_coin > coin_selected)
							++coin_selected;
						else if (to_coin < coin_selected)
							--coin_selected;
						else
						{
							while ((int)piles[to_pile-1].coins.size() >= to_coin && (int)piles[to_pile - 1].coins.size() >= 1)
								piles[to_pile-1].coins.pop_back();
                            nrCoins[to_pile] = to_coin;
                            if (to_coin == 1) {
                                piles[to_pile - 1].coins.clear();
                                nrCoins[to_pile] = 0;
                            }
							pile_updated = true;
							game_begin = false;
							updated = true;
							coin_selected = nrCoins[to_pile];
                            thinking = moving = false;
							if (!coin_selected)
							{
								pile_selected++;
								coin_selected = 1;
							}
						}
						start = std::chrono::high_resolution_clock::now();
                        if (sum() == 0)
                        {
                            game_end = game_just_ended = true;
                        }
					}
				}
			}
			for (size_t i = 0; i < 5; i++)
			{
				piles[i].hovered_cell = 0;
			}
            if (pile_selected > 5) pile_selected = 5;
			if (nrCoins[pile_selected] >= coin_selected)
				piles[pile_selected-1].hovered_cell = coin_selected;
			else piles[pile_selected-1].hovered_cell = nrCoins[pile_selected];
		}
    }
    if (game_end == true) // game has ended with a winner
    {
        /* decide winner and modify score accordingly */
        if (game_just_ended)
        {
            if (sum() == 1)
            {
                if (turn == 0)
                    { wstrcpy(winner, player1); ++score1; }
                else
                    { wstrcpy(winner, player2); ++score2; }
            }
            else
            {
                if (turn == 1)
                    { wstrcpy(winner, player2); ++score2; }
                else
                    { wstrcpy(winner, player1); ++score1; }
            }

        }

        /* display winner */
        int i = print(winner, 35 + (10 - wstrlen(winner) - 1), int(10));
        print("won", i);

        /* display score */
        i = print(player1, 31 + (10 - wstrlen(player1) - 1), int(12));
        i = print(score1, i);
        i = print('|', i);
        i = print(score2, i);
        print(player2, i);

        print("Click anywhere to start a new game", 12, 24);

        if (!pile_updated && (//can't begin new game in the same frame as it ended
            keyboard.isKeyPressed(VK_RETURN) || keyboard.isKeyPressed(VK_SPACE)))//restart upon key press
                restart_game = true;

        if (!pile_updated)//can't begin new game in the same frame as it ended
            new_game_hidden.update();//restart upon mouse press
    }

    if(updated)//a move has been made
    {
        turn = 1 - turn;//next player's turn
        updated = false;
    }

    if(restart_game)
    {
		for (size_t i = 0; i < 5; i++)
		{
			piles[i].reset(14 * (i + 1), unsigned short (rand() % 5 + 1));
		}
        nrCoins[2] = 2;//necessary... for some reason

        turn = default_turn;
        restart_game = false;
        score_updated = false;
        game_begin = true;

        coin_selected = 1;
        pile_selected = 1;
    }

}


#endif // GAME_H_INCLUDED
