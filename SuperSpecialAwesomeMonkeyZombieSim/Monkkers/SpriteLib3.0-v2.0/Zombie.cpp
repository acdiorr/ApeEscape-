#include "Zombie.h"
#include "Utilities.h"


float Zombie::getMovementSpeed()
{
    return this->movementSpeed;
}

float Zombie::getZombieHealth()
{
    return this->health;
}

void Zombie::dealDamage(float damageAmount)
{
    this->health -= damageAmount;
}

void Zombie::zombieUpdate(PhysicsBody ZombiePhysicsBody, std::vector <int>* zEnts, int Zentity)
{
    if (this->timeSinceLastAttack > 0) {
        this->timeSinceLastAttack -= Timer::deltaTime;
    }
    else {
        //Face Player
        {
            float zToPX = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition().x - ZombiePhysicsBody.GetPosition().x;
            float zToPY = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition().y - ZombiePhysicsBody.GetPosition().y;
            if (zToPY == 0) {
                float angle = atan(-zToPX / 0.000001) * 57.29577951308f;
                if (angle > this->zombieTurningSpeed) {
                    ZombiePhysicsBody.SetRotationAngleDeg(ZombiePhysicsBody.GetRotationAngleDeg() + this->zombieTurningSpeed * Timer::deltaTime);
                }
                else {
                    ZombiePhysicsBody.SetRotationAngleDeg(ZombiePhysicsBody.GetRotationAngleDeg() + angle * Timer::deltaTime);
                }
            }
            else {
                float angle = atan(-zToPX / zToPY) * 57.29577951308f;
                if (angle > this->zombieTurningSpeed) {
                    ZombiePhysicsBody.SetRotationAngleDeg(ZombiePhysicsBody.GetRotationAngleDeg() + this->zombieTurningSpeed * Timer::deltaTime);
                }
                else {
                    ZombiePhysicsBody.SetRotationAngleDeg(ZombiePhysicsBody.GetRotationAngleDeg() + angle * Timer::deltaTime);
                }
            }
        }
        //Move in the direction the zombie is facing
        {
            float angle = ZombiePhysicsBody.GetRotationAngleDeg();
            b2Vec2(float32(this->movementSpeed * sin(-angle* 0.01745329f)), float32(this->movementSpeed * cos(-angle * 0.01745329f)));
        }
        //Attack Player if within range
        {
            float zToPX = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition().x - ZombiePhysicsBody.GetPosition().x;
            float zToPY = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition().y - ZombiePhysicsBody.GetPosition().y;
            float distance = sqrt(zToPX * zToPX + zToPY * zToPY);
            //Check if player is in attack range
            if (distance < this->zombieAttackRange) {
                //Check if player is in enemy attack cone
                float worldAngleZtoP;
                //Uses near 0 for atan to prevent errors
                if (zToPY == 0) {
                    worldAngleZtoP = atan(-zToPX / 0.000001) * 57.29577951308f;
                }
                else {
                    worldAngleZtoP = atan(-zToPX / zToPY) * 57.29577951308f;
                }
                float zombieFacingAngle = ZombiePhysicsBody.GetRotationAngleDeg();
                //Set Zombie Facing Angle within the correct range
                while (zombieFacingAngle >= 360.f) {
                    zombieFacingAngle -= 360.f;
                }
                while (zombieFacingAngle < 0.f) {
                    zombieFacingAngle += 360.f;
                }
                //Final Check
                if (worldAngleZtoP > (zombieFacingAngle - this->zombieAttackAngle) && worldAngleZtoP < (zombieFacingAngle + this->zombieAttackAngle)) {
                    this->timeSinceLastAttack = this->attackCooldown;
                    ECS::GetComponent<Player>(MainEntities::MainPlayer()).takeDamage(1);
                }
            }
        }
    }
    if (this->health < 1) {
        killZombie(zEnts, Zentity);
    }
}

void Zombie::makeZombie()
{
}

void Zombie::killZombie(std::vector <int> *zEnts, int entity)
{
    for (int x = 0; x < zEnts->size(); x++) {
        if (zEnts->at(x) == entity) {
            zEnts->erase(zEnts->begin() + x);
        }
    }
    PhysicsBody::m_bodiesToDelete.push_back(entity);
}
