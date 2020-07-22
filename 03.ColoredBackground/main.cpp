#include <Windows.h>
#include <stdio.h>
#include "resource.h"

#define WIN_WIDTH 800
#define WIN_HIGHT 600

#define IDT_TIMER1 201

HWND ghwnd = NULL;

DWORD dwStyle;
WINDOWPLACEMENT wpPrev = {sizeof(WINDOWPLACEMENT)};

bool gbFullscreen = false;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	char szAppName[] = "MyWindow";
	
	RECT rect;
	UINT HorPos;
	UINT VerPos;
	
	//initializing class 12 members
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbWndExtra = 0;
	wndclass.cbClsExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	
	//Registering above window
	RegisterClassEx(&wndclass);
	
	//SystemParametersInfo
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);

	HorPos = (rect.right - WIN_WIDTH) / 2;
	VerPos = (rect.bottom - WIN_HIGHT) / 2;
	
	//Creating Window
	hwnd = CreateWindow(szAppName,
						TEXT("MyWindow"),
						WS_OVERLAPPEDWINDOW,
						HorPos,
						VerPos,
						WIN_WIDTH,
						WIN_HIGHT,
						NULL,
						NULL,
						hInstance,
						NULL
						);
	ghwnd = hwnd;
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);
	
	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return(int(msg.wParam));
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	//function declarations
	void ToggleFullscreen(void);
	
	//variable Declarations
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rc;
	HBRUSH hbr;
	static int R = 0;
	static int G = 0;
	static int B = 0;
	static int counter = 0;
	TCHAR str[] = TEXT("Hello World");
	
		
	switch(iMsg)
	{
		case WM_CREATE:
			SetTimer(hwnd, IDT_TIMER1, 1000, NULL);
			break;
			
		case WM_TIMER: 
			switch (wParam) 
			{ 
				case IDT_TIMER1: 
					counter++;
					switch(counter % 6)
					{
						case 1:
							R = 255; G = 0; B = 0;
							break;
						case 2:
							R = 0; G = 255; B = 0;
							break;
							
						case 3:
							R = 0; G = 0; B = 255;
							break;
							
						case 4:
							R = 0; G = 255; B = 255;
							break;
							
						case 5:
							R = 255; G = 0; B = 255;
							break;
							
						case 0:
							R = 255; G = 255; B = 0;
							break;
					}
					InvalidateRect(hwnd, NULL, TRUE);
					break;
			}
			break;

		case WM_PAINT:
			GetClientRect(hwnd, &rc);
			hdc = BeginPaint(hwnd, &ps);
			SetBkMode(hdc, TRANSPARENT); 
			SetTextColor(hdc, RGB(255, 255, 255));
			hbr = CreateSolidBrush(RGB(R,G,B));
			FillRect(hdc, &rc, hbr);
			DrawText(hdc, str, -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			EndPaint(hwnd, &ps);
			break;
						
		case WM_KEYDOWN:			
				switch(LOWORD(wParam))
				{
					case VK_ESCAPE:
						DestroyWindow(hwnd);
						break;
					
					case 'F':
					case 'f':
						ToggleFullscreen();
						break;
				}
			break;
			
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
			
	}
	
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void ToggleFullscreen(void)
{
	//variable declarations
	MONITORINFO mInfo = {sizeof(MONITORINFO)};

	//code
	if(gbFullscreen == false)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		if(dwStyle && WS_OVERLAPPEDWINDOW)
		{
			if(GetWindowPlacement(ghwnd, &wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mInfo))
			{
				SetWindowLong(ghwnd, GWL_STYLE, (dwStyle & ~(WS_OVERLAPPEDWINDOW)));
				SetWindowPos(	ghwnd,
								HWND_TOP,
								mInfo.rcMonitor.left,
								mInfo.rcMonitor.top,
								mInfo.rcMonitor.right - mInfo.rcMonitor.left,
								mInfo.rcMonitor.bottom - mInfo.rcMonitor.top,
								SWP_NOZORDER | SWP_FRAMECHANGED);
			}
		}
		ShowCursor(FALSE);
		gbFullscreen = true;
	}

	else
	{
		SetWindowLong(ghwnd, GWL_STYLE, (dwStyle | WS_OVERLAPPEDWINDOW));
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowPos(ghwnd,
					HWND_TOP,
					0,
					0,
					0,
					0,
					SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);

				ShowCursor(TRUE);
				gbFullscreen = false;
	}
}
