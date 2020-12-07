#include "Zombie.h"
#include "Utilities.h"

Zombie::Zombie()
{
}

Zombie::Zombie(std::string& fileName, const std::string& animationJSON, int width, int height, Sprite* sprite, AnimationController* controller, Transform* transform, PhysicsBody* body)
{
    InitZombie(fileName, animationJSON, width, height, sprite, controller, transform, body);
}

void Zombie::InitZombie(std::string& fileName, const std::string& animationJSON, int width, int height, Sprite* sprite, AnimationController* controller, Transform* transform, PhysicsBody* body)
{
    //Store references to the components
    m_sprite = sprite;
    m_animController = controller;
    m_transform = transform;
    m_physBody = body;

    //Initialize UVs
    m_animController->InitUVs(fileName);

    //Loads the texture and sets width and height
    m_sprite->LoadSprite(fileName, width, height, true, m_animController);
    m_animController->SetVAO(m_sprite->GetVAO());
    m_animController->SetTextureSize(m_sprite->GetTextureSize());

    //Loads in the animations json file
    nlohmann::json animations = File::LoadJSON(animationJSON);
    std::string animationNames[2] = { "idle", "moving" };

    int length = sizeof(animationNames) / sizeof(animationNames[0]);
    for (int i = 0; i < length; i++)
    {
        m_animController->AddAnimation(animations[animationNames[i]]);
    }

    //Set Default Animation
    m_animController->SetActiveAnim(0);
}

void Zombie::AnimationUpdate()
{
    int activeAnimation = 0;

    if (m_moving)
    {
        //Puts it into the WALK category
        activeAnimation = (int)ZombAnimEnums::Moving;
    }

    else
    {
        activeAnimation = (int)ZombAnimEnums::Standing;
    }

    //SetActiveAnimation(activeAnimation + (int)m_facing);
    SetActiveAnimation(activeAnimation);
}

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

void Zombie::zombieUpdate(PhysicsBody ZombiePhysicsBody, std::vector <unsigned int>* zEnts, int Zentity)
{
    //std::cout << health << std::endl;
    
    //Initialize Variables
    vec3 movement = vec3(0, 0, 0);
    vec2 ZtoP = vec2(0, 0);
    
    //Check if the zombie is on cooldown
    if (this->timeSinceLastAttack > 0) {
        this->timeSinceLastAttack -= Timer::deltaTime;
    }
    //Zombie AI
    else {
        //Rotating Zombie in the correct direction
        {
            //Get Vector from the Zombie to the player
            ZtoP = vec2(ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition().x - ZombiePhysicsBody.GetPosition().x, ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition().y - ZombiePhysicsBody.GetPosition().y);
            //Angle from the Zombie to the Player
            double ZtoPangle = atan2(ZtoP.x, ZtoP.y) * 57.29577951308;
            ZombiePhysicsBody.SetRotationAngleDeg(-ZtoPangle);
        }
        //Move in the direction the zombie is facing
        {
            float angle = ZombiePhysicsBody.GetRotationAngleDeg();
            movement = vec3(movementSpeed * sin(-angle* 0.01745329f), movementSpeed * cos(-angle * 0.01745329f), 0);
            m_physBody->SetVelocity(movement);
            if (m_physBody->GetBody()->GetLinearVelocity() == b2Vec2(0.f, 0.f))
            {
                m_moving = false;
            }
            else
            {
                m_moving = true;
            }

        }
        //Attack Player if within range
        {
            ZtoP = vec2(ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition().x - ZombiePhysicsBody.GetPosition().x, ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition().y - ZombiePhysicsBody.GetPosition().y);
            float distance = sqrt(ZtoP.x * ZtoP.x + ZtoP.y * ZtoP.y);
            //Check if player is in attack range
            if (distance < this->zombieAttackRange) {
                this->timeSinceLastAttack = this->attackCooldown;
                ECS::GetComponent<Player>(MainEntities::MainPlayer()).takeDamage(1);
            }
        }
    }
    AnimationUpdate();
    if (this->health < 1) {
        killZombie(zEnts, Zentity);
    }
}


void Zombie::killZombie(std::vector <unsigned int> *zEnts, int entity)
{
    for (int x = 0; x < zEnts->size(); x++) {
        if (zEnts->at(x) == entity) {
            zEnts->erase(zEnts->begin() + x);
        }
    }
    PhysicsBody::m_bodiesToDelete.push_back(entity);
}

void Zombie::AttachBody(PhysicsBody* body)
{
    m_physBody = body;
}

void Zombie::AttachAnimation(AnimationController* controller)
{
    m_animController = controller;
}

void Zombie::SetActiveAnimation(int anim)
{
    m_animController->SetActiveAnim(anim);
}
