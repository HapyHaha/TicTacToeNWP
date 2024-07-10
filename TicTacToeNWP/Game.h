#pragma once
#include <windows.h>
#include <array>
#include <string>

enum CellState { EMPTY, X, O };

class Game {
public:
    Game();
    void Initialize();
    bool OnLButtonClick(int x, int y);
    void OnPaint(HDC hdc, RECT& rect);
    bool CheckWinner(std::wstring& message);
    void ResetGame();

private:
    std::array<std::array<CellState, 3>, 3> board;
    bool isXTurn;

    void DrawSymbol(HDC hdc, RECT& rect, CellState state);
};
