#pragma once
#include "RoundSource.h"
#include "EllipseMirror.h"
#include "RoundScreen.h"
class CMain
{
public:
	CRoundSource m_left_source,m_right_source;
	CEllipseMirror m_ellipse_mirror;
	CRoundScreen m_left_screen,m_right_screen;
	void Go();
	CMain(void);
	~CMain(void);
};

extern CMain g_main;