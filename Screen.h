#pragma once
class CScreen
{
public:
	double m_x,m_y;
public:
	virtual void Paint()=NULL;
	virtual bool Go(class CRay &ray)=NULL;
	CScreen(void);
	~CScreen(void);
};

