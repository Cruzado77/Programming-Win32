#define UNICODE
#define _UNICODE

#define DEBUG


#include <windows.h>
#include <math.h>

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
               static TCHAR szAppName[] = TEXT("Altwind");

               HWND           hwnd;
               MSG                msg;
               WNDCLASS   wndclass;
               HBRUSH hBrush = CreateSolidBrush(RGB(230,255,250));

               wndclass.style                    = CS_HREDRAW | CS_VREDRAW;
               wndclass.lpfnWndProc    = WndProc;
               wndclass.cbClsExtra           = 0;
               wndclass.cbWndExtra        = 0;
               wndclass.hInstance            = hInstance;
               wndclass.hIcon                  = LoadIcon(NULL,IDI_APPLICATION);
               wndclass.hCursor	= LoadCursor(NULL,IDC_ARROW);
               wndclass.hbrBackground = hBrush;
               wndclass.lpszMenuName = NULL;
               wndclass.lpszClassName	= szAppName;

               if(!RegisterClass(&wndclass))
	{
		return -1;
	}

	hwnd = CreateWindow(szAppName,TEXT("Alternate and Winding Fill Modeç"),
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

	DeleteObject(hBrush);
	return msg.wParam;
}

LRESULT CALLBACK WndProc (HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	static POINT aptFigure[10]  = {
			10,70,	50,70,	50,10,	90,10,	90,50,
			30,50,	30,90,	70,90,	70,30,	10,30
	};
	static int cxClient, cyClient;
	HDC hdc;
	PAINTSTRUCT ps;
	POINT apt[10];

	switch(message)
	{
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd,&ps);

		SelectObject(hdc, GetStockObject(GRAY_BRUSH));

		for(register int i = 0; i < 10; i++)
		{
			apt[i].x = cxClient * aptFigure[i].x / 200;
			apt[i].y = cyClient * aptFigure[i].y / 100;
		}

		SetPolyFillMode(hdc,ALTERNATE);
		Polygon(hdc,apt,10);

		for(register int i = 0; i< 10; i++)
		{
			apt[i].x += cxClient / 2;
		}

		SetPolyFillMode(hdc,WINDING);
		Polygon(hdc,apt,10);

		EndPaint(hwnd,&ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd,message,wParam,lParam);
}
