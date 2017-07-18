#pragma once
#include "Screen.h"
class CRoundScreen : public CScreen
{
private:
	class CRoundSource * m_round_source;
	struct SReceive
	{
		double self;
		double other;
	};
	SReceive * m_receive;
	void Delete();
public:
	void Set(class CRoundSource &round_source);
	virtual void Paint();
	virtual bool Go(class CRay &ray);
	void Receive(class CRay &ray,bool self);
	CRoundScreen(void);
	~CRoundScreen(void);
};

