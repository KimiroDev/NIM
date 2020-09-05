#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include "variables.h"

//can be clicked and have text
class Button
{
public:
    //        position  /        size        / displayed text / function called when pressed
    Button(int i, int j, int isize, int jsize, const wchar_t name[], void (*foo)())
    {
        textsize = 0;
        while(name[textsize])
        {
            text[textsize]=name[textsize];
            ++textsize;
        }
        text[textsize] = L'\0';
        func = foo;
        left_margin = i;
        j1 = j;
        right_margin = i + isize;
        j2 = j + jsize;
    }

    Button(int i, int j, int isize, int jsize, const wchar_t name[])
    {
        textsize = 0;
        while (name[textsize])
        {
            text[textsize] = name[textsize];
            ++textsize;
        }
        text[textsize] = L'\0';
        left_margin = i;
        j1 = j;
        right_margin = i + isize;
        j2 = j + jsize;
    }

    Button(int i, int j, int isize, int jsize, const char name[], void (*foo)())
    {
        textsize = 0;
        while (name[textsize])
        {
            text[textsize] = (wchar_t)name[textsize];
            ++textsize;
        }
        text[textsize] = L'\0';
        func = foo;

        left_margin = i;
        right_margin = i + isize;
        j1 = j;
        j2 = j + jsize;

    }
    Button(){}

    virtual void update()
    {
        if(mouse.isClicked() && mi >= left_margin && mi < right_margin && mj >= j1 && mj < j2)
            func();
    }

    int update2()
    {
        if (mouse.isClicked() && mi >= left_margin && mi < right_margin && mj >= j1 && mj < j2)
        {
            return 1;
        }
    }

    void draw()
    {
        int aux = 0;
        for(int i = left_margin; i < right_margin; i++)
        {
            for(int j = j1; j < j2; j++)
            {
                if(aux>=0 && aux < textsize)
                    screen[i * screen_width + j] = text[aux++];
            }
        }
    }

    void draw(wchar_t *screen, int screen_width, int i = 0, int j = 0)
    {
        int aux = 0;
        while (text[aux] != 0)
        {
            screen[i * screen_width + j + aux] = text[aux];
            ++aux;
        }
    }

    wchar_t* name()
    {
        return text;
    }

protected:

    wchar_t text[200];
    unsigned short textsize;
    short left_margin, right_margin, j1, j2;
    void (*func)();
};

#endif // BUTTON_H_INCLUDED
