// Circle_2d_374946.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Circle_2d_374946.h"
#include <cmath>
#include <time.h>
#include <queue>
#include <stdio.h>

#define MAX_LOADSTRING 100
#define R_CIRCLE 150
#define D_CIRCLE_MARG 60

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
VOID DRAW_TEST(HDC hdc, int ak, int xbegin, int ybegin, int xend, int yend, int index);
RECT LINE_COORDINATES(int xcircle, int ycircle);
VOID DRAW_Waves_and_Ship(HDC hdc, int ak, int xend, int yend);
VOID DRAW_Margaritka(HDC hdc, int ak, int xend, int yend, REAL angle);
INT GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
VOID Example_BMPbuffer(HDC hdc, int ak, int xend, int yend, REAL angle);
VOID Save_Margaritka(HDC hdc, int ak, int xend, int yend, REAL angle);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CIRCLE2D374946, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CIRCLE2D374946));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    GdiplusShutdown(gdiplusToken);
    return (int) msg.wParam;
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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CIRCLE2D374946));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CIRCLE2D374946);
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
    static int count_lines = 0;
    static bool draw_circle = true;
    static int ak = 0;
    static REAL angle = 0.0f;

    switch (message)
    {
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
    case WM_RBUTTONDOWN:
        count_lines++;
        if (count_lines == 8)
        {
            count_lines = 1;
            draw_circle = true;
            InvalidateRect(hWnd, NULL, true);
        }
        else
        {
            InvalidateRect(hWnd, NULL, false);
        }
        //UpdateWindow(hWnd);
        break;
    case WM_LBUTTONDOWN:
        angle += 25.0f;
        InvalidateRect(hWnd, NULL, false);

        if (angle >= 360)
        {
            InvalidateRect(hWnd, NULL, true);
            angle = 0.0f;
        }
        //UpdateWindow(hWnd);
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            RECT rt;
            GetClientRect(hWnd, &rt);
            /*if (count_lines > 0)
            {
                int xcircle = rt.right / 2;
                int ycircle = rt.bottom / 2;
                RECT line = LINE_COORDINATES(xcircle, ycircle);
                DRAW_TEST(hdc, 2, line.left, line.top, line.right, line.bottom, count_lines);
            }
            if(draw_circle)
            {
                Pen bluePen(Color(255, 0, 0, 255));
                DRAW_TEST(hdc, 1, rt.right / 2 - R_CIRCLE, rt.bottom / 2 - R_CIRCLE, R_CIRCLE * 2, R_CIRCLE * 2, 1);
                draw_circle = false;
            }*/
            //DRAW_Waves_and_Ship(hdc, count_ship, rt.right, rt.bottom);
            //DRAW_Margaritka(hdc, ak, rt.right, rt.bottom, angle);
            Save_Margaritka(hdc, ak, rt.right, rt.bottom, angle);
            EndPaint(hWnd, &ps);
        }
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

RECT LINE_COORDINATES(int xcircle, int ycircle) {
    double res;
    RECT rect;
    srand(time(0));

    do
    {
        int x = xcircle - (rand() % R_CIRCLE);
        int y = ycircle - (rand() % R_CIRCLE);
        res = sqrt(pow(xcircle - x, 2) + pow(ycircle - y, 2));
        rect.left = x;
        rect.top = y;
    } while (res > R_CIRCLE);

    char message[55];
    sprintf_s(message, 55, "Line %d %d circle %d %d\n", rect.left, rect.top, xcircle, ycircle);
    OutputDebugStringA(message);

    do
    {
        int x = xcircle + (rand() % R_CIRCLE);
        int y = ycircle + (rand() % R_CIRCLE);
        res = sqrt(pow(x - xcircle, 2) + pow(y - ycircle, 2));
        rect.right = x;
        rect.bottom = y;
    } while (res > R_CIRCLE);

    return rect;
}

