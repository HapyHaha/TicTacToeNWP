#include <windows.h>
#include "Game.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    Game* game = reinterpret_cast<Game*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    switch (uMsg) {
    case WM_CREATE:
    {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        game = reinterpret_cast<Game*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(game));
        game->Initialize(hwnd);
    }
    return 0;

    case WM_LBUTTONDOWN: {
        if (game) {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            game->OnLButtonClick(x, y);
        }
        return 0;
    }

    case WM_PAINT: {
        if (game) {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            game->OnPaint(hdc, ps.rcPaint);
            EndPaint(hwnd, &ps);
        }
        return 0;
    }

    case WM_SIZE:
        if (game) {
            game->Resize(LOWORD(lParam), HIWORD(lParam));
        }
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_SETCURSOR: {
        if (LOWORD(lParam) == HTCLIENT) {
            SetCursor(LoadCursor(nullptr, IDC_ARROW));
            return TRUE;
        }
        break;
    }

    case WM_MOUSEMOVE: {
        POINT pt;
        GetCursorPos(&pt);
        ScreenToClient(hwnd, &pt);

        RECT rect;
        GetClientRect(hwnd, &rect);
        if (pt.x < rect.left || pt.x > rect.right || pt.y < rect.top || pt.y > rect.bottom) {
            SetCursor(LoadCursor(nullptr, IDC_ARROW));
        }
        break;
    }

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

    Game ticTacToeGame;

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Tic Tac Toe",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 300,
        nullptr,
        nullptr,
        hInstance,
        &ticTacToeGame
    );

    if (hwnd == nullptr) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = { };
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
