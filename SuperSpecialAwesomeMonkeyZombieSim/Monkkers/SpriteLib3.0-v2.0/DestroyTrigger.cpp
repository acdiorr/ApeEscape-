#include "DestroyTrigger.h"
#include "ECS.h"

void DestroyTrigger::OnTrigger()
{
}

void DestroyTrigger::OnEnter(int entity)
{
	if (!triggered)
	{
		for (int i = 0; i < m_targetEntities.size(); i++)
		{
			PhysicsBody::m_bodiesToDelete.push_back(m_targetEntities[i]);
		}
		triggered = true;
	}
}

void DestroyTrigger::OnExit()
{
}
