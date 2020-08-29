#ifndef DROPBOX_H_INCLUDED
#define DROPBOX_H_INCLUDED

#include "variables.h"
#include "button.h"

bool __ok = false;
void setOkToTrue(){
    __ok = !__ok;
}

void dropBox(int ipos, int jpos, int isize, std::vector<wchar_t*>& wlist, wchar_t* output)
{
    widgets_updated = true;//prevents multiple drop boxes to be activated in the same frame
    COORD c = {(short)jpos, (short)ipos};//used for display
    std::vector<Button>butons;//based on the vector with names
    for(size_t i = 0; i < wlist.size(); ++i){
        butons.push_back(Button(ipos+i*2+3, jpos-2, 1, isize, wlist[i], setOkToTrue));
    }
    bool naming = false;//true when the player is writing a new name
    unsigned short new_name_iterator = 0;//iterator for new_name (see below)
    unsigned short cell_selected = 0;//if using keys, keeps track of the current selected cell
    int cell_count2 = (wlist.size() * 2 + 3/*additional empty space*/) * isize;
    wchar_t new_name[11] = {0};//stores the temporary name while writing
    wchar_t *screen2 = new wchar_t[cell_count2];//graphic buffer
    wchar_t add_wchr[10] = L"add name";//button text
    Button add(ipos+1, jpos+1, 1, isize-2, add_wchr, setOkToTrue);//used for writing new names

    //a couple print functions - handy
    auto print2 = [](wchar_t n[], int linear_pos, wchar_t* scr)
    {
        int i = 0;
        while (n[i] != 0)
        {
            scr[linear_pos + i] = n[i];
            ++i;
        }
        return linear_pos + i + 1;
    };
    auto print2xy = [print2, isize](wchar_t n[], int x, int y, wchar_t* scr)
    {
        int pos = y*isize + x;
        return print2(n, pos, scr);
    };
    auto print3 = [screen2](wchar_t n, int linear_pos)
    {
        screen2[linear_pos] = n;
        return linear_pos + 1;
    };
    auto print3xy = [screen2, isize, print3](wchar_t n, int x, int y)
    {
        int pos = y * isize + x;
        print3(n, pos);
    };

    bool loopbool = true;
    while(loopbool)
    {
        //update input information
        mouse.update();
        keyboard.update();

        //clear screen
        for(int i = cell_count2 - 1; i >= 0; i--)
            screen2[i] = L' ';

        //borders
        for(unsigned i = 0; i < wlist.size() * 2 + 2; ++i)//left & right
        {
            screen2[i * isize] = 9474;
            screen2[i * isize + isize - 1] = 9474;
        }
        for(int i = 0; i < isize; ++i)//does not have upper border
        {
            screen2[cell_count2-i] = 9644;
        }

        //corners
        screen2[cell_count2-isize]=9492;
        screen2[cell_count2-1] = 9496;

        //if the `add name` button was pressed...
        add.update();
        if(__ok)
        {
            naming = true;
            __ok = false;
        }
        //...the function enters in `naming mode`

        if(!naming)
            add.draw(screen2, isize, 1, 1);
        else
        {
            //if a letter/number ha been pressed add it to the "temporary name" on screen
            print2xy(new_name, 1, 1, screen2);
            if(keyboard.isCharPressed())
            {
                if(new_name_iterator < 10)new_name[new_name_iterator] = keyboard.keyPressed();
                if(new_name_iterator < 11)++new_name_iterator;
            }
            //deleting the last character
            if(keyboard.isKeyPressed(VK_BACK))
            {
                if(new_name_iterator > 0)--new_name_iterator;
                new_name[new_name_iterator] = 0;
            }
        }

        //check to see if any name has been picked
        for(unsigned i = 0; i < wlist.size(); i++)
        {
            if(!naming)butons[i].update();
            butons[i].draw(screen2, isize, i*2+3, 1);
            if(__ok)
            {
                int j = 0;
                while(wlist[i][j]!=0)
                {
                    output[j] = wlist[i][j];
                    ++j;
                }
                output[j] = 0;
                __ok = false;
                loopbool = false;
            }
        }

        //if user presses Enter the process of adding a new name is over
        if(naming && keyboard.isKeyPressed(VK_RETURN))
        {
            naming = false;
            wchar_t* nn = new wchar_t[11];
            for(size_t i = 0; i < 11; ++i)
                nn[i] = new_name[i];

            auto it = wlist.begin();
            wlist.insert(it, nn);
            if (wlist.size() > 5)
                wlist.pop_back();

            loopbool = false;
            for(int i = 0; i < 10; ++i)
                output[i] = new_name[i];
        }

        //key usage handler
        if (cell_selected == 0)//no cell selected yet
        {
            if ((keyboard.isKeyPressed(VK_DOWN) ||
                keyboard.isKeyPressed('S')) && !naming)
            {
                cell_selected = 1;
            }
        }
        else if (cell_selected == 1)//add name cell (first)
        {
            print3xy('<', isize-2, 1);
            if (keyboard.isKeyPressed(VK_RETURN) ||
                keyboard.isKeyPressed(VK_SPACE))
            {
                naming = true;
            }
            if ((keyboard.isKeyPressed(VK_DOWN) ||
                keyboard.isKeyPressed('S')) && !naming)
            {
                cell_selected = 2;
            }
        }
        else //a name is hovered
        {
            print3xy('<', isize-2, cell_selected*2-1);

            if (keyboard.isKeyPressed(VK_RETURN) ||
                keyboard.isKeyPressed(VK_SPACE))
            {
                naming = false;
                loopbool = false;
                for(int i = 0; i < 10; ++i)
                    output[i] = wlist[cell_selected-2][i];
            }

            if ((keyboard.isKeyPressed(VK_DOWN) ||
                keyboard.isKeyPressed('S')) && !naming)
            {
                if (cell_selected <= butons.size())cell_selected++;
            }
            else if ((keyboard.isKeyPressed(VK_UP) ||
                keyboard.isKeyPressed('W')) && !naming)
            {
                cell_selected--;
            }
        }

        if (keyboard.isKeyPressed(VK_ESCAPE))
            loopbool = false;

        //display (each row)
        c.Y = (short)ipos;
        for(unsigned un = 0; un < wlist.size() * 2 + 3; un++)
        {
            WriteConsoleOutputCharacter(hConsole, screen2 + un * isize, isize, c, &dwBytesWritten);
            c.Y = c.Y + 1;
        }

        Sleep(1);

        if(mouse.isClicked() && (mi < ipos - 2 || mi > ipos + cell_count2 / isize - 1 || mj < jpos || mj >= jpos + isize))
            loopbool = false;
    }

}

