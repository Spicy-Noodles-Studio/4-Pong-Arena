#include "Menu.h"
#include <ComponentRegister.h>
#include <InputSystem.h>
#include <InterfaceSystem.h>
#include <SoundEmitter.h>
#include <SceneManager.h>
#include <GameObject.h>

#include "GameManager.h"

Menu::Menu(GameObject* gameObject) : UserComponent(gameObject), inputSystem(nullptr), interfaceSystem(nullptr), soundEmitter(nullptr), sceneManager(nullptr), mainCamera(nullptr), gameManager(nullptr)
{
	interfaceSystem = InterfaceSystem::GetInstance();
}

Menu::~Menu()
{
	inputSystem = nullptr;
	interfaceSystem = nullptr;
	soundEmitter = nullptr;
	sceneManager = nullptr;
	mainCamera = nullptr;
	gameManager = nullptr;
}

void Menu::start()
{
	mainCamera = findGameObjectWithName("MainCamera");
	if (notNull(mainCamera))
		soundEmitter = mainCamera->getComponent<SoundEmitter>();

	inputSystem = InputSystem::GetInstance();
	sceneManager = SceneManager::GetInstance();
	gameManager = GameManager::GetInstance();
}

void Menu::buttonClick(const std::string& sound)
{
	if (notNull(soundEmitter)) soundEmitter->playSound(sound);
}

bool Menu::backButtonClick()
{
	if (notNull(sceneManager)) sceneManager->changeScene("MainMenu");
	buttonClick(backSound);
	return false;
}