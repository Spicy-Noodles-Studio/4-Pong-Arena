#include "PlayerUI.h"
#include <GameObject.h>
#include <UILayout.h>
#include <Camera.h>

#include "PlayerIndex.h"
#include "Health.h"

#include <ComponentRegister.h>

REGISTER_FACTORY(PlayerUI);

PlayerUI::PlayerUI(GameObject* gameObject) : UserComponent(gameObject), mainCamera(nullptr), health(nullptr), name(""), playerHUD(nullptr)
{

}

PlayerUI::~PlayerUI()
{

}

void PlayerUI::start()
{
	// Initialize name to search through layout
	playerIndex = gameObject->getComponent<PlayerIndex>();

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
		playerHUD = cameraLayout->getRoot().getChild(name + "Background");

	playerHUD.setVisible(true);

	//Initialize layout aspect
	updateHealth();
}

void PlayerUI::update(float deltaTime)
{
	updateHealth();
}

void PlayerUI::updateHealth()
{
	std::string color;

	if (playerIndex->getId() == 1)
		color = "[colour = 'FFFF0000']";
	else if (playerIndex->getId() == 2)
		color = "[colour='FF0000FF']";
	else if (playerIndex->getId() == 3)
		color = "[colour='FFFFFF00']";
	else
		color = "[colour='FF00FF00']";

	if (playerIndex != nullptr && health != nullptr)
		playerHUD.getChild(name + "Text").setText(color + "P" + std::to_string(playerIndex->getId()) + ": " + std::to_string(health->getHealth()));
}