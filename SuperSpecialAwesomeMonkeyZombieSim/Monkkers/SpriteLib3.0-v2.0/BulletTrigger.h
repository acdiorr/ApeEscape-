#pragma once
#include "Trigger.h"
class BulletTrigger :
    public Trigger
{
	void OnTrigger() override;

	void OnEnter(int entity) override;
	void OnExit() override;
protected:
	bool bulletTypeChecked = false;
	float penetration = 0;
	float damage = 0;
};