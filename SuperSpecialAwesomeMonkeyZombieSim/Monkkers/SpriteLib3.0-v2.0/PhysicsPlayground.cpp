#include "PhysicsPlayground.h"
#include "Utilities.h"

#include <random>

PhysicsPlayground::PhysicsPlayground(std::string name)
	: Scene(name)
{
	//No gravity this is a top down scene
	m_gravity = b2Vec2(0.f, 0.f);
	m_physicsWorld->SetGravity(m_gravity);
	m_physicsWorld->SetContactListener(&listener);
}

void PhysicsPlayground::InitScene(float windowWidth, float windowHeight)
{
	//Dynamically allocates the register
	m_sceneReg = new entt::registry;

	//Attach the register
	ECS::AttachRegister(m_sceneReg);

	//Sets up aspect ratio for the camera
	float aspectRatio = windowWidth / windowHeight;

	//Setup MainCamera Entity
	{
		/*Scene::CreateCamera(m_sceneReg, vec4(-75.f, 75.f, -75.f, 75.f), -100.f, 100.f, windowWidth, windowHeight, true, true);*/

		//Creates Camera entity
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainCamera(entity, true);

		//Creates new orthographic camera
		ECS::AttachComponent<Camera>(entity);
		ECS::AttachComponent<HorizontalScroll>(entity);
		ECS::AttachComponent<VerticalScroll>(entity);

		vec4 temp = vec4(-75.f, 75.f, -75.f, 75.f);
		ECS::GetComponent<Camera>(entity).SetOrthoSize(temp);
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);

		//Attaches the camera to vert and horiz scrolls
		ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<VerticalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
	}

	//Setup new Entity
	{
		/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		//std::string fileName = "HelloWorld.png";
		//ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 100, 60);
		//ECS::GetComponent<Sprite>(entity).SetTransparency(0.5f);
		//ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 0.f));
	}

	//Ramlethal entity
	{
		/*Scene::CreatePhysicsSprite(m_sceneReg, "LinkStandby", 80, 60, 1.f, vec3(0.f, 30.f, 2.f), b2_dynamicBody, 0.f, 0.f, true, true)*/

		auto entity = ECS::CreateEntity();
		ECS::SetIsMainPlayer(entity, true);

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<CanJump>(entity);

		//Sets up the components
		std::string fileName = "ramFunny.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 32, 32);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 2.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(0.f), float32(30.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		//tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);
		//tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY)/2.f), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);
		std::vector<b2Vec2> points = { b2Vec2(-tempSpr.GetWidth() / 2.f, -tempSpr.GetHeight() / 2.f), b2Vec2(tempSpr.GetWidth() / 2.f, -tempSpr.GetHeight() / 2.f), b2Vec2(0, tempSpr.GetHeight() / 2.f) };
		tempPhsBody = PhysicsBody(entity, BodyType::TRIANGLE, tempBody, points, vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | GROUND | TRIGGER, 0.5f, 3.f);

		tempPhsBody.SetRotationAngleDeg(0.f);
		tempPhsBody.SetFixedRotation(true);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		tempPhsBody.SetGravityScale(1.f);
	}


	//Setup Lobby

	// Bottom Wall
	CreateBoxEntity("l.png", 300, 10, -10.f, -30.f);
	// Right Wall 1 
	//Removed for first barrier
	//CreateBoxEntity("l.png", 10, 100, 140.f, 15.f);
	// Top Wall 1 
	CreateBoxEntity("l.png", 100, 10, 90.f, 60.f);
	// Right Wall 2
	CreateBoxEntity("l.png", 10, 100, 45.f, 110.f);
	// "Door 1" (Remove when doing full map)
	CreateBoxEntity("l.png", 10, 99, 55.f, 209.f);
	// Top Wall 2
	CreateBoxEntity("l.png", 60, 10, 20.f, 262.f);
	// "Door 2" (Remove when doing full map)
	CreateBoxEntity("l.png", 80, 10, -50.f, 272.f);
	// Top Wall 3 
	CreateBoxEntity("l.png", 60, 10, -120.f, 262.f);
	// Left Wall 
	CreateBoxEntity("l.png", 10, 285, -155.f, 115.f);
	//Start of First Barrier
	//CreateBoxEntity("BarrierImage.png", 10, 40, 140.f, 15.f);
	//Right wall top side of barrier
	CreateBoxEntity("l.png", 10, 30, 140.f, 50.f);
	//Right Wall Left Side of Barrier
	CreateBoxEntity("l.png", 10, 30, 140.f, -20.f);
	//Right/Top horizontal wall to create zombie hallway
	CreateBoxEntity("l.png", 100, 10, 190.f, 40.f);
	//Right/Bottom Horizontal wall to create zombie hallway
	CreateBoxEntity("l.png", 100, 10, 190.f, -10.f);
	//End of barrier hallway
	CreateBoxEntity("1.png", 10, 60, 235.f, 15.f);

	//end trigger for barriers
	endTriggerEntity = CreateEndTrigger(false, "barrierimage.png", 10, 10, 30.f, -20.f, 80.f, 460.f, 317.f);

	// #1 Lobby Barrier
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		destroyBarrier = entity;

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "BarrierImage.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 10, 40);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(140.f), float32(15.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, OBJECTS, ENVIRONMENT | PLAYER | ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	}
	//Setup inside trigger for moving barrier #1 Away
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		endTrigEntity = entity;

		//Add components
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<Trigger*>(entity);
		ECS::AttachComponent<Sprite>(entity);

		//Sets up components
		std::string fileName = "BarrierImage.png";
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 80.f));
		ECS::GetComponent<Trigger*>(entity) = new EndTrigger();
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(destroyBarrier);
		EndTrigger* temp = (EndTrigger*)ECS::GetComponent<Trigger*>(entity);
		//where to move entity
		temp->movement = b2Vec2(450.f, 30.f);

		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(147.f), float32(15.7f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(5.f - shrinkX), float(40.f - shrinkY), vec2(0.f, 0.f), true, TRIGGER, ENEMY | OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
	}

	//Setup outside trigger for moving barrier #1 Back
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		returnBarEntity = entity;

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<Trigger*>(entity);

		//Sets up components
		std::string fileName = "BarrierImage.png";
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 80.f));
		ECS::GetComponent<Trigger*>(entity) = new AdjustBarrierHealthTrigger();
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(destroyBarrier);
		EndTrigger* temp = (EndTrigger*)ECS::GetComponent<Trigger*>(entity);
		//where to move entity
		temp->movement = b2Vec2(140.f, 15.f);

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(132.f), float32(15.7f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(5.f - shrinkX), float(40.f - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
	}

	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
}

