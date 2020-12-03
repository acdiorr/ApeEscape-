#pragma once
#include <iostream>
#include "Box2D/Box2D.h"

class Weapon
{
private:
	std::string name; // Name of the gun

	//Active (Stuff the player will care about)
	int current_mag; // How much ammo you got in your gun right now?
	int reserve_ammo; // How much ammo you have to reload your gun with

	//BTS (The KPOP Group)
	int full_mag; // How much ammo the gun holds with a reload
	int maxReserveAmmo; // How much reserve ammo can you hold?
	bool isTwoHanded; // Used to switch between two handed and one handed animation set
	float timeBetweenShots; // How fast can I shomt gung (Time in seconds), may rename to rateOfFire
	float reloadTime;
	int burstSize; // Number of bullets from a single burst
	float burstTime; // Time between burst shots
	float accuracy; // Firing Cone Angle in Degrees
	bool FullAuto;
	bool TriggerRelease = true; // If true, player will need to press the fire button again to shoot again.

	//Update
	float updateReload;
	float updateTimeBetweenShots;

	//Animation Properties
	//std::string HUDSprite;
	//std::string AmmoSprite;
	//std::string sprite;
	//std::string animation json
	//Maybe we should make an Animation Class that stores anim related stuff to access it easily?

	std::string FiringSound;
	std::string ReloadSound;
	std::string SlideSound;
	std::string EmptyTriggerPull;
	
	std::string bullet_type; //Standard, Explosive, etc.


public:
	//Use this to set the player's current gun. createWeapon is a bit of an antiquated name.
	void createWeapon(std::string name);
	void reload(); //Place in update so it runs every frame. When 
	bool fire();
	void addAmmo(int pickup);
	std::string getName();
	void weaponUpdate();
	int getBurstSize();
	float getAccuracy();
};

