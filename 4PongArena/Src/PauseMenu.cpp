#include "PauseMenu.h"
#include <InputSystem.h>
#include <InterfaceSystem.h>
#include <GameObject.h>
#include <SceneManager.h>
#include "GameManager.h"
#include "UILayout.h"

#include <ComponentRegister.h>

REGISTER_FACTORY(PauseMenu);

bool PauseMenu::backButtonClick()
{
	pauseMenu.setVisible(false);
	GameManager::GetInstance()->pause(false);
	SceneManager::GetInstance()->changeScene("MainMenu");
	return false;
}

bool PauseMenu::optionsButton()
{
	pauseMenu.setVisible(false);
	pauseMenu.setAlwaysOnTop(false);

	optionsMenu.setVisible(true);
	optionsMenu.setAlwaysOnTop(true);
	optionsMenu.setEnabled(true);

	InterfaceSystem::GetInstance()->clearControllerMenuInput();
	InterfaceSystem::GetInstance()->initControllerMenuInput(&optionsMenu);

	return false;
}

PauseMenu::PauseMenu(GameObject* gameObject) : UserComponent(gameObject), pauseMenu(NULL),optionsMenu(NULL)
{
	InterfaceSystem::GetInstance()->registerEvent("resumeButtonClick", UIEvent("ButtonClicked", [this]() {setPaused(false); return false;}));
	InterfaceSystem::GetInstance()->registerEvent("pauseOptionsButtonClick", UIEvent("ButtonClicked", [this]() {optionsButton(); return false; }));
	InterfaceSystem::GetInstance()->registerEvent("pauseBackButtonClick", UIEvent("ButtonClicked", [this]() {backButtonClick(); return false; }));

}

PauseMenu::~PauseMenu()
{
	InterfaceSystem::GetInstance()->unregisterEvent("resumeButtonClick");
	InterfaceSystem::GetInstance()->unregisterEvent("pauseOptionsButtonClick");
	InterfaceSystem::GetInstance()->unregisterEvent("pauseBackButtonClick");
}

void PauseMenu::start()
{
	UILayout* cameraLayout = findGameObjectWithName("MainCamera")->getComponent<UILayout>();
	optionsMenu = findGameObjectWithName("OptionsMenuScreen")->getComponent<UILayout>()->getRoot();
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
	pauseMenu.setAlwaysOnTop(paused);
	GameManager::GetInstance()->pause(paused);
}

bool PauseMenu::isVisible()
{
	return pauseMenu.isVisible();
}