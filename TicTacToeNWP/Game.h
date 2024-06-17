#pragma once
#include <windows.h>

class Game {
public:
    Game();
    void Initialize(HWND hwnd);
    void OnButtonClick(int id);
    void Resize(int width, int height);

private:
    HWND buttons[9];
    char board[3][3];
    bool isXTurn;
    void ResetGame();
    void CheckWinner();
};
