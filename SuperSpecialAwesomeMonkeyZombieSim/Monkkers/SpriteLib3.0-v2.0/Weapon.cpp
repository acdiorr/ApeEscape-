#include "Weapon.h"

Weapon::Weapon(std::string name)
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
}
/*
float Weapon::reload(float time, bool reload)
{
	if (time > 0) {
		time = time - Timer::deltaTime;
		if (time < 0) {
			if (this->current_mag < full_mag && (this->full_mag - this->current_mag) <= this->reserve_ammo) {
				this->current_mag = this->full_mag; //Fills Mag
				this->reserve_ammo = this->full_mag - this->current_mag; //Removes used ammo
				return 0;
			}
		}
		return;
	}
}
*/

void Weapon::fire()
{

}

void Weapon::addAmmo(int pickup)
{
	this->reserve_ammo = this->reserve_ammo + pickup;
	if (this->reserve_ammo > this->maxReserveAmmo) {
		this->reserve_ammo = this->maxReserveAmmo;
	}
}
