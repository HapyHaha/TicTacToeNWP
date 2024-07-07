#include "Game.h"
#include "Resource.h"
#include <string>
#include <array>

Game::Game() : isXTurn(true), hwnd(nullptr) {
    ResetGame();
}

std::wstring Game::LoadStringResource(HINSTANCE hInstance, int id) {
    wchar_t buffer[256];
    int length = LoadString(hInstance, id, buffer, sizeof(buffer) / sizeof(buffer[0]));
    return std::wstring(buffer, length);
}

void Game::Initialize(HWND hwnd) {
    this->hwnd = hwnd;
    ResetGame();
}

void Game::OnLButtonClick(int x, int y) {
    int cellWidth = 300 / 3;
    int cellHeight = 300 / 3;

    int col = x / cellWidth;
    int row = y / cellHeight;

    if (board[row][col] == ' ') {
        board[row][col] = isXTurn ? 'X' : 'O';
        isXTurn = !isXTurn;
        InvalidateRect(hwnd, nullptr, TRUE);
        UpdateWindow(hwnd);
        CheckWinner();
    }
}

void Game::OnPaint(HDC hdc, RECT& rect) {
    int cellWidth = rect.right / 3;
    int cellHeight = rect.bottom / 3;

    HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
    FillRect(hdc, &rect, hBrush);
    DeleteObject(hBrush);

    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            RECT cellRect = { col * cellWidth, row * cellHeight, (col + 1) * cellWidth, (row + 1) * cellHeight };
            DrawText(hdc, board[row][col] == ' ' ? L" " : (board[row][col] == 'X' ? L"X" : L"O"), -1, &cellRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        }
    }

    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    SelectObject(hdc, hPen);
    for (int i = 1; i < 3; ++i) {
        MoveToEx(hdc, i * cellWidth, 0, nullptr);
        LineTo(hdc, i * cellWidth, rect.bottom);
        MoveToEx(hdc, 0, i * cellHeight, nullptr);
        LineTo(hdc, rect.right, i * cellHeight);
    }
    DeleteObject(hPen);
}

bool Game::CheckLine(char a, char b, char c) {
    return (a == b && b == c && a != ' ');
}

void Game::CheckWinner() {
    std::wstring xWins = LoadStringResource(nullptr, IDS_XWINS);
    std::wstring oWins = LoadStringResource(nullptr, IDS_OWINS);
    std::wstring gameOver = LoadStringResource(nullptr, IDS_GAMEOVER);
    std::wstring draw = LoadStringResource(nullptr, IDS_DRAW);

    for (int i = 0; i < 3; ++i) {
        if (CheckLine(board[i][0], board[i][1], board[i][2])) {
            MessageBox(hwnd, (board[i][0] == 'X' ? xWins.c_str() : oWins.c_str()), gameOver.c_str(), MB_OK);
            ResetGame();
            InvalidateRect(hwnd, nullptr, TRUE);
            return;
        }
        if (CheckLine(board[0][i], board[1][i], board[2][i])) {
            MessageBox(hwnd, (board[0][i] == 'X' ? xWins.c_str() : oWins.c_str()), gameOver.c_str(), MB_OK);
            ResetGame();
            InvalidateRect(hwnd, nullptr, TRUE);
            return;
        }
    }

    if (CheckLine(board[0][0], board[1][1], board[2][2])) {
        MessageBox(hwnd, (board[0][0] == 'X' ? xWins.c_str() : oWins.c_str()), gameOver.c_str(), MB_OK);
        ResetGame();
        InvalidateRect(hwnd, nullptr, TRUE);
        return;
    }
    if (CheckLine(board[0][2], board[1][1], board[2][0])) {
        MessageBox(hwnd, (board[0][2] == 'X' ? xWins.c_str() : oWins.c_str()), gameOver.c_str(), MB_OK);
        ResetGame();
        InvalidateRect(hwnd, nullptr, TRUE);
        return;
    }

    bool drawGame = true;
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            if (board[row][col] == ' ') {
                drawGame = false;
                break;
            }
        }
    }

    if (drawGame) {
        MessageBox(hwnd, draw.c_str(), gameOver.c_str(), MB_OK);
        ResetGame();
        InvalidateRect(hwnd, nullptr, TRUE);
    }
}

void Game::ResetGame() {
    isXTurn = true;
    for (auto& row : board) {
        for (char& cell : row) {
            cell = ' ';
        }
    }
}

void Game::Resize(int width, int height) {
    InvalidateRect(hwnd, nullptr, TRUE);
}
