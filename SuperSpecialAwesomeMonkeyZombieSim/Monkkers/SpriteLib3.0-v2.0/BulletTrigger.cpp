#include "BulletTrigger.h"
#include "ECS.h"

void BulletTrigger::OnTrigger()
{
}

void BulletTrigger::OnEnter(int entity)
{
	//initializes the bullet damage and penetration
	if (!bulletTypeChecked) {
		bulletTypeChecked = true;
		std::string gunName = ECS::GetComponent<Weapon>(MainEntities::MainPlayer()).getName();
		if (gunName == "Pistol") {
			this->damage = 1.f;
			this->penetration = 1.f;
		}
		if (gunName == "Sniper") {
			this->damage = 8.f;
			this->penetration = 5.f;
		}
	}

	if (ECS::GetComponent<PhysicsBody>(m_triggerEntity).getEntityCategoryType() == ENEMY) {
		this->penetration -= 1.f;
		//ECS::GetComponent<Zombie>(m_triggerEntity).DamageZombie(this->damage); // Send him to Brazil
		if (penetration < 0) {
			PhysicsBody::m_bodiesToDelete.push_back(m_targetEntities[0]);
		}
	}
	else { // This occurs when the bullet collides with the enviroment or an object.
		PhysicsBody::m_bodiesToDelete.push_back(m_targetEntities[0]);
	}
}

void BulletTrigger::OnExit()
{
}