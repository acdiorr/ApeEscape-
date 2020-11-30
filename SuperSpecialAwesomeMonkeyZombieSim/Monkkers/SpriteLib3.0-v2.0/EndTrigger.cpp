#include "EndTrigger.h"
#include "ECS.h"

void EndTrigger::OnTrigger()
{
	//Trigger::OnTrigger();

}

void EndTrigger::OnEnter(int entity)
{
	//Trigger::OnEnter();

	if (!triggered)
	{
		timer = 10;
		//while (timer != 0) {
			//OnUpdate();
		//}
	}

	triggered = false;
}

void EndTrigger::OnExit()
{
	//Trigger::OnExit();
}

void EndTrigger::OnUpdate()
{
	std::cout << "Timer is " << timer << std::endl;
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
