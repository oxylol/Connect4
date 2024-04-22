#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Windowsx.h>
#include <sstream>
#include <vector>
#include "Connect4.h"

using namespace std;

// Globala variabler --------------------------------------------------------
LPCTSTR ClsName = "Colors and Lines";
LPCTSTR WndName = "Window";
HDC dc;
HPEN Pen[4];
HBRUSH Brush[3];
LPARAM mouse;
RECT zone;
Connect4 c4;
HWND hw;

// Funktioner ---------------------------------------------------------------
ATOM    doRegister(HINSTANCE);
BOOL 	initInstance(HINSTANCE, int);
LRESULT CALLBACK winProc(HWND, UINT, WPARAM, LPARAM);
// Initializes elements like drawing zone when the window is created.
void init(HWND hWnd) {

    Connect4 c4;
    zone.left = 0;
    zone.right = 1200;
    zone.top = 0;
    zone.bottom = 150;

    dc = GetDC(hWnd);
    Brush[1] = CreateSolidBrush(RGB(200, 0, 0));
    Brush[2] = CreateSolidBrush(RGB(200, 200, 0));
    Brush[0] = (HBRUSH)SelectObject(dc, Brush[1]);

    Pen[1] = CreatePen(PS_SOLID, 20, RGB(255, 255, 255));
    Pen[2] = CreatePen(PS_SOLID, 15, RGB(230, 0, 0));
    Pen[3] = CreatePen(PS_SOLID, 15, RGB(230, 230, 0));
    Pen[0] = (HPEN)SelectObject(dc, Pen[1]);
}
// destroyes GDI objects and releases dc when the window is destroyed.
void destroy(HWND hWnd) {
    SelectObject(dc, Pen[0]);
    SelectObject(dc, Brush[0]);
    DeleteObject(Brush[0]);
    DeleteObject(Brush[1]);
    DeleteObject(Brush[2]);

    DeleteObject(Pen[2]);
    DeleteObject(Pen[1]);
    DeleteObject(Pen[0]);
    ReleaseDC(hWnd, dc);
}
//---------------------------------------------------------------------------
int WINAPI WinMain(_In_ HINSTANCE hi, _In_opt_ HINSTANCE hp, _In_ LPSTR lp, _In_ int n) {
    UNREFERENCED_PARAMETER(hp);
    UNREFERENCED_PARAMETER(lp);
    if (!(doRegister(hi)) || !(initInstance(hi, n))) {
        MessageBox(NULL, (LPCSTR)"Fel, Kan ej registrera eller skapa fönstret i windows",
            (LPCSTR)"ERROR", MB_ICONERROR | MB_OK);
        return 0;
    }

    CreateWindow(
        (LPCSTR)"BUTTON",
        (LPCSTR)"RESET",
        BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD,
        1055,               // X Pos
        150,          // Y Pos
        70, 40, hw,
        (HMENU)1,                // ID 
        hp, NULL);

    CreateWindow(
        (LPCSTR)"BUTTON",
        (LPCSTR)"RESET SCORE",
        BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD,
        1055,               // X Pos
        240,          // Y Pos
        110, 40, hw,
        (HMENU)2,                // ID 
        hp, NULL);

    CreateWindow(
        (LPCSTR)"BUTTON",
        (LPCSTR)"PLAY VS AI",
        BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD,
        1055,               // X Pos
        280,          // Y Pos
        110, 40, hw,
        (HMENU)3,                // ID 
        hp, NULL);
    CreateWindow(
        (LPCSTR)"BUTTON",
        (LPCSTR)"SAVE",
        BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD,
        1055,               // X Pos
        320,          // Y Pos
        110, 40, hw,
        (HMENU)4,                // ID 
        hp, NULL);
    CreateWindow(
        (LPCSTR)"BUTTON",
        (LPCSTR)"LOAD",
        BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD,
        1055,               // X Pos
        360,          // Y Pos
        110, 40, hw,
        (HMENU)5,                // ID 
        hp, NULL);

    MSG Msg;
    while (GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}
//---------------------------------------------------------------------------
ATOM doRegister(HINSTANCE hi) {
    WNDCLASSEX wincl;
    wincl.hInstance = hi;
    wincl.lpszClassName = ClsName;
    wincl.lpfnWndProc = winProc;
    wincl.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
    wincl.cbSize = sizeof(WNDCLASSEX);
    wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));

    return RegisterClassEx(&wincl);
}
//---------------------------------------------------------------------------
BOOL initInstance(HINSTANCE hInstance, int nCmdShow) {
    HWND hWnd = CreateWindowEx(
        0,                  //Här kan man sätta olika variationsvärden för utseendet på fönstret
        ClsName,            //(Klassnamn) 
        (LPCSTR)ClsName,    //Namnet på Fönstret
        WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_BORDER, // Olika utseende som man kan ställa in
        CW_USEDEFAULT,      // X position på fönstret
        CW_USEDEFAULT,      // X position på fönstret
        1200,      // Bredd på fönstret
        1125,      // Höjd på fönstret
        NULL,               //Hanterare till parent-window
        NULL,               //Ingen meny (än)
        hInstance,          //Programmets instance hanterare
        NULL                //Ingen Window Creation Data
    );
    hw = hWnd;

    if (!hWnd) {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return TRUE;
}
//---------------------------------------------------------------------------
LRESULT CALLBACK winProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
    static bool pushed = true;
    PAINTSTRUCT ps;
    switch (Msg) {
    case WM_CLOSE:
        PostQuitMessage(0);
        break;
    case WM_CREATE:
        init(hWnd);
        break;
    case WM_DESTROY:
        destroy(hWnd);
        break;
    case WM_PAINT:
        BeginPaint(hWnd, &ps);
        (HPEN)SelectObject(dc, Pen[2]);

        for (int x = 0; x < 7; x++)
        {
            for (int y = 0; y < 6; y++)
            {
                if (c4.dotPos[x][y] != 0)
                {
                    if (c4.dotPos[x][y] == 2)
                    {
                        (HPEN)SelectObject(dc, Pen[3]);
                        (HBRUSH)SelectObject(dc, Brush[2]);
                    }
                    else
                    {
                        (HPEN)SelectObject(dc, Pen[2]);
                        (HBRUSH)SelectObject(dc, Brush[1]);
                    }
                    if (!c4.fourInRow.empty())
                    {
                        for (int i = 0; i < 4; i++)
                        {
                            if (x == c4.fourInRow[i].row && y == c4.fourInRow[i].col)
                            {
                                (HPEN)SelectObject(dc, Pen[1]);
                            }
                        }
                    }
                    Ellipse(dc, x * 150 - 50 + 70, 975 - 50 - y * 150, x * 150 + 50 + 70, 975 + 50 - y * 150);
                }
            }
        }

        if (!c4.yellow)
        {
            (HPEN)SelectObject(dc, Pen[2]);
            (HBRUSH)SelectObject(dc, Brush[1]);
        }
        else
        {
            (HPEN)SelectObject(dc, Pen[3]);
            (HBRUSH)SelectObject(dc, Brush[2]);
        }

        Ellipse(dc, LOWORD(mouse) - 50, 70 - 50, LOWORD(mouse) + 50, 70 + 50);
        (HPEN)SelectObject(dc, Pen[0]);
        for (int i = 0; i <= 1050; i += 150)
        {
            MoveToEx(dc, i, 150, (LPPOINT)NULL);
            LineTo(dc, i, 1150);
        }
        for (int i = 0; i <= 1000; i += 150)
        {
            MoveToEx(dc, 0, i + 150, (LPPOINT)NULL);
            LineTo(dc, 1050, i + 150);
        }
        if (!c4.fourInRow.empty())
        {
            if (!c4.yellow)
            {
                MessageBox(hWnd, "YELLOW WINS!", "WIN", MB_APPLMODAL);
                c4.y++;
            }
            else
            {
                MessageBox(hWnd, "RED WINS!", "WIN", MB_APPLMODAL);
                c4.r++;
            }
            c4.reset();
            InvalidateRect(hWnd, NULL, true);
        }
        if (c4.r > 0 || c4.y > 0)
        {
            TextOut(dc, 1055, 200, "RED:", lstrlen("RED:"));
            TextOut(dc, 1090, 200, to_string(c4.r).c_str(), lstrlen(to_string(c4.r).c_str()));
            TextOut(dc, 1055, 220, "YELLOW:", lstrlen("YELLOW:"));
            TextOut(dc, 1120, 220, to_string(c4.y).c_str(), lstrlen(to_string(c4.r).c_str()));
        }
        EndPaint(hWnd, &ps);
        break;
    case WM_LBUTTONDOWN:
    {
        if (c4.fourInRow.empty())
        {
            int a = LOWORD(lParam) / 150;
            c4.click(a);
        }
    }
    InvalidateRect(hWnd, NULL, false);
    break;
    case WM_MOUSEMOVE:
        InvalidateRect(hWnd, &zone, true);
        mouse = lParam;
        break;
    case WM_COMMAND:
        if (LOWORD(wParam) == 1) {
            c4.reset();
        }
        else if (LOWORD(wParam) == 2)
        {
            c4.r = 0;
            c4.y = 0;
        }
        else if (LOWORD(wParam) == 3)
        {
            c4.vsAi = !c4.vsAi;
        }
		else if (LOWORD(wParam) == 4)
		{
			c4.save();
		}
		else if (LOWORD(wParam) == 5)
		{
			c4.load();
		}
        InvalidateRect(hWnd, NULL, true);
        break;
    default:
        return DefWindowProc(hWnd, Msg, wParam, lParam);
    }
    return 0;
}
//---------------------------------------------------------------------------
