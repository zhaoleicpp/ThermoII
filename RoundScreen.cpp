#include "stdafx.h"
#include "RoundScreen.h"
#include "RoundSource.h"
#include "Ray.h"
#include "Tool.h"
#include "UserInterface.h"
#define _USE_MATH_DEFINES
#include <math.h>

CRoundScreen::CRoundScreen(void)
{
	m_receive=NULL;
}


CRoundScreen::~CRoundScreen(void)
{
	Delete();
}
void CRoundScreen::Delete()
{
	if(m_receive)
	{
		delete []m_receive;
		m_receive=NULL;
	}
}
void CRoundScreen::Set(CRoundSource &round_source)
{
	Delete();
	m_round_source=&round_source;
	int count;
	count=m_round_source->m_size*CRoundSource::m_point_count_coefficient;
	m_receive=new SReceive [count];
	memset(m_receive,0,sizeof(SReceive)*count);
}
void CRoundScreen::Receive(CRay &ray,bool self)
{
	int count;
	count=m_round_source->m_size*CRoundSource::m_point_count_coefficient;
	double x,y;
	x=ray.m_x0+ray.m_kx*ray.m_length;
	y=ray.m_y0+ray.m_ky*ray.m_length;
	x-=m_round_source->m_x;
	y-=m_round_source->m_y;
	double angle;
	angle=GetAngle(x,y);
	int index;
	index=(int)(angle/(M_PI*2/count)+0.5);
	if(index<0)
		index+=count;
	if(index>=count)
		index-=count;
	if(self)
		m_receive[index].self+=ray.m_brightness;
	else
		m_receive[index].other+=ray.m_brightness;
}
void CRoundScreen::Paint()
{
	int count;
	count=m_round_source->m_size*CRoundSource::m_point_count_coefficient;
	double self_min,self_max;
	self_min=1e100;
	self_max=-1e100;
	double other_min,other_max;
	other_min=1e100;
	other_max=-1e100;
	double sum_min,sum_max;
	sum_min=1e100;
	sum_max=-1e100;
	for(int index=0;index<count;index++)
	{
		double self,other,sum;
		self=m_receive[index].self;
		other=m_receive[index].other;
		sum=self+other;
		if(self<self_min)
			self_min=self;
		if(self>self_max)
			self_max=self;
		if(other<other_min)
			other_min=other;
		if(other>other_max)
			other_max=other;
		if(sum<sum_min)
			sum_min=sum;
		if(sum>sum_max)
			sum_max=sum;
	}
	double radius;
	radius=m_round_source->m_size*CRoundSource::m_radius_coefficient;
	for(int loop=0;loop<count;loop++)
	{
		double c0,c1;
		c0=M_PI*2*(loop+0.5)/count;
		c1=M_PI*2*(loop-0.5)/count;
		double x0,y0,x1,y1;
		x0=radius*cos(c0);
		y0=radius*sin(c0);
		x1=radius*cos(c1);
		y1=radius*sin(c1);

		x0+=m_round_source->m_x;
		y0+=m_round_source->m_y;
		x1+=m_round_source->m_x;
		y1+=m_round_source->m_y;

		double self,other,sum;
		self=m_receive[loop].self;
		other=m_receive[loop].other;
		sum=self+other;
		unsigned char red,green,blue;

		//if(self_max-self_min<1e-50)
		//	green=128;
		//else
		//	green=(unsigned char)(255*(self-self_min)/(self_max-self_min));

		//if(other_max-other_min<1e-50)
		//	green=128;
		//else
		//	green=(unsigned char)(255*(other-other_min)/(other_max-other_min));

		if(sum_max-sum_min<1e-50)
			red=128;
		else
			red=(unsigned char)(255*(sum-sum_min)/(sum_max-sum_min));
		if(red<50)
			red=50;
		green=5;
		blue=50;
		g_user_interface->Line(x0,y0,x1,y1,red,green,blue);
	}
}
bool CRoundScreen::Go(class CRay &ray)
{
	double x0,y0,r;
	x0=m_round_source->m_x;
	y0=m_round_source->m_y;
	r=m_round_source->m_size*m_round_source->m_radius_coefficient;
	double rx,ry;
	rx=ray.m_x0-x0;
	ry=ray.m_y0-y0;
	if(pow(rx,2)+pow(ry,2)<=pow(r,2))
		ASSERT(0);
	double A,B,C;
#define kx ray.m_kx
#define ky ray.m_ky
#define Power pow
#define Sqrt sqrt
	A=Power(kx,2) + Power(ky,2);
	B=Power(kx,2)*(Power(ky,2)*(r - rx)*(r + rx) + 2*kx*ky*rx*ry + Power(kx,2)*(r - ry)*(r + ry));
	C=-(Power(ky,2)*rx) + kx*ky*ry;
	if(B<=0)
		return false;
	double x1,y1,x2,y2;
	if(abs(kx)<1e-10)
	{
		x1=x2=rx;
		y1=sqrt(pow(r,2)-pow(x1,2));
		y2=-y1;
	}
	else
	{
		x1=-((Sqrt(B) + C)/A);
		y1=-((ky*(Sqrt(B) + C + A*rx))/(A*kx)) + ry;
		x2=(Sqrt(B) - C)/A;
		y2=-((ky*(-Sqrt(B) + C + A*rx))/(A*kx)) + ry;
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
	if(t1<t2)
	{
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

#undef kx
#undef ky
#undef Power
#undef Sqrt

	x+=m_round_source->m_x;
	y+=m_round_source->m_y;
	ray.m_length=sqrt(pow(x-ray.m_x0,2)+pow(y-ray.m_y0,2));
	return true;
}
