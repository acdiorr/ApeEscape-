#include "CreationTrigger.h"
#include "ECS.h"

void CreationTrigger::OnTrigger()
{
	//Trigger::OnTrigger();
}

void CreationTrigger::OnEnter(int entity)
{
	//Trigger::OnEnter();

	if (!triggered)
	{
		{
			
		}

		triggered = true;
	}
}

void CreationTrigger::OnExit()
{
	//Trigger::OnExit();

}