//Exercise 1
VOID DRAW_TEST(HDC hdc, int ak, int xbegin, int ybegin, int xend, int yend, int index)
{
    Graphics graphics(hdc); // инициация на графичен обект върху екрана
    //Rect ellipseRect(xend / 2, yend / 2, xend / 4, yend / 4);  //дефиниране на елипса с горен ляв ъгъл 
    //на описващия правоъгълник в средата на растера и рамери ¼ растер
    Rect ellipseRect(xbegin, ybegin, xend, yend);
    //Pen bluePen(Color(255, 0, 0, 255)); // дефиниране на син молив
    //Pen redPen(Color(255, 255, 0, 0)); // дефиниране на червен молив

    Pen redPen(Color(255, 255, 0, 0));
    Pen orangePen(Color(255, 255, 150, 0));
    Pen yellowPen(Color(255, 255, 230, 0));
    Pen greenPen(Color(255, 0, 255, 0));
    Pen bluePen(Color(255, 0, 0, 255));
    Pen purplePen(Color(255, 150, 0, 255));
    Pen pinkPen(Color(255, 255, 0, 255));
    Pen* pens[] = { &redPen, &orangePen, &yellowPen, &greenPen, &bluePen, &purplePen, &pinkPen };

    if (ak == 1)  graphics.DrawEllipse(&bluePen, ellipseRect); //чертае елипса
    else  graphics.DrawLine(pens[index - 1], xbegin, ybegin, xend, yend); //чертае отсечка
}

