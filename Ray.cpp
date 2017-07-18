#include "StdAfx.h"
#include "Ray.h"
#include "UserInterface.h"

#define _USE_MATH_DEFINES
#include <math.h>


CRay::CRay(void)
{
}


CRay::~CRay(void)
{
}

void CRay::Paint(int color)
{
	int gray;
	gray=(int)(255*m_brightness);
	//if(gray<0)
	//	gray=0;
	if(gray<100)
		gray=100;
	if(gray>255)
		gray=255;
	unsigned char red,green,blue;
	red=(unsigned char)gray;
	green=(unsigned char)gray;
	blue=(unsigned char)gray;
	/*
	switch(color)
	{
	case 0:
		red=(unsigned char)gray;
		green=0;
		blue=0;
		break;
	case 1:
		red=0;
		green=(unsigned char)gray;
		blue=0;
		break;
	default:
		red=(unsigned char)gray/3;
		green=(unsigned char)gray/3;
		blue=(unsigned char)gray/3;
		break;
	case -1:
		red=0;
		green=0;
		blue=(unsigned char)gray;
		break;
	}
	*/
	double x1,y1;
	x1=m_x0+m_kx*m_length;
	y1=m_y0+m_ky*m_length;
	g_user_interface->Line(m_x0,m_y0,x1,y1,red,green,blue);
}
