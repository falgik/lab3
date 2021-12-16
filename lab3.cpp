// lab3.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "lab3.h"
#include <string>

#define TIMEOUT 20
struct Settings
{
    Settings() { ZeroMemory(text, sizeof(TCHAR) * 50); };
    int brR, brG, brB;
    int pnR, pnG, pnB;
    bool isAuthor;
    int scale;
    int dtY, dtM, dtD;
    TCHAR text[50];
};





#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    DlgProc(HWND, UINT, WPARAM, LPARAM);


void drawCircle(HDC dc, int x, int y, int r)
{
    Ellipse(dc, x - r, y - r, x + r, y + r);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.
    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB3, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB3));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB3));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB3);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   return TRUE;
}

Settings globalSettings;
Settings localSettings;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        ZeroMemory(&globalSettings, sizeof(globalSettings));
        break;
    case WM_COMMAND:
        {           
            switch (LOWORD(wParam))
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DlgProc);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

    case WM_PAINT:
    {

        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        if (globalSettings.isAuthor)
        {
            
            int scale = globalSettings.scale + 1;

            HBRUSH br = CreateSolidBrush(RGB(globalSettings.brR, globalSettings.brG, globalSettings.brB));
            HPEN pn = CreatePen(0, 5, RGB(globalSettings.pnR, globalSettings.pnG, globalSettings.pnB));

            SelectObject(hdc, br);
            SelectObject(hdc, pn);


            POINT p;
            MoveToEx(hdc, 250/ scale, 50/ scale, &p);
            LineTo(hdc, 450/ scale, 400/ scale);
            LineTo(hdc, 50/ scale, 400/ scale);
            LineTo(hdc, 250/ scale, 50/ scale);

            drawCircle(hdc, 250/scale, 50 / scale, 50 / scale);
            drawCircle(hdc, 450 / scale, 400 / scale, 50 / scale);
            drawCircle(hdc, 50 / scale, 400 / scale, 50 / scale);

            SetBkMode(hdc, TRANSPARENT);
            TextOut(hdc, 10, 10, globalSettings.text, _countof(globalSettings.text));  

            std::wstring buf;

            buf += std::to_wstring( globalSettings.dtD + 1);
            buf += L".";
            buf += std::to_wstring( globalSettings.dtM + 1);
            buf += L".";
            buf += std::to_wstring( globalSettings.dtY + 1900);
            
            TextOut(hdc, 10, 30, buf.c_str(), buf.size());

            DeleteObject(br);
            DeleteObject(pn);
        }
        EndPaint(hWnd, &ps);
    }
    break;

    case WM_DESTROY:
    {
        PostQuitMessage(0);
    }
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int timer;

    switch (message)
    {
    case WM_INITDIALOG:
    {
      
        timer = TIMEOUT;
        SetTimer(hDlg, 1, 1000, NULL);
        localSettings = globalSettings;

        SetScrollRange(GetDlgItem(hDlg, IDC_SCP_R), SB_CTL, 0, 255, TRUE);
        SetScrollRange(GetDlgItem(hDlg, IDC_SCP_G), SB_CTL, 0, 255, TRUE);
        SetScrollRange(GetDlgItem(hDlg, IDC_SCP_B), SB_CTL, 0, 255, TRUE);

        SetScrollRange(GetDlgItem(hDlg, IDC_SCBR_R), SB_CTL, 0, 255, TRUE);
        SetScrollRange(GetDlgItem(hDlg, IDC_SCBR_G), SB_CTL, 0, 255, TRUE);
        SetScrollRange(GetDlgItem(hDlg, IDC_SCBR_B), SB_CTL, 0, 255, TRUE);

        SetScrollPos(GetDlgItem(hDlg, IDC_SCP_R), SB_CTL, localSettings.pnR, TRUE);
        SetScrollPos(GetDlgItem(hDlg, IDC_SCP_G), SB_CTL, localSettings.pnG, TRUE);
        SetScrollPos(GetDlgItem(hDlg, IDC_SCP_B), SB_CTL, localSettings.pnB, TRUE);

        SetScrollPos(GetDlgItem(hDlg, IDC_SCBR_R), SB_CTL, localSettings.brR, TRUE);
        SetScrollPos(GetDlgItem(hDlg, IDC_SCBR_G), SB_CTL, localSettings.brG, TRUE);
        SetScrollPos(GetDlgItem(hDlg, IDC_SCBR_B), SB_CTL, localSettings.brB, TRUE);

        CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, IDC_RADIO1 + localSettings.scale);
        SendDlgItemMessage(hDlg, IDC_CHECK1, BM_SETCHECK, localSettings.isAuthor, 0);

        for (int i = 1900; i < 2022; i++)
        {
            TCHAR buf[50];
            _stprintf_s(buf, TEXT("%d"), i);
            SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)buf);
        }
        for (int i = 1; i < 13; i++)
        {
            TCHAR buf[50];
            _stprintf_s(buf, TEXT("%d"), i);
            SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM)buf);
        }
        for (int i = 1; i < 32; i++)
        {
            TCHAR buf[50];
            _stprintf_s(buf, TEXT("%d"), i);
            SendMessage(GetDlgItem(hDlg, IDC_COMBO3), CB_ADDSTRING, 0, (LPARAM)buf);
        }

        SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_SETCURSEL, localSettings.dtY, 0);
        SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_SETCURSEL, localSettings.dtM, 0);
        SendMessage(GetDlgItem(hDlg, IDC_COMBO3), CB_SETCURSEL, localSettings.dtD, 0);


      



        SetDlgItemText(hDlg, IDC_EDIT1,globalSettings.text);
        return (INT_PTR)TRUE;
    }
    case WM_TIMER:
    {
        TCHAR buf[50];
        _stprintf_s(buf, _T("%d"), timer);
        SetDlgItemText(hDlg, IDC_STATIC1234, buf);
        timer--;
        if (timer < 0)
        {
            KillTimer(hDlg,1);
            EndDialog(hDlg, LOWORD(wParam));
        }
    }
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hDlg, &ps);

        SetDlgItemInt(hDlg, IDC_P_R, localSettings.pnR,0);
        SetDlgItemInt(hDlg, IDC_P_G, localSettings.pnG,0);
        SetDlgItemInt(hDlg, IDC_P_B, localSettings.pnB,0);

        SetDlgItemInt(hDlg, IDC_BR_R, localSettings.brR, 0);
        SetDlgItemInt(hDlg, IDC_BR_G, localSettings.brG, 0);
        SetDlgItemInt(hDlg, IDC_BR_B, localSettings.brB, 0);

        EndPaint(hDlg, &ps);
    }
        break;
    case WM_HSCROLL:
    {
        timer = TIMEOUT;
        HWND hScrol = (HWND)lParam;
        int* temp = nullptr;

        if (hScrol == GetDlgItem(hDlg, IDC_SCP_R))
            temp = &localSettings.pnR;
        else if (hScrol == GetDlgItem(hDlg, IDC_SCP_G))
            temp = &localSettings.pnG;
        else if (hScrol == GetDlgItem(hDlg, IDC_SCP_B))
            temp = &localSettings.pnB;
        else if (hScrol == GetDlgItem(hDlg, IDC_SCBR_R))
            temp = &localSettings.brR;
        else if (hScrol == GetDlgItem(hDlg, IDC_SCBR_G))
            temp = &localSettings.brG;
        else if (hScrol == GetDlgItem(hDlg, IDC_SCBR_B))
            temp = &localSettings.brB;
        
        switch (LOWORD(wParam))
        {
        case SB_LINELEFT: (*temp)--; break;
        case SB_LINERIGHT: (*temp)++; break;
        case SB_PAGELEFT: (*temp) -= 5; break;
        case SB_PAGERIGHT: (*temp) += 5; break;
        case SB_THUMBPOSITION: (*temp) = HIWORD(wParam); break;
        }

        (*temp) = max(0, min((*temp), 255));

        if ((*temp) != GetScrollPos(hScrol, SB_CTL))
        {
            SetScrollPos((HWND)lParam, SB_CTL, (*temp), TRUE);
            InvalidateRect(hDlg, 0, 0);
        }
      
        return TRUE;
    }
    case WM_COMMAND:
    {

        timer = TIMEOUT;
        switch (LOWORD(wParam))
        {
        case IDC_RADIO1: localSettings.scale = 0; break;
        case IDC_RADIO2: localSettings.scale = 1; break;
        case IDC_RADIO3: localSettings.scale = 2; break;
        case IDC_CHECK1: localSettings.isAuthor = !localSettings.isAuthor; break;
        case IDOK:
            localSettings.dtY = SendMessage(GetDlgItem(hDlg, IDC_COMBO1), CB_GETCURSEL, 0, 0);
            localSettings.dtM = SendMessage(GetDlgItem(hDlg, IDC_COMBO2), CB_GETCURSEL, 0, 0);
            localSettings.dtD = SendMessage(GetDlgItem(hDlg, IDC_COMBO3), CB_GETCURSEL, 0, 0);

            GetWindowText(GetDlgItem(hDlg,IDC_EDIT1), localSettings.text, 50);
           

            

            globalSettings = localSettings;
            EndDialog(hDlg, LOWORD(wParam));
            InvalidateRect(GetParent(hDlg), 0, 1);
            return (INT_PTR)TRUE;
            break;
        case IDCANCEL:
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
            break;
        }
    }
        break;
    break;
    }

    return (INT_PTR)FALSE;
}