//Exercise 2
VOID DRAW_Waves_and_Ship(HDC hdc, int ak, int xend, int yend)
{

    // ak – помощен брояч
    // xend, yend – размер на растера

    Graphics graphics(hdc);  // инициализация на графичен режим

    int dx = xend / 9;			       // определяне на стъпка по х	
    int dy = yend / 2;			       // определяне на стъпка по y
    Point p[] = {			// вълни при положение 1
           Point(0, dy + 5),   // стартова точка на първия сплайн
           Point(dx, dy + 30),    // първа управляваща точка на първия сплайн
           Point(2 * dx, dy - 30),    // втора управляваща точка на първия сплайн
           Point(3 * dx, dy + 5),  // крайна точка на първия сплайн и 
                             // стартова точка на втория сплайн
           Point(4 * dx, dy + 30),   // първа управляваща точка на втория сплайн
           Point(5 * dx, dy - 30),  // втора управляваща точка на втория сплайн
           Point(6 * dx, dy + 5),  // крайна точка на втория сплайн и 
                             // стартова точка на третия сплайн
           Point(7 * dx, dy + 30),  // първа управляваща точка на третия сплайн
           Point(8 * dx, dy - 30),  // втора управляваща точка на третия сплайн
           Point(xend,dy + 5)   // крайна точка на третия сплайн
    };
    Point p1[] = {		// вълни при положение 2
           Point(0, dy + 10),
           Point(dx, dy + 20),
           Point(2 * dx, dy - 20),
           Point(3 * dx, dy + 10),
           Point(4 * dx, dy + 20),
           Point(5 * dx, dy - 20),
           Point(6 * dx, dy + 10),
           Point(7 * dx, dy + 20),
           Point(8 * dx, dy - 20),
           Point(xend, dy + 10)
    };
    Point p2[] = {		// вълни при положение 3
           Point(0, dy + 5),
           Point(dx, dy + 10),
           Point(2 * dx, dy - 10),
           Point(3 * dx, dy + 5),
           Point(4 * dx, dy + 10),
           Point(5 * dx, dy - 10),
           Point(6 * dx, dy + 5),
           Point(7 * dx, dy + 10),
           Point(8 * dx, dy - 10),
           Point(xend, dy + 5)
    };
    Point p3[] = {		// вълни при положение 4
           Point(0, dy),
           Point(dx, dy - 30),
           Point(2 * dx, dy + 30),
           Point(3 * dx, dy),
           Point(4 * dx, dy - 30),
           Point(5 * dx, dy + 30),
           Point(6 * dx, dy),
           Point(7 * dx, dy - 30),
           Point(8 * dx, dy + 30),
           Point(xend, dy)
    };

    Pen Neavypen(Color(63, 0, 168, 196)); //цветове на вълните - молив
    SolidBrush Neavybr(Color(63, 0, 168, 196)); //цветове на вълните - четка
    GraphicsPath myPath, myPath1;

    myPath.StartFigure();
    switch (ak)
    {
    case 0:
    {
        myPath.AddLine(0, yend, 0, dy + 5);
        myPath.AddBeziers(p2, 10);
        myPath.AddLine(xend, dy + 5, xend, yend);
    }
    break;
    case 1:
    {
        myPath.AddLine(0, yend, 0, dy + 10);
        myPath.AddBeziers(p1, 10);
        myPath.AddLine(xend, dy + 10, xend, yend);
    }
    break;
    case 2:
    {
        myPath.AddLine(0, yend, 0, dy + 5);
        myPath.AddBeziers(p2, 10);
        myPath.AddLine(xend, dy + 5, xend, yend);
    }
    break;
    case 3:
    {
        myPath.AddLine(0, yend, 0, dy);
        myPath.AddBeziers(p3, 10);
        myPath.AddLine(xend, dy, xend, yend);
    }
    break;
    case 4:
    {
        myPath.AddLine(0, yend, 0, dy + 10);
        myPath.AddBeziers(p, 10);
        myPath.AddLine(xend, dy + 10, xend, yend);
    }
    break;
    }

    myPath.CloseFigure();

    myPath1.StartFigure(); // съставна фигура - кораб

    myPath1.AddLine(xend / 4, yend / 2 - 30, xend / 4 + xend / 5, yend / 2 - 30);
    myPath1.AddBezier(xend / 4, yend / 2 - 30, xend / 4 + xend / 12, yend / 2 + 60, xend / 4 + xend / 6, yend / 2 + 60, xend / 4 + xend / 5, yend / 2 - 30);
    myPath1.AddLine(xend / 4 + xend / 10, yend / 2 - 30, xend / 4 + xend / 10, yend / 2 - 80);
    myPath1.AddLine(xend / 4 + xend / 10 - 1, yend / 2 - 80, xend / 4 + xend / 10 - 1, yend / 2 - 30);
    myPath1.CloseFigure();
    SolidBrush Shipbr(Color(164, 0, 168, 16)); //оцветяване на кораба


    Matrix myMatrix; //иницииране на матрица за трансформации

    if ((ak == 1) || (ak == 3))
    {
        //задаване на отместване до началото на координатната система
        myMatrix.Translate(-float(xend / 4 + xend / 10), -float(yend / 4));
        myMatrix.Rotate(-8.0f);  //задаване на ротация
        myPath1.Transform(&myMatrix);
        float sx = float(1) + float(ak) / 10;
        myMatrix.Scale(sx, sx); //задаване на мащабиране
        myMatrix.Translate(+float(1.8 * (xend / 4 + xend / 10)), +float(3 * (yend / 5)), MatrixOrderAppend);//задаване на транслация
        myPath1.Transform(&myMatrix); // придвижване на кораба
    }
    if (ak == 2)
    {

        myMatrix.Scale(1.125f, 1.125f);
        myMatrix.Translate(-20.0f, -10.0f, MatrixOrderAppend);
        myPath1.Transform(&myMatrix); // придвижване на кораба 
    }

    if (ak == 4)
    {
        myMatrix.Translate(-float(xend / 4 + xend / 10), -float(yend / 4));
        myMatrix.Rotate(+5.0f);
        myPath1.Transform(&myMatrix);
        myMatrix.Scale(1.3f, 1.3f);
        myMatrix.Translate(+float(2.2 * (xend / 4 + xend / 10)), +float(1.8 * (yend / 4)), MatrixOrderAppend);
        myPath1.Transform(&myMatrix);
    }

    graphics.FillPath(&Shipbr, &myPath1);

    graphics.FillPath(&Neavybr, &myPath);
}

