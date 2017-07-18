#include "StdAfx.h"
#include "UserInterface.h"
#define _USE_MATH_DEFINES
#include <math.h>


CUserInterface *g_user_interface=NULL;

CUserInterface::CUserInterface(CWnd &window):CClientDC(&window)
{
	m_left_x=0;
	m_right_x=700;
	m_up_y=50;
	m_down_y=-50;

	m_width=1024;
	m_height=768;
	UpdateSize();
}


CUserInterface::~CUserInterface(void)
{
}
CUserInterface * CUserInterface::Create(class CWnd &window)
{
	return new CUserInterface(window);
}
void CUserInterface::UpdateSize()
{
	double pixel_from_mm_x,pixel_from_mm_y;
	pixel_from_mm_x=m_width/(-m_left_x+m_right_x);
	pixel_from_mm_y=m_height/(-m_down_y+m_up_y);
	m_pixel_from_mm=min(pixel_from_mm_x,pixel_from_mm_y);
}
void CUserInterface::SetSize(int width,int height)
{
	if(width<=4)
		width=4;
	width=((width+3)/4)*4;
	m_width=width;
	if(height<=4)
		height=4;
	height=((height+3)/4)*4;
	m_height=height;
	UpdateSize();
	Update();
}
void CUserInterface::SetPixel(int x,int y,unsigned char red,unsigned char green,unsigned char blue,bool do_anyway)
{
	int index;
	index=(m_height-1-y)*m_width+x;
	index*=3;
	unsigned char &old_red=m_pixel[index+2];
	unsigned char &old_green=m_pixel[index+1];
	unsigned char &old_blue=m_pixel[index+0];

	bool do_;
	do_=false;
	if(do_anyway)
		do_=true;
	else
	{
		if(red+green+blue>old_red+old_green+old_blue)
			do_=true;
	}
	if(do_)
	{
		old_red=red;
		old_green=green;
		old_blue=blue;
	}
}
void CUserInterface::PreparePaint()
{
	for(int y=0;y<m_height;y++)
		for(int x=0;x<m_width;x++)
		{
			double x_mm,y_mm;
			MmFromPixel(x_mm,y_mm,x,y);
			if(m_left_x<=x_mm&&x_mm<=m_right_x&&m_down_y<=y_mm&&y_mm<=m_up_y)
				SetPixel(x,y,0,0,0,true);
			else
				SetPixel(x,y,100,100,100,true);
		}
}
void CUserInterface::FinishPaint(CDC &dc)
{
	CDC * dest;
	dest=&dc;
	if(dest==NULL)
		dest=this;
	BITMAPINFO info;
	info.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	info.bmiHeader.biWidth=m_width;
	info.bmiHeader.biHeight=m_height;
	info.bmiHeader.biPlanes=1;
	info.bmiHeader.biBitCount=24;
	info.bmiHeader.biCompression=0;
	info.bmiHeader.biSizeImage=m_width*m_height*3;
	info.bmiHeader.biXPelsPerMeter=40000;
	info.bmiHeader.biYPelsPerMeter=30000;
	info.bmiHeader.biClrUsed=100;
	info.bmiHeader.biClrImportant=100;
	::SetDIBitsToDevice(dest->m_hDC,0,0,m_width,m_height,0,0,0,m_height,m_pixel,&info,DIB_RGB_COLORS);
	CRect rect;
	rect.left=rect.top=0;
	rect.right=m_width;
	rect.bottom=m_height;
	dest->DrawText(m_text,&rect,0);
}
void CUserInterface::Update(bool skip_screen)
{
	PreparePaint();
	//for(int index=0;index<g_source_count;index++)
	//	g_source[index].Paint();
	//for(int index=0;index<g_glass_count;index++)
	//	g_glass[index].Paint();
	//for(int index=0;index<g_screen_count;index++)
	//	g_screen[index].Paint();
	//for(int index=0;index<g_frosted_glass_count;index++)
	//	g_frosted_glass[index].Paint();
	//for(int index=0;index<g_white_board_count;index++)
	//	g_white_board[index].Paint();
	//g_main.Paint(skip_screen);
	FinishPaint();
}
void CUserInterface::OnPaint(CDC &dc)
{
	FinishPaint(dc);
}
void CUserInterface::MmFromPixel(double &x_mm,double &y_mm,int x_pixel,int y_pixel)
{
	if(&x_mm)
		x_mm=x_pixel/m_pixel_from_mm+m_left_x;
	if(&y_mm)
		y_mm=y_pixel/m_pixel_from_mm+m_down_y;
}
void CUserInterface::SetPixel(double x,double y,unsigned char red,unsigned char green,unsigned char blue)
{
	if(!(m_left_x<=x&&x<=m_right_x&&m_down_y<=y&&y<=m_up_y))
		return ;
	int int_x;
	int_x=(int)((x-m_left_x)*m_pixel_from_mm+0.5);
	if(int_x<0||int_x>=m_width)
		return ;
	int int_y;
	int_y=(int)((-y-m_down_y)*m_pixel_from_mm+0.5);
	if(int_y<0||int_y>=m_height)
		return ;
	SetPixel(int_x,int_y,red,green,blue);
}
double CUserInterface::GetPixelPerMm()
{
	return m_pixel_from_mm;
}
void CUserInterface::Line(double x0,double y0,double x1,double y1,unsigned char red,unsigned char green,unsigned char blue)
{
	double d;
	d=sqrt(pow(x0-x1,2)+pow(y0-y1,2));
	int loop_count;
	loop_count=(int)(d*m_pixel_from_mm*2+0.5);
	for(int loop=0;loop<loop_count;loop++)
	{
		double x,y;
		x=x0+(-x0+x1)*loop/(loop_count-1);
		y=y0+(-y0+y1)*loop/(loop_count-1);
		SetPixel(x,y,red,green,blue);
	}
}