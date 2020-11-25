#pragma once
#include <iostream>
class Bullet
{
	float angle;
	float damage;
	float penetration;

	Bullet(float angle, float damage, float penetration);
	Bullet(std::string name);
};

