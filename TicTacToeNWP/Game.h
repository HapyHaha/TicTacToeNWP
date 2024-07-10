#pragma once
#include <windows.h>
#include <array>
#include <string>

class Game {
public:
    void Initialize(HWND hwnd);
    void OnLButtonClick(HWND hwnd, int x, int y);
    void OnPaint(HDC hdc, RECT& rect);

private:
    std::array<std::array<int, 3>, 3> board;
    bool isXTurn;
    int cellWidth, cellHeight;

    void ResetGame(HWND hwnd);
    void CheckWinner(HWND hwnd);
    void DrawSymbol(HDC hdc, RECT& rect, wchar_t symbol);
};
