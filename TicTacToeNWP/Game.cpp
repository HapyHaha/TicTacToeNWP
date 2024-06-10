#include "Game.h"

Game::Game() {
}

void Game::Initialize(HWND hwnd) {
    for (int i = 0; i < 9; ++i) {
        buttons[i] = CreateWindow(
            L"BUTTON",
            L"",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            (i % 3) * 100, (i / 3) * 100, 100, 100,
            hwnd,
            (HMENU)(1000 + i),
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL
        );
    }
}

void Game::Resize(int width, int height) {
    int buttonWidth = width / 3;
    int buttonHeight = height / 3;

    for (int i = 0; i < 9; ++i) {
        int row = i / 3;
        int col = i % 3;
        MoveWindow(buttons[i], col * buttonWidth, row * buttonHeight, buttonWidth, buttonHeight, TRUE);
    }
}
