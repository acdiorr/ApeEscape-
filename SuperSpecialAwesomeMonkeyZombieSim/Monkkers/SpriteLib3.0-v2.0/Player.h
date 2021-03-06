#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "BackEnd.h"
#include <string>
#define TOPDOWN

enum class AnimEnums
{
	IDLE = 0,
	MOVING = 1,
	
};

/*
enum class AnimTypes
{
#ifdef TOPDOWN
	IDLE = 0,
	WALK = 1,
	//ATTACK = 8
#endif
#ifndef TOPDOWN
	IDLE = 0,
	WALK = 2,
	//ATTACK = 4
#endif
};
*/

class Player
{
public:
	Player();
	Player(std::string& fileName, std::string& animationJSON, int width, int height, 
		Sprite* sprite, AnimationController* controller, Transform* transform, bool hasPhys = false, PhysicsBody* body = nullptr);

	void InitPlayer(std::string& fileName, std::string& animationJSON, int width, int height, 
		Sprite* sprite, AnimationController* controller, Transform* transform, bool hasPhys = false, PhysicsBody* body = nullptr);


	void Update();
	void MovementUpdate();
	void AnimationUpdate();
	void AttachBody(PhysicsBody* body);
	void AttachAnimation(AnimationController* controller);
	//Damages the player based on the damage you want him to take
	void takeDamage(int damage);

	//Updates the Vignette depending on the health number
	void updateVignette(int health);

	//Updates the vignette pos to be centered on the camera
	void updateVignettePos();

	//Adds a specified number of points to the score count
	void addPoints(int amount);
	//Returns true if the transaction was successful, and removes the amount from the player's point pool. Otherwise returns false.
	bool spendPoints(int amount);



private:
	void SetActiveAnimation(int anim);

	int bloodOne;
	int bloodTwo;
	int bloodThree;
	int bloodFour;

	int MaxHealth = 5;
	int Health = 5;
	int Score = 0;

	//Basically, any animation OTHER than moving will not have a cancel, and we'll be checking whether or not that animation is done
	bool m_moving = false;
	//Are you currently attacking?????
	bool m_attacking = false;
	//Have we locked the player from moving during this animation?
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
	//Does this player have physics?
	bool m_hasPhysics = false;

	//Default animation direction (feel free to change this to suit your game. If you're making a side-scroller, left or right would be better
	//AnimDir m_facing = LEFT;
};

#endif // !__PLAYER_H__