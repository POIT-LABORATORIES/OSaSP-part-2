// Sprite.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Sprite.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
// Имя загружаемой bmp.
WCHAR fileName[] = L"frog.bmp";
// Загруженный .bmp и его атрибуты.
struct ImgStruct{
    HBITMAP hBitmap;
    int x;
    int y;
    int height;
    int width;
} img;
// Выбран .bmp.
BOOL bClicked = FALSE;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void                ShowBitmap(HWND, HBITMAP, int, int);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SPRITE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPRITE));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            // Передача msg в WndProc
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

//
// CreateUserBitmap()
//
// Назначение: загрузка картинки, создание описателя Bitmap
//
HBITMAP CreateUserBitmap(WCHAR fName[])
{
    HBITMAP hBitmap = (HBITMAP)LoadImage(
        NULL, (LPCWSTR)fName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (hBitmap == NULL)
    {
        MessageBox(NULL, __T("Cannot load image"), __T("Error"), MB_OK);
        return NULL;
    }
    else
        return hBitmap;
}

//
// ShowBitmap()
//
// Назначение: прорисовка Bitmap в заданном окне с описателем hWnd.
//
void ShowBitmap(HWND hWnd, HBITMAP hBmp, int x, int y)
{
    BITMAP bm;
    HDC hdcMem;
    DWORD dwSize;
    POINT ptSize, ptOrg;

    HDC hdc = GetDC(hWnd);
    hdcMem = CreateCompatibleDC(hdc);
    SelectObject(hdcMem, hBmp);
    SetMapMode(hdcMem, GetMapMode(hdc));
    GetObject(hBmp, sizeof(BITMAP), (LPVOID)&bm);
    ptSize.x = bm.bmWidth;
    ptSize.y = bm.bmHeight;
    DPtoLP(hdc, &ptSize, 1);
    ptOrg.x = 0;
    ptOrg.y = 0;
    DPtoLP(hdcMem, &ptOrg, 1);
    BitBlt(
        hdc, x, y, ptSize.x, ptSize.y,
        hdcMem, ptOrg.x, ptOrg.y, SRCAND
    );
    DeleteDC(hdcMem);
    ReleaseDC(hWnd, hdc);
}


//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_DBLCLKS;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPRITE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+7);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SPRITE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, L"Программа Спрайт", WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void AlignBitmapInRect(HWND hWnd, ImgStruct *img)
{
    RECT rect = { 0 };
    GetClientRect(hWnd, &rect);
    if (img->x <= rect.left) //rect.left
    {
        img->x += 200;
    }
    if (img->y <= rect.top) //rect.top
    {
        img->y += 200;
    }
    if ((img->x+img->width) >= (rect.right - rect.left)) // >= (rect.right - rect.left)
    {
        img->x -= 200;
    }
    if ((img->y + img->height) >= (rect.bottom - rect.top)) // >= (rect.bottom - rect.top)
    {
        img->y -= 200;
    }
}

void MoveBitmap(HWND hWnd, HBITMAP hBitmap, int direction)
{
    switch (direction)
    {
        case 1:
        {
            img.x -= 50;
        }
        break;
        case 2:
        {
            img.y -= 50;
        }
        break;
        case 3:
        {
            img.x += 50;
        }
        break;
        case 4:
        {
            img.y += 50;
        }
        break;
        default:
        {

        }
    }
    AlignBitmapInRect(hWnd, &img);
    InvalidateRect(hWnd, NULL, TRUE);
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //BOOL fClicked = FALSE;

    switch (message)
    {
    case WM_CREATE:
        {
            BITMAP bm;

            img.hBitmap = CreateUserBitmap(fileName);
            // Получение размеров окна программы.
            RECT rect = { 0 };
            GetWindowRect(hWnd, &rect);
            img.x = ((rect.right - rect.left) / 2) - img.width;
            img.y = ((rect.bottom - rect.top) / 2) - img.height;
            // Получение ширины и высоты bmp.
            GetObject(img.hBitmap, sizeof(BITMAP), (LPVOID)&bm);
            img.width = bm.bmWidth;
            img.height = bm.bmHeight;
        }
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
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
            ShowBitmap(hWnd, img.hBitmap, img.x, img.y);
        }
        break;
    case WM_KEYDOWN:
        {
            switch (wParam)
            {
            case VK_UP:
                {
                    MoveBitmap(hWnd, img.hBitmap, 2);
                }
                break;
            case VK_DOWN:
                {
                    MoveBitmap(hWnd, img.hBitmap, 4);
                }
                break;
            case VK_LEFT:
                {
                    MoveBitmap(hWnd, img.hBitmap, 1);
                }
                break;
            case VK_RIGHT:
                {
                    MoveBitmap(hWnd, img.hBitmap, 3);
                }
                break;
            default:
                {
                }
                break;
            }
            break;
        }
        break;
    case WM_MOUSEWHEEL:
        {
            // Проверка нажатия клавиши <Shift>.
            if (wParam & MK_SHIFT)
            {
                if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
                {
                    MoveBitmap(hWnd, img.hBitmap, 3);
                }
                else
                {
                    MoveBitmap(hWnd, img.hBitmap, 1);
                }
            }
            else
            {
                if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
                {
                    MoveBitmap(hWnd, img.hBitmap, 4);
                }
                else
                {
                    MoveBitmap(hWnd, img.hBitmap, 2);
                }
            }
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;
    case WM_LBUTTONDOWN:
        {
        if ((LOWORD(lParam) >= img.x) and (LOWORD(lParam) <= img.x + img.width)
            and (HIWORD(lParam) >= img.y) and (HIWORD(lParam) <= img.y + img.height))
        {
            bClicked = TRUE;
        }
        else
            bClicked = FALSE;
        }
        break; 
    case WM_MOUSEMOVE:
        {
            if (bClicked)
            {
                img.x = LOWORD(lParam) - img.width/2;
                img.y = HIWORD(lParam) - img.height/2;
                AlignBitmapInRect(hWnd, &img);
                InvalidateRect(hWnd, NULL, TRUE);
            }
        }
        break;
    case WM_LBUTTONUP:
        bClicked = FALSE;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
