#pragma once
class CMirror
{
public:
	double m_x,m_y;
public:
	virtual void Paint()=NULL;
	virtual bool Go(class CRay &out,CRay &in)=NULL;
public:
	CMirror(void);
	~CMirror(void);
};

