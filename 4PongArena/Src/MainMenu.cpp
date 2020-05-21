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
	SceneManager::GetInstance()->changeScene("ConfigurationMenu");
	buttonClick(buttonSound);
	return false;
}

bool MainMenu::optionsButtonClick()
{
	SceneManager::GetInstance()->changeScene("OptionsMenu");
	buttonClick(buttonSound);
	return false;
}

bool MainMenu::exitButtonClick()
{
	WindowManager::GetInstance()->closeWindow();
	buttonClick(backSound);
	return false;
}

bool MainMenu::controlsButtonClick()
{
	SceneManager::GetInstance()->changeScene("ControlsMenu");
	buttonClick(buttonSound);
	return false;
}

bool MainMenu::creditsButtonClick()
{
	SceneManager::GetInstance()->changeScene("Credits");
	buttonClick(buttonSound);
	return false;
}

void MainMenu::initMusic()
{
	if (!GameManager::GetInstance()->isMenuMusicPlaying())
	{
		GameManager::GetInstance()->setMenuMusic(true);
		GameManager::GetInstance()->playMusic("Menu_loop");
		GameManager::GetInstance()->setMusicVolume(0.4);
	}
}

MainMenu::MainMenu(GameObject* gameObject) : Menu(gameObject), inputSystem(nullptr)
{
	InterfaceSystem* interfaceSystem = InterfaceSystem::GetInstance();

	if (interfaceSystem != nullptr) {
		interfaceSystem->registerEvent("playButtonClick", UIEvent("ButtonClicked", [this]() {return playButtonClick(); }));
		interfaceSystem->registerEvent("optionsButtonClick", UIEvent("ButtonClicked", [this]() {return optionsButtonClick(); }));
		interfaceSystem->registerEvent("exitButtonClick", UIEvent("ButtonClicked", [this]() {return exitButtonClick(); }));

		interfaceSystem->registerEvent("controlsButtonClick", UIEvent("ButtonClicked", [this]() {return controlsButtonClick(); }));
		interfaceSystem->registerEvent("creditsButtonClick", UIEvent("ButtonClicked", [this]() {return creditsButtonClick(); }));
	}
}

MainMenu::~MainMenu()
{
	InterfaceSystem* interfaceSystem = InterfaceSystem::GetInstance();
	
	if (interfaceSystem != nullptr) {
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