#include <Windows.h>
#include <stdio.h>
#include "resource.h"

#define WIN_WIDTH 800
#define WIN_HIGHT 600

#define IDT_TIMER1 101

HWND ghwnd = NULL;
HWND ghwnd2 = NULL;
HWND ghwnd3 = NULL;

DWORD dwStyle;
WINDOWPLACEMENT wpPrev = {sizeof(WINDOWPLACEMENT)};

bool gbFullscreen = false;
FILE *gpDbgFile = NULL;

FILE *gpClassNames = NULL;
FILE *gpChildClassNames = NULL;
static int isAccessed = 0; 

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

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{

	TCHAR ClassNames[100] = {0};
	
	GetClassName(hwnd, ClassNames, sizeof(ClassNames));
		
	if(strncmp(ClassNames, "Notepad", 5) == 0)
	{
			ghwnd2 = hwnd;
			isAccessed = 1;
			return FALSE;
	}
	
	return TRUE;
}

BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam)
{
	TCHAR ChildClassNames[100] = {0};

	GetClassName(hwnd, ChildClassNames, sizeof(ChildClassNames));
		
	if(strncmp(ChildClassNames, "Edit", 5) == 0)
	{
			ghwnd3 = hwnd;
			return FALSE;
	}
	
	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	//function declarations
	void ToggleFullscreen(void);
	
	//variable Declarations
	static HMENU hmenu;
	HBRUSH hbr = NULL;
	UINT iFlag = 2;
	UINT count = 0;
	TCHAR tMsg[] = "Notepad Accessed Externally\n";


	HDC hdc;
	PAINTSTRUCT ps;
	RECT rc;
	static int R = 255;
	static int G = 255;
	static int B = 255;
	TCHAR str[] = TEXT("[1]  F: Toggle Fullscreen\n\n\
						[2]  G: Access Notepad\n\n\
						[3]  H: Access Text Editable Area Of Notepad\n\n\
						[4]  J: Hide Menubar\n\n\
						[5]  K: Show Menubar\n\n\
						[6]  L: Hide Menubar\n\n\
						[7]  M: Minimize Notepad\n\n\
						[8]  N: Maximize Notepad\n\n\
						[9]  B: Restore Notepad\n\n\
						[10]  V: Display Message on Notepad\n\n\
						[11]  Q: Quit Notepad\n\n\
						[12]  ESCAPE: Quit Application\n\n");

	switch(iMsg)
	{
		case WM_CREATE:
			MessageBox(hwnd, TEXT("Run Notepad First...!!!"), TEXT("Message"), MB_OK);
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

					case 'G':
					case 'g':
						EnumWindows(EnumWindowsProc, 0);
						if(isAccessed == 1)
						{
							MessageBox(hwnd, TEXT("Notepad Accessed...!!!"), TEXT("Message"), MB_OK);
							hmenu = GetMenu(ghwnd2);
						}
						else
							MessageBox(hwnd, TEXT("Notepad Access Failed...!!!"), TEXT("Message"), MB_OK);
						break;

					case 'H':
					case 'h':
						EnumChildWindows(ghwnd2, EnumChildProc, 0);
						MessageBox(hwnd, TEXT("Notepad Accessed: Handle to Edit Child Window!!!"), TEXT("Message"), MB_OK);
						break;

					case 'j':
					case 'J':
						SetMenu(ghwnd2, NULL);
						SetFocus(ghwnd);
						SetForegroundWindow(ghwnd);
						break;

					case 'K':
					case 'k':
						SetMenu(ghwnd2, hmenu);
						SetFocus(ghwnd);
						SetForegroundWindow(ghwnd);
						break;

					case 'L':
					case 'l':
						SetWindowText(ghwnd2, "Hello Notepad");
						SetFocus(ghwnd);
						SetForegroundWindow(ghwnd);
						break;

					case 'M':
					case 'm':
						ShowWindow(ghwnd2, SW_MINIMIZE);
						SetFocus(ghwnd);
						SetForegroundWindow(ghwnd);
						break;

					case 'n':
					case 'N':
						ShowWindow(ghwnd2, SW_MAXIMIZE);
						SetFocus(ghwnd);
						SetForegroundWindow(ghwnd);
						break;

					case 'B':
					case 'b':
						ShowWindow(ghwnd2, SW_RESTORE);
						SetFocus(ghwnd);
						SetForegroundWindow(ghwnd);
						break;

					case 'v':
					case 'V':
						while(count != 28)
						{
							PostMessage(ghwnd3, WM_CHAR, (WPARAM)tMsg[count], (LPARAM)0);
							count++;
						}
						SetFocus(ghwnd);
						SetForegroundWindow(ghwnd);
						break;

					case 'Q':
					case 'q':
						PostMessage(ghwnd2, WM_QUIT, (WPARAM)0, (LPARAM)0);
						SetFocus(hwnd);
						break;

					default:
						break;
				}
			}
			break;

		case WM_PAINT:
			GetClientRect(hwnd, &rc);
			hdc = BeginPaint(hwnd, &ps);
			SetBkColor(hdc, RGB(0, 0, 0));
			SetTextColor(hdc, RGB(R, G, B));
			DrawText(hdc, str, -1, &rc,  DT_VCENTER |DT_LEFT);
			EndPaint(hwnd, &ps);
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