VOID DRAW_Margaritka(HDC hdc, int ak, int xend, int yend, REAL angle)
{

    // ak – помощен брояч
    // xend, yend – размер на растера

    Graphics graphics(hdc);  // инициализация на графичен режим

    int end_point_x = xend / 2 + 150, end_point_y = yend - 300;
    int start_x = xend / 2, start_y = yend;

    //1.Stem
    Pen greenPen(Color(164, 0, 168, 16), 3);
    graphics.DrawBezier(&greenPen, xend / 2, yend, xend / 2, yend - 100, xend / 2 + 50, yend - 200, end_point_x, end_point_y);

    //2.Green leaf
    SolidBrush greenleaf(Color(164, 0, 168, 16));
    GraphicsPath myPath_g;

    myPath_g.StartFigure();
    Point greenLeaf[] = {
           Point(start_x, start_y),
           Point(start_x - 200, start_y - 50 - 150),
           Point(start_x + 50, start_y - 80 - 150),
           Point(start_x, start_y)
    };

    myPath_g.AddBezier(greenLeaf[0], greenLeaf[1], greenLeaf[2], greenLeaf[3]);
    graphics.FillPath(&greenleaf, &myPath_g);
    myPath_g.CloseFigure();

    //3.Leaf
    SolidBrush leaflebr(Color(255, 230, 230, 250));
    GraphicsPath myPath;

    myPath.StartFigure();
    Point leaf[] = {		
           Point(end_point_x, end_point_y),
           Point(end_point_x - D_CIRCLE_MARG, end_point_y - D_CIRCLE_MARG / 2 - 150),
           Point(end_point_x + D_CIRCLE_MARG, end_point_y - D_CIRCLE_MARG / 2 - 150),
           Point(end_point_x, end_point_y)
    };

    myPath.AddBezier(leaf[0], leaf[1], leaf[2], leaf[3]);
    graphics.FillPath(&leaflebr, &myPath);
    myPath.CloseFigure();

    Matrix myMatrix; //иницииране на матрица за трансформации

    myMatrix.RotateAt(angle, PointF(end_point_x, end_point_y));
    myPath.Transform(&myMatrix);

    graphics.FillPath(&leaflebr, &myPath);

    //4.Circle
    Rect ellipse(end_point_x - D_CIRCLE_MARG / 2, end_point_y - D_CIRCLE_MARG / 2, D_CIRCLE_MARG, D_CIRCLE_MARG);
    Pen yellowPen(Color(255, 255, 230, 0));
    SolidBrush ellipsebr(Color(255, 255, 230, 0));
    graphics.DrawEllipse(&yellowPen, ellipse);
    graphics.FillEllipse(&ellipsebr, ellipse);
}

INT GetEncoderClsid(const WCHAR* format, CLSID* pClsid)  // helper function
{//помощна функция за извличане на параметри на енкодера
    UINT  num = 0;          // number of image encoders
    UINT  size = 0;         // size of the image encoder array in bytes

    ImageCodecInfo* pImageCodecInfo = NULL;

    GetImageEncodersSize(&num, &size);
    if (size == 0)
        return -1;  // Failure

    pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
    if (pImageCodecInfo == NULL)
        return -1;  // Failure

    GetImageEncoders(num, size, pImageCodecInfo);

    for (UINT j = 0; j < num; ++j)
    {
        if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
        {
            *pClsid = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return j;  // Success
        }
    }

    free(pImageCodecInfo);
    return -1;  // Failure
}

