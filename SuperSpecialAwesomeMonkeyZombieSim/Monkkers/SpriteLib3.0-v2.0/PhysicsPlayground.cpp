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

	
	// Setup Map (Breaking it off into sections, naming each wall accordingly)

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
	CreateBoxEntity("l.png", 10, 99, 45.f, 209.f);
	// Top Wall 2
	CreateBoxEntity("l.png", 60, 10, 20.f, 262.f);
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

	// Setup Theatre (+ hallway)

	// Left Wall (for hallway)
	CreateBoxEntity("l.png", 10, 200, -95.f, 357.f);
	// Right Wall (for hallway)
	CreateBoxEntity("l.png", 10, 200, -15.f, 357.f);
	// Bottom Wall 1 (Left of hallway)
	CreateBoxEntity("l.png", 200, 10, -190.f, 452.f);
	// Bottom Wall 2 (Right of hallway)
	CreateBoxEntity("l.png", 200, 10, 80.f, 452.f);
	// Left Wall 1 
	CreateBoxEntity("l.png", 10, 120, -290.f, 507.f);
	// Right Wall 1 
	CreateBoxEntity("l.png", 10, 120, 175.f, 507.f);
	// Bottom Wall 3 (Left side above Left Wall 1)
	CreateBoxEntity("l.png", 50, 10, -320.f, 562.f);
	// Bottom Wall 4 (Right side above Right Wall 1)
	CreateBoxEntity("l.png", 50, 10, 205.f, 562.f);
	// Left Wall 2 (Left side above Bottom Wall 3)
	CreateBoxEntity("l.png", 10, 320, -340.f, 727.f);
	// Right Wall 2(Right side above Bottom Wall 4)
	CreateBoxEntity("l.png", 10, 330, 225.f, 722.f);
	// Top Wall
	CreateBoxEntity("l.png", 566, 10, -53.f, 882.f);
	
	//Health Vignette
	EffectManager::CreateEffect(Vignette, BackEnd::GetWindowWidth(), BackEnd::GetWindowHeight());
	VignetteEffect* vigH = (VignetteEffect*)EffectManager::GetEffect(EffectManager::GetVignetteHandle());

	vigH->SetInnerRadius(0.0f);
	vigH->SetOuterRadius(0.5f);
	vigH->SetOpacity(0.0f);


	//end trigger for barriers
	endTriggerEntity = CreateEndTrigger(false, "barrierimage.png", 10, 10, 30.f, -20.f, 80.f, 460.f, 317.f);
	// #1 Lobby Barrier
	//Setup trigger
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
		ECS::AttachComponent<Player>(entity);
		ECS::AttachComponent<AnimationController>(entity);
		ECS::AttachComponent<Weapon>(entity);

		//Sets up the components
		std::string fileName = "spritesheets/CharacterSprite.png";
		std::string animations = "characterAnimations.json";

		ECS::GetComponent<Player>(entity).InitPlayer(fileName, animations, 20, 20, &ECS::GetComponent<Sprite>(entity), &ECS::GetComponent<AnimationController>(entity),
			&ECS::GetComponent<Transform>(entity), true, &ECS::GetComponent<PhysicsBody>(entity));
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 2.f));
		ECS::GetComponent<Weapon>(entity).createWeapon("M1911");

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

	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
}

void PhysicsPlayground::Update()
{
	((EndTrigger*)(ECS::GetComponent<Trigger*>(endTrigEntity)))->OnUpdate();
	((AdjustBarrierHealthTrigger*)(ECS::GetComponent<Trigger*>(returnBarEntity)))->OnUpdate();
	auto& player = ECS::GetComponent<Player>(MainEntities::MainPlayer());

	player.Update();
	if (m_lerpEnabled)
	{
		m_tVal += Timer::deltaTime;
		m_lerpval = Util::Lerp<float> (m_val1, m_val2, m_tVal);
	}
	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<Weapon>(MainEntities::MainPlayer()).weaponUpdate();
}

void PhysicsPlayground::GUI()
{
	GUIWindowUI();

	if (m_firstWindow)
	{
		GUIWindowOne();
	}

	if (m_secondWindow)
	{
		GUIWindowTwo();
	}
}

void PhysicsPlayground::GUIWindowUI()
{
	ImGui::Begin("Test");

	ImGui::Checkbox("Enable First Window", &m_firstWindow);
	ImGui::SameLine();
	if (ImGui::Button("Toggle Second Window", ImVec2(180.f, 15.f)))
	{
		m_secondWindow = !m_secondWindow;
	}

	ImGui::End();
}

