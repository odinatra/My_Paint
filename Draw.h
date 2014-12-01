#define _USE_MATH_DEFINES //For using pi
#include <windows.h> 
#include <math.h> //For ellipse and spiral

class Elements{
public:
int lastx, lasty, x, y;		//Variables used in drawing.
int n = 0;					//Switch used for determining what would be drawn (by default - line)
//Elements of window
HWND hButLine, hButRect, hButEllipse, hEditRed, hButSetPen, hEditGreen, hEditBlue, hEditThick, hButSpiral, hButPencil;
HPEN p; //Pen used for drawing
void init_elem(HWND hwnd)
{
	hButLine = ButInit(0, TEXT("Line"), hwnd, 101);
	hButRect = ButInit(24, TEXT("Rectangle"), hwnd, 102);
	hButEllipse = ButInit(48, TEXT("Ellipse"), hwnd, 103);
	hButSpiral = ButInit(72, TEXT("Spiral"), hwnd, 104);
	hButPencil = ButInit(96, TEXT("Pencil"), hwnd, 105);

	hEditRed = EditInit(120, hwnd, 201, (LPARAM)TEXT("0"));
	hEditGreen = EditInit(144, hwnd, 202, (LPARAM)TEXT("0"));
	hEditBlue = EditInit(168, hwnd, 203, (LPARAM)TEXT("0"));
	hEditThick = EditInit(192, hwnd, 204, (LPARAM)TEXT("1"));

	hButSetPen = ButInit(216, TEXT("Set pen"), hwnd, 106);
}
void init_pen()
{

	//We are getting inputted value, then if it more than max. allowed value we reduce it to 255.
	//Then we convert it to int. Using this data we give new parameters to pen ;
	p = CreatePen(PS_SOLID,
		check_return(hEditThick),
		RGB(check_return(hEditRed),
		check_return(hEditGreen),
		check_return(hEditBlue)));
}
private:
	int check_return(HWND ExtShow)
	{
		wchar_t buffer[4];

		SendMessage(ExtShow,
			WM_GETTEXT,
			4,//Getting 3 digits+endline symbol
			reinterpret_cast<LPARAM>(buffer));

		int variable = _wtoi(buffer);// converting from symbol to int
		//Checking if inputted value is in allowed limits
		if (variable > 255)
		{
			variable = 255;
			SendMessage(ExtShow, WM_SETTEXT, NULL, (LPARAM)TEXT("255")); //Changing value to max. allowed
		}
		return variable;
	}
	HWND ButInit(int button_y, LPCWSTR txt, HWND hwndr, int iden)
	{
		return CreateWindowEx(NULL,
			TEXT("BUTTON"), //Instructon to initalize button
			txt, //Text on a button
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,//Styles to prevent tabbing and using the default push button 
			0,
			button_y,//X and Y coordinates in window. In this case buttonі located in right side, in line, so x never changes
			100,
			24,//Size of button in pixels
			hwndr,//handle of parent window button 
			(HMENU)iden,//identifier
			GetModuleHandle(NULL),
			NULL);
	}

	HWND EditInit(int button_y, HWND hwndr, int iden, LPARAM txt)
	{
		HWND temp;
		temp = CreateWindowEx(WS_EX_CLIENTEDGE,
			TEXT("EDIT"),//Creating standart edit control element
			TEXT(""),
			WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_NUMBER, //Accepts numbers only
			0,
			button_y,
			100,
			24,
			hwndr,
			(HMENU)iden,
			GetModuleHandle(NULL),
			NULL);
		//Limiting maximum number of characters in edit controls to 3
		SendMessage(temp,
			EM_LIMITTEXT,
			3,
			NULL);
		//Setting display of default values at the start
		SendMessage(temp,
			WM_SETTEXT,
			NULL,
			txt);
		return temp;
	}
};

//Functions for drawing primitives contain 5 variables - HDC, coordinates when left mouse button is pressed,
//and coordinates when mouse button is released. It is explained in detail in WndProc comments
class Figures{
	public:
void line(HDC _hdc, int x1, int y1, int x2, int y2)//This function draws line by the given four coordinates.
{

	MoveToEx(_hdc, x1, y1, NULL);
	LineTo(_hdc, x2, y2);
}
void rectangle(HDC _hdc, int x1, int y1, int x2, int y2)//This function draws rectangle by the given four coordinates.
{
	MoveToEx(_hdc, x1, y1, NULL);
	LineTo(_hdc, x1, y2);
	LineTo(_hdc, x2, y2);
	LineTo(_hdc, x2, y1);
	LineTo(_hdc, x1, y1);
}

void ellipse(HDC _hdc, int x1, int y1, int x2, int y2) //Function draws ellipse using four coordinates. Not the most effective, but it works
{
	//coordinates of center
	int xCenter = (x1 + x2) / 2;
	int yCenter = (y1 + y2) / 2;
	//radiuses for x and y
	int Rx = (x1 - x2) / 2;
	int Ry = (y1 - y2) / 2;
	MoveToEx(_hdc, xCenter + Rx, yCenter, NULL); // Setting starting drawing point for 0 degree angle

	for (int i = 1; i <= 360; i++)
	{
		//Lines drawn for 360 degrees using standart ellipse formula.
		//Since cos and sin take arguments in radians, they are converted
		LineTo(_hdc, xCenter + int(Rx*cos(i*(M_PI / 180))), int(yCenter + Ry*sin(i*(M_PI / 180))));
	}
}

void spiral(HDC _hdc, int x1, int y1, int x2, int y2)//Function draws spiral using four coordinates
{
	int xCenter = (x1 + x2) / 2;
	int yCenter = (y1 + y2) / 2; //coordinates of center
	double d;//detail of spiral
	//radiuses for x and y
	int Rx = (x2 - x1) / (2 * 15);
	int Ry = (y2 - y1) / (2 * 15);
	MoveToEx(_hdc, xCenter, yCenter, NULL); //Setting starting drawing point in center
	for (d = 0; d <= 8 * M_PI; d += 0.005)
		//Formula is similiar to ellipse, but radius is changed with each step
		LineTo(_hdc, xCenter + (sin(d)*d)*Rx, yCenter + (sin(d + (M_PI / 2))*(d + (M_PI / 2))*Ry));
}
};
