#include "Game.h"
#include "Resource.h"

Game::Game() : isXTurn(true) {
    ResetGame();
}

void Game::Initialize() {
    ResetGame();
}

bool Game::OnLButtonClick(int x, int y, RECT& rect) {
    int cellWidth = (rect.right - rect.left) / 3;
    int cellHeight = (rect.bottom - rect.top) / 3;

    int col = x / cellWidth;
    int row = y / cellHeight;

    if (board[row][col] == EMPTY) {
        board[row][col] = isXTurn ? X : O;
        isXTurn = !isXTurn;
        return true;
    }
    return false;
}

void Game::OnPaint(HDC hdc, RECT& rect) {
    int cellWidth = (rect.right - rect.left) / 3;
    int cellHeight = (rect.bottom - rect.top) / 3;

    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            RECT cellRect = { col * cellWidth, row * cellHeight, (col + 1) * cellWidth, (row + 1) * cellHeight };
            if (board[row][col] != EMPTY) {
                DrawSymbol(hdc, cellRect, board[row][col]);
            }
        }
    }

    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HPEN oldPen = (HPEN)SelectObject(hdc, hPen);
    for (int i = 1; i < 3; ++i) {
        MoveToEx(hdc, i * cellWidth, 0, nullptr);
        LineTo(hdc, i * cellWidth, rect.bottom);
        MoveToEx(hdc, 0, i * cellHeight, nullptr);
        LineTo(hdc, rect.right, i * cellHeight);
    }
    SelectObject(hdc, oldPen);
    DeleteObject(hPen);
}

void Game::DrawSymbol(HDC hdc, RECT& rect, CellState state) {
    int padding = (rect.right - rect.left) / 10;
    InflateRect(&rect, -padding, -padding);

    int thickness = (rect.right - rect.left) / 15;
    HPEN hPen = CreatePen(PS_SOLID, thickness, RGB(0, 0, 0));
    HPEN oldPen = (HPEN)SelectObject(hdc, hPen);

    if (state == X) {
        MoveToEx(hdc, rect.left, rect.top, nullptr);
        LineTo(hdc, rect.right, rect.bottom);
        MoveToEx(hdc, rect.right, rect.top, nullptr);
        LineTo(hdc, rect.left, rect.bottom);
    }
    else if (state == O) {
        Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);
    }

    SelectObject(hdc, oldPen);
    DeleteObject(hPen);
}

bool Game::CheckWinner(std::wstring& message) {
    auto getMessageBoxString = [&](CellState winner) -> std::wstring {
        if (winner == X) {
            return LoadStringResource(nullptr, IDS_XWINS);
        }
        else if (winner == O) {
            return LoadStringResource(nullptr, IDS_OWINS);
        }
        else if (winner == EMPTY) {
            return LoadStringResource(nullptr, IDS_DRAW);
        }
        return std::wstring();
    };

    for (int i = 0; i < 3; ++i) {
        if (board[i][0] != EMPTY && board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            message = getMessageBoxString(board[i][0]);
            return true;
        }
        if (board[0][i] != EMPTY && board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            message = getMessageBoxString(board[0][i]);
            return true;
        }
    }

    if (board[0][0] != EMPTY && board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        message = getMessageBoxString(board[0][0]);
        return true;
    }
    if (board[0][2] != EMPTY && board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        message = getMessageBoxString(board[0][2]);
        return true;
    }

    bool drawGame = true;
    for (const auto& row : board) {
        for (CellState cell : row) {
            if (cell == EMPTY) {
                drawGame = false;
                break;
            }
        }
    }

    if (drawGame) {
        message = getMessageBoxString(EMPTY);
        return true;
    }
    return false;
}

void Game::ResetGame() {
    isXTurn = true;
    for (auto& row : board) {
        for (CellState& cell : row) {
            cell = EMPTY;
        }
    }
}

std::wstring Game::LoadStringResource(HINSTANCE hInstance, int id) {
    wchar_t buffer[256];
    LoadString(hInstance, id, buffer, sizeof(buffer) / sizeof(buffer[0]));
    return std::wstring(buffer);
}