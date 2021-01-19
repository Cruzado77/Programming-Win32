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
               static TCHAR szAppName[] = TEXT("SineWave");

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

	hwnd = CreateWindow(szAppName,TEXT("Sine Wave"),
		WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,
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
LRESULT CALLBACK WndProc (HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	static int cxClient, cyClient;
	HDC hdc;
	int i;
	PAINTSTRUCT ps;
	POINT apt[NUM];

	switch(message)
	{
//	case WM_CREATE:
//		return 0;012
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd,&ps);

		MoveToEx(hdc,0,cyClient / 2,NULL);
		LineTo(hdc,cxClient,cyClient / 2);

		for (i = 0;i< NUM;i++)
		{
			apt[i].x = i * cxClient / NUM;
			apt[i].y = (int) (cyClient / 2 * (1 - sin(TWOPI * i / NUM)));
		}

		Polyline(hdc,apt,NUM);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd,message,wParam,lParam);
}
