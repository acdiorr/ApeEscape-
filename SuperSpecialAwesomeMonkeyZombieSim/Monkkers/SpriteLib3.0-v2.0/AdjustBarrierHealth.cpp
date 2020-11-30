#include "ECS.h"
#include "Utilities.h"
#include "AdjustBarrierHealth.h"

void AdjustBarrierHealthTrigger::OnTrigger()
{
	Trigger::OnTrigger();
}

void AdjustBarrierHealthTrigger::OnEnter(int entity)
{
	Trigger::OnEnter(0);

	if (!triggered)
	{
		timer = 10;
	}

	triggered = false;
}

void AdjustBarrierHealthTrigger::OnExit()
{
	Trigger::OnExit();
}

void AdjustBarrierHealthTrigger::OnUpdate()
{
	std::cout << "Adjust Timer is " << timer << std::endl;
	if (timer > 0)
	{
		timer -= Timer::deltaTime;
		if (timer <= 0)
		{

			timer = 0;

			//if (barrierhealth > minhealth) {
				//barrierhealth = barrierhealth - 10;
				//std::cout << barrierhealth << std::endl;
			//}

			//Trigger::OnEnter();
			for (int i = 0; i < m_targetEntities.size(); i++)
			{
				//std::cout << "barrierhealth is " << barrierhealth << std::endl;
				//if (barrierhealth == 0)
				{
					ECS::GetComponent<PhysicsBody>(m_targetEntities[i]).SetPosition(movement, true);
				}
			}
			//exit(0);
		}
	}
}

