#ifndef PRINT_H_INCLUDED
#define PRINT_H_INCLUDED

#include "variables.h"

int print(int n, int linear_pos)
{
    if (!n)
    {
        screen[linear_pos] = L'0';
        return linear_pos + 2;
    }

    //digit count
    int len = 0, aux = n;
    while(aux){aux/=10;++len;}
    aux = len;

    if (n < 0)
    {
        screen[linear_pos] = L'-';
        ++linear_pos;
        n*=(-1);
    }

    while (n)
    {
        screen[linear_pos + len - 1] = n%10 + '0';
        n/=10; --len;
    }

    return linear_pos + aux + 1;
}

int print(int n, int x, int y)
{
    int pos = y * screen_width + x;
    return print(n, pos);
}

int print(unsigned n, int linear_pos)
{
    return print((int)n, linear_pos);
}

int print(unsigned n, int x, int y)
{
    int pos = y * screen_width + x;
    return print((int)n, pos);
}

int print(size_t n, int linear_pos)
{
    return print((unsigned)n, linear_pos);
}

int print(size_t n, int x, int y)
{
    return print((unsigned)n, x, y);
}

int print(int n[], int _count, int linear_position)
{
    int i = 0;
    while(_count)
    {
        linear_position = print(n[i], linear_position);
        --_count;++i;
    }
    return linear_position;
}

int print(int n[], int _count, int x, int y)
{
    int pos = y * screen_width + x;
    print(n, _count, pos);
    return pos + 2;
}

//returns the linear position of the end of
//the string + 1 (good for printing multiple
//strings consecutively)
int print(wchar_t n[], int linear_pos)
{
    int i = 0;
    while (n[i] != 0)
    {
        screen[linear_pos + i] = n[i];
        ++i;
    }
    return linear_pos + i + 1;
}

int print(const char n[], int linear_pos)
{
    int i = 0;
    while (n[i] != 0)
    {
        screen[linear_pos + i] = n[i];
        ++i;
    }
    return linear_pos + i + 1;
}

int print(const char n[], int x, int y)
{
    int pos = y*screen_width + x;
    return print(n, pos);
}

int print(const wchar_t n[], int linear_pos)
{
    int i = 0;
    while (n[i] != 0)
    {
        screen[linear_pos + i] = n[i];
        ++i;
    }
    return linear_pos + i + 1;
}

int print(const wchar_t n[], int x, int y)
{
    int pos = y*screen_width + x;
    return print(n, pos);
}

int print(wchar_t n[], int x, int y)
{
    int pos = y*screen_width + x;
    return print(n, pos);
}

int print(wchar_t n, int linear_position)
{
    screen[linear_position] = n;
    return linear_position + 2;
}

int print(wchar_t n, int x, int y)
{
    int pos = y * screen_width + x;
    return print(n, pos);
}

int print(char n, int linear_position)
{
    screen[linear_position] = (wchar_t)n;
    return linear_position + 2;
}

int print(char n, int x, int y)
{
    int pos = y * screen_width + x;
    return print(n, pos);
}

int print(int n[], int _count, int linear_position, wchar_t delimitator)
{
    int i = 0;
    while(_count > 1)
    {
        linear_position = print(n[i], linear_position);
        linear_position = print(delimitator, linear_position-1);
        --_count;++i;
    }

    linear_position = print(n[i], linear_position);

    return linear_position;
}

int print(int n[], int _count, int x, int y, wchar_t delimitator)
{
    int pos = y * screen_width + x;
    return print(n, _count, pos, delimitator);
}

int print(int n[], int _count, int linear_position, char delimitator)
{
    return print(n, _count, linear_position, (wchar_t)delimitator);
}

int print(int n[], int _count, int x, int y, char delimitator)
{
    int pos = y * screen_width + x;
    return print(n, _count, pos, (wchar_t)delimitator);
}

#endif // PRINT_H_INCLUDED
