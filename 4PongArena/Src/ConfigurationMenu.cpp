#include "ConfigurationMenu.h"
#include <InterfaceSystem.h>
#include <InputSystem.h>
#include <SceneManager.h>
#include <GameObject.h>
#include <UILayout.h>

#include "GameManager.h"
#include "Score.h"
#include <ComponentRegister.h>
#include <MathUtils.h>

REGISTER_FACTORY(ConfigurationMenu);

void ConfigurationMenu::checkInput()
{
	for (int i = 0; i < 5; i++)
	{
		int slotIndex = isSlotFilled(i);

		bool enterButton = (i < 4 && inputSystem->getButtonPress(i, "A")) || (i == 4 && inputSystem->getKeyPress("Space"));
		bool exitButton = (i < 4 && (inputSystem->getButtonPress(i, "B") || !inputSystem->isControllerConnected(i))) || (i == 4 && inputSystem->getKeyPress("ESCAPE"));

		if (filledSlots < 4 && slotIndex == -1 && enterButton)
			fillSlot(filledSlots, i);
		else if (slotIndex != -1 && exitButton)
		{
			clearSlot(slotIndex);
			reorderSlots(slotIndex);
		}
	}
}

void ConfigurationMenu::fillSlot(int slotIndex, int deviceIndex)
{
	slots[slotIndex].first = deviceIndex;

	slots[slotIndex].second.setVisible(true);
	slots[slotIndex].second.getChild("PlayerText").setText("P" + std::to_string(slotIndex + 1));
	slots[slotIndex].second.getChild("IndexText").setText("Index: " + std::to_string(deviceIndex));

	if (deviceIndex == 4)
		slots[slotIndex].second.getChild("TypeText").setText("Keyboard");
	else
		slots[slotIndex].second.getChild("TypeText").setText("Controller");

	filledSlots++;
}

int ConfigurationMenu::isSlotFilled(int index)
{
	int i = 0;
	while (i < filledSlots && slots[i].first != index)
		i++;

	if (slots[i].first == index)
		return i;

	return -1;
}

void ConfigurationMenu::clearSlot(int index)
{
	slots[index].first = -1;
	slots[index].second.setVisible(false);

	filledSlots--;
}

void ConfigurationMenu::reorderSlots(int index)
{
	for (int i = index; i < filledSlots; i++)
	{
		fillSlot(i, slots[i + 1].first);
		clearSlot(i + 1);
	}
}

bool ConfigurationMenu::changeFiller(bool value)
{
	IA = value;
	GameManager::GetInstance()->setIA(value);

	return false;
}

bool ConfigurationMenu::changeHealth(int value)
{
	health += value;

	if (health < MIN_HEALTH) health = MIN_HEALTH;
	if (health > MAX_HEALTH) health = MAX_HEALTH;

	configurationLayout->getRoot().getChild("Health").setText(std::to_string(health));

	return false;
}

bool ConfigurationMenu::changeTime(int value)
{
	time += value;

	if (time < MIN_TIME) time = MIN_TIME;
	if (time > MAX_TIME) time = MAX_TIME;

	configurationLayout->getRoot().getChild("Time").setText(std::to_string(time));

	return false;
}

bool ConfigurationMenu::changeSong(int value)
{
	songIndex += value;

	if (songIndex < 0) songIndex = 0;
	if (songIndex > songNames.size() - 1) songIndex = songNames.size() - 1;

	configurationLayout->getRoot().getChild("Song").setText(songNames[songIndex]);

	return false;
}

bool ConfigurationMenu::changeLevelBase(int value)
{
	levelBaseType += value;

	if (levelBaseType < 0) levelBaseType = 0;
	if (levelBaseType > BASE_TYPES) levelBaseType = BASE_TYPES;

	configurationLayout->getRoot().getChild("BaseImage").setVisible(true);
	configurationLayout->getRoot().getChild("BaseImage").setProperty("Image", "base" + std::to_string(levelBaseType + 1));

	return false;
}

