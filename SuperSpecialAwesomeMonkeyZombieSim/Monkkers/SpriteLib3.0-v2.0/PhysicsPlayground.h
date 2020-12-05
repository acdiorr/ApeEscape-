#pragma once

#include "Scene.h"
#include "PhysicsPlaygroundListener.h"
#include "SoundFunctions.h"
class PhysicsPlayground : public Scene
{
public:
	PhysicsPlayground(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;

	void GUI() override;

	void GUIWindowUI();
	void GUIWindowOne();
	void GUIWindowTwo();


	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;

protected:
	bool m_firstWindow = false;
	bool m_secondWindow = false;

	bool m_lerpEnabled = false;
	float m_lerpval;
	float m_tVal = 0.f;
	float m_val1 = 0.f;
	float m_val2 = 1.f;

	std::string m_fileInput;

	PhysicsPlaygroundListener listener;
	unsigned endTriggerEntity;
	unsigned endTrigEntity;
	unsigned returnBarEntity;

	void spawnZombie(float posX, float posY);
	std::vector <unsigned int> zombieEnts;
	void killZombie(int entity);


	int player = 0;
	int wall = 1;
	int ball = 2;
	int elevator = 3;
	int wall2 = 4;
	int stairs = 5;
	int wall3 = 6;
	int noTouch = 7;
	int image = 8;
	int destroyBarrier = 0;
	int MoveTheatre1 = 0;
	int MoveTheatre2 = 0;
};
