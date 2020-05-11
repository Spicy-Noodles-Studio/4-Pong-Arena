#include "PlayerUI.h"
#include <GameObject.h>
#include <UILayout.h>
#include <Camera.h>

#include "PlayerIndex.h"
#include "Health.h"

#include <ComponentRegister.h>

REGISTER_FACTORY(PlayerUI);

PlayerUI::PlayerUI(GameObject* gameObject) : UserComponent(gameObject), mainCamera(nullptr), health(nullptr), name(""), playerHUD(nullptr), playerIndicator(nullptr)
{

}

PlayerUI::~PlayerUI()
{

}

void PlayerUI::start()
{
	// Initialize name to search through layout
	PlayerIndex* playerIndex= gameObject->getComponent<PlayerIndex>();

	if (playerIndex != nullptr)
		name = "Player" + std::to_string(playerIndex->getId());

	// Get health component to update stats
	health = gameObject->getComponent<Health>();

	// Get camera
	GameObject* cameraObject = findGameObjectWithName("MainCamera");

	if (cameraObject != nullptr)
		mainCamera = cameraObject->getComponent<Camera>();

	// Get player layout
	UILayout* cameraLayout = nullptr;

	if (cameraObject != nullptr)
		cameraLayout = cameraObject->getComponent<UILayout>();

	// Get UI elements for PlayerIndicator and PlayerStatsPanel
	if (cameraLayout != nullptr)
	{
		playerHUD = cameraLayout->getRoot().getChild(name + "Background");
		playerIndicator = cameraLayout->getRoot().getChild(name + "Indicator");
	}

	playerHUD.setVisible(true);
	playerIndicator.setVisible(true);

	for (int i = 0; i < playerHUD.getChildCount(); i++)
		playerHUD.getChildAtIndex(i).setInheritsAlpha(false);

	//Initialize layout aspect
	updateHealth();
}

void PlayerUI::update(float deltaTime)
{
	updateIndicator();
	updateHealth();
}

void PlayerUI::updateIndicator()
{
	if (mainCamera == nullptr)
		return;

	Vector3 pos = mainCamera->worldToScreen(gameObject->transform->getPosition());
	playerIndicator.setPosition((float)pos.x - 0.025f, (float)pos.y - 0.025f);
}

void PlayerUI::updateHealth()
{
	if (health != nullptr)
		playerHUD.getChild(name + "HealthText").setText("Health: " + std::to_string(health->getHealth()));
}