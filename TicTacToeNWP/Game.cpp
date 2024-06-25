#include "Game.h"
#include "Resource.h"
#include <string>

Game::Game() : isXTurn(true) {
    ResetGame();
}

std::wstring LoadStringResource(HINSTANCE hInstance, int id) {
    wchar_t buffer[256];
    int length = LoadString(hInstance, id, buffer, sizeof(buffer) / sizeof(buffer[0]));
    return std::wstring(buffer, length);
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

void Game::OnButtonClick(int id) {
    int row = id / 3;
    int col = id % 3;

    if (board[row][col] == ' ') {
        board[row][col] = isXTurn ? 'X' : 'O';
        SetWindowText(buttons[id], isXTurn ? L"X" : L"O");
        isXTurn = !isXTurn;
        CheckWinner();
    }
}

void Game::CheckWinner() {
    HINSTANCE hInstance = GetModuleHandle(NULL);

    std::wstring xWins = LoadStringResource(hInstance, IDS_XWINS);
    std::wstring oWins = LoadStringResource(hInstance, IDS_OWINS);
    std::wstring gameOver = LoadStringResource(hInstance, IDS_GAMEOVER);
    std::wstring draw = LoadStringResource(hInstance, IDS_DRAW);

    for (int i = 0; i < 3; ++i) {
        if (board[i][0] != ' ' && board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            MessageBox(NULL, (board[i][0] == 'X' ? xWins.c_str() : oWins.c_str()), gameOver.c_str(), MB_OK);
            ResetGame();
            return;
        }
        if (board[0][i] != ' ' && board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            MessageBox(NULL, (board[0][i] == 'X' ? xWins.c_str() : oWins.c_str()), gameOver.c_str(), MB_OK);
            ResetGame();
            return;
        }
    }

    if (board[0][0] != ' ' && board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        MessageBox(NULL, (board[0][0] == 'X' ? xWins.c_str() : oWins.c_str()), gameOver.c_str(), MB_OK);
        ResetGame();
        return;
    }
    if (board[0][2] != ' ' && board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        MessageBox(NULL, (board[0][2] == 'X' ? xWins.c_str() : oWins.c_str()), gameOver.c_str(), MB_OK);
        ResetGame();
        return;
    }
    bool drawGame = true;
    for (int i = 0; i < 9; ++i) {
        if (board[i / 3][i % 3] == ' ') {
            drawGame = false;
            break;
        }
    }

    if (drawGame) {
        MessageBox(NULL, draw.c_str(), gameOver.c_str(), MB_OK);
        ResetGame();
    }
}

void Game::ResetGame() {
    isXTurn = true;
    for (int i = 0; i < 9; ++i) {
        board[i / 3][i % 3] = ' ';
        SetWindowText(buttons[i], L"");
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