/*void dropBox2(int ipos, int jpos, std::vector<wchar_t*>& list, wchar_t* output)
{
    widgets_updated = true;//prevents multiple drop boxes to be activated in the same frame
    std::vector<Button>butons;//based on the vector with names
    size_t jsize = 0;
    for (int i = 0; i < list.size(); i++)
    {
        if (wstrlen(list[i]) > jsize)
            jsize = wstrlen(list[i]);
        print(list[i], jpos, ipos + i*2 + 1);
    }
    print((int)jsize, jpos, ipos);

    // frame 
    for (unsigned j = jpos; j <= jpos + jsize; j++)
        print('_', j, ipos-1);

    //print(wchar_t(9496), ipos * screen_width + jpos);
    //print(wchar_t(9492), ipos * screen_width + jpos);
}*/

class DropDown
{
public:

    DropDown(int x_pos, int y_pos, std::vector<const wchar_t*>&names);
    DropDown(int x_pos, int y_pos);

    void render();
    void setList(std::vector<const wchar_t*>list_);
    void update();

    ~DropDown() = default;

    int selected_option = 0;
    bool focused = false;
    int x = 0, y = 0;
    bool active = false;

private:
    
    std::vector<const wchar_t*>list;
    unsigned size_x = 0, size_y = 0;
    std::vector<Button> buttons;
    Button dropdown_button;
};

