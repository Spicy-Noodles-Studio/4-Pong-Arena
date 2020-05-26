#include "MainMenu.h"
#include <ComponentRegister.h>
#include <InterfaceSystem.h>
#include <RenderSystem.h>
#include <SceneManager.h>
#include <WindowManager.h>
#include <SoundEmitter.h>
#include <GameObject.h>

#include "GameManager.h"

REGISTER_FACTORY(MainMenu);

bool MainMenu::playButtonClick()
{
	if (notNull(sceneManager)) sceneManager->changeScene("ConfigurationMenu");
	buttonClick(buttonSound);
	return false;
}

bool MainMenu::optionsButtonClick()
{
	if (notNull(sceneManager)) sceneManager->changeScene("OptionsMenu");
	buttonClick(buttonSound);
	return false;
}

bool MainMenu::exitButtonClick()
{
	WindowManager* windowManager = WindowManager::GetInstance();
	if (notNull(windowManager)) windowManager->closeWindow();
	buttonClick(backSound);
	return false;
}

bool MainMenu::controlsButtonClick()
{
	if (notNull(sceneManager)) sceneManager->changeScene("ControlsMenu");
	buttonClick(buttonSound);
	return false;
}

bool MainMenu::creditsButtonClick()
{
	if (notNull(sceneManager)) sceneManager->changeScene("Credits");
	buttonClick(buttonSound);
	return false;
}

void MainMenu::initMusic()
{
	if (notNull(gameManager) && !gameManager->isMenuMusicPlaying())
	{
		gameManager->setMenuMusic(true);
		gameManager->playMusic("Menu_loop");
		gameManager->setMusicVolume(0.4);
	}
}

MainMenu::MainMenu(GameObject* gameObject) : Menu(gameObject)
{
	if (notNull(interfaceSystem)) {
		interfaceSystem->registerEvent("playButtonClick", UIEvent("ButtonClicked", [this]() {return playButtonClick(); }));
		interfaceSystem->registerEvent("optionsButtonClick", UIEvent("ButtonClicked", [this]() {return optionsButtonClick(); }));
		interfaceSystem->registerEvent("exitButtonClick", UIEvent("ButtonClicked", [this]() {return exitButtonClick(); }));

		interfaceSystem->registerEvent("controlsButtonClick", UIEvent("ButtonClicked", [this]() {return controlsButtonClick(); }));
		interfaceSystem->registerEvent("creditsButtonClick", UIEvent("ButtonClicked", [this]() {return creditsButtonClick(); }));
	}
}

MainMenu::~MainMenu()
{	
	if (notNull(interfaceSystem)) {
		interfaceSystem->unregisterEvent("playButtonClick");
		interfaceSystem->unregisterEvent("optionsButtonClick");
		interfaceSystem->unregisterEvent("exitButtonClick");

		interfaceSystem->unregisterEvent("controlsButtonClick");
		interfaceSystem->unregisterEvent("creditsButtonClick");
	}
}

void MainMenu::start()
{
	Menu::start();
	initMusic();
}