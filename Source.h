#pragma once
class CSource
{
public:
	double m_x,m_y;
public:
	virtual void Paint()=NULL;
	virtual void Get(class CRay &ray,int index)=NULL;
	virtual int GetCount()=NULL;
	CSource(void);
	~CSource(void);
};
