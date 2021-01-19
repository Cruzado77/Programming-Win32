#define UNICODE
#define _UNICODE

#include <windows.h>
#include <stdlib.h>

int cxClient, cyClient;

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
void DrawRectangle(HWND hwnd);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR
		szCmdLine,int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("RandRect");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;

	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	wndclass.hCursor = LoadCursor(NULL,IDC_ARROW);
	wndclass.hIcon = LoadIcon(NULL,IDI_APPLICATION);
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;

	if(!RegisterClass(&wndclass)) return -1;

	hwnd = CreateWindow (szAppName,TEXT("Random Rectangles"),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,CW_USEDEFAULT,
			CW_USEDEFAULT, CW_USEDEFAULT,
			NULL,NULL,hInstance,NULL);
	 ShowWindow(hwnd,iCmdShow);
	 UpdateWindow(hwnd);

	 while (TRUE)
	 {
	 	if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
			DrawRectangle(hwnd);
	 }
	 return msg.wParam;
}

void Show (HWND hwnd,HDC hdc, int xText,int yText, int iMapMode,
	TCHAR * szMapMode)
{
	TCHAR szBuffer[60];
	RECT rect;

	SaveDC(hdc);

	SetMapMode(hdc,iMapMode);
	GetClientRect(hwnd,&rect);
	DPtoLP(hdc,(PPOINT) &rect,2);

	RestoreDC(hdc,-1);

	TextOut(hdc,xText,yText,szBuffer,wsprintf(szBuffer,TEXT("%-20s %7d %7d %7d %7d"),
			szMapMode,rect.left,rect.right,rect.top,rect.bottom));
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam,LPARAM lParam)
{
	switch (message)
	{
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd,message,wParam,lParam);
}

void DrawRectangle(HWND hwnd)
{
	HBRUSH hBrush;
	HDC hdc;
	RECT rect;

	if(cxClient == 0 || cyClient == 0)
	{
		return;
	}

	SetRect(&rect,rand () % cxClient, rand() % cyClient,
		           rand()  % cxClient, rand() % cyClient);
	hBrush = CreateSolidBrush(
			RGB(rand() % 256, rand() % 256, rand() % 256));
	hdc = GetDC(hwnd);

	FillRect(hdc,&rect,hBrush);
	ReleaseDC(hwnd,hdc);
	DeleteObject(hBrush);
}
