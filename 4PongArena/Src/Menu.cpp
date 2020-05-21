#include "Menu.h"
#include <ComponentRegister.h>
#include <InputSystem.h>
#include <InterfaceSystem.h>
#include <SoundEmitter.h>
#include <SceneManager.h>
#include <GameObject.h>

Menu::Menu(GameObject* gameObject) : UserComponent(gameObject), inputSystem(nullptr), interfaceSystem(nullptr), soundEmitter(nullptr), mainCamera(nullptr)
{

}

Menu::~Menu()
{

}

void Menu::start()
{
	mainCamera = findGameObjectWithName("MainCamera");
	if (mainCamera != nullptr) 
		soundEmitter = mainCamera->getComponent<SoundEmitter>();

	inputSystem = InputSystem::GetInstance();
	interfaceSystem = InterfaceSystem::GetInstance();
}

void Menu::buttonClick(const std::string& sound)
{
	if (soundEmitter != nullptr) soundEmitter->playSound(sound);
}

bool Menu::backButtonClick()
{
	SceneManager::GetInstance()->changeScene("MainMenu");
	buttonClick(backSound);
	return false;
}