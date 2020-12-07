#include "Player.h"
#include <cstring>
Player::Player()
{
}

Player::Player(std::string& fileName, std::string& animationJSON, int width, int height, Sprite* sprite, 
					AnimationController* controller, Transform* transform, bool hasPhys, PhysicsBody* body)
{
	InitPlayer(fileName, animationJSON, width, height, sprite, controller, transform, hasPhys, body);
}

void Player::InitPlayer(std::string& fileName, std::string& animationJSON, int width, int height, Sprite* sprite, 
							AnimationController* controller, Transform* transform, bool hasPhys, PhysicsBody* body)
{
	//Store references to the components
	m_sprite = sprite;
	m_animController = controller;
	m_transform = transform;
	m_hasPhysics = hasPhys;
	if (hasPhys)
	{
		m_physBody = body;
	}

	//Initialize UVs
	m_animController->InitUVs(fileName);

	//Loads the texture and sets width and height
	m_sprite->LoadSprite(fileName, width, height, true, m_animController);
	m_animController->SetVAO(m_sprite->GetVAO());
	m_animController->SetTextureSize(m_sprite->GetTextureSize());

	//Loads in the animations json file
	nlohmann::json animations = File::LoadJSON(animationJSON);
	std::string animationNames[2] = {"idle", "moving"};

	int length = sizeof(animationNames) / sizeof(animationNames[0]);
	for (int i = 0; i < length; i++)
	{
		m_animController->AddAnimation(animations[animationNames[i]].get<Animation>());
	}

	//Set Default Animation
	m_animController->SetActiveAnim(0);

	//Blood1
	{
		auto& Cam = ECS::GetComponent<Camera>(MainEntities::MainCamera());

		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Sets up components
		std::string fileName = "bloodTwo.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 320, 170);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(Cam.GetPosition().x, Cam.GetPosition().y, 6.f));

		bloodOne = entity;

		ECS::GetComponent<Sprite>(bloodOne).SetTransparency(0);


	}

	//Blood2
	{
		auto& Cam = ECS::GetComponent<Camera>(MainEntities::MainCamera());

		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Sets up components
		std::string fileName = "bloodOne.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 320, 170);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(Cam.GetPosition().x, Cam.GetPosition().y, 7.f));

		bloodTwo = entity;

		ECS::GetComponent<Sprite>(bloodTwo).SetTransparency(0);


	}

	//Blood3
	{
		auto& Cam = ECS::GetComponent<Camera>(MainEntities::MainCamera());

		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Sets up components
		std::string fileName = "bloodThree.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 320, 170);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(Cam.GetPosition().x, Cam.GetPosition().y, 8.f));

		bloodThree = entity;

		ECS::GetComponent<Sprite>(bloodThree).SetTransparency(0);


	}
	//Blood4
	{
		auto& Cam = ECS::GetComponent<Camera>(MainEntities::MainCamera());

		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Sets up components
		std::string fileName = "bloodFour.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 320, 170);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(Cam.GetPosition().x, Cam.GetPosition().y, 9.f));

		bloodFour = entity;

		ECS::GetComponent<Sprite>(bloodFour).SetTransparency(0);


	}


}

void Player::Update()
{
	if (!m_locked)
	{
		MovementUpdate();
	}

	AnimationUpdate();
	updateVignettePos();
}

void Player::updateVignettePos() {
	auto& Cam = ECS::GetComponent<Camera>(MainEntities::MainCamera());

	ECS::GetComponent<Transform>(bloodOne).SetPosition(vec3(Cam.GetPosition().x, Cam.GetPosition().y, 6.f));
	ECS::GetComponent<Transform>(bloodTwo).SetPosition(vec3(Cam.GetPosition().x, Cam.GetPosition().y, 7.f));
	ECS::GetComponent<Transform>(bloodThree).SetPosition(vec3(Cam.GetPosition().x, Cam.GetPosition().y, 8.f));
	ECS::GetComponent<Transform>(bloodFour).SetPosition(vec3(Cam.GetPosition().x, Cam.GetPosition().y, 9.f));

}

