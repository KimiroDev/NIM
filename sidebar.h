#ifndef SIDEBAR_H_INCLUDED
#define SIDEBAR_H_INCLUDED

Button new_game(16, 1, 2, 8, "new game\0", setrestart_gametotrue);
//when pressed the game resets
Button ebutton(-2, screen_width-6, 3, 10, "", setrunningtofalse);
//when pressed exit(0)is called
Button options_scene(21, 1, 1, 8, "options", optionsScene);
//when pressed the current scene switches to the options scene
Button game_scene(19, 3, 1, 4, "game", gameScene);
//when pressed the current scene switches to the game scene
Button how_to_play_scene(23, 2, 2, 6, "how to play", howToPlayScene);
//when pressed the current scene switches to the how to play scene
Button change_turn(2, 1, 1, 8, " ", changeDefaultTurn);
//changes default turn (will later be replaced to just switch names)

Button one_player(5, 1, 1, 8, "1 player", players_1);
//changes to single player mode
Button two_players(6, 1, 1, 9, "2 players", players_2);
//changes to two players mode

///used for debugging
#include <conio.h>

//9660 triangle down
//9644 horizontal line
//8596 double arrow
wchar_t ch = '0';
int intt = 0;
void sideBar()
{
    //clear sidebar
    for (int px = 0; px < 11; ++px)
        for (int py = 0; py < 25; ++py)
            screen[py * screen_width + px] = L' ';

    //Show who's player's turn is
    print("Turn:", 2, 0);
    if(turn==0)
        print(player1, 0, 1);
    else
        print(player2, 0, 1);

    //update buttons
    new_game.update();
    ebutton.update();
    options_scene.update();
    game_scene.update();
    how_to_play_scene.update();
	one_player.update();
	two_players.update();

    if (game_begin || sum() == 1 || cols() == 0)
    {
        change_turn.update();
        int i = print((wchar_t)8596, 1, 2);
        i = print("switch", i-1);
        print((wchar_t)8596, i-1);
    }

    //display buttons
    new_game.draw();
    print("(n)", 3, 17);
    if(keyboard.isKeyPressed('N'))
            setrestart_gametotrue();

    options_scene.draw();
    game_scene.draw();
    how_to_play_scene.draw();

	if (game_mode == false)
		print('>', 0, 5);
	else print('>', 0, 6);
	one_player.draw();
	two_players.draw();

    {//shows which button (scene toggle button) is selected
        if (scene == 0)//how to play
        {
            print((wchar_t)WHITE_SPACE, 0, 23);
            print((wchar_t)WHITE_SPACE, 9, 23);

            print((wchar_t)WHITE_SPACE, 0, 24);
            print((wchar_t)WHITE_SPACE, 9, 24);
        }
        else if (scene == 1)//game scene
        {
            print((wchar_t)WHITE_SPACE, 0, 19);
            print((wchar_t)WHITE_SPACE, 9, 19);
        }
        else if (scene == 2)//options scene
        {
            print((wchar_t)WHITE_SPACE, 0, 21);
            print((wchar_t)WHITE_SPACE, 9, 21);
        }
    }

    //switching scenes
    if(keyboard.isKeyPressed(VK_TAB))
    {
        scene++;
        if (scene > 2) scene = 0;
    }

    ///debbuging
    if (scene == 0)
    {
        print("Still in", 1, 10);
        print("Developmen", 0, 11);
        print("t!", 0, 12);
    }

    //sidebar line
    for(int i=0; i<screen_height; i++)
        screen[i*screen_width + 10] = WHITE_SPACE;

}

#endif // SIDEBAR_H_INCLUDED
