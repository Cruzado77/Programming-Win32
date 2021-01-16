#define UNICODE
#define _UNICODE

//#define DEBUG

#include <windows.h>
#include "devcaps.h"


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
               static TCHAR szAppName[] = TEXT("DevCaps1");

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

	TCHAR name[] = TEXT("Device Capabilities");
	hwnd = CreateWindow(szAppName,name,
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
LRESULT CALLBACK WndProc (HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	static int cxChar, cxCaps,cyChar;
	HDC hdc;
	int i;
	TCHAR szBuffer[10];
	PAINTSTRUCT ps;
	TEXTMETRIC tm;

	switch(message)
	{
	case WM_CREATE:
		hdc = GetDC(hwnd);

		GetTextMetrics(hdc,&tm);

		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;

		ReleaseDC(hwnd,hdc);

		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd,&ps);

		for (i=0; i<NUMLINES; i++)
		{
			TextOut(hdc,0, cyChar * i,devcaps[i].szLabel,lstrlen(devcaps[i].szLabel));

			TextOut(hdc,14 * cxCaps, cyChar * i,devcaps[i].szDesc,lstrlen(devcaps[i].szDesc));

			SetTextAlign(hdc,TA_RIGHT | TA_TOP);

			TextOut(hdc,14 * cxCaps + 35 * cxChar,
				 cyChar * i,szBuffer,wsprintf(szBuffer,TEXT("%5d"),
						GetDeviceCaps(hdc,devcaps[i].iIndex)
			));

			SetTextAlign(hdc,TA_LEFT | TA_TOP);
		}

		EndPaint(hwnd,&ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd,message,wParam,lParam);
}
