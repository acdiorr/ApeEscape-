#include "Weapon.h"
#include "Utilities.h"

void Weapon::createWeapon(std::string name)
{
	this->name = name;
	if (name == "M1911") {
		//BTS
		this->full_mag = 8;
		this->maxReserveAmmo = this->full_mag * 4;
		this->isTwoHanded = false;
		this->timeBetweenShots = 0.096f;
		this->reloadTime = 1.63f;
		this->burstSize = 1;
		this->burstTime = 0.f;
		this->accuracy = 10.f;

		//Active
		this->current_mag = this->full_mag - 1;
		this->reserve_ammo = this->full_mag * 3; // Could manually set, but I fell 4 Magazines total is a good amount

		//Bullet
		this->bullet_type = "Pistol";
	}
	if (name == "Dragunov") {
		
	}
}

float Weapon::reload(float time, bool reload)
{
	if (this->current_mag < this->full_mag && (this->full_mag - this->current_mag) <= this->reserve_ammo) {
		this->current_mag = this->full_mag; //Fills Mag
		this->reserve_ammo = this->full_mag - this->current_mag; //Removes used ammo
		return 0;
	}
}


void Weapon::fire(b2World PhysicsWorld)
{
	float BulletVelocity = 500.f;
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	//Creates entity
	auto entity = ECS::CreateEntity();

	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);
	ECS::AttachComponent<Trigger*>(entity);

	//Sets up components
	std::string fileName = "BulletStreak.jpg";
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1, 5);
	b2Vec2 BarrelPosition = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition();
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(BarrelPosition.x, BarrelPosition.y, 80.f));
	ECS::GetComponent<Trigger*>(entity) = new BulletTrigger();
	ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(entity);
	ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);

	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);
	float shrinkX = 0.f;
	float shrinkY = 0.f;
	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_dynamicBody;
	tempDef.position.Set(float32(300.f), float32(-80.f));
	tempDef.bullet = true;
	float angle = player.GetRotationAngleDeg();
	tempDef.linearVelocity = b2Vec2(float32(BulletVelocity * sin(angle)), float32(BulletVelocity * cos(angle)));
	tempBody = PhysicsWorld.CreateBody(&tempDef);

	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, ENVIRONMENT | ENEMY | OBJECTS);
	tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
	ECS::GetComponent<PhysicsBody>(entity).SetRotationAngleDeg(angle);
}

void Weapon::addAmmo(int pickup)
{
	this->reserve_ammo = this->reserve_ammo + pickup;
	if (this->reserve_ammo > this->maxReserveAmmo) {
		this->reserve_ammo = this->maxReserveAmmo;
	}
}

std::string Weapon::getName()
{
	return this->name;
}
