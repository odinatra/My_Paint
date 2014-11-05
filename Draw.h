#define _USE_MATH_DEFINES //For using pi
#include <windows.h> 
#include <math.h> //For ellipse and spiral

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

