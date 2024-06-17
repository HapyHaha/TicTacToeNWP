#include <windows.h>
#include "Game.h"

Game ticTacToeGame;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
        ticTacToeGame.Initialize(hwnd);
        return 0;

    case WM_COMMAND:
        if (LOWORD(wParam) >= 1000 && LOWORD(wParam) < 1009) {
            int id = LOWORD(wParam) - 1000;
            ticTacToeGame.OnButtonClick(id);
        }
        return 0;

    case WM_SIZE:
        ticTacToeGame.Resize(LOWORD(lParam), HIWORD(lParam));
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"TicTacToeWindowClass";

    WNDCLASS wc = { };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Tic Tac Toe",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 300,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
