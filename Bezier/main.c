#define UNICODE
#define _UNICODE

#define DEBUG


#include <windows.h>
#include <math.h>

#define NUM	1000
#define TWOPI  (2 * 3.14159)

#ifdef DEBUG
#include <stdlib.h>
#include <stdio.h>
#endif // DEBUG


LRESULT CALLBACK WndProc (HWND,UINT,WPARAM,LPARAM);


int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int iCmdShow)
{
	#ifdef DEBUG
	printf("TCHAR(%lu bytes) HINSTANCE(%lu bytes) WPARAM(%lu bytes)\n",
		(unsigned long) sizeof(TCHAR),(unsigned long) sizeof(HINSTANCE),(unsigned long) sizeof(WPARAM));
	#endif // DEBUG
               static TCHAR szAppName[] = TEXT("Bezier");

               HWND           hwnd;
               MSG                msg;
               WNDCLASS   wndclass;

               wndclass.style                    = CS_HREDRAW | CS_VREDRAW;
               wndclass.lpfnWndProc    = WndProc;
               wndclass.cbClsExtra           = 0;
               wndclass.cbWndExtra        = 0;
               wndclass.hInstance            = hInstance;
               wndclass.hIcon                 	 = LoadIcon(NULL,IDI_APPLICATION);
               wndclass.hCursor	= LoadCursor(NULL,IDC_ARROW);
               wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
               wndclass.lpszMenuName = NULL;
               wndclass.lpszClassName	= szAppName;

               if(!RegisterClass(&wndclass))
	{
		return -1;
	}

	hwnd = CreateWindow(szAppName,TEXT("Bezier Splines"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,CW_USEDEFAULT,
		CW_USEDEFAULT,CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);
	while(GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

void DrawBezier (HDC hdc, POINT apt[])
{
	TCHAR szTemp[100];

	PolyBezier(hdc,apt,4);

	MoveToEx(hdc,apt[0].x,apt[0].y,NULL);
	LineTo(hdc,apt[1].x,apt[1].y);

	MoveToEx(hdc,apt[2].x,apt[2].y,NULL);
	LineTo(hdc,apt[3].x,apt[3].y);
	TextOut(hdc,0,0,szTemp,
		wsprintf(szTemp,TEXT("(%d,%d) (%d,%d) (%d,%d) (%d,%d)"),
			apt[0].x,apt[0].y,apt[1].x,
			apt[1].y,apt[2].x,apt[2].y,
			apt[3].x,apt[3].y));
}

LRESULT CALLBACK WndProc (HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	static POINT apt[4];
	static int cxClient, cyClient;
	HDC hdc;
	PAINTSTRUCT ps;

	switch(message)
	{
//	case WM_CREATE:
//		return 0;012
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);

		apt[0].x = cxClient / 4;
		apt[0].y = cyClient / 2;

		apt[1].x = cxClient / 2;
		apt[1].y = cyClient / 4;

		apt[2].x = cxClient / 2;
		apt[2].y = 3 * cyClient / 4;


		apt[3].x = 3 * cxClient / 4;
		apt[3].y = cyClient / 2;
		return 0;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MOUSEMOVE:
		if(wParam & MK_LBUTTON || wParam & MK_RBUTTON)
		{
			hdc = GetDC(hwnd);

			SelectObject(hdc,GetStockObject(WHITE_PEN));
			DrawBezier(hdc,apt);

			if(wParam & MK_LBUTTON)
			{
				apt[1].x = LOWORD(lParam);
				apt[1].y = HIWORD(lParam);
			}

			if(wParam & MK_RBUTTON)
			{
				apt[2].x = LOWORD(lParam);
				apt[2].y = HIWORD(lParam);
			}

			SelectObject(hdc, GetStockObject(BLACK_PEN));
			DrawBezier (hdc,apt);
			ReleaseDC(hwnd,hdc);
//			InvalidateRect(hwnd,NULL,TRUE);
		}
		return 0;
	case WM_PAINT:
		InvalidateRect(hwnd,NULL,TRUE);

		hdc = BeginPaint(hwnd,&ps);

		DrawBezier(hdc,apt);

		EndPaint(hwnd,&ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd,message,wParam,lParam);
}
