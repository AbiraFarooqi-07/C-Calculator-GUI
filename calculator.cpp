#include <windows.h>
#include <cwchar>
#include <cstdlib>

HINSTANCE hInst;

#define ID_DISPLAY 100
#define ID_OPDISPLAY 110 
#define ID_0 200
#define ID_1 201
#define ID_2 202
#define ID_3 203
#define ID_4 204
#define ID_5 205
#define ID_6 206
#define ID_7 207
#define ID_8 208
#define ID_9 209
#define ID_ADD 300
#define ID_SUB 301
#define ID_MUL 302
#define ID_DIV 303
#define ID_EQ 304
#define ID_CLR 305

double firstNum = 0;
wchar_t op = 0;


HFONT hFontBtn, hFontDisplay, hFontOp;
HBRUSH hBgBrush;
COLORREF numColor = RGB(167, 141, 120);   
COLORREF opColor = RGB(110, 71, 59);     
COLORREF bgColor = RGB(41, 28, 14);      
COLORREF eqColor = RGB(239, 228, 210); 

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void AddButton(HWND hWnd, LPCWSTR text, int x, int y, int w, int h, int id);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    hInst = hInstance;

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"CalcClass";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);

    HWND hWnd = CreateWindow(
        L"CalcClass", L"Beautiful Calculator",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        500, 200, 320, 460,
        NULL, NULL, hInstance, NULL
    );

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