DropDown::DropDown(int x_pos, int y_pos, std::vector<const wchar_t*>&names)
{
    x = x_pos;
    y = y_pos;
    list = names;
    size_y = unsigned(names.size() * 2 + 1);
    size_x = 0;
    for (int i = 0; i < names.size(); i++)
    {
        if (wstrlen(list[i]) > size_x)
            size_x = unsigned(wstrlen(list[i]));
    }
    size_x += 4;
    dropdown_button = Button(y-1, x, 3, size_x+1, L"");
}

DropDown::DropDown(int x_pos, int y_pos)
{
    x = x_pos;
    y = y_pos;
    size_x = 10;
    size_y = 3;
    dropdown_button = Button(y - 1, x, 3, 10, L"");
}

void DropDown::setList(std::vector<const wchar_t*>list_)
{
    list = list_;
    size_x = 0;
    for (int i = 0; i < list_.size(); i++)
    {
        if (wstrlen(list[i]) > size_x)
            size_x = unsigned(wstrlen(list[i]));
    }
    size_x += 4;
    size_y = unsigned(list_.size() * 2 + 1);

    buttons.clear();
    for (size_t i = 0; i < list.size(); i++)
    {
        buttons.push_back(Button(y + (i + 1) * 2, x + 1, 1, int(size_x - 4), list[ int(i) ]));
    }
    dropdown_button = Button(y-1, x, 3, size_x+1, L"");
}

void DropDown::render()
{
    if (active == false)
    {
        for (size_t i = 1; i < size_x; i++)//border(top/bottom)
        {
            print((wchar_t)9472, x + i, int(y)-1);
            print((wchar_t)9472, x + i, int(y + 1));
        }
        
        print((wchar_t)9474, x, y);//border(left/right)
        print((wchar_t)9474, x+size_x, y);

        print((wchar_t)9484, x, int(y - 1));//corners
        print((wchar_t)9488, x + size_x, int(y) - 1);
        print((wchar_t)9492, x, int(y + 1));
        print((wchar_t)9496, x + size_x, int(y + 1));
        print((wchar_t)9660, x + size_x - 1, int(y));//triangle

        print(list[selected_option], x + 2, y);
    }
    else
    {
        for (size_t i = 1; i < size_x; i++)//border(top/bottom)
        {
            print((wchar_t)9472, x + i, int(y) - 1);
            print((wchar_t)9472, x + i, y + size_y);
        }
        for (size_t i = 0; i < size_y; i++)//border(left/right)
        {
            print((wchar_t)9474, x, y+i);
            print((wchar_t)9474, x + size_x, y + i);
        }
        print((wchar_t)9484, x, int(y) - 1);//corners
        print((wchar_t)9488, x + size_x, int(y) - 1);
        print((wchar_t)9492, x, y + size_y);
        print((wchar_t)9496, x + size_x, y + size_y);
        print((wchar_t)9660, x + int(size_x) - 1, y);//triangle

        print(list[selected_option], x + 2, y);

        for (size_t i = 0; i < list.size(); i++)
        {
            buttons[i].draw();
        }
    }
}

void DropDown::update()
{
    if (active == false)
    {
        if (dropdown_button.update2() == 1)
            active = true;
    }
    else
    {
        if (dropdown_button.update2() == 1)
            active = false;
        //else if (mouse.isClicked() and mj > x + size_x)
        //    active = false;
        for (size_t i = 0; i < buttons.size(); i++)
        {
            if (buttons[i].update2() == 1)
            {
                difficulty = i + 1;
                active = false;
                selected_option = i;
                break;
            }
        }
    }
}

#endif // DROPBOX_H_INCLUDED