bool ConfigurationMenu::changeLevelObstacles(int value)
{
	levelObstaclesType += value;

	if (levelObstaclesType < 0) levelObstaclesType = 0;
	if (levelObstaclesType > OBSTACLES_TYPES) levelObstaclesType = OBSTACLES_TYPES;

	if (levelObstaclesType == 0)
		configurationLayout->getRoot().getChild("ObstaclesImage").setVisible(false);
	else
	{
		configurationLayout->getRoot().getChild("ObstaclesImage").setVisible(true);
		configurationLayout->getRoot().getChild("ObstaclesImage").setProperty("Image", "obstacles" + std::to_string(levelObstaclesType));
	}

	return false;
}

bool ConfigurationMenu::changeLevelForces(int value)
{
	levelForcesType += value;

	if (levelForcesType < 0) levelForcesType = 0;
	if (levelForcesType > FORCES_TYPES) levelForcesType = FORCES_TYPES;

	if (levelForcesType == 0)
		configurationLayout->getRoot().getChild("ForcesImage").setVisible(false);
	else
	{
		configurationLayout->getRoot().getChild("ForcesImage").setVisible(true);
		configurationLayout->getRoot().getChild("ForcesImage").setProperty("Image", "forces" + std::to_string(levelForcesType));
	}

	return false;
}

bool ConfigurationMenu::randomizeLevel()
{
	levelBaseType = random(0, BASE_TYPES + 1);
	levelObstaclesType = random(0, OBSTACLES_TYPES + 1);
	levelForcesType = random(0, FORCES_TYPES + 1);

	changeLevelBase(0);
	changeLevelObstacles(0);
	changeLevelForces(0);

	return false;
}

bool ConfigurationMenu::startButtonClick()
{
	GameManager* gameManager = GameManager::GetInstance();

	std::vector<Player> players;
	for (int i = 0; i < filledSlots; i++)
	{
		Player aux;
		aux.id = i + 1;
		aux.index = slots[i].first;

		players.push_back(aux);
	}

	gameManager->setPlayers(players);
	gameManager->setHealth(health);
	gameManager->setInitialTime(time);

	gameManager->setLevelBase(levelBaseType);
	gameManager->setLevelObstacles(levelObstaclesType);
	gameManager->setLevelForces(levelForcesType);

	gameManager->setSong(songNames[songIndex]);


	if (!IA && filledSlots > 1 || IA)
		SceneManager::GetInstance()->changeScene("Game");

	return false;
}

bool ConfigurationMenu::backButtonClick()
{
	SceneManager::GetInstance()->changeScene("MainMenu");
	return false;
}