VOID Save_Margaritka(HDC hdc, int ak, int xend, int yend, REAL angle)
{

    // ak – помощен брояч
    // xend, yend – размер на растера
    static Graphics screen(hdc);  // инициализация на графичен режим
    static Bitmap bmp(xend, yend, &screen); //създаване на обект - буфер
    Graphics graphics(&bmp); // създаване на обект от буфера
    
    if (angle == 0)
    {
        graphics.Clear(Color(255, 255, 255));
    }

    int end_point_x = xend / 2 + 150, end_point_y = yend - 300;
    int start_x = xend / 2, start_y = yend;

    //1.Stem
    Pen greenPen(Color(164, 0, 168, 16), 3);
    graphics.DrawBezier(&greenPen, xend / 2, yend, xend / 2, yend - 100, xend / 2 + 50, yend - 200, end_point_x, end_point_y);

    //2.Green leaf
    SolidBrush greenleaf(Color(164, 0, 168, 16));
    GraphicsPath myPath_g;

    myPath_g.StartFigure();
    Point greenLeaf[] = {
           Point(start_x, start_y),
           Point(start_x - 200, start_y - 50 - 150),
           Point(start_x + 50, start_y - 80 - 150),
           Point(start_x, start_y)
    };

    myPath_g.AddBezier(greenLeaf[0], greenLeaf[1], greenLeaf[2], greenLeaf[3]);
    graphics.FillPath(&greenleaf, &myPath_g);
    myPath_g.CloseFigure();

    //3.Leaf
    SolidBrush leaflebr(Color(255, 230, 230, 250));
    GraphicsPath myPath;

    myPath.StartFigure();
    Point leaf[] = {
           Point(end_point_x, end_point_y),
           Point(end_point_x - D_CIRCLE_MARG, end_point_y - D_CIRCLE_MARG / 2 - 150),
           Point(end_point_x + D_CIRCLE_MARG, end_point_y - D_CIRCLE_MARG / 2 - 150),
           Point(end_point_x, end_point_y)
    };

    myPath.AddBezier(leaf[0], leaf[1], leaf[2], leaf[3]);
    graphics.FillPath(&leaflebr, &myPath);
    myPath.CloseFigure();

    Matrix myMatrix; //иницииране на матрица за трансформации

    myMatrix.RotateAt(angle, PointF(end_point_x, end_point_y));
    myPath.Transform(&myMatrix);

    graphics.FillPath(&leaflebr, &myPath);

    //4.Circle
    Rect ellipse(end_point_x - D_CIRCLE_MARG / 2, end_point_y - D_CIRCLE_MARG / 2, D_CIRCLE_MARG, D_CIRCLE_MARG);
    Pen yellowPen(Color(255, 255, 230, 0));
    SolidBrush ellipsebr(Color(255, 255, 230, 0));
    graphics.DrawEllipse(&yellowPen, ellipse);
    graphics.FillEllipse(&ellipsebr, ellipse);

    screen.DrawImage(&bmp, 0, 0); // визуализация на буфера върху екрана

    if (angle + 30 >= 360)
    {
        //промени в параметрите на енкодера 
        CLSID clsid;
        EncoderParameters encoderParameters;
        ULONG             quality;

        GetEncoderClsid(L"image/jpeg", &clsid);
        encoderParameters.Count = 1;
        encoderParameters.Parameter[0].Guid = EncoderQuality;
        encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
        encoderParameters.Parameter[0].NumberOfValues = 1;

        // запис на JPEG формат изображение с компресия 99.
        quality = 99;
        encoderParameters.Parameter[0].Value = &quality;

        bmp.Save(L"Margaritka.jpg", &clsid, &encoderParameters);
        bmp.Save(L"Margaritka.bmp", &clsid, &encoderParameters);
        bmp.Save(L"Margaritka.gif", &clsid, &encoderParameters);
    }
}

VOID Example_BMPbuffer(HDC hdc, int ak, int xend, int yend, REAL angle)
{

    Graphics graphics(hdc);  // инициализация на графичен режим 
    Bitmap bmp(xend, yend, &graphics); //създаване на обект - буфер
    Graphics imgr(&bmp); // създаване на обект от буфера
    imgr.Clear(Color(255, 255, 255)); //задаване на базов цвят бял екран

 // изчертаване на елипса в буфера

    //SolidBrush brush(Color(255, 0, 0, 255));
    //imgr.FillEllipse(&brush, 20, 30, 80, 50);
    DRAW_Margaritka(hdc, ak, xend, yend, angle);

    graphics.DrawImage(&bmp, 0, 0); // визуализация на буфера върху екрана

    //промени в параметрите на енкодера 
    CLSID clsid;
    EncoderParameters encoderParameters;
    ULONG             quality;

    GetEncoderClsid(L"image/jpeg", &clsid);
    encoderParameters.Count = 1;
    encoderParameters.Parameter[0].Guid = EncoderQuality;
    encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;
    encoderParameters.Parameter[0].NumberOfValues = 1;

    // запис на JPEG формат изображение с компресия 99.
    quality = 99;
    encoderParameters.Parameter[0].Value = &quality;

    bmp.Save(L"Proba_elips.jpg", &clsid, &encoderParameters);
}