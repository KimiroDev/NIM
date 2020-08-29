#ifndef COMPATIBILITY_H_INCLUDED
#define COMPATIBILITY_H_INCLUDED

#include "variables.h"
#include <VersionHelpers.h>

HWND hwnd;//used in console manipulation
COORD zerozero = {0, 0};
HANDLE hConsole;
DWORD dwBytesWritten = 0;//miscellaneous
float move_window_x = 10.4f, move_window_y = 14.0;

//used to resize the console cell
//important for w10 compatibility
/*typedef struct _CONSOLE_FONT_INFOEX
{
    ULONG cbSize;
    DWORD nFont;
    COORD dwFontSize;
    UINT  FontFamily;
    UINT  FontWeight;
    WCHAR FaceName[LF_FACESIZE];
}CONSOLE_FONT_INFOEX, *PCONSOLE_FONT_INFOEX;*/
//idk
extern "C" {
    BOOL WINAPI SetCurrentConsoleFontEx(HANDLE hConsoleOutput,
                                    BOOL bMaximumWindow,
                                    PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx);
}

void setFontSize(int w, int h)
{
    _CONSOLE_FONT_INFOEX fontStructure={0};
    fontStructure.cbSize=sizeof(fontStructure);
    fontStructure.dwFontSize.X=w;
    fontStructure.dwFontSize.Y=h;
    HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
    SetCurrentConsoleFontEx(hConsole, true, &fontStructure);
}

void updateVersion()
{
    DWORD dwVersion = 0;
    DWORD dwMinorVersion = 0;
    
    if(!IsWindows10OrGreater())
    {
        //setting correct window size
        setFontSize(12, 20);///win10

        move_window_x = 12.5;///win10 used to be .5
        move_window_y = 22; ///win10
        cell_size_px_x = 10;
        cell_size_px_y = 20;
    }

    const HWND hDesktop = GetDesktopWindow();
    RECT desktop_size;
    GetWindowRect(hDesktop, &desktop_size);
    MoveWindow(hwnd,
               int(desktop_size.right/2 - screen_width*move_window_x/2), int(desktop_size.bottom/2 - screen_height*move_window_y/2),
               int(screen_width*move_window_x), int(screen_height*move_window_y), TRUE);
}

#endif // COMPATIBILITY_H_INCLUDED
