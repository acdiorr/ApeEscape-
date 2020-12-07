#include "WeaponHud.h"
#include "Utilities.h"

WeaponHud::WeaponHud()
{
}

WeaponHud::WeaponHud(std::string& fileName, const std::string& animationJSON, int width, int height, Sprite* sprite, AnimationController* controller, Transform* transform)
{
	InitWeaponHud(fileName, animationJSON, width, height, sprite, controller, transform);
}

void WeaponHud::InitWeaponHud(std::string& fileName, const std::string& animationJSON, int width, int height, Sprite* sprite, AnimationController* controller, Transform* transform)
{
    //Store references to the components
    m_sprite = sprite;
    m_animController = controller;
    m_transform = transform;

    //Initialize UVs
    m_animController->InitUVs(fileName);

    //Loads the texture and sets width and height
    m_sprite->LoadSprite(fileName, width, height, true, m_animController);
    m_animController->SetVAO(m_sprite->GetVAO());
    m_animController->SetTextureSize(m_sprite->GetTextureSize());

    //Loads in the animations json file
    nlohmann::json animations = File::LoadJSON(animationJSON);
    std::string animationNames[3] = { "Pistol", "AK47", "Sniper" };

    int length = sizeof(animationNames) / sizeof(animationNames[0]);
    for (int i = 0; i < length; i++)
    {
        m_animController->AddAnimation(animations[animationNames[i]]);
    }

    //Set Default Animation
    m_animController->SetActiveAnim(0);
}

void WeaponHud::update(std::string weapon)
{
    auto& Cam = ECS::GetComponent<Camera>(MainEntities::MainCamera());
    m_weapon = 0;
    if (weapon == "M1911")
    {
        m_weapon = 0;
    }
    else if (weapon == "AK47")
    {
        m_weapon = 1;
    }

    else if (weapon == "Dragunov")
    {
        m_weapon = 2;
    }
    m_transform->SetPosition(vec3(Cam.GetPosition().x + 80.f, Cam.GetPosition().y - 50.f, 5.f));

    AnimationUpdate();
}

void WeaponHud::AnimationUpdate()
{
    int activeAnimation = 0;
    //Puts it into the weapon category
    switch (m_weapon)
    {
    case 0:
        activeAnimation = (int)WeaponHudEnums::PISTOL;
        break;
    case 1:
        activeAnimation = (int)WeaponHudEnums::AK47;
        break;
    case 2:
        activeAnimation = (int)WeaponHudEnums::DRAGANOV;
        break;
    default:
        activeAnimation = (int)WeaponHudEnums::PISTOL;
        break;
    }
    

    SetActiveAnimation(activeAnimation);

}

void WeaponHud::SetActiveAnimation(int anim)
{
    m_animController->SetActiveAnim(anim);
}


void WeaponHud::AttachTransform(Transform* transform)
{
    m_transform = transform;
}

void WeaponHud::AttachAnimation(AnimationController* controller)
{
    m_animController = controller;
}