void Player::updateVignette(int health) {
	switch (health) {
	case 5:
		ECS::GetComponent<Sprite>(bloodOne).SetTransparency(0.f);
		ECS::GetComponent<Sprite>(bloodTwo).SetTransparency(0.f);
		ECS::GetComponent<Sprite>(bloodThree).SetTransparency(0.f);
		ECS::GetComponent<Sprite>(bloodFour).SetTransparency(0.f);

		break;
	case 4:

		ECS::GetComponent<Sprite>(bloodOne).SetTransparency(1.f);
		ECS::GetComponent<Sprite>(bloodTwo).SetTransparency(0.f);
		ECS::GetComponent<Sprite>(bloodThree).SetTransparency(0.f);
		ECS::GetComponent<Sprite>(bloodFour).SetTransparency(0.f);

		break;
	case 3:
		ECS::GetComponent<Sprite>(bloodOne).SetTransparency(1.f);
		ECS::GetComponent<Sprite>(bloodTwo).SetTransparency(1.f);
		ECS::GetComponent<Sprite>(bloodThree).SetTransparency(0.f);
		ECS::GetComponent<Sprite>(bloodFour).SetTransparency(0.f);

		break;
	case 2:
		ECS::GetComponent<Sprite>(bloodOne).SetTransparency(1.f);
		ECS::GetComponent<Sprite>(bloodTwo).SetTransparency(1.f);
		ECS::GetComponent<Sprite>(bloodThree).SetTransparency(1.f);
		ECS::GetComponent<Sprite>(bloodFour).SetTransparency(0.f);

		break;
	case 1:
		ECS::GetComponent<Sprite>(bloodOne).SetTransparency(1.f);
		ECS::GetComponent<Sprite>(bloodTwo).SetTransparency(1.f);
		ECS::GetComponent<Sprite>(bloodThree).SetTransparency(1.f);
		ECS::GetComponent<Sprite>(bloodFour).SetTransparency(1.f);

		break;
	}
}

void Player::takeDamage(int damage) {
	if (this->Health > 0) {
		this->Health -= damage;
		Player::updateVignette(this->Health);
	}
}

void Player::addPoints(int amount) {
	this->Score += amount;
}

bool Player::spendPoints(int amount) {
	if (this->Score > amount) {
		this->Score -= amount;
		return true;
	}
	return false;
}

void Player::MovementUpdate()
{
	m_moving = false;
	float rotationalSpeed = 3.f;
	if (m_hasPhysics)
	{
		float speed = 15.f;
		vec3 vel = vec3(0.f, 0.f, 0.f);

		if (Input::GetKey(Key::Shift))
		{
			speed *= 3.f;
		}

		if (Input::GetKey(Key::W))
		{
			vel = vel + vec3(0.f, 1.f, 0.f);
			//m_facing = UP;
			m_moving = true;
		}
		if (Input::GetKey(Key::S))
		{
			vel = vel + vec3(0.f, -1.f, 0.f);
			//m_facing = DOWN;
			m_moving = true;
		}

		if (Input::GetKey(Key::A))
		{
			vel = vel + vec3(-1.f, 0.f, 0.f);
			//m_facing = LEFT;
			m_moving = true;
		}
		if (Input::GetKey(Key::D))
		{
			vel = vel + vec3(1.f, 0.f, 0.f);
			//m_facing = RIGHT;
			m_moving = true;
		}

		if (Input::GetKey(Key::RightArrow))
		{
			m_physBody->SetRotationAngleDeg(m_physBody->GetRotationAngleDeg() - rotationalSpeed);
		}
		if (Input::GetKey(Key::LeftArrow))
		{
			m_physBody->SetRotationAngleDeg(m_physBody->GetRotationAngleDeg() + rotationalSpeed);
		}

		m_physBody->SetVelocity(vel * speed);
	}

	else
	{
		//Regular Movement
		float speed = 15.f;

#ifdef TOPDOWN
		if (Input::GetKey(Key::W))
		{
			m_transform->SetPositionY(m_transform->GetPositionY() + (speed * Timer::deltaTime));
			//m_facing = UP;
			m_moving = true;
		}
		if (Input::GetKey(Key::S))
		{
			m_transform->SetPositionY(m_transform->GetPositionY() - (speed * Timer::deltaTime));
			//m_facing = DOWN;
			m_moving = true;
		}
#endif

		if (Input::GetKey(Key::A))
		{
			m_transform->SetPositionX(m_transform->GetPositionX() - (speed * Timer::deltaTime));
			//m_facing = LEFT;
			m_moving = true;
		}
		if (Input::GetKey(Key::D))
		{
			m_transform->SetPositionX(m_transform->GetPositionX() + (speed * Timer::deltaTime));
			//m_facing = RIGHT;
			m_moving = true;
		}
	}

}

void Player::AnimationUpdate()
{
	int activeAnimation = 0;

	if (m_moving)
	{
		//Puts it into the WALK category
		activeAnimation = (int)AnimEnums::MOVING;
	}
	/*
	else if (m_attacking)
	{
		activeAnimation = ATTACK;

		//Check if the attack animation is done
		if (m_animController->GetAnimation(m_animController->GetActiveAnim()).GetAnimationDone())
		{
			//Will auto set to idle
			m_locked = false;
			m_attacking = false;
			//Resets the attack animation
			m_animController->GetAnimation(m_animController->GetActiveAnim()).Reset();

			activeAnimation = IDLE;
		}
	}
	*/
	else
	{
		activeAnimation = (int)AnimEnums::IDLE;
	}

	//SetActiveAnimation(activeAnimation + (int)m_facing);
	SetActiveAnimation(activeAnimation);
}

void Player::AttachBody(PhysicsBody* body)
{
	m_physBody = body;
}

void Player::AttachAnimation(AnimationController* controller)
{
	m_animController = controller;
}

void Player::SetActiveAnimation(int anim)
{
	m_animController->SetActiveAnim(anim);
}
