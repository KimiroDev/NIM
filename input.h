#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

#include "variables.h"

class Mouse
{
public:

    void update()//update the state of the mouse
    {
        if(GetKeyState(VK_LBUTTON) < 0)
        {
            if(held)
                clicked = false;
            else
                clicked = held = true;
        }
        else
            clicked = held = false;

        GetCursorPos(&position);
        GetWindowRect(hwnd, &r);

        position.y = position.y - r.top - 30;//30 is the size of the title bar
        position.x = position.x - r.left - 8;//idk what 8 is it's also for correction

        #ifdef VARIABLES_H_INCLUDED
        //find out over which cell the mouse hovers
        mi = (this->pos().y)/cell_size_px_y;
        mj = (this->pos().x)/cell_size_px_x;
        #endif // VARIABLES_H_INCLUDED
    }

    POINT pos() const noexcept {
        return this->position;
    }

    bool isHeld() const noexcept {
        return this->held;
    }

    bool isClicked() const noexcept {
        return this->clicked;
    }

    Mouse() {
        clicked = false;
        held = false;
        position.x = position.y =0;
    }

private:

    RECT r;
    bool held;
    bool clicked;
    POINT position;

}mouse;

class Keyboard
{
public:

    void update() noexcept {
        _any_pressed = _any_held = false;
        key = 0;
        for (int i = 5; i <= 250; ++i)
        {
            if(GetKeyState(i) < 0)
            {
                if(keys[(unsigned)i] == 0)
                {
                    keys[(unsigned)i] = 1;
                    key = (wchar_t)i;
                    _any_pressed = true;
                }
                else if (keys[(unsigned)i] == 1)
                {
                    keys[(unsigned)i] = 2;
                }
                else _any_held = true;
            }
            else keys[(unsigned)i] = 0;
        }

        for (int i = 186; i <= 192; ++i)
            if (keys[i] > 0){
                keys[L";=,-./`"[i-186]] = keys[i];
                key = L";=,-./`"[i-186];
            }
        for (int i = 0; i < 4; ++i)
            if (keys[_miscellaneous[i]] > 0){
                keys[L"[]'\\"[i]] = keys[_miscellaneous[i]];
                key = L"[]'\\"[i];
            }

        if (keys[VK_SHIFT] > 0)
        {
            for (int i = '0'; i <= '9'; ++i)
                if (keys[i] > 0)
                {
                    keys[L")!@#$%^&*("[i-'0']] = keys[i];
                    key = L")!@#$%^&*("[i-'0'];
                }
            for (int i = 0; i < 10; ++i)
            {
                if (keys[L";=,-./`[]'\\"[i]] > 0)
                {
                    keys[L":+<_>?~{}\"|"[i]] = keys[L";=,-./`[]'\\"[i]];
                    key = L":+<_>?~{}\"|"[i];
                }
            }
            if (keys[L'\\'] > 0) {keys[L'|'] = keys[L'\\'];key = L'|';}
        }
        else
        {
            bool _big_alpha = false;
            for (int i = 'A'; i <= 'Z'; ++i)
                if (keys[i] > 0)
                {
                    keys[i + 'a' - 'A'] = keys[i];
                    key = i + 'a' - 'A';
                    _big_alpha = true;
                }
            if (!_big_alpha)
            {
                int i = 0;
                for (wchar_t c : L"okjm")
                {
                    if (keys[(int)c] > 0)
                    {
                        keys[L"/+*-"[i]] = keys[(int)c];
                        key = L"/+*-"[i];
                    }
                    ++i;
                }
            }

        }//VK_SHIFT not pressed

        if (keys[220] > 0)
        {
            if (keys[VK_SHIFT] > 0) {
                keys['|'] = keys[220];
                key = '|';
            } else {
                keys['\\'] = keys[220];
                key = '\\';
            }
        }


    }//void update

    bool isKeyPressed() const noexcept {
        return _any_pressed;
    }

    bool isKeyHeld() const noexcept {
        return _any_held;
    }

    bool isKeyPressed(int __key) const noexcept {
        if (__key == 39 && keys[222] == 1) return false;
        return keys[__key] == 1 && keys[VK_SHIFT] == 0;
    }

    bool isKeyHeld(int __key) const noexcept {
        if (__key == 39 && keys[222] == 2) return false;
        return keys[__key] == 2 && keys[VK_SHIFT] == 0;
    }

    bool isKeyPressed(char __key) const noexcept {
        if (__key == '\'') return keys[222] == 1;
        else if (__key < '0' || __key > 'z'){
            if (keys[VK_SHIFT] > 0 && keys[(int)__key] == 1)
                return true;
        }
        else if (keys[(int)__key] == 1) return true;
        return false;
    }

    bool isKeyHeld(char __key) const noexcept {
        if (__key == '\'') return keys[222] == 2;
        else if (__key < '0' || __key > 'z'){
            if (keys[VK_SHIFT] > 0 && keys[(int)__key] == 2)
                return true;
        }
        else if (keys[(int)__key] == 1) return true;
        return false;
    }

    bool isAlphaPressed() const noexcept {
        if (isKeyHeld(VK_CONTROL)) return false;
        for (int i = 'A'; i <= 'Z'; ++i)
            if (keys[i] == 1)
                return true;
        return false;
    }

    bool isAlphaHeld() const noexcept {
        if (isKeyHeld(VK_CONTROL)) return false;
        for (int i = 'A'; i <= 'Z'; ++i)
            if (keys[i] == 2)
                return true;
        return false;
    }

    bool isAlphaNumericPressed() const noexcept {
        if (isKeyHeld(VK_CONTROL)) return false;
        for (int i = '0'; i <= 'Z'; ++i)
            if (keys[i] == 1)
                return true;
        return false;
    }

    bool isAlphaNumericHeld() const noexcept {
        if (isKeyHeld(VK_CONTROL)) return false;
        for (int i = '0'; i <= 'Z'; ++i)
            if (keys[i] == 2)
                return true;
        return false;
    }

    bool isCharPressed() const noexcept {
        if (isKeyHeld(VK_CONTROL)) return false;
        if (keys[222] == 1) return true;

        for (wchar_t c : L"-=[];\\,./_+\"|?`~* ")
            if ((int)c >= 37 && (int)c <=40)
                {if(keys[VK_SHIFT] > 0 && keys[(int)c] == 1)return true;}
            else if (keys[(int)c] == 1)
                return true;

        for (size_t i = (size_t)'0'; i <= (size_t)'Z'; ++i)
            if (keys[i] == 1)
                return true;

        return false;
    }

    bool isCharHeld() const noexcept {
        if (isKeyHeld(VK_CONTROL)) return false;
        if (keys[222] == 2) return true;

        for (wchar_t c : L"-=[];\\,./_+\"|?`~* ")
            if ((int)c >= 37 && (int)c <=40)
                {if(keys[VK_SHIFT] > 0 && keys[(int)c] == 2)return true;}
            else if (keys[(int)c] == 2)
                return true;

        for (size_t i = (size_t)'0'; i <= (size_t)'Z'; ++i)
            if (keys[i] == 2)
                return true;

        return false;
    }

    wchar_t keyPressed() const noexcept {
        return this->key;
    }

protected:

    wchar_t key;
    int keys[256]{0};
    bool _any_pressed, _any_held;
    const int _miscellaneous[4] = {219, 221, 222, 226};

}keyboard;

#endif // INPUT_H_INCLUDED
