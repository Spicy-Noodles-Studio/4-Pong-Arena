#include "PlayerUI.h"
#include <GameObject.h>
#include <UILayout.h>
#include <Camera.h>

#include "PlayerIndex.h"
#include "Health.h"

#include <ComponentRegister.h>

REGISTER_FACTORY(PlayerUI);

PlayerUI::PlayerUI(GameObject* gameObject) : UserComponent(gameObject), mainCamera(nullptr), health(nullptr), name(""), playerHUD(nullptr), playerIndex(nullptr)
{

}

PlayerUI::~PlayerUI()
{
	mainCamera = nullptr;
	playerIndex = nullptr;
	health = nullptr;
}

void PlayerUI::start()
{
	if (notNull(gameObject)) {
		// Initialize name to search through layout
		playerIndex = gameObject->getComponent<PlayerIndex>();
		// Get health component to update stats
		health = gameObject->getComponent<Health>();

		if (notNull(playerIndex))
			name = "Player" + std::to_string(playerIndex->getId());
	}

	// Get camera
	GameObject* cameraObject = findGameObjectWithName("MainCamera");

	if (notNull(cameraObject)) {
		mainCamera = cameraObject->getComponent<Camera>();

		// Get UI elements for PlayerIndicator and PlayerStatsPanel
		 UILayout* cameraLayout = cameraObject->getComponent<UILayout>();
		if (cameraLayout != nullptr)
			playerHUD = cameraLayout->getRoot().getChild(name + "Background");
	}
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
	if (notNull(playerIndex)) {
		if (playerIndex->getId() == 1)
			color = "[colour = 'FFFF0000']";
		else if (playerIndex->getId() == 2)
			color = "[colour='FF0000FF']";
		else if (playerIndex->getId() == 3)
			color = "[colour='FFFFFF00']";
		else
			color = "[colour='FF00FF00']";

		if (notNull(health))
			playerHUD.getChild(name + "Text").setText(color + "P" + std::to_string(playerIndex->getId()) + ": " + std::to_string(health->getHealth()));
	}
}