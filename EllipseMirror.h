#pragma once
#include "Mirror.h"
class CEllipseMirror : public CMirror
{
public:
	double m_a,m_b,m_c;//c^2=a^2-b^2
public:
	void Set(double a,double b);
	void Paint();
	bool Go(class CRay &out,CRay &in);
	CEllipseMirror(void);
	~CEllipseMirror(void);
};

