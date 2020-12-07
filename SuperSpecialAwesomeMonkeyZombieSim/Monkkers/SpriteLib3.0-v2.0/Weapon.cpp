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
		this->accuracy = 2.f;
		this->FullAuto = false;
		this->BarrelOffsetX = 0;
		this->BarrelOffsetY = 0;

		//Active
		this->current_mag = this->full_mag - 1;
		this->reserve_ammo = this->full_mag * 3; // Could manually set, but I fell 4 Magazines total is a good amount

		//Bullet
		this->bullet_type = "Pistol";
	}
	if (name == "Dragunov") {
		//BTS
		this->full_mag = 10;
		this->maxReserveAmmo = this->full_mag * 2;
		this->isTwoHanded = true;
		this->timeBetweenShots = 0.2564f;
		this->reloadTime = 3.75f;
		this->burstSize = 1;
		this->burstTime = 0.f;
		this->accuracy = 1.f;
		this->FullAuto = false;
		this->BarrelOffsetX = 0;
		this->BarrelOffsetY = 0;

		//Active
		this->current_mag = this->full_mag - 1;
		this->reserve_ammo = this->full_mag * 1;

		//Bullet
		this->bullet_type = "Sniper";
	}
	if (name == "AK47") {
		//BTS
		this->full_mag = 30;
		this->maxReserveAmmo = this->full_mag * 4;
		this->isTwoHanded = true;
		this->timeBetweenShots = 0.08f;
		this->reloadTime = 2.5f;
		this->burstSize = 1;
		this->burstTime = 0.f;
		this->accuracy = 5.f;
		this->FullAuto = true;
		this->BarrelOffsetX = 0;
		this->BarrelOffsetY = 0;

		//Active
		this->current_mag = this->full_mag - 1;
		this->reserve_ammo = this->full_mag * 2;

		//Bullet
		this->bullet_type = "AR";
	}
}

void Weapon::reload()
{
	if (this->updateReload > 0) {
		return;
	}
	else if (this->current_mag < this->full_mag && (this->full_mag - this->current_mag) <= this->reserve_ammo) {
		this->reserve_ammo -= (this->full_mag - this->current_mag); //Removes used ammo
		this->updateReload = this->reloadTime;
		//Play Reload Sound Effect
		return;
	}
	return;
}

//Returns true if gun can shoot, as well as handles all the correct BTS stuff
bool Weapon::fire()
{
	if (this->FullAuto) { // Uh oh guess who's booelean logic was fucky wucky
		if (this->updateTimeBetweenShots > 0 || this->updateReload > 0) {
			return false;
		}
		else if (this->current_mag > 0) {
			this->current_mag -= 1;
			this->updateTimeBetweenShots = this->timeBetweenShots;
			return true;
		}
		else if (this->current_mag == 0) {
			//Empty Trigger Pull Sound
			return false;
		}
		return false;
	}
	if (!this->FullAuto && this->TriggerRelease) {
		if (this->updateTimeBetweenShots > 0 || this->updateReload > 0) {
			return false;
		}
		else if (this->current_mag > 0) {
			this->current_mag -= 1;
			this->updateTimeBetweenShots = this->timeBetweenShots;
			this->TriggerRelease = false;
			return true;
		}
		else if (this->current_mag == 0) {
			//Empty Trigger Pull Sound
			return false;
		}
		return false;
	}
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
	return this->bullet_type;
}

void Weapon::weaponUpdate()
{
	if (this->updateTimeBetweenShots > 0) {
		this->updateTimeBetweenShots -= Timer::deltaTime;
	}
	if (this->updateReload > 0) {
		this->updateReload -= Timer::deltaTime;
		if (this->updateReload <= 0) {
			this->current_mag = this->full_mag; //Fills Mag
			//Play Slide Click Noise
		}
	}
	if(!Input::GetKey(Key::RightContol)) { // I would use get key up but it is broken LUL
		this->TriggerRelease = true;
	}
}

int Weapon::getBurstSize()
{
	return this->burstSize;
}

float Weapon::getAccuracy()
{
	return this->accuracy;
}

float Weapon::getBarrelOffsetX()
{
	return this->BarrelOffsetX;
}

float Weapon::getBarrelOffsetY()
{
	return this->BarrelOffsetY;
}