void PhysicsPlayground::Update()
{
	((EndTrigger*)(ECS::GetComponent<Trigger*>(endTrigEntity)))->OnUpdate();
	((AdjustBarrierHealthTrigger*)(ECS::GetComponent<Trigger*>(returnBarEntity)))->OnUpdate();

}


void PhysicsPlayground::KeyboardHold()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& canJump = ECS::GetComponent<CanJump>(MainEntities::MainPlayer());
	float speed = 15.f;
	b2Vec2 vel = b2Vec2(0.f, 0.f);

	if (Input::GetKey(Key::Shift))
	{
		speed *= 1.8f;
	}

	if (Input::GetKey(Key::W))
	{
		vel += b2Vec2(0.f, 1.f);
	}
	if (Input::GetKey(Key::A))
	{
		vel += b2Vec2(-1.f, 0.f);
	}
	if (Input::GetKey(Key::S))
	{
		vel += b2Vec2(0.f, -1.f);
	}
	if (Input::GetKey(Key::D))
	{
		vel += b2Vec2(1.f, 0.f);
	}

	if (Input::GetKey(Key::S))
	{
		vel += b2Vec2(0.f, -1.f);
	}
	if (Input::GetKey(Key::W))
	{
		vel += b2Vec2(0.f, 1.f);
	}

	if (Input::GetKey(Key::RightArrow))
	{
		player.SetRotationAngleDeg(player.GetRotationAngleDeg() - (3));
	}
	if (Input::GetKey(Key::LeftArrow))
	{
		player.SetRotationAngleDeg(player.GetRotationAngleDeg() + (3));

	}

	player.GetBody()->SetLinearVelocity(b2Vec2(vel.x * speed, vel.y * speed));
}

void PhysicsPlayground::KeyboardDown()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());

	if (Input::GetKeyDown(Key::T))
	{
		PhysicsBody::SetDraw(!PhysicsBody::GetDraw());
	}

}

void PhysicsPlayground::KeyboardUp()
{


}
