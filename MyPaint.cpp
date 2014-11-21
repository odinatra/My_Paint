// Name: ^A Paint Program in Visual C++^
// Description:This shows how to make a simple free-hand drawing program in Visual C++. The code is commented.
// By: Dzyuba Mykhailo

#include <windows.h> 
#include "draw.h" 

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
Image g;
//  Default WinMain for Windows applications
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("Paint");//Winddow Class name
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_CROSS);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;
	if (!RegisterClass(&wndclass))
	{
		int nResult = GetLastError();
		MessageBox(NULL,
			TEXT("Window class creation failed"),
			TEXT("Window Class Failed"),
			MB_ICONERROR);
		return 0;
	}
	g.p = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	hwnd = CreateWindow(szAppName, // window class name
		TEXT("MyPaint C++"), // window caption
		WS_OVERLAPPEDWINDOW,// window style
		CW_USEDEFAULT,// initial x position
		CW_USEDEFAULT,// initial y position
		CW_USEDEFAULT,// initial x size
		CW_USEDEFAULT,// initial y size
		NULL,// parent window handle
		NULL,// window menu handle
		hInstance, // program instance handle
		NULL);// creation parameters

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0))				//The Message Loop
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}



LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	hdc = GetDC(hwnd);							//hdc is handle to device context
	//Initializing pen for drawing. It is reinitialized every cycle
	SelectObject(hdc, g.p);
	switch (message)
	{
	case WM_CREATE:
	{
					  //Initilizing elements of main window
					  //All buttons and edits are using same set of parameters
					  g.init_elem(hwnd);

	}return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		//Switch primitive depending on button pressed
		case 101:
		{g.n = 1;
		} return 0;
		case 102:
		{g.n = 2;
		} return 0;
		case 103:
		{g.n = 3;
		}
		return 0;
		case 104:
		{g.n = 4;
		} return 0;
		case 105:
		{g.n = 5;
		} return 0;
		//Changing pen values to inputed in edit controls. 
		case 106:
		{
			//We are getting inputted value, then if it more than max. allowed value we reduce it to 255.
			// Then we convert it to int. Using this data we give new parameters to pen ;
					g.init_pen();
		}
		return 0;
		}
		return 0;
	case WM_LBUTTONDOWN:					//If Left mouse button is pressed
		g.lastx = LOWORD(lParam);			//Store the x-coordiante in lastx
		g.lasty = HIWORD(lParam);			//Store the y-coordinate in lasty
		return 0;
	case WM_LBUTTONUP:					//If Left mouse button is released
		g.x = LOWORD(lParam);					//Store the current x 
		g.y = HIWORD(lParam);					//Store the current y
		//Depending on n following action will take place
		if (g.n == 1)  line(hdc, g.lastx, g.lasty, g.x, g.y);		//Drawing the line from the last pair of coordiates to current
		if (g.n == 2)  rectangle(hdc, g.lastx, g.lasty, g.x, g.y); //Drawing the rectangle with diagonal from last pair of coordiates to current
		if (g.n == 3)  ellipse(hdc, g.lastx, g.lasty, g.x, g.y); //Drawing the ellipse with center in the middle of two sets of coordinates
		if (g.n == 4)  spiral(hdc, g.lastx, g.lasty, g.x, g.y);//Drawing the spiral with center in the middle of two sets of coordinates
		InvalidateRect(hwnd, NULL, NULL);
		return 0;
		//If we want to draw with pencil, then we have to track mouse movement
	case WM_SIZE:
	{
					GetClientRect(hwnd, &rect);
	}
		break;
	case WM_ERASEBKGND:
		return TRUE;
	case WM_MOUSEMOVE:						//If mouse is moving on the client area 
		if (wParam & MK_LBUTTON & (g.n == 5))			//If Left mouse button is down and 'Pencil' button was presed then draw
		{
		g.x = LOWORD(lParam);					//Store the current x
		g.y = HIWORD(lParam);					//Store the current y
		line(hdc, g.lastx, g.lasty, g.x, g.y);		//Draw the line frome the last pair of coordiates to current
		g.lastx = g.x;						//The current x becomes the lastx for next line to be drawn
		g.lasty = g.y;						//The current y becomes the lasty for next line to be drawn
		}
		ReleaseDC(hwnd, hdc);
		return 0;
	
	//Startup of paint element
	case WM_PAINT:
		InvalidateRect(hwnd, NULL, NULL);
//		UpdateWindow(hwnd);
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);
		//Information about edit controls
		TextOut(hdc, 100, 120, TEXT("Red"), 3);
		TextOut(hdc, 100, 144, TEXT("Green"), 5);
		TextOut(hdc, 100, 168, TEXT("Blue"), 4);
		TextOut(hdc, 100, 192, TEXT("Thickness"), 9);
		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}