ConfigurationMenu::ConfigurationMenu(GameObject* gameObject) : UserComponent(gameObject), inputSystem(nullptr), configurationLayout(nullptr), startButton(NULL)
{
	InterfaceSystem* interfaceSystem = InterfaceSystem::GetInstance();

	interfaceSystem->registerEvent("checkBoxClick", UIEvent("ToggleClicked", [this]() {return changeFiller(!IA); }));

	interfaceSystem->registerEvent("-healthButtonClick", UIEvent("ButtonClicked", [this]() {return changeHealth(-1); }));
	interfaceSystem->registerEvent("+healthButtonClick", UIEvent("ButtonClicked", [this]() {return changeHealth(+1); }));

	interfaceSystem->registerEvent("-timeButtonClick", UIEvent("ButtonClicked", [this]() {return changeTime(-10); }));
	interfaceSystem->registerEvent("+timeButtonClick", UIEvent("ButtonClicked", [this]() {return changeTime(+10); }));

	interfaceSystem->registerEvent("-songButtonClick", UIEvent("ButtonClicked", [this]() {return changeSong(-1); }));
	interfaceSystem->registerEvent("+songButtonClick", UIEvent("ButtonClicked", [this]() {return changeSong(+1); }));

	interfaceSystem->registerEvent("-levelBaseButtonClick", UIEvent("ButtonClicked", [this]() {return changeLevelBase(-1); }));
	interfaceSystem->registerEvent("+levelBaseButtonClick", UIEvent("ButtonClicked", [this]() {return changeLevelBase(+1); }));
	interfaceSystem->registerEvent("-levelObstaclesButtonClick", UIEvent("ButtonClicked", [this]() {return changeLevelObstacles(-1); }));
	interfaceSystem->registerEvent("+levelObstaclesButtonClick", UIEvent("ButtonClicked", [this]() {return changeLevelObstacles(+1); }));
	interfaceSystem->registerEvent("-levelForcesButtonClick", UIEvent("ButtonClicked", [this]() {return changeLevelForces(-1); }));
	interfaceSystem->registerEvent("+levelForcesButtonClick", UIEvent("ButtonClicked", [this]() {return changeLevelForces(+1); }));

	interfaceSystem->registerEvent("randomizeButtonClick", UIEvent("ButtonClicked", [this]() {return randomizeLevel(); }));

	interfaceSystem->registerEvent("startButtonClick", UIEvent("ButtonClicked", [this]() {return startButtonClick(); }));
	interfaceSystem->registerEvent("backButtonClick", UIEvent("ButtonClicked", [this]() {return backButtonClick(); }));
}

ConfigurationMenu::~ConfigurationMenu()
{
	InterfaceSystem* interfaceSystem = InterfaceSystem::GetInstance();

	interfaceSystem->unregisterEvent("checkBoxClick");

	interfaceSystem->unregisterEvent("-healthButtonClick");
	interfaceSystem->unregisterEvent("+healthButtonClick");

	interfaceSystem->unregisterEvent("-timeButtonClick");
	interfaceSystem->unregisterEvent("+timeButtonClick");

	interfaceSystem->unregisterEvent("-songButtonClick");
	interfaceSystem->unregisterEvent("+songButtonClick");

	interfaceSystem->unregisterEvent("-levelBaseButtonClick");
	interfaceSystem->unregisterEvent("+levelBaseButtonClick");
	interfaceSystem->unregisterEvent("-levelObstaclesButtonClick");
	interfaceSystem->unregisterEvent("+levelObstaclesButtonClick");
	interfaceSystem->unregisterEvent("-levelForcesButtonClick");
	interfaceSystem->unregisterEvent("+levelForcesButtonClick");

	interfaceSystem->unregisterEvent("randomizeButtonClick");


	interfaceSystem->unregisterEvent("startButtonClick");
	interfaceSystem->unregisterEvent("backButtonClick");
}

void ConfigurationMenu::start()
{
	inputSystem = InputSystem::GetInstance();

	GameObject* mainCamera = findGameObjectWithName("MainCamera");

	if (mainCamera != nullptr)
		configurationLayout = mainCamera->getComponent<UILayout>();
	if (configurationLayout != nullptr)
		startButton = configurationLayout->getRoot().getChild("StartButton");

	IA = false;

	filledSlots = 0;
	health = 5;
	time = 60;

	//levelNames = std::vector<std::string>(4, "level"); // Placeholder
	//levelIndex = 0;

	levelBaseType = 0;
	levelObstaclesType = 0;
	levelForcesType = 0;

	songNames = std::vector<std::string>(4, "song"); // Placeholder
	songIndex = 0;

	slots = std::vector<std::pair<int, UIElement>>(4, { -1, NULL });
	for (int i = 0; i < 4; i++)
	{
		if (configurationLayout != nullptr)
			slots[i] = { -1 , configurationLayout->getRoot().getChild("Slot" + std::to_string(i + 1)).getChild("Connected" + std::to_string(i + 1)) };
	}
}

void ConfigurationMenu::update(float deltaTime)
{
	checkInput();
}