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

	if (notNull(interfaceSystem)) {
		interfaceSystem->clearControllerMenuInput();
		interfaceSystem->initControllerMenuInput(&optionsMenu);
	}

	buttonClick(buttonSound);
	return false;
}

bool PauseMenu::exitButtonClick()
{
	if (notNull(gameManager)) {
		gameManager->setPaused(false);
		gameManager->stopMusic(gameManager->getSong());
	}

	if (notNull(sceneManager)) sceneManager->changeScene("MainMenu");

	buttonClick(backSound);

	return false;
}

PauseMenu::PauseMenu(GameObject* gameObject) : Menu(gameObject), countdown(nullptr), game(nullptr), pauseMenu(NULL), pausePanel(NULL), optionsMenu(NULL)
{
	if (notNull(interfaceSystem)) {
		interfaceSystem->registerEvent("resumeButtonClick", UIEvent("ButtonClicked", [this]() {setPaused(false); return false; }));
		interfaceSystem->registerEvent("pauseOptionsButtonClick", UIEvent("ButtonClicked", [this]() {optionsButtonClick(); return false; }));
		interfaceSystem->registerEvent("pauseExitButtonClick", UIEvent("ButtonClicked", [this]() {return exitButtonClick(); }));
	}
}

PauseMenu::~PauseMenu()
{
	if (notNull(interfaceSystem)) {
		interfaceSystem->unregisterEvent("resumeButtonClick");
		interfaceSystem->unregisterEvent("pauseOptionsButtonClick");
		interfaceSystem->unregisterEvent("pauseExitButtonClick");
	}
	game = nullptr;
	countdown = nullptr;
}

void PauseMenu::start()
{
	Menu::start();

	if (notNull(mainCamera)) {
		UILayout* cameraLayout = mainCamera->getComponent<UILayout>();
		if (notNull(cameraLayout))
		{
			pauseMenu = cameraLayout->getRoot().getChild("PauseBackground");
			pausePanel = cameraLayout->getRoot().getChild("Pause");
		}
	}
	GameObject* countdownObject = findGameObjectWithName("Countdown");
	GameObject* object = findGameObjectWithName("Game");
	GameObject* optionsMenuObject = findGameObjectWithName("OptionsMenuScreen");

	if (notNull(countdownObject)) countdown = countdownObject->getComponent<Countdown>();
	if (notNull(object)) game = object->getComponent<Game>();
	if (notNull(optionsMenuObject)) {
		UILayout* optionsLayout = optionsMenuObject->getComponent<UILayout>();
		if (notNull(optionsLayout)) optionsMenu = optionsLayout->getRoot();
	}
}

void PauseMenu::preUpdate(float deltaTime)
{
	bool countdownEnded = notNull(countdown) && countdown->hasStarted() && !countdown->isCounting();
	bool gameEnded = notNull(game) && game->getTime() != 0;

	if (countdownEnded && gameEnded && notNull(inputSystem) && (inputSystem->getKeyPress("ESCAPE") || checkControllersInput()) && !optionsMenu.isVisible() && notNull(gameManager))
		setPaused(!gameManager->isPaused());
}

bool PauseMenu::checkControllersInput()
{
	bool result = false;

	int i = 0;
	while (i < 4 && !result)
	{
		if (notNull(inputSystem) && (inputSystem->getButtonPress(i, "START") || (inputSystem->getButtonPress(i, "B") && pauseMenu.isVisible())))
			result = true;
		i++;
	}

	return result;
}

void PauseMenu::setPaused(bool paused)
{
	checkNullAndBreak(gameManager);
	if (paused == gameManager->isPaused()) return;

	pauseMenu.setVisible(paused);
	pauseMenu.setAlwaysOnTop(paused);

	pausePanel.setVisible(paused);
	pauseMenu.setAlwaysOnTop(paused);

	gameManager->setPaused(paused);
	buttonClick(buttonSound);
}

bool PauseMenu::isVisible()
{
	return pauseMenu.isVisible();
}