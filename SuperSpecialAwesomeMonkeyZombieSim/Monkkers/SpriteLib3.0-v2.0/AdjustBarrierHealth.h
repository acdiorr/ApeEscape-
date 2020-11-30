#pragma once

#include "Trigger.h"
#include <Box2D/Box2D.h>
#include "timer.h"

class AdjustBarrierHealthTrigger : public Trigger
{
public:
	void OnTrigger() override;

	void OnEnter() override;
	void OnExit() override;
	void OnUpdate();
	
	int maxhealth = 100;
	int minhealth = 0;
	int barrierhealth = 100;

	float scaler;
	int fixtur;
	b2Vec2 movement;
protected:
	bool triggered = false;
	float timer = 0;
};