void PhysicsPlayground::GUIWindowOne()
{
	//Window begin
	ImGui::Begin("Side Docked Window");
	//is the buffer initialized
	static bool init = false;

	//Buffers
	const unsigned int BUF_LEN = 512;
	static char buf[BUF_LEN];
	//Image load
	static std::string imageLoad = "LinkStandby.png";
	//World gravity slider
	float gravity[2] = { m_physicsWorld->GetGravity().x, m_physicsWorld->GetGravity().y };

	if (!init)
	{
		memset(buf, 0, BUF_LEN);
		init = true;
	}
	m_physicsWorld->SetAllowSleeping(true);

	//ImGui content
	if (ImGui::TreeNode("Vignette Effect"))
	{
		if (EffectManager::GetVignetteHandle() != -1)
		{
			if (ImGui::TreeNode("Properties"))
			{
				VignetteEffect* vig = (VignetteEffect*)EffectManager::GetEffect(EffectManager::GetVignetteHandle());
				if (m_lerpEnabled)
				{

					if (m_tVal >= 1.f)
					{
						m_lerpEnabled = false;
						m_tVal = 0.f;
						m_lerpval = m_val2;
					}
					vig->SetOuterRadius(m_lerpval);
				}
				float innerRad = vig->GetInnerRadius();
				float outerRad = vig->GetOuterRadius();
				float opacity = vig->GetOpacity();
				
				

				if (ImGui::SliderFloat("Inner Radius", &innerRad, 0.f, outerRad))
				{
					vig->SetInnerRadius(innerRad);
				}
				if (ImGui::SliderFloat("Outer Radius", &outerRad, innerRad, 1.f))
				{
					vig->SetOuterRadius(outerRad);
				}
				if (ImGui::SliderFloat("Opacity", &opacity, 0.f, 1.f))
				{
					vig->SetOpacity(opacity);
				}

				if (ImGui::Button("Transition out!", ImVec2(150.f, 30.f)))
				{
					vig->SetInnerRadius(0.f);
					m_lerpEnabled = true;
					m_val1 = 0.f;
					m_val2 = 1.f;
				}
				if (ImGui::Button("Transition In", ImVec2(150.f, 30.f)))
				{
					vig->SetInnerRadius(0.f);
					m_lerpEnabled = true;
					m_val1 = 1.f;
					m_val2 = 0.f;
				}

				ImGui::TreePop();
			}

			if (ImGui::Button("Remove Effect", ImVec2(100.f, 30.f)))
			{
				EffectManager::RemoveEffect(EffectManager::GetVignetteHandle());
			}
		}
		else
		{
			if (ImGui::Button("Apply Effect", ImVec2(100.f, 30.f)))
			{
				EffectManager::CreateEffect(Vignette, BackEnd::GetWindowWidth(), BackEnd::GetWindowHeight());
			}
		}


		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Sepia Effect"))
	{
		if (EffectManager::GetSepiaHandle() != -1)
		{
			if (ImGui::TreeNode("Properties"))
			{
				SepiaEffect* sep = (SepiaEffect*)EffectManager::GetEffect(EffectManager::GetSepiaHandle());
				float intensity = sep->GetIntensity();

				if (ImGui::SliderFloat("Inner Radius", &intensity, 0.f, 1.f))
				{
					sep->SetIntensity(intensity);
				}

				ImGui::TreePop();
			}

			if (ImGui::Button("Remove Effect", ImVec2(100.f, 30.f)))
			{
				EffectManager::RemoveEffect(EffectManager::GetSepiaHandle());
			}
		}
		else
		{
			if (ImGui::Button("Apply Effect", ImVec2(100.f, 30.f)))
			{
				EffectManager::CreateEffect(Sepia, BackEnd::GetWindowWidth(), BackEnd::GetWindowHeight());
			}
		}


		ImGui::TreePop();
	}

	ImGui::Separator();

	ImGui::Text("Editing World Variables!");
	if (ImGui::SliderFloat2("World Gravity", gravity, -1000.f, 1000.f, "%.2f"))
	{
		m_physicsWorld->SetGravity(b2Vec2(gravity[0], gravity[1]));
		m_physicsWorld->SetAllowSleeping(false);
	}

	ImGui::Separator();

	ImGui::Text("Displaying image dynamically");
	ImGui::InputText("Input filename for image", buf, BUF_LEN, ImGuiInputTextFlags_CharsNoBlank);

	if (ImGui::Button("Confirm File", ImVec2(100.f, 30.f)))
	{
		imageLoad = buf;
	}

	ImGui::Image((void*)(intptr_t)TextureManager::FindTexture(imageLoad)->GetID(), ImVec2(150.f, 150.f), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::Separator();

	ImGui::Text("Editing Colors");
	ImGui::ColorPicker4("Scene Background Color", &m_clearColor.x);

	ImGui::End();
}

void PhysicsPlayground::GUIWindowTwo()
{
	//Second docked window
	ImGui::Begin("Second Window");
	//Image load
	static std::string imageLoad1 = "FunnyButton.png";
	static std::string imageLoad2 = "LikeButton.png";
	static std::string hahaPressed = "";
	static std::string likePressed = "";

	ImGui::BeginTabBar("Tab Bar Test");

	if (ImGui::BeginTabItem("Tab 1"))
	{
		ImGui::Text("You are within Tab 1");

		if (ImGui::ImageButton((void*)(intptr_t)TextureManager::FindTexture(imageLoad1)->GetID(), ImVec2(100.f, 100.f), ImVec2(0, 1), ImVec2(1, 0)))
		{
			hahaPressed = "You shouldn't have pressed that...";
		}

		ImGui::Text("%s", hahaPressed.c_str());

		ImGui::EndTabItem();
	}

	if (ImGui::BeginTabItem("Tab 2"))
	{
		ImGui::Text("You are within Tab 2");

		if (ImGui::ImageButton((void*)(intptr_t)TextureManager::FindTexture(imageLoad2)->GetID(), ImVec2(100.f, 100.f), ImVec2(0, 1), ImVec2(1, 0)))
		{
			likePressed = "LIKED!!!";
		}

		ImGui::Text("%s", likePressed.c_str());

		ImGui::EndTabItem();
	}

	ImGui::EndTabBar();

	ImGui::End();
}

void PhysicsPlayground::KeyboardHold()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& playerPlayer = ECS::GetComponent<Player>(MainEntities::MainPlayer());
	auto& playerWeapon = ECS::GetComponent<Weapon>(MainEntities::MainPlayer());
	auto& canJump = ECS::GetComponent<CanJump>(MainEntities::MainPlayer());
	float speed = 15.f;
	b2Vec2 vel = b2Vec2(0.f, 0.f);
	float rotationalSpeed = 3.f;

	if (Input::GetKey(Key::Shift))
	{
		speed *= 3.8f;
	}

	if (Input::GetKey(Key::One))
	{
		playerPlayer.updateVignette(1);
	}
	if (Input::GetKey(Key::Two))
	{
		playerPlayer.updateVignette(2);
	}
	if (Input::GetKey(Key::Three))
	{
		playerPlayer.updateVignette(3);
	}
	if (Input::GetKey(Key::Four))
	{
		playerPlayer.updateVignette(4);
	}
	if (Input::GetKey(Key::Five))
	{
		playerPlayer.updateVignette(5);
	}
	


	if (Input::GetKey(Key::RightContol)) {
		if (playerWeapon.fire()) {
			//Creates the bullet
			float BulletVelocity = 500.f;
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
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(BarrelPosition.x + playerWeapon.getBarrelOffsetX(), BarrelPosition.y + playerWeapon.getBarrelOffsetY(), 80.f));
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
			tempDef.position.Set(BarrelPosition.x + playerWeapon.getBarrelOffsetX(), BarrelPosition.y + playerWeapon.getBarrelOffsetY());
			tempDef.bullet = true;
			float angle = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetRotationAngleDeg();
			angle += std::rand() % (2 * (int)playerWeapon.getAccuracy()) - (int)playerWeapon.getAccuracy();
			tempDef.linearVelocity = b2Vec2(float32(BulletVelocity * sin(-angle * 0.01745329f)), float32(BulletVelocity * cos(-angle * 0.01745329f)));
			//std::cout << angle << std::endl;
			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, ENVIRONMENT | ENEMY | OBJECTS);
			tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
			ECS::GetComponent<PhysicsBody>(entity).SetRotationAngleDeg(angle);
			ECS::GetComponent<Player>(MainEntities::MainPlayer()).AttachBody(&ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()));
		}
		
	}
}

void PhysicsPlayground::KeyboardDown()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& playerWeapon = ECS::GetComponent<Weapon>(MainEntities::MainPlayer());

	if (Input::GetKeyDown(Key::T))
	{
		PhysicsBody::SetDraw(!PhysicsBody::GetDraw());
	}

	//Shooting
	if (Input::GetKeyDown(Key::R))
	{
		playerWeapon.reload();
	}
}

void PhysicsPlayground::KeyboardUp()
{


}
