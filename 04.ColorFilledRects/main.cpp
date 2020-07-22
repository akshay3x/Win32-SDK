#include <Windows.h>
#include <stdio.h>
#include "resource.h"

#define WIN_WIDTH 800
#define WIN_HIGHT 600

#define IDT_TIMER1 101

HWND ghwnd = NULL;

DWORD dwStyle;
WINDOWPLACEMENT wpPrev = {sizeof(WINDOWPLACEMENT)};

bool gbFullscreen = false;
FILE *gpDbgFile = NULL;

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
	
	if(fopen_s(&gpDbgFile, "DebugLog.TXT", "w")!= 0)
	{
		MessageBox(NULL, TEXT("Can Not Open Desired File..."),TEXT("Error"), MB_OK);
		exit(0);
	}
	else
	{
		fprintf(gpDbgFile, "Log File Created\n");
		fprintf(gpDbgFile, "-----------------\n");
	}
	
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
	fprintf(gpDbgFile, "DEBUG:Class Registerd\n");
	
	//SystemParametersInfo
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);

	HorPos = (rect.right - WIN_WIDTH) / 2;
	VerPos = (rect.bottom - WIN_HIGHT) / 2;
	
	//Creating Window
	hwnd = CreateWindow(szAppName,
						TEXT("MyWindowFileIO"),
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
	fprintf(gpDbgFile, "DEBUG:Window Created\n\n");
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
	RECT rcBar;
	
	HBRUSH hbr;
	
	static int R = 255;
	static int G = 255;
	static int B = 255;
	static int R1 = 0;
	static int G1 = 0;
	static int B1 = 0;
	static int counter = 0;
	static int check = 0;
	TCHAR str[] = TEXT("Hello World");
	
	
	GetClientRect(hwnd, &rc);	

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
				InvalidateRect(hwnd, NULL, TRUE);
				break;				
			}
			break;

		case WM_PAINT:
			hdc = BeginPaint(hwnd, &ps);
			check = counter % 8;
			switch (check)
			{				
				case 7:
				rcBar.top = rc.top;
				rcBar.left = (rc.right / 7) * 6;
				rcBar.bottom = rc.bottom;
				rcBar.right = (rc.right / 7) * 7;
				
				hbr = CreateSolidBrush(RGB(255, 255, 0));
				FillRect(hdc, &rcBar, hbr);
				fprintf(gpDbgFile, "DEBUG:7YELLOW\n");
				
				case 6:
				rcBar.top = rc.top;
				rcBar.left = (rc.right / 7) * 5;
				rcBar.bottom = rc.bottom;
				rcBar.right = (rc.right / 7) * 6;
				
				hbr = CreateSolidBrush(RGB(255, 0, 255));
				FillRect(hdc, &rcBar, hbr);
				fprintf(gpDbgFile, "DEBUG:6MAGENTA\n");
				
				case 5:
				rcBar.top = rc.top;
				rcBar.left = (rc.right / 7) * 4;
				rcBar.bottom = rc.bottom;
				rcBar.right = (rc.right / 7) * 5;
				
				hbr = CreateSolidBrush(RGB(0, 255, 255));
				FillRect(hdc, &rcBar, hbr);
				fprintf(gpDbgFile, "DEBUG:5CYAN\n");
				
				case 4:
				rcBar.top = rc.top;
				rcBar.left = (rc.right / 7) * 3;
				rcBar.bottom = rc.bottom;
				rcBar.right = (rc.right / 7) * 4;
				
				hbr = CreateSolidBrush(RGB(0, 0, 255));
				FillRect(hdc, &rcBar, hbr);
				fprintf(gpDbgFile, "DEBUG:4BLUE\n");
				
				case 3:
				rcBar.top = rc.top;
				rcBar.left = (rc.right / 7) * 2;
				rcBar.bottom = rc.bottom;
				rcBar.right = (rc.right / 7) * 3;
			
				hbr = CreateSolidBrush(RGB(0, 255, 0));
				FillRect(hdc, &rcBar, hbr);
				fprintf(gpDbgFile, "DEBUG:3GREEN\n");
				
				case 2:
				rcBar.top = rc.top;
				rcBar.left = (rc.right / 7) * 1;
				rcBar.bottom = rc.bottom;
				rcBar.right = (rc.right / 7) * 2;
				
				hbr = CreateSolidBrush(RGB(255, 0, 0));
				FillRect(hdc, &rcBar, hbr);
				fprintf(gpDbgFile, "DEBUG:2RED\n");
				
				case 1:
				rcBar.top = rc.top;
				rcBar.left = (rc.right / 7) * 0;
				rcBar.bottom = rc.bottom;
				rcBar.right = (rc.right / 7) * 1;
				
				hbr = CreateSolidBrush(RGB(255, 255, 255));
				FillRect(hdc, &rcBar, hbr);
				fprintf(gpDbgFile, "DEBUG:1WHITE\n");
				fprintf(gpDbgFile, "----------------------\n\n");
			}
			EndPaint(hwnd, &ps);
			break;
						
			case WM_KEYDOWN:
			{				
				switch(LOWORD(wParam))
				{
					case VK_ESCAPE:
						DestroyWindow(hwnd);
						break;
					
					case 'F':
					case 'f':
						ToggleFullscreen();
						break;
					
					default:
						break;
				}
			}
			break;
			
		case WM_DESTROY:
			fprintf(gpDbgFile, "---------------------\n");
			fprintf(gpDbgFile, "DEBUG:Log File Closed\n");
			fclose(gpDbgFile);
			gpDbgFile = NULL;
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