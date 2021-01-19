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
               static TCHAR szAppName[] = TEXT("LineDemo");

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

	hwnd = CreateWindow(szAppName,TEXT("Line Demo"),
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
	PAINTSTRUCT ps;

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

		Rectangle(hdc,cxClient / 8,cyClient / 8,7 * cxClient / 8,
			7 * cyClient / 8);
		MoveToEx(hdc,0,0,NULL);
		LineTo(hdc,cxClient,cyClient);

		MoveToEx(hdc,0,cyClient,NULL);
		LineTo(hdc,cxClient,0);

		Ellipse(hdc,cxClient / 8,cyClient / 8, 7 * cxClient / 8, 7 * cyClient / 8);

		RoundRect(hdc,cxClient / 4, cyClient / 4,
			3 * cxClient / 4, 3 * cyClient / 4,
			cxClient / 4, cyClient / 4);
		EndPaint(hwnd,&ps);

		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd,message,wParam,lParam);
}
