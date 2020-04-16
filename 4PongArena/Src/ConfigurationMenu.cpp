#include "ConfigurationMenu.h"
#include <InterfaceSystem.h>
#include <InputSystem.h>
#include <SceneManager.h>
#include <GameObject.h>
#include <UILayout.h>

#include "GameManager.h"

#include <ComponentRegister.h>

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

	if (!startButton.isVisible() && filledSlots >= MIN_PLAYERS)
		startButton.setVisible(true);
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

	if (startButton.isVisible() && filledSlots < MIN_PLAYERS)
		startButton.setVisible(false);
}

void ConfigurationMenu::reorderSlots(int index)
{
	for (int i = index; i < filledSlots; i++)
	{
		fillSlot(i, slots[i + 1].first);
		clearSlot(i + 1);
	}
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

bool ConfigurationMenu::changeLevel(int value)
{
	levelIndex += value;

	if (levelIndex < 0) levelIndex = 0;
	if (levelIndex > levelNames.size() - 1) levelIndex = levelNames.size() - 1;

	configurationLayout->getRoot().getChild("Level").setText(levelNames[levelIndex]);

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
	gameManager->setTime(time);

	gameManager->setLevel(levelNames[levelIndex]);
	gameManager->setSong(songNames[songIndex]);

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

	interfaceSystem->registerEvent("-healthButtonClick", UIEvent("ButtonClicked", [this]() {return changeHealth(-1); }));
	interfaceSystem->registerEvent("+healthButtonClick", UIEvent("ButtonClicked", [this]() {return changeHealth(+1); }));

	interfaceSystem->registerEvent("-timeButtonClick", UIEvent("ButtonClicked", [this]() {return changeTime(-10); }));
	interfaceSystem->registerEvent("+timeButtonClick", UIEvent("ButtonClicked", [this]() {return changeTime(+10); }));

	interfaceSystem->registerEvent("-songButtonClick", UIEvent("ButtonClicked", [this]() {return changeSong(-1); }));
	interfaceSystem->registerEvent("+songButtonClick", UIEvent("ButtonClicked", [this]() {return changeSong(+1); }));

	interfaceSystem->registerEvent("-levelButtonClick", UIEvent("ButtonClicked", [this]() {return changeLevel(-1); }));
	interfaceSystem->registerEvent("+levelButtonClick", UIEvent("ButtonClicked", [this]() {return changeLevel(+1); }));

	interfaceSystem->registerEvent("startButtonClick", UIEvent("ButtonClicked", [this]() {return startButtonClick(); }));
	interfaceSystem->registerEvent("backButtonClick", UIEvent("ButtonClicked", [this]() {return backButtonClick(); }));
}

ConfigurationMenu::~ConfigurationMenu()
{
	InterfaceSystem* interfaceSystem = InterfaceSystem::GetInstance();

	interfaceSystem->unregisterEvent("-healthButtonClick");
	interfaceSystem->unregisterEvent("+healthButtonClick");

	interfaceSystem->unregisterEvent("-timeButtonClick");
	interfaceSystem->unregisterEvent("+timeButtonClick");

	interfaceSystem->unregisterEvent("-songButtonClick");
	interfaceSystem->unregisterEvent("+songButtonClick");

	interfaceSystem->unregisterEvent("-levelButtonClick");
	interfaceSystem->unregisterEvent("+levelButtonClick");

	interfaceSystem->unregisterEvent("startButtonClick");
}

void ConfigurationMenu::start()
{
	inputSystem = InputSystem::GetInstance();

	GameObject* mainCamera = findGameObjectWithName("MainCamera");

	if (mainCamera != nullptr)
		configurationLayout = mainCamera->getComponent<UILayout>();
	if (configurationLayout != nullptr)
		startButton = configurationLayout->getRoot().getChild("StartButton");

	filledSlots = 0;
	health = 5;
	time = 60;

	levelNames = std::vector<std::string>(4, "level"); // Placeholder
	levelIndex = 0;

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