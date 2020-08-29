#ifndef PILE_H_INCLUDED
#define PILE_H_INCLUDED

text unselected_coin_texture = L" ######## ############################## ######## ";
text selected_coin_texture = L" ######## ############################## ######## ";
//selected_coin_texture is painted to white in main() (the white character can't be copy-pasted)

class Coin
{
public:
    int left_margin, j1, right_margin, j2;

    Coin(int i, int j, int isize, int jsize)
    {
        left_margin = i;
        j1 = j;
        right_margin = i + isize;
        j2 = j + jsize;
    }

    bool hovered(int mi, int mj)
    {
        if(mi >= left_margin && mi < right_margin && mj >= j1 && mj <j2)
        {
            if((mi==left_margin || mi == right_margin-1) && (mj == j1 || mj == j2-1))
                return false;
            return true;
        }
        return false;
    }

    bool _clicked(int mi, int mj)
    {
        if(hovered(mi, mj) && mouse.isClicked())
            return true;
        return false;
    }
};

//current and last hovered element by mouse
int  mcoin_selected = 0,  mpile_selected = 0;
int lmcoin_selected = 0, lmpile_selected = 0;
bool input_type = true;//true=mouse false=keyboard
bool has_moved = false;//remembers if the selected coin/pile has
//been moved after a change of input type (mouse or keyboard)

//contains coins. when (almost) all Pile instances
//are empty the game is won
//hard coded size of 10 cells
//useless to declare >5 coins
class Pile
{
public:

    /*   position (left side), number of coins it will have*/
    Pile(unsigned short i , unsigned short nrcoins)
    {
        left_margin = i;
        right_margin = left_margin + 10;
        nPileWidth = 10;

        coins.clear();
        for(int ii = 1; ii <= nrcoins; ii++)
            coins.push_back(Coin(screen_height-(ii*5), i, 5, 10));
    }

    //does the same as the constructor
    void reset(unsigned short i , unsigned short nrcoins)
    {
        left_margin = i;
        right_margin = left_margin + 10;
        nPileWidth = 10;
        coins.clear();

        for(int ii = 1; ii <= nrcoins; ii++)
            coins.push_back(Coin(screen_height-(ii*5), i, 5, 10));
    }

    int update(int mi, int mj)
    {

        if (pile_turn > 5)//end of frame, when all piles have been updated
        {
            pile_turn = 1;

            if (input_type == false)
            {
                has_moved = false;
                if (keyboard.isKeyPressed(VK_LEFT)
                  ||keyboard.isKeyPressed('A')) {pile_selected--;has_moved=true;}
                if (keyboard.isKeyPressed(VK_UP)
                  ||keyboard.isKeyPressed('W')) {coin_selected++;has_moved=true;}
                if (keyboard.isKeyPressed(VK_RIGHT)
                  ||keyboard.isKeyPressed('D')) {pile_selected++;has_moved=true;}
                if (keyboard.isKeyPressed(VK_DOWN)
                  ||keyboard.isKeyPressed('S')) {coin_selected--;has_moved=true;}
            }

            if (coin_selected < 1)coin_selected = 1;
                if (pile_selected < 1)
                {
                    pile_selected = 5;
                    while (nrCoins[pile_selected] == 0)
                        --pile_selected;
                }
                if (coin_selected > 5)coin_selected = 5;
                if (pile_selected > 5)pile_selected = 1;
        }

        if (coins.size() == 0)
        {
            if(pile_selected == pile_turn)
            {
                if (keyboard.isKeyPressed(VK_LEFT)
                  ||keyboard.isKeyPressed('A'))
                    while (nrCoins[pile_selected] == 0)--pile_selected;
                else while (nrCoins[pile_selected] == 0)++pile_selected;
            }
        }
        else
        {
            hovered_cell = 0;
            bool found = false;

            for(size_t i = 0; i< coins.size(); i++){
                if(coins[i].hovered(mi, mj))
                {
                    found = true;
                    mcoin_selected = int(i + 1);
                    i = coins.size();//i goes to end and ends loop

                    if (mouse.isClicked())
                    {
                        while ((int)coins.size() >= mcoin_selected)
                            coins.pop_back();
                        updated = true;
                        pile_updated = true;
                        game_begin = false;
                        coin_selected = (int)coins.size();
                    }
                }
            }

            if (found)
            {
                mpile_selected = pile_turn;
            }

            if (input_type == true)
            {
                if(found)
                {
                    hovered_cell = mcoin_selected;
                }


                if (
                    keyboard.isKeyPressed('W') ||
                    keyboard.isKeyPressed('A') ||
                    keyboard.isKeyPressed('S') ||
                    keyboard.isKeyPressed('D') ||
                    keyboard.isKeyPressed(VK_LEFT) ||
                    keyboard.isKeyPressed(VK_UP)   ||
                    keyboard.isKeyPressed(VK_RIGHT)||
                    keyboard.isKeyPressed(VK_DOWN)
                   )
                {
                    input_type = false;
                    lmcoin_selected = mcoin_selected;
                    lmpile_selected = mpile_selected;

                    if (keyboard.isKeyPressed(VK_LEFT)
                      ||keyboard.isKeyPressed('A'))
                    {
                        coin_selected = mcoin_selected;
                        pile_selected = mpile_selected - 1;
                    }
                    if (keyboard.isKeyPressed(VK_UP)
                      ||keyboard.isKeyPressed('W'))
                    {
                        coin_selected = mcoin_selected + 1;
                        pile_selected = mpile_selected;
                    }
                    if (keyboard.isKeyPressed(VK_RIGHT)
                      ||keyboard.isKeyPressed('D'))
                    {
                        coin_selected = mcoin_selected;
                        pile_selected = mpile_selected + 1;
                    }
                    if (keyboard.isKeyPressed(VK_DOWN)
                      ||keyboard.isKeyPressed('S'))
                    {
                        coin_selected = mcoin_selected - 1;
                        pile_selected = mpile_selected;
                    }
                }

            }
            else if (input_type == false)
            {
                ///
                if(pile_selected == pile_turn)
                    hovered_cell = coin_selected;

                if(pile_turn == pile_selected)
                {
                    hovered_cell = coin_selected;

                    if (coin_selected >= (int)coins.size()
                      &&(keyboard.isKeyPressed(VK_DOWN) || keyboard.isKeyPressed('S')))
                    {
                        if (coin_selected > (int)coins.size() || has_moved)
                            coin_selected = int(coins.size() - 1);
                    }
                    if (coin_selected > (int)coins.size()
                      &&(keyboard.isKeyPressed(VK_UP) || keyboard.isKeyPressed('W')))
                        --coin_selected;

                    if(hovered_cell > (int)coins.size())
                            hovered_cell = (int)coins.size();

                    if ((keyboard.isKeyPressed(VK_RETURN) || keyboard.isKeyPressed(VK_SPACE))
                      &&pile_updated == false)
                    {
                        while((int)coins.size() >= hovered_cell)
                                    coins.pop_back();
                        pile_updated = true;
                        game_begin = false;
                        updated = true;
                        coin_selected = (int)coins.size();
                        if (!coin_selected)
                        {
                            pile_selected++;
                            if (pile_selected > 5)
                                pile_selected = 5;
                            coin_selected = 1;
                        }
                    }
                }

                if (mcoin_selected != lmcoin_selected
                  ||mpile_selected != lmpile_selected)
                    input_type = true;
            }
        }

        ++pile_turn;
        return (int)coins.size();
    }

    void draw(wchar_t*& screen)
    {

        int i = -1;
        while(++i<nPileWidth*screen_height)gBuffer[i]=L' ';

        if(hovered_cell)
        {
            for(int i = 0; i<hovered_cell-1; i++)
            {
                for(int j = 0; j<50; j++)
                {
                    gBuffer[i*nPileWidth*5 + j] = unselected_coin_texture[j];
                }
            }
            for(unsigned short i = hovered_cell-1; i<coins.size(); i++)
                for(int j = 0; j<50; j++)
                    gBuffer[i*nPileWidth*5 + j] = selected_coin_texture[j];
        }
        else
        {
            for(unsigned short i = 0; i<coins.size(); i++)
            for(int j = 0; j<50; j++)
        {
           gBuffer[i*nPileWidth*5 + j] = unselected_coin_texture[j];
        }
        }

        int qq = 0;
        bool done = false;
        int gb = screen_height*nPileWidth-1;

        while(!done && qq < screen_height)
        {
            for(int ww = 0; ww < nPileWidth; ww++)
            {

                if(gb)
                {
                    int aux = qq*screen_width+ww+left_margin;
                    if(aux < cell_count) screen[aux] = gBuffer[gb--];
                }

                else done=true;//*/
            }
            qq++;
        }//*/
    }
	int hovered_cell = 0;
	std::vector<Coin>coins;

private:

    int left_margin = 0, right_margin = 0;
    unsigned short nPileWidth = 0;
    wchar_t gBuffer[25*50];
};

#endif // PILE_H_INCLUDED
