#pragma once
#include "PhysicsBody.h"

class Zombie
{
private:
	float health = 8.f; // How much alive the undead has inside their meatbox
	float movementSpeed = 4.f; // Speeeed
	float attackDamage = 1.f; // How much damage the zombie can deal to an enemy
	float zombieAttackAngle = 45.f; // Checks to see if the player is within the enemy's damage cone. May go unused
	float zombieAttackRange = 10.f; // How far zonkies can attack from
	float zombieTurningSpeed = 45.f;
	float attackCooldown = 2.f; // How long zongies gotta wait to begin attacking/moving
	float timeSinceLastAttack;

public:
	float getMovementSpeed(); // Returns how quick the zonkie can nae nae across the room
	float getZombieHealth(); // Returns how much health the zombie currently has. We will probably never need this.
	void dealDamage(float damageAmount); // This number will be subtracted from the zombies total health
	void zombieUpdate(PhysicsBody ZombiePhysicsBody); // Moves the zombie in the direction of the player, and performs an attack if the player is within a certain range
	void makeZombie(); //Zombie Constructor if we need one
	void killZombie(); //Use this to kill da mf Zonkie
};

