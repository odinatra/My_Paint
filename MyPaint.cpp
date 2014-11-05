//**************************************
// Name: ^A Paint Program in Visual C++^
// Description:This shows how to make a simple free-hand drawing program in Visual C++. The code is commented.
// By: Dzyuba Mykhailo
// Using code by Niloy Mondal
//This code is copyrighted and has// limited warranties.Please see http://www.Planet-Source-Code.com/vb/scripts/ShowCode.asp?txtCodeId=4600&lngWId=3//for details.//**************************************

#define _USE_MATH_DEFINES //For using pi
#include <windows.h> 
#include <math.h> //For ellipse and spiral
#include "draw.h" //For ellipse and spiral

int lastx, lasty, x, y;		//Variables used in drawing.
int n;//Switch used for determining what would be drawn
int r = 0, g = 0, b = 0; //Color values
int t = 1; //Thickness of pen
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//Elements of window
HWND hButLine, hButRect, hButEllipse, hEditRed, hButLine4, hEditGreen, hEditBlue, hEditThick, hButSpiral, hButPencil; 
HPEN p; //Pen used for drawing

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

//Functions for drawing primitives contain 5 variables - HDC, coordinates when left mouse button is pressed,
//and coordinates when mouse button is released. It is explained in detail in WndProc comments


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	hdc = GetDC(hwnd);							//hdc is handle to device context
	//Initializing pen for drawing. It is reinitialized every cycle
	p = CreatePen(PS_SOLID, t, RGB(r, g, b));	
	SelectObject(hdc, p);
	
	switch (message)
	{
	case WM_CREATE:
	{
					  //Initilizing elements of main window
					  //All buttons and edits are using same set of parameters, so only one would be commented
					  hButLine = CreateWindowEx(NULL,
						  TEXT("BUTTON"), //Instructon to initalize button
						  TEXT("Line"), //Text on a button
						  WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,//Styles to prevent tabbing and using the default push button 
						  0,
						  0,//X and Y coordinates in window. In this case button located in upeer right corner
						  100,
						  24,//Size of button in pixels
						  hwnd,//handle of parent window button 
						  (HMENU)101,//identifier
						  GetModuleHandle(NULL),
						  NULL);
					  hButRect = CreateWindowEx(NULL,
						  TEXT("BUTTON"),
						  TEXT("Rectangle"),
						  WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
						  0,
						  24,
						  100,
						  24,
						  hwnd,
						  (HMENU)102,
						  GetModuleHandle(NULL),
						  NULL);
					  hButEllipse = CreateWindowEx(NULL,
						  TEXT("BUTTON"),
						  TEXT("Ellipse"),
						  WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
						  0,
						  48,
						  100,
						  24,
						  hwnd,
						  (HMENU)103,
						  GetModuleHandle(NULL),
						  NULL);
					  hButSpiral = CreateWindowEx(NULL,
						  TEXT("BUTTON"),
						  TEXT("Spiral"),
						  WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
						  0,
						  72,
						  100,
						  24,
						  hwnd,
						  (HMENU)104,
						  GetModuleHandle(NULL),
						  NULL);
					  hButPencil = CreateWindowEx(NULL,
						  TEXT("BUTTON"),
						  TEXT("Pencil"),
						  WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
						  0,
						  96,
						  100,
						  24,
						  hwnd,
						  (HMENU)105,
						  GetModuleHandle(NULL),
						  NULL);
					  hEditRed = CreateWindowEx(WS_EX_CLIENTEDGE,
						  TEXT("EDIT"),
						  TEXT(""),
						  WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_NUMBER,
						  0,
						  120,
						  100,
						  24,
						  hwnd,
						  (HMENU)201,
						  GetModuleHandle(NULL),
						  NULL);
					  hEditGreen = CreateWindowEx(WS_EX_CLIENTEDGE,
						  TEXT("EDIT"),//Creatin standart edit control element
						  TEXT(""),
						  WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_NUMBER, //Accepts only numbers
						  0,
						  144,
						  100,
						  24,
						  hwnd,
						  (HMENU)202,
						  GetModuleHandle(NULL),
						  NULL);
					  hEditBlue = CreateWindowEx(WS_EX_CLIENTEDGE,
						  TEXT("EDIT"),
						  TEXT(""),
						  WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_NUMBER,
						  0,
						  168,
						  100,
						  24,
						  hwnd,
						  (HMENU)203,
						  GetModuleHandle(NULL),
						  NULL);
					  hEditThick = CreateWindowEx(WS_EX_CLIENTEDGE,
						  TEXT("EDIT"),
						  TEXT(""),
						  WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_NUMBER,
						  0,
						  192,
						  100,
						  24,
						  hwnd,
						  (HMENU)204,
						  GetModuleHandle(NULL),
						  NULL);
					  hButLine4 = CreateWindowEx(NULL,
						  TEXT("BUTTON"),
						  TEXT("Set pen"),
						  WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
						  0,
						  216,
						  100,
						  24,
						  hwnd,
						  (HMENU)106,
						  GetModuleHandle(NULL),
						  NULL);

					  //Limiting maximum number of characters in edit controls to 3
					  SendMessage(hEditRed,
						  EM_LIMITTEXT,
						  3,
						  NULL);
					  SendMessage(hEditGreen,
						  EM_LIMITTEXT,
						  3,
						  NULL);
					  SendMessage(hEditBlue,
						  EM_LIMITTEXT,
						  3,
						  NULL);
					  SendMessage(hEditThick,
						  EM_LIMITTEXT,
						  3,
						  NULL);
					  
					  //Setting display of default values at the start
					  SendMessage(hEditRed,
						  WM_SETTEXT,
						  NULL,
					      (LPARAM)TEXT("0"));
					  SendMessage(hEditGreen,
						  WM_SETTEXT,
						  NULL,
						  (LPARAM)TEXT("0"));
					  SendMessage(hEditBlue,
						  WM_SETTEXT,
						  NULL,
						  (LPARAM)TEXT("0"));
					  SendMessage(hEditThick,
						  WM_SETTEXT,
						  NULL,
						  (LPARAM)TEXT("1"));
	}return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		//Switch primitive depending on button pressed
		case 101:
		{n = 1;
		} return 0;
		case 102:
		{
		n = 2;
		} return 0;
		case 103:
		{n = 3;
		}
		return 0;
		case 104:
		{n = 4;
		} return 0;
		case 105:
		{n = 5;
		} return 0;
		//Changing pen values to inputed in edit controls. If e
		case 106:
		{
			wchar_t buffer_red[4], buffer_green[4], buffer_blue[4], buffer_thick[4]; //Buffers for input
			SendMessage(hEditRed,
				WM_GETTEXT,
				4,//Getting 3 digits+endline symbol
				reinterpret_cast<LPARAM>(buffer_red));
			SendMessage(hEditGreen,
				WM_GETTEXT,
				4,
				reinterpret_cast<LPARAM>(buffer_green));
			SendMessage(hEditBlue,
				WM_GETTEXT,
				4,
				reinterpret_cast<LPARAM>(buffer_blue));
			SendMessage(hEditThick,
				WM_GETTEXT,
				4,
				reinterpret_cast<LPARAM>(buffer_thick));
			
			//Checking if inputted value is in allowed limits
			if (_wtoi(buffer_red) > 255)
			{
				r = 255;
				SendMessage(hEditRed, WM_SETTEXT, NULL, (LPARAM) TEXT("255")); //Changing value to allowed
			}
			else r = _wtoi(buffer_red); 
			if (_wtoi(buffer_green)>255) 
			{
				g = 255;
				SendMessage(hEditGreen, WM_SETTEXT, NULL, (LPARAM)TEXT("255"));
			}
			else g = _wtoi(buffer_green);
			if (_wtoi(buffer_blue)>255)
			{
				b = 255;
				SendMessage(hEditBlue, WM_SETTEXT, NULL, (LPARAM)TEXT("255"));
			}
			else b = _wtoi(buffer_blue);
			if (_wtoi(buffer_thick)>255) 
			{
				t = 255;
				SendMessage(hEditThick, WM_SETTEXT, NULL, (LPARAM)TEXT("255"));
			}
			else t = _wtoi(buffer_thick);
		}
		return 0;

		}
		return 0;
	case WM_LBUTTONDOWN:					//If Left mouse button is pressed
		lastx = LOWORD(lParam);			//Store the x-coordiante in lastx
		lasty = HIWORD(lParam);			//Store the y-coordinate in lasty
		return 0;
	case WM_LBUTTONUP:					//If Left mouse button is released
		x = LOWORD(lParam);					//Store the current x 
		y = HIWORD(lParam);					//Store the current y
		//Depending on n following action will take place
		if (n == 1)  line(hdc, lastx, lasty, x, y);		//Drawing the line from the last pair of coordiates to current
		if (n == 2)  rectangle(hdc, lastx, lasty, x, y); //Drawing the rectangle with diagonal from last pair of coordiates to current
		if (n == 3)  ellipse(hdc, lastx, lasty, x, y); //Drawing the ellipse with center in the middle of two sets of coordinates
		if (n == 4)  spiral(hdc, lastx, lasty, x, y);//Drawing the spiral with center in the middle of two sets of coordinates
		return 0;
		//If we want to draw with pencil, then we have to track mose movement
	case WM_MOUSEMOVE:						//If mouse is moving on the client area 
		if (wParam & MK_LBUTTON & (n == 5))			//If Left mouse button is down and 'Pencil' button was presed then draw
		{
		x = LOWORD(lParam);					//Store the current x
		y = HIWORD(lParam);					//Store the current y
		line(hdc, lastx, lasty, x, y);		//Draw the line frome the last pair of coordiates to current
		lastx = x;						//The current x becomes the lastx for next line to be drawn
		lasty = y;						//The current y becomes the lasty for next line to be drawn
		}
		ReleaseDC(hwnd, hdc);
		return 0;
	
	//Startup of paint element
	case WM_PAINT:
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
