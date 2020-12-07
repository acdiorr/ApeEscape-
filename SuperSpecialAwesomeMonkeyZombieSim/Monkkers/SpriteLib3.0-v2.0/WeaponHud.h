#pragma once
#include "PhysicsBody.h"

#include "Sprite.h"

enum class WeaponHudEnums
{
	PISTOL = 0,
	AK47 = 1,
	DRAGANOV = 3
};

class WeaponHud
{
private:
	void SetActiveAnimation(int anim);

	//A reference to our sprite
	Sprite* m_sprite = nullptr;
	//A reference to our animation controller
	AnimationController* m_animController = nullptr;
	//A reference to our player transform
	Transform* m_transform = nullptr;
	//Physics importance
	//A reference to our physics body
	//PhysicsBody* m_physBody = nullptr;
	int m_weapon = 0;

public:
	WeaponHud();

	WeaponHud(std::string& fileName, const std::string& animationJSON, int width, int height,
		Sprite* sprite, AnimationController* controller, Transform* transform);
	void InitWeaponHud(std::string& fileName, const std::string& animationJSON, int width, int height,
		Sprite* sprite, AnimationController* controller, Transform* transform);

	void update(std::string weapon);

	void AnimationUpdate();
	void AttachTransform(Transform* transform);
	void AttachAnimation(AnimationController* controller);
};