#include "PauseMenu.h"
#include <InputSystem.h>
#include <InterfaceSystem.h>
#include <GameObject.h>

#include "GameManager.h"
#include "UILayout.h"

#include <ComponentRegister.h>

REGISTER_FACTORY(PauseMenu);

PauseMenu::PauseMenu(GameObject* gameObject) : UserComponent(gameObject), pauseMenu(NULL)
{
	InterfaceSystem::GetInstance()->registerEvent("resumeButtonClick", UIEvent("ButtonClicked", [this]() {setPaused(false); return false;}));
}

PauseMenu::~PauseMenu()
{

}

void PauseMenu::start()
{
	UILayout* cameraLayout = findGameObjectWithName("MainCamera")->getComponent<UILayout>();

	if (cameraLayout != nullptr)
		pauseMenu = cameraLayout->getRoot().getChild("PauseBackground");

	inputSystem = InputSystem::GetInstance();
}

void PauseMenu::update(float deltaTime)
{
	if (inputSystem->getKeyPress("ESCAPE"))
		setPaused(!GameManager::GetInstance()->isPaused());
}

void PauseMenu::setPaused(bool paused)
{
	pauseMenu.setVisible(paused);
	GameManager::GetInstance()->pause(paused);
}

bool PauseMenu::isVisible()
{
	return pauseMenu.isVisible();
}