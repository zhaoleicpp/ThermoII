#include "stdafx.h"
#include "RoundSource.h"
#include "UserInterface.h"
#include "Ray.h"
#define _USE_MATH_DEFINES
#include <math.h>

const double CRoundSource::m_radius_coefficient=1.0;

CRoundSource::CRoundSource(void)
{
}


CRoundSource::~CRoundSource(void)
{
}

void CRoundSource::Get(class CRay &ray,int index)
{
	int point_index;
	int ray_index;
	point_index=index/m_ray_count_per_point;
	ray_index=index%m_ray_count_per_point;
	double normal;
	normal=M_PI*2*point_index/m_point_count;
	ray.m_x0=m_x+m_radius*cos(normal);
	ray.m_y0=m_y+m_radius*sin(normal);
	double shift;
	if(m_ray_count_per_point==1)
		shift=0;
	else
		shift=(M_PI/2)*(ray_index-(m_ray_count_per_point-1)/2.0)/((m_ray_count_per_point-1)/2.0);
	double angle;
	angle=normal+shift;
	ray.m_kx=cos(angle);
	ray.m_ky=sin(angle);
	ray.m_brightness=m_send_rate*cos(shift);//configure
}

int CRoundSource::GetCount()
{
	return m_point_count*m_ray_count_per_point;
}

void CRoundSource::Paint()
{
	for(int loop=0;loop<m_point_count;loop++)
	{
		double c0,c1;
		c0=M_PI*2*(loop+0.5)/m_point_count;
		c1=M_PI*2*(loop-0.5)/m_point_count;
		double x0,y0,x1,y1;
		x0=m_radius*cos(c0);
		y0=m_radius*sin(c0);
		x1=m_radius*cos(c1);
		y1=m_radius*sin(c1);

		x0+=m_x;
		y0+=m_y;
		x1+=m_x;
		y1+=m_y;

		g_user_interface->Line(x0,y0,x1,y1,100,200,255);
	}
}
