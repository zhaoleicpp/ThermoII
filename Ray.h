#pragma once
class CRay
{
public:
	double m_x0,m_y0;
	double m_kx,m_ky;
	double m_length;
	double m_brightness;
public:
	void Paint(int color);
	CRay(void);
	~CRay(void);
};

