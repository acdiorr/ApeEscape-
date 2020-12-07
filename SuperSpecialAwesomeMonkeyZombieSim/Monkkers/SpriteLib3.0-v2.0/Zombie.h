//#pragma once
#ifndef __ZOMBIE_H__
#define __ZOMBIE_H__

#include "PhysicsBody.h"

#include "Sprite.h"

enum class ZombAnimEnums
{
	Standing = 0,
	Moving = 1,
};

class Zombie
{
private:
	float health = 8.f; // How much alive the undead has inside their meatbox
	float movementSpeed = 4.f; // Speeeed
	float attackDamage = 1.f; // How much damage the zombie can deal to an enemy
	float zombieAttackAngle = 45.f; // Checks to see if the player is within the enemy's damage cone. May go unused
	float zombieAttackRange = 20.f; // How far zonkies can attack from
	float zombieTurningSpeed = 45.f;
	float attackCooldown = 2.f; // How long zongies gotta wait to begin attacking/moving
	float timeSinceLastAttack = 0;

	void SetActiveAnimation(int anim);

	//Basically, any animation OTHER than moving will not have a cancel, and we'll be checking whether or not that animation is done
	bool m_moving = false;
	//Are you currently attacking?????
	bool m_attacking = false;
	//Have we locked the zombie from moving during this animation?
	bool m_locked = false;
	//A reference to our sprite
	Sprite* m_sprite = nullptr;
	//A reference to our animation controller
	AnimationController* m_animController = nullptr;
	//A reference to our player transform
	Transform* m_transform = nullptr;
	//Physics importance
	//A reference to our physics body
	PhysicsBody* m_physBody = nullptr;

public:
	Zombie();

	Zombie(std::string& fileName, const std::string& animationJSON, int width, int height,
		Sprite* sprite, AnimationController* controller, Transform* transform, PhysicsBody* body = nullptr);
	void InitZombie( std::string& fileName, const std::string& animationJSON, int width, int height,
		Sprite* sprite, AnimationController* controller, Transform* transform, PhysicsBody* body = nullptr); //Zombie Constructor if we need one


	void AnimationUpdate();
	void AttachBody(PhysicsBody* body);
	void AttachAnimation(AnimationController* controller);


	float getMovementSpeed(); // Returns how quick the zonkie can nae nae across the room
	float getZombieHealth(); // Returns how much health the zombie currently has. We will probably never need this.
	void dealDamage(float damageAmount); // This number will be subtracted from the zombies total health
	void zombieUpdate(PhysicsBody* ZombiePhysicsBody); // Moves the zombie in the direction of the player, and performs an attack if the player is within a certain range
	void killZombie(std::vector <unsigned int> *zEnts, int entity); //Use this to kill da mf Zonkie
	bool zombieUpdate2(std::vector <unsigned int>* zEnts, int Zentity);
};
#endif