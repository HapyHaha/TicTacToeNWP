#pragma once
#include <windows.h>
#include <array>
#include <string>

class Game {
public:
    Game();
    void Initialize(HWND hwnd);
    void OnLButtonClick(int x, int y);
    void OnPaint(HDC hdc, RECT& rect);
    void Resize(int width, int height);

private:
    std::array<std::array<char, 3>, 3> board;
    bool isXTurn;
    void ResetGame();
    void CheckWinner();
    bool CheckLine(char a, char b, char c);
    std::wstring LoadStringResource(HINSTANCE hInstance, int id);
    HWND hwnd;
};
