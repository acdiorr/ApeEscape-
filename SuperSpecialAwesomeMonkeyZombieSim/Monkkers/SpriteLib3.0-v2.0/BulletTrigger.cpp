#include "BulletTrigger.h"
#include "ECS.h"
#include "BackEnd.h"

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
		if (gunName == "AR") {
			this->damage = 2.f;
			this->penetration = 1.f;
		}
	}

	if (ECS::GetComponent<PhysicsBody>(entity).getEntityCategoryType() == ENEMY) {
		std::cout << "Enemy Shot" << std::endl;
		//std::cout << damage << std::endl;
		this->penetration -= 1.f;
		//std::cout << penetration << std::endl;
		ECS::GetComponent<Zombie>(entity).dealDamage(this->damage); // Send him to Brazil
		ECS::GetComponent<Player>(MainEntities::MainPlayer()).addPoints(10);

		if (penetration <= 0) {
			if (!beingDeleted) {
				beingDeleted = true;
				PhysicsBody::m_bodiesToDelete.push_back(m_triggerEntity);
			}
		}
	}
	else { // This occurs when the bullet collides with the enviroment or an object.
		if (!beingDeleted) {
			beingDeleted = true;
			PhysicsBody::m_bodiesToDelete.push_back(m_triggerEntity);
		}
	}
}

void BulletTrigger::OnExit()
{
}