void AddButton(HWND hWnd, LPCWSTR text, int x, int y, int w, int h, int id)
{
    HWND btn = CreateWindow(L"BUTTON", text,
        WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        x, y, w, h, hWnd, (HMENU)(INT_PTR)id, hInst, NULL);
    SendMessage(btn, WM_SETFONT, (WPARAM)hFontBtn, TRUE);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static HWND hDisplay, hOpDisplay;

    switch (msg)
    {
    case WM_CREATE:
    {
        
        hFontBtn = CreateFont(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
        hFontDisplay = CreateFont(26, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
        hFontOp = CreateFont(16, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");

        
        hBgBrush = CreateSolidBrush(bgColor);

       
        hDisplay = CreateWindow(L"EDIT", L"",
            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_RIGHT | ES_READONLY,
            20, 50, 260, 60,
            hWnd, (HMENU)ID_DISPLAY, hInst, NULL);
        SendMessage(hDisplay, WM_SETFONT, (WPARAM)hFontDisplay, TRUE);

        hOpDisplay = CreateWindow(L"STATIC", L"",
            WS_VISIBLE | WS_CHILD | SS_RIGHT,
            20, 20, 260, 20,
            hWnd, (HMENU)ID_OPDISPLAY, hInst, NULL);
        SendMessage(hOpDisplay, WM_SETFONT, (WPARAM)hFontOp, TRUE);
        SetWindowText(hOpDisplay, L"");

        int x = 20, y = 130;
        int btnW = 60, btnH = 50; 
        int spacing = 10; 

        // Row 1
        AddButton(hWnd, L"7", x, y, btnW, btnH, ID_7);
        AddButton(hWnd, L"8", x + btnW + spacing, y, btnW, btnH, ID_8);
        AddButton(hWnd, L"9", x + 2 * (btnW + spacing), y, btnW, btnH, ID_9);
        AddButton(hWnd, L"+", x + 3 * (btnW + spacing), y, btnW, btnH, ID_ADD);

        // Row 2
        y += btnH + spacing;
        AddButton(hWnd, L"4", x, y, btnW, btnH, ID_4);
        AddButton(hWnd, L"5", x + btnW + spacing, y, btnW, btnH, ID_5);
        AddButton(hWnd, L"6", x + 2 * (btnW + spacing), y, btnW, btnH, ID_6);
        AddButton(hWnd, L"-", x + 3 * (btnW + spacing), y, btnW, btnH, ID_SUB);

        // Row 3
        y += btnH + spacing;
        AddButton(hWnd, L"1", x, y, btnW, btnH, ID_1);
        AddButton(hWnd, L"2", x + btnW + spacing, y, btnW, btnH, ID_2);
        AddButton(hWnd, L"3", x + 2 * (btnW + spacing), y, btnW, btnH, ID_3);
        AddButton(hWnd, L"*", x + 3 * (btnW + spacing), y, btnW, btnH, ID_MUL);

        // Row 4
        y += btnH + spacing;
        AddButton(hWnd, L"0", x, y, btnW, btnH, ID_0);
        AddButton(hWnd, L"C", x + btnW + spacing, y, btnW, btnH, ID_CLR);
        AddButton(hWnd, L"=", x + 2 * (btnW + spacing), y, btnW, btnH, ID_EQ);
        AddButton(hWnd, L"/", x + 3 * (btnW + spacing), y, btnW, btnH, ID_DIV);

        break;
    }

    case WM_COMMAND:
    {
        int id = LOWORD(wParam);
        wchar_t buffer[100], res[50];

        if (id >= ID_0 && id <= ID_9)
        {
            GetWindowText(hDisplay, buffer, 100);
            swprintf(buffer + wcslen(buffer), 100 - wcslen(buffer), L"%d", id - ID_0);
            SetWindowText(hDisplay, buffer);
        }
        else if (id == ID_CLR)
        {
            SetWindowText(hDisplay, L"");
            SetWindowText(hOpDisplay, L"");
            firstNum = 0; op = 0;
        }
        else if (id >= ID_ADD && id <= ID_DIV)
        {
            GetWindowText(hDisplay, buffer, 100);
            firstNum = _wtof(buffer);

            
            if (id == ID_ADD) op = L'+';
            else if (id == ID_SUB) op = L'-';
            else if (id == ID_MUL) op = L'*';
            else op = L'/';

            SetWindowText(hDisplay, L""); 

            
            swprintf(buffer, 100, L"%.0f %c", firstNum, op);
            SetWindowText(hOpDisplay, buffer);
        }
        else if (id == ID_EQ)
        {
            GetWindowText(hDisplay, buffer, 100);
            double second = _wtof(buffer);
            double result = 0;

            if (op == L'+') result = firstNum + second;
            else if (op == L'-') result = firstNum - second;
            else if (op == L'*') result = firstNum * second;
            else if (op == L'/') result = (second != 0) ? firstNum / second : 0;

            swprintf(res, 50, L"%.2f", result);
            SetWindowText(hDisplay, res);
            SetWindowText(hOpDisplay, L""); 
        }
        break;
    }

    case WM_DRAWITEM:
    {
        LPDRAWITEMSTRUCT pdis = (LPDRAWITEMSTRUCT)lParam;
        HBRUSH hBrush;
        COLORREF textColor;
        wchar_t text[50];
        GetWindowText(pdis->hwndItem, text, 50);

        if (pdis->CtlID >= ID_0 && pdis->CtlID <= ID_9)
        {
            hBrush = CreateSolidBrush(numColor);
            textColor = RGB(0, 0, 0);
        }
        else if (pdis->CtlID == ID_EQ)
        {
            hBrush = CreateSolidBrush(eqColor);
            textColor = RGB(0, 0, 0);
        }
        else
        {
            hBrush = CreateSolidBrush(opColor);
            textColor = RGB(255, 255, 255);
        }

        FillRect(pdis->hDC, &pdis->rcItem, hBrush);
        SetTextColor(pdis->hDC, textColor);
        SetBkMode(pdis->hDC, TRANSPARENT);
        DrawText(pdis->hDC, text, -1, &pdis->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        DeleteObject(hBrush);
        return TRUE;
    }

    case WM_CTLCOLOREDIT:
        SetTextColor((HDC)wParam, RGB(255, 255, 255));
        SetBkColor((HDC)wParam, bgColor);
        return (LRESULT)hBgBrush;

    case WM_ERASEBKGND:
    {
        RECT rect;
        GetClientRect(hWnd, &rect);
        FillRect((HDC)wParam, &rect, hBgBrush);
        return 1;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}
