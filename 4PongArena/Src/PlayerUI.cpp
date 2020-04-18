#include "PlayerUI.h"
#include <GameObject.h>
#include <UILayout.h>
#include <Camera.h>

#include "PlayerController.h"
#include "IAPaddle.h"
#include "Health.h"

#include <ComponentRegister.h>

REGISTER_FACTORY(PlayerUI);

void PlayerUI::updateIndicator()
{
	if (mainCamera == nullptr)
		return;

	Vector3 pos = mainCamera->worldToScreen(gameObject->transform->getPosition());
	playerIndicator.setPosition((float)pos.x - 0.005f, (float)pos.y - 0.24f);
}

PlayerUI::PlayerUI(GameObject* gameObject) : UserComponent(gameObject), mainCamera(nullptr), health(nullptr), name(""), playerHUD(nullptr), playerIndicator(nullptr)
{

}

PlayerUI::~PlayerUI()
{

}

void PlayerUI::start()
{
	// Initialize name to search through layout
	PlayerController* playerController = gameObject->getComponent<PlayerController>();

	if (playerController != nullptr)
		name = "Player" + std::to_string(playerController->getPlayer().id);
	else
	{
		IAPaddle* paddleIA = gameObject->getComponent<IAPaddle>();

		if (paddleIA != nullptr)
			name = "Player" + std::to_string(paddleIA->getId());
	}

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

void PlayerUI::updateHealth()
{
	if (health != nullptr)
		playerHUD.getChild(name + "HealthText").setText("Health: " + std::to_string(health->getHealth()));
}