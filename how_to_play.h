#ifndef HOW_TO_PLAY_H_INCLUDED
#define HOW_TO_PLAY_H_INCLUDED


wchar_t *buffer = new wchar_t[80*25+1]{L' '};
int px = 11, py = 0;
wchar_t prev_char;

#include <fstream>
void howToPlay()
{
    buffer[py * screen_width + px] = prev_char;

    if (keyboard.isKeyHeld(VK_CONTROL) &&
        keyboard.isKeyPressed('s'))
    {
        std::ofstream fout("text.txt");
        for (int i = 0; i <= 80*25; ++i)
        {
            char ch = (buffer[i] == L' '? 222 : buffer[i]);
            fout << ch;
            if (i%80 == 0 && i > 2)fout<<std::endl;
        }
        fout.close();
    }
    if (keyboard.isKeyHeld(VK_CONTROL) &&
        keyboard.isKeyPressed('l'))
    {
        char ch;int i = 0;
        std::ifstream fin("text.txt");
        while (fin >> ch)
        {
            if (ch == (char)222) ch = ' ';
            buffer[i] = (wchar_t)ch;
            ++i;
            if(i>=80*25)break;
        }
        fin.close();
    }


    if (keyboard.isKeyPressed(VK_DOWN))
        ++py;
    if (keyboard.isKeyPressed(VK_UP))
        --py;
    if (keyboard.isKeyPressed(VK_RIGHT))
        ++px;
    if (keyboard.isKeyPressed(VK_LEFT))
        --px;
    if (keyboard.isKeyPressed(VK_HOME))
        px = 11;
    if (keyboard.isKeyPressed(VK_RETURN))
        py++,px=11;
    if (mouse.isClicked())
        px = mj > 10 ? mj : 11, py = mi;
    if (px < 11) {px = 79;--py;}
    if (py < 0)  py = 0;
    if (px > 79) {px = 11;++py;}
    if (py > 24) py = 24;//*/

    if (keyboard.isCharPressed())
    {
        buffer[py * screen_width + px] = keyboard.keyPressed();
        if (px + 1 < 81)
            ++px;
        else
        {
            px = 11;
            ++py;
        }
    }
    //*/
    if (keyboard.isKeyPressed(VK_BACK))
    {
        buffer[py * screen_width + px] = L' ';
        if (px - 1 > 10)
            --px;
        else
        {
            px = 79;
            --py;
        }
    }

    prev_char = buffer[py * screen_width + px];
    buffer[py * screen_width + px] = WHITE_SPACE;

    for (int i = 0; i < 80*25; ++i)
    {
        screen[i] = buffer[i];
    }
}

#endif // HOW_TO_PLAY_H_INCLUDED
