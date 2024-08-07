#include <windows.h>
#include "Game.h"
#include "Resource.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static Game game;

    switch (uMsg) {
    case WM_CREATE:
        game.Initialize();
        return 0;

    case WM_LBUTTONDOWN: {
        RECT rect;
        GetClientRect(hwnd, &rect);
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        if (game.OnLButtonClick(x, y, rect)) {
            InvalidateRect(hwnd, nullptr, TRUE);

            std::wstring message;
            if (game.CheckWinner(message)) {
                std::wstring gameOverTitle = game.LoadStringResource(nullptr, IDS_GAMEOVER);
                MessageBox(hwnd, message.c_str(), gameOverTitle.c_str(), MB_OK);
                game.ResetGame();
                InvalidateRect(hwnd, nullptr, TRUE);
            }
        }
        return 0;
    }

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        RECT rect;
        GetClientRect(hwnd, &rect);
        game.OnPaint(hdc, rect);
        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_SIZE:
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"TicTacToeWindowClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.style = CS_HREDRAW | CS_VREDRAW;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Tic Tac Toe",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 300,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (hwnd == nullptr) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
