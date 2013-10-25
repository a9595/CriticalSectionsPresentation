#include <windows.h>
#include <tchar.h>
#include <ctime>  
LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

int cxClient, cyClient ;
CRITICAL_SECTION  CS;
HDC         hdc, hdc_left, hdc_up ;
PAINTSTRUCT ps ;
RECT        rect ;
RECT		tmpRect, tmpRect2, back_Rect, back_Rect2;
HBRUSH		hBrush_black, hBrush_white;
HPEN		hPen1, hPen2;
DWORD		first_thread, second_thread;
double tmp1,tmp2, tmp3,tmp4;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT ("CritSect") ;
	HWND         hwnd ;
	MSG          msg ;
	WNDCLASS     wndclass ;

	wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
	wndclass.lpfnWndProc   = WndProc ;
	wndclass.cbClsExtra    = 0 ;
	wndclass.cbWndExtra    = 0 ;
	wndclass.hInstance     = hInstance ;
	wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
	wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
	wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
	wndclass.lpszMenuName  = NULL ;
	wndclass.lpszClassName = szAppName ;

	if (!RegisterClass (&wndclass))
	{
		MessageBox (NULL, TEXT ("This program requires Windows NT!"), 
			szAppName, MB_ICONERROR) ;
		return 0 ;
	}

	hwnd = CreateWindow (szAppName,                  // window class name
		TEXT ("Colors Developed by Andriy Kovalchuk"), // window caption
		WS_OVERLAPPEDWINDOW,        // window style
		CW_USEDEFAULT,              // initial x position
		CW_USEDEFAULT,              // initial y position
		CW_USEDEFAULT,              // initial x size
		CW_USEDEFAULT,              // initial y size
		NULL,                       // parent window handle
		NULL,                       // window menu handle
		hInstance,                  // program instance handle
		NULL) ;                     // creation parameters

	ShowWindow (hwnd, iCmdShow) ;
	UpdateWindow (hwnd) ;

	while (GetMessage (&msg, NULL, 0, 0))
	{
		TranslateMessage (&msg) ;
		DispatchMessage (&msg) ;
	}
	return msg.wParam ;
}

DWORD WINAPI Car_left(void* hWnd)
{
	//car-left
	
	tmp1 = 0.01;	tmp2 = 0.05;
	bool in_cs = false;
	do 
	{
		back_Rect = tmpRect;
		SetRect(&tmpRect, rect.right*tmp1, rect.bottom*0.45, rect. right*tmp2, rect.bottom*0.55);
		InvalidateRect((HWND)hWnd, &back_Rect, TRUE);
		InvalidateRect((HWND)hWnd, &tmpRect, TRUE);

		tmp1+=0.001;
		tmp2+=0.001;
		if(tmp2 >= 0.4 && tmp2 <0.6)
		{
			EnterCriticalSection(&CS);
			in_cs = true;
		}
		if(tmp2 >= 0.6 && in_cs)
			LeaveCriticalSection(&CS);
		Sleep(2);
	} while (tmp2 < 1);
	
	return 0;
}
DWORD WINAPI Car_up(void* hWnd)
{
	//car-up
	
	
	tmp3 = 0.01;	tmp4 = 0.1;
	bool in_cs = false;
	do 
	{
		back_Rect2 = tmpRect2;
		SetRect(&tmpRect2, rect.right*0.47, rect.bottom*tmp3, rect.right*0.53, rect.bottom*tmp4);
		InvalidateRect((HWND)hWnd, &back_Rect2, TRUE);
		InvalidateRect((HWND)hWnd, &tmpRect2, TRUE);

		tmp3+=0.001;
		tmp4+=0.001;
		if(tmp4 >= 0.4 && tmp4 <0.6)
		{
			EnterCriticalSection(&CS);
			in_cs = true;
		}
		if(tmp4 >= 0.6 && in_cs)
		{
			LeaveCriticalSection(&CS);
		}

		Sleep(2);
	} while (tmp4 <1);
	
	
	return 0;
}
LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_CREATE:
		hBrush_black = (HBRUSH)GetStockObject(BLACK_BRUSH);
		hBrush_white = (HBRUSH)GetStockObject(WHITE_BRUSH);
	    hPen1 = (HPEN)GetStockObject(BLACK_PEN);
		hPen2 = (HPEN)GetStockObject(WHITE_PEN);
		return 0;
	case WM_SIZE:
		cxClient = LOWORD (lParam) ;
		cyClient = HIWORD (lParam) ;
		return 0 ;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		SelectObject(hdc, hPen1);
		GetClientRect (hwnd, &rect) ;
		MoveToEx(hdc, rect.right * 0.40, rect.top, NULL); // |
		LineTo(hdc, rect.right * 0.40 , rect.bottom*0.40);

		MoveToEx(hdc, rect.right * 0.60, rect.top, NULL);//    |
		LineTo(hdc, rect.right * 0.60 , rect.bottom*0.40);

		MoveToEx(hdc, rect.right * 0.40 , rect.bottom*0.40, NULL); // --- 
		LineTo(hdc, rect.left, rect.bottom*0.40);

		MoveToEx(hdc, rect.right * 0.60 , rect.bottom*0.40, NULL); // ---
		LineTo(hdc, rect.right, rect.bottom*0.40);

		MoveToEx(hdc, rect.right * 0.40 , rect.bottom*0.60, NULL); // ---
		LineTo(hdc, rect.left, rect.bottom*0.60);

		MoveToEx(hdc, rect.right * 0.60 , rect.bottom*0.60, NULL); // ---
		LineTo(hdc, rect.right, rect.bottom*0.60);

		MoveToEx(hdc, rect.right * 0.40 , rect.bottom*0.60, NULL); // |
		LineTo(hdc, rect.right*0.40, rect.bottom);

		MoveToEx(hdc, rect.right * 0.60 , rect.bottom*0.60, NULL);//    |
		LineTo(hdc, rect.right*0.60, rect.bottom);

		//Car-up
		SelectObject(hdc, hPen2);
		SelectObject(hdc, hBrush_white);
		Rectangle(hdc, back_Rect2.left, back_Rect2.top, back_Rect2.right, back_Rect2.bottom);
		SelectObject(hdc, hPen1);
		SelectObject(hdc, hBrush_black);
		Rectangle(hdc, tmpRect2.left, tmpRect2.top, tmpRect2.right, tmpRect2.bottom);

		//Car-left
		SelectObject(hdc, hPen2);
		SelectObject(hdc, hBrush_white);
		Rectangle(hdc, back_Rect.left, back_Rect.top, back_Rect.right, back_Rect.bottom);
		SelectObject(hdc, hPen1);
		SelectObject(hdc, hBrush_black);
		Rectangle(hdc, tmpRect.left, tmpRect.top, tmpRect.right, tmpRect.bottom);

		EndPaint(hwnd, &ps);
		return 0;
	case WM_KEYDOWN:
		if(wParam == VK_SPACE)
		{
			InitializeCriticalSection(&CS);
			CreateThread(NULL, 0, Car_left, hwnd, 0, &first_thread);
			CreateThread(NULL, 0, Car_up, hwnd, 0, &second_thread);
			
		}
		return 0;
	case WM_DESTROY:
		DeleteCriticalSection(&CS);
		PostQuitMessage (0) ;
		return 0 ;
	}
	return DefWindowProc (hwnd, message, wParam, lParam) ;
}

