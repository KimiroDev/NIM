#ifndef OPTIONS_H_INCLUDED
#define OPTIONS_H_INCLUDED

#include "variables.h"
#include "dropbox.h"
#include "button.h"

std::vector<const wchar_t*> difficulties;
text player_names_wchr = L"Player names:";

void db1()
{
    if (!widgets_updated) dropBox(4, 30, 13, names, player1);
}
void db2()
{
    if (!widgets_updated) dropBox(4, 46, 13, names, player2);
}

text box_wchr           = {9484, 9472, 9472, 9472, 9472, 9472, 9472, 9472, 9472, 9472, 9472, 9472, 9488,
                           9474, L' ', L' ', L' ', L' ', L' ', L' ', L' ', L' ', L' ', L' ', 9660, 9474,
                           9492, 9472, 9472, 9472, 9472, 9472, 9472, 9472, 9472, 9472, 9472, 9472, 9496, 0};
text score_reset_text   = {9484, 9472, 9472, 9472, 9472, 9472, 9472, 9472, 9472, 9472, 9472, 9472, 9488,
                           9474, L'R', L'e', L's', L'e', L't', L' ', L's', L'c', L'o', L'r', L'e', 9474,
                           9492, 9472, 9472, 9472, 9472, 9472, 9472, 9472, 9472, 9472, 9472, 9472, 9496, 0};
text change_turn_text   = {9484, 9472, 9472, 9472, 9472, 9472, 9472, 9472, 9472, 9472, 9472, 9472, 9472, 9472, 9472, 9488,
                           9474, L'C', L'h', L'a', L'n', L'g', L'e', L' ', L'd', L'e', L'f', L'a', L'u', L'l', L't', 9474,
                           9492, 9472, 9472, 9472, 9472, 9472, 9472, 9472, 9472, 9472, 9472, 9472, 9472, 9472, 9472, 9496, 0};
text acc_check_box_text = {L' ', L'_', L' ',
                           9474, L' ', 9474,
                           L' ', 175, L' ', 0};

void drawBox(int ipos, int jpos, int isize, int jsize)
{
    wchar_t ch = WHITE_SPACE;
    for(int i = ipos; i < ipos+isize-1; i++)
    {
        print(ch, jpos, i);
        print(ch, jpos + jsize-1, i);
    }
    for(int j = jpos; j < jpos+jsize-1; j++)
    {
        print(ch, j, ipos);
        print(ch, j, ipos+isize-1);
    }
    print(ch, jpos+jsize-1, ipos+isize-1);
}

void swapNames()
{
    wchar_t *aux = new wchar_t[11];
    wstrcpy(aux, player1);
    wstrcpy(player1, player2);
    wstrcpy(player2, aux);
    delete [] aux;
    int au = score1;
    score1 = score2;
    score2 = au;
}

/* The boxes that are used to input custom names */
Button p1_naming_box(2, 30, 3, 13, box_wchr, db1);
Button p2_naming_box(2, 46, 3, 13, box_wchr, db2);

/* The button that sets both scores to 0 */
Button reset_score(6, 51, 3, 13, score_reset_text, resetScore);

/* swaps names */
Button swap_names(3, 44, 1, 1, "", swapNames);

/* difficulty dropdown */
DropDown difficulty_drop(28, 11);

static int cell = 0;

void optionsScene()
{
    cell = 0;
    scene = 2;
}

void options()
{

    print(player_names_wchr, 15, 3);

    p1_naming_box.update();
    swap_names.update();
    p2_naming_box.update();

    p1_naming_box.draw();
    print((wchar_t)8596, 44, 3);
    p2_naming_box.draw();
    print(player1, 31, 3);print((wchar_t)9660, 41, 3);
    print(player2, 47, 3);print((wchar_t)9660, 57, 3);

    reset_score.draw();

    switch (cell)
    {
    case 0: /* no key has been pressed yet */
        {
            if (keyboard.isKeyPressed(VK_LEFT) ||
                keyboard.isKeyPressed(VK_UP) ||
                keyboard.isKeyPressed(VK_RIGHT) ||
                keyboard.isKeyPressed(VK_DOWN) ||
                keyboard.isKeyPressed('A') ||
                keyboard.isKeyPressed('W') ||
                keyboard.isKeyPressed('S') ||
                keyboard.isKeyPressed('D')
            )
                cell = 1;

            break;
        }
    case 1:
        {
            if (keyboard.isKeyPressed(VK_RIGHT) ||
                keyboard.isKeyPressed('D'))
                    { cell = 2; break; }
            if (keyboard.isKeyPressed(VK_LEFT) ||
                keyboard.isKeyPressed('A'))
                    { cell = 3; break; }

            if(keyboard.isKeyPressed(VK_DOWN) ||
               keyboard.isKeyPressed('S')     ||
               keyboard.isKeyPressed(VK_UP)   ||
               keyboard.isKeyPressed('W'))
                {cell = 4; break;}

            drawBox(2, 30, 3, 13);

            if(keyboard.isKeyPressed(VK_RETURN) ||
               keyboard.isKeyPressed(VK_SPACE))
                db1();

            break;
        }
    case 2:
        {
            drawBox(4, 44, 1, 1);

            if (keyboard.isKeyPressed('A')
              ||keyboard.isKeyPressed(VK_LEFT))
                { cell = 1; break; }

            if (keyboard.isKeyPressed('D')
              ||keyboard.isKeyPressed(VK_RIGHT))
                { cell = 3; break; }

            if (keyboard.isKeyPressed('S') || keyboard.isKeyPressed('W')
              ||keyboard.isKeyPressed(VK_DOWN) || keyboard.isKeyPressed(VK_UP))
                {cell = 4; break; }

            if (keyboard.isKeyPressed(VK_SPACE) || keyboard.isKeyPressed(VK_RETURN))
                { swapNames(); }

            break;
        }
    case 3:
        {
            drawBox(2, 46, 3, 13);
            if (keyboard.isKeyPressed(VK_RIGHT) ||
                keyboard.isKeyPressed('D'))
                    {cell = 1; break; }

            if (keyboard.isKeyPressed(VK_LEFT) ||
                keyboard.isKeyPressed('A'))
                    {cell = 2; break; }

            if(keyboard.isKeyPressed(VK_DOWN) ||
               keyboard.isKeyPressed('S') ||
               keyboard.isKeyPressed(VK_UP) ||
               keyboard.isKeyPressed('W'))
                {cell = 4; break;}

            if(keyboard.isKeyPressed(VK_RETURN) ||
               keyboard.isKeyPressed(VK_SPACE))
                db2();

            break;
        }
    case 4:
        {
            drawBox(6, 51, 3, 13);

            if(keyboard.isKeyPressed(VK_UP) ||
               keyboard.isKeyPressed('W'))
                cell = 3;

            if(keyboard.isKeyPressed(VK_DOWN) ||
               keyboard.isKeyPressed('S'))
                cell = 1;

            if(keyboard.isKeyPressed(VK_RETURN) ||
               keyboard.isKeyPressed(VK_SPACE))
                resetScore();

            break;
        }
    default: break;
    }//switch

    int iterator;
    iterator = print("Score:", 15, 7);
    iterator = print(player1, iterator);
    iterator = print(score1 , iterator);
    iterator = print('|'    , iterator);
    iterator = print(score2 , iterator);
    iterator = print(player2, iterator);
    reset_score.update();

    print("Difficulty:", 15, 11);
    difficulty_drop.update();
    difficulty_drop.render();
}

#endif // OPTIONS_H_INCLUDED
