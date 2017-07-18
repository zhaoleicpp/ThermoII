#include "stdafx.h"
#include "Tool.h"
#define _USE_MATH_DEFINES
#include <math.h>

double GetAngle(double x,double y)
{
    double angle;
    double rho;
	rho=x*x+y*y;
	rho=sqrt(rho);
    if(rho==0)
        angle=0;
    else
    {
	    angle=acos(x/rho);
	    if(y<0)
		    angle=M_PI*2-angle;
    }
	return angle;
}
