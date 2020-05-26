#include "PauseMenu.h"
#include <ComponentRegister.h>
#include <InputSystem.h>
#include <InterfaceSystem.h>
#include <SceneManager.h>
#include <GameObject.h>
#include <UILayout.h>

#include "Countdown.h"
#include "Game.h"
#include "GameManager.h"

REGISTER_FACTORY(PauseMenu);

bool PauseMenu::optionsButtonClick()
{
	pauseMenu.setVisible(false);
	pauseMenu.setAlwaysOnTop(false);

	optionsMenu.setVisible(true);
	optionsMenu.setAlwaysOnTop(true);
	optionsMenu.setEnabled(true);

	if (InterfaceSystem::GetInstance() != nullptr) {
		InterfaceSystem::GetInstance()->clearControllerMenuInput();
		InterfaceSystem::GetInstance()->initControllerMenuInput(&optionsMenu);
	}

	buttonClick(buttonSound);
	return false;
}

bool PauseMenu::exitButtonClick()
{
	if (GameManager::GetInstance() != nullptr) {
		GameManager::GetInstance()->setPaused(false);
		GameManager::GetInstance()->stopMusic(GameManager::GetInstance()->getSong());
	}
	
	if (SceneManager::GetInstance() != nullptr) SceneManager::GetInstance()->changeScene("MainMenu");
	
	buttonClick(backSound);
	
	return false;
}

PauseMenu::PauseMenu(GameObject* gameObject) : Menu(gameObject), inputSystem(nullptr), countdown(nullptr), game(nullptr), pauseMenu(NULL), pausePanel(NULL), optionsMenu(NULL)
{
	inputSystem = InputSystem::GetInstance();

	if (InterfaceSystem::GetInstance() != nullptr) {
		InterfaceSystem::GetInstance()->registerEvent("resumeButtonClick", UIEvent("ButtonClicked", [this]() {setPaused(false); return false; }));
		InterfaceSystem::GetInstance()->registerEvent("pauseOptionsButtonClick", UIEvent("ButtonClicked", [this]() {optionsButtonClick(); return false; }));
		InterfaceSystem::GetInstance()->registerEvent("pauseExitButtonClick", UIEvent("ButtonClicked", [this]() {return exitButtonClick(); }));
	}
}

PauseMenu::~PauseMenu()
{
	if (InterfaceSystem::GetInstance() != nullptr) {
		InterfaceSystem::GetInstance()->unregisterEvent("resumeButtonClick");
		InterfaceSystem::GetInstance()->unregisterEvent("pauseOptionsButtonClick");
		InterfaceSystem::GetInstance()->unregisterEvent("pauseExitButtonClick");
	}
	inputSystem = nullptr;
	game = nullptr;
	countdown = nullptr;
}

void PauseMenu::start()
{
	Menu::start();

	if (findGameObjectWithName("MainCamera") != nullptr) {
		UILayout* cameraLayout = findGameObjectWithName("MainCamera")->getComponent<UILayout>();
		if (cameraLayout != nullptr)
		{
			pauseMenu = cameraLayout->getRoot().getChild("PauseBackground");
			pausePanel = cameraLayout->getRoot().getChild("Pause");
		}
	}
	if (findGameObjectWithName("Countdown") != nullptr) countdown = findGameObjectWithName("Countdown")->getComponent<Countdown>();
	if (findGameObjectWithName("Game") != nullptr) game = findGameObjectWithName("Game")->getComponent<Game>();
	if (findGameObjectWithName("OptionsMenuScreen") != nullptr) optionsMenu = findGameObjectWithName("OptionsMenuScreen")->getComponent<UILayout>()->getRoot();
}

void PauseMenu::preUpdate(float deltaTime)
{
	if (countdown != nullptr && countdown->hasStarted() && !countdown->isCounting() && game != nullptr && game->getTime() != 0 && inputSystem != nullptr 
		&& (inputSystem->getKeyPress("ESCAPE") || checkControllersInput()) && !optionsMenu.isVisible() && GameManager::GetInstance() != nullptr)
		setPaused(!GameManager::GetInstance()->isPaused());
}

bool PauseMenu::checkControllersInput()
{
	bool result = false;

	int i = 0;
	while (i < 4 && !result)
	{
		if (inputSystem != nullptr && (inputSystem->getButtonPress(i, "START") || (inputSystem->getButtonPress(i, "B") && pauseMenu.isVisible())))
			result = true;

		i++;
	}

	return result;
}

void PauseMenu::setPaused(bool paused)
{
	if (GameManager::GetInstance() != nullptr && paused == GameManager::GetInstance()->isPaused())
		return;

	pauseMenu.setVisible(paused);
	pauseMenu.setAlwaysOnTop(paused);

	pausePanel.setVisible(paused);
	pauseMenu.setAlwaysOnTop(paused);

	if (GameManager::GetInstance() != nullptr) GameManager::GetInstance()->setPaused(paused);
	buttonClick(buttonSound);
}

bool PauseMenu::isVisible()
{
	return pauseMenu.isVisible();
}