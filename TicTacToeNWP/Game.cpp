#include "Game.h"
#include "Resource.h"


std::wstring LoadStringResource(HINSTANCE hInstance, int id) {
    wchar_t buffer[256];
    int length = LoadString(hInstance, id, buffer, sizeof(buffer) / sizeof(buffer[0]));
    return std::wstring(buffer, length);
}

void Game::Initialize(HWND hwnd) {
    ResetGame(hwnd);

    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    cellWidth = clientRect.right / 3;
    cellHeight = clientRect.bottom / 3;
}

void Game::OnLButtonClick(HWND hwnd, int x, int y) {
    int col = x / cellWidth;
    int row = y / cellHeight;

    if (board[row][col] == 0) {
        board[row][col] = isXTurn ? 1 : 2;
        isXTurn = !isXTurn;
        InvalidateRect(hwnd, nullptr, TRUE);
        UpdateWindow(hwnd);
        CheckWinner(hwnd);
    }
}

void Game::OnPaint(HDC hdc, RECT& rect) {
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            RECT cellRect = { col * cellWidth, row * cellHeight, (col + 1) * cellWidth, (row + 1) * cellHeight };
            if (board[row][col] != 0) {
                DrawSymbol(hdc, cellRect, board[row][col] == 1 ? L'X' : L'O');
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

void Game::DrawSymbol(HDC hdc, RECT& rect, wchar_t symbol) {
    int padding = (rect.right - rect.left) / 10; 
    InflateRect(&rect, -padding, -padding);

    int thickness = (rect.right - rect.left) / 15; 
    HPEN hPen = CreatePen(PS_SOLID, thickness, RGB(0, 0, 0));
    HPEN oldPen = (HPEN)SelectObject(hdc, hPen);

    if (symbol == L'X') {
        MoveToEx(hdc, rect.left, rect.top, nullptr);
        LineTo(hdc, rect.right, rect.bottom);
        MoveToEx(hdc, rect.right, rect.top, nullptr);
        LineTo(hdc, rect.left, rect.bottom);
    }
    else if (symbol == L'O') {
        Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);
    }

    SelectObject(hdc, oldPen);
    DeleteObject(hPen);
}

void Game::CheckWinner(HWND hwnd) {
    auto getMessageBoxString = [&](int winner) {
        if (winner == 1) {
            return LoadStringResource(nullptr, IDS_XWINS);
        }
        else if (winner == 2) {
            return LoadStringResource(nullptr, IDS_OWINS);
        }
        else if (winner == 0) {
            return LoadStringResource(nullptr, IDS_DRAW);
        }
        return std::wstring();
    };

    for (int i = 0; i < 3; ++i) {
        if (board[i][0] != 0 && board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            MessageBox(hwnd, getMessageBoxString(board[i][0]).c_str(), getMessageBoxString(-1).c_str(), MB_OK);
            ResetGame(hwnd);
            InvalidateRect(hwnd, nullptr, TRUE);
            return;
        }
        if (board[0][i] != 0 && board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            MessageBox(hwnd, getMessageBoxString(board[0][i]).c_str(), getMessageBoxString(-1).c_str(), MB_OK);
            ResetGame(hwnd);
            InvalidateRect(hwnd, nullptr, TRUE);
            return;
        }
    }

    if (board[0][0] != 0 && board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        MessageBox(hwnd, getMessageBoxString(board[0][0]).c_str(), getMessageBoxString(-1).c_str(), MB_OK);
        ResetGame(hwnd);
        InvalidateRect(hwnd, nullptr, TRUE);
        return;
    }
    if (board[0][2] != 0 && board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        MessageBox(hwnd, getMessageBoxString(board[0][2]).c_str(), getMessageBoxString(-1).c_str(), MB_OK);
        ResetGame(hwnd);
        InvalidateRect(hwnd, nullptr, TRUE);
        return;
    }

    bool drawGame = true;
    for (const auto& row : board) {
        for (int cell : row) {
            if (cell == 0) {
                drawGame = false;
                break;
            }
        }
    }

    if (drawGame) {
        MessageBox(hwnd, getMessageBoxString(0).c_str(), getMessageBoxString(-1).c_str(), MB_OK);
        ResetGame(hwnd);
        InvalidateRect(hwnd, nullptr, TRUE);
    }
}

void Game::ResetGame(HWND hwnd) {
    isXTurn = true;
    for (auto& row : board) {
        for (int& cell : row) {
            cell = 0;
        }
    }

    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    cellWidth = clientRect.right / 3;
    cellHeight = clientRect.bottom / 3;
}
