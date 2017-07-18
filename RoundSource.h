#pragma once
#include "Source.h"
class CRoundSource :public CSource
{
public:
	static const double m_radius_coefficient;
	static const int m_point_count_coefficient=10;
	int m_size;
#define m_radius (m_size*m_radius_coefficient)
#define m_point_count (m_size*m_point_count_coefficient)
	static const int m_ray_count_per_point=5;//!!!!!!!!!!! 100;
	double m_send_rate;
public:
	virtual void Get(class CRay &ray,int index);
	virtual int GetCount();
	virtual void Paint();
	CRoundSource(void);
	~CRoundSource(void);
};

