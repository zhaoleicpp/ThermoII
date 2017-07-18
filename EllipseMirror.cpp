#include "stdafx.h"
#include "EllipseMirror.h"
#include "UserInterface.h"
#include "Ray.h"
#include "Tool.h"
#define _USE_MATH_DEFINES
#include <math.h>


CEllipseMirror::CEllipseMirror(void)
{
}


CEllipseMirror::~CEllipseMirror(void)
{
}
void CEllipseMirror::Set(double a,double b)
{
	m_a=a;
	m_b=b;
	m_c=sqrt(abs(pow(a,2)-pow(b,2)));
}
void CEllipseMirror::Paint()
{
	static const int loop_count=100;
	for(int loop=-loop_count;loop<=loop_count-1;loop++)
	{
		double x0,y0;
		x0=loop;
		x0/=loop_count;
		y0=sqrt(1-pow(x0,2));
		x0*=m_a;
		y0*=m_b;
		double x1,y1;
		x1=loop+1;
		x1/=loop_count;
		y1=sqrt(1-pow(x1,2));
		x1*=m_a;
		y1*=m_b;
		g_user_interface->Line(m_x+x0,m_y+y0,m_x+x1,m_y+y1,200,0,255);
		g_user_interface->Line(m_x+x0,m_y-y0,m_x+x1,m_y-y1,200,0,255);
	}
}
bool CEllipseMirror::Go(CRay &out,CRay &in)
{
	out.m_brightness=in.m_brightness;
	out.m_length=0;

	double rx,ry;
	rx=in.m_x0-m_x;
	ry=in.m_y0-m_y;
#define a m_a
#define b m_b
#define kx in.m_kx
#define ky in.m_ky
#define Power pow
#define Sqrt sqrt
	if(pow(rx,2)/pow(m_a,2)+pow(ry,2)/pow(m_b,2)>=1+1e-10)
		return false;
	double x1,y1,x2,y2;
	{
		if(abs(kx)<1e-10)
		{
			x1=x2=rx;
			y1=sqrt(1-pow(x1/a,2))*b;
			y2=-y1;
		}
		else
		{
			double A,B,C,D;
			A=Power(b,2)*Power(kx,2) + Power(a,2)*Power(ky,2);
			B=Power(ky*rx - kx*ry,2);
			C=a*Power(ky,2)*rx - a*kx*ky*ry;
			D=-(b*Power(kx,2)*ky*rx) + b*Power(kx,3)*ry;
			x1=(a*(C - b*Sqrt((A - B)*Power(kx,2))))/A;
			y1=(b*(D - a*Sqrt((A - B)*Power(kx,2))*ky))/(A*kx);
			x2=(a*(C + b*Sqrt((A - B)*Power(kx,2))))/A;
			y2=(b*(D + a*Sqrt((A - B)*Power(kx,2))*ky))/(A*kx);
		}
	}
	double t1,t2;
	if(abs(kx)>abs(ky))
	{
		t1=(-rx+x1)/kx;
		t2=(-rx+x2)/kx;
	}
	else
	{
		t1=(-ry+y1)/ky;
		t2=(-ry+y2)/ky;
	}
	double x,y,t;
	if(t1>t2)
	{
		//both t1>0 t2<0
		//and t1>0 t2¡Ö0
		x=x1;
		y=y1;
		t=t1;
	}
	else
	{
		x=x2;
		y=y2;
		t=t2;
	}
#undef a
#undef b
#undef kx
#undef ky
#undef Power
#undef Sqrt
	{
		double kx,ky;
		//https://www.zybang.com/question/62bb21673df4d1166e88c3bfc6181172.html
		kx=pow(m_b,2)*x;
		ky=pow(m_a,2)*y;
		double r;
		r=sqrt(pow(kx,2)+pow(ky,2));
		kx/=r;
		ky/=r;
		double normal;
		normal=GetAngle(kx,ky)+M_PI;
		double in_gangle;
		in_gangle=GetAngle(in.m_kx,in.m_ky)+M_PI;
		double out_angle;
		out_angle=normal+(normal-in_gangle);
		out.m_kx=cos(out_angle);
		out.m_ky=sin(out_angle);
	}
	out.m_x0=x;
	out.m_y0=y;
	out.m_x0+=m_x;
	out.m_y0+=m_y;
	in.m_length=sqrt(pow(out.m_x0-in.m_x0,2)+pow(out.m_y0-in.m_y0,2));
	return true;
}
