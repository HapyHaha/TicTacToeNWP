#pragma once
#include <windows.h>

class Game {
public:
    Game();
    void Initialize(HWND hwnd);
    void Resize(int width, int height);

private:
    HWND buttons[9];
};
