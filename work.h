#ifndef WORK_H_INCLUDED
#define WORK_H_INCLUDED

void work()
{
    /* windows 10 compatibility stuff */
    hwnd = GetConsoleWindow();
    updateVersion();

    /* window output set up */
    hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleTitle(L"NIM");
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD prev_mode;
    GetConsoleMode(hConsole, &prev_mode);
    SetConsoleMode(hConsole, ENABLE_QUICK_EDIT_MODE);

    /* miscellaneous */
    for (int i = 0; i < 50; ++i)
        if (selected_coin_texture[i] == L'#') selected_coin_texture[i] = WHITE_SPACE;
    for (int i = 0; i < cell_count; ++i)
        screen[i] = L' ';
    for (int i = 0; i < cell_count; ++i)
        buffer[i] = L' ';

    wchar_t temp[10] = L"Jonathan";
    names.push_back(temp);
    wchar_t temp2[10] = L"Alice";
    names.push_back(temp2);
    wchar_t temp3[10] = L"Bob";
    names.push_back(temp3);

    difficulties.push_back(L"easy");
    difficulties.push_back(L"medium");
    difficulties.push_back(L"hard");
    difficulty_drop.setList(difficulties);
    difficulty_drop.selected_option = 1;

    srand(unsigned(time(0)));

    piles.push_back(Pile(14, rand()%5+1));
	piles.push_back(Pile(28, rand() % 5 + 1));
	piles.push_back(Pile(42, rand() % 5 + 1));
	piles.push_back(Pile(56, rand() % 5 + 1));
	piles.push_back(Pile(70, rand() % 5 + 1));

    auto timer_start = std::chrono::high_resolution_clock::now();
    auto timer_end = std::chrono::high_resolution_clock::now();

    while(running)
    {
        //used in limiting fps
        timer_start = std::chrono::high_resolution_clock::now();

        /* update input information */
        mouse.update();
        keyboard.update();


        widgets_updated = false;

        /* clear screen */
        for(int i = 0; i < cell_count; i++)
            screen[i]=L' ';

        if (scene == 0)
            howToPlay();
        else if(scene == 1)
            game();
        else if (scene == 2)
            options();

        sideBar();

        /* display */
        WriteConsoleOutputCharacter(hConsole, screen, cell_count, zerozero, &dwBytesWritten);

        if(!running)exit(0);

        //limit to 60 fps
        timer_end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(timer_end-timer_start);
        if (duration.count() < 16)
            Sleep(16 - duration.count());
    }
}

#endif // WORK_H_INCLUDED
