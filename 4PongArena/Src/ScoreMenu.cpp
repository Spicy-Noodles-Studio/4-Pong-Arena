
#include "ScoreMenu.h"


#include <ComponentRegister.h>
#include <SceneManager.h>
#include <GameObject.h>
#include <UILayout.h>
#include <UIElement.h>
#include <InputSystem.h>
#include <Camera.h>
#include <InterfaceSystem.h>
#include "GameManager.h"
#include "Score.h"

REGISTER_FACTORY(ScoreMenu);



bool ScoreMenu::resetButtonClick()
{
	manager->setPlayersAlive(manager->getTotalPlayers());

	manager->setTime(manager->getInitialTime());
	manager->setLevel(manager->getLastLevel());
	manager->setSong(manager->getLastSong());

	//change scene
	SceneManager::GetInstance()->changeScene("Game");
	return false;
}

bool ScoreMenu::backButtonClick()
{
	
	SceneManager::GetInstance()->changeScene("MainMenu");
	return false;
}

ScoreMenu::ScoreMenu(GameObject* gameObject) : UserComponent(gameObject), player1Text(NULL), player2Text(NULL), player3Text(NULL), player4Text(NULL),
													player1Panel(NULL), player2Panel(NULL), player3Panel(NULL), player4Panel(NULL)
{
	InterfaceSystem* interfaceSystem = InterfaceSystem::GetInstance();
	interfaceSystem->registerEvent("resetButtonClick", UIEvent("ButtonClicked", [this]() {return resetButtonClick(); }));
	interfaceSystem->registerEvent("backButtonClick", UIEvent("ButtonClicked", [this]() {return backButtonClick(); }));
}

ScoreMenu::~ScoreMenu()
{
	InterfaceSystem* interfaceSystem = InterfaceSystem::GetInstance();
	interfaceSystem->unregisterEvent("resetButtonClick");
	interfaceSystem->unregisterEvent("backButtonClick");
}

void ScoreMenu::start()
{
	
	player1Text = findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("P1");
	player1Text.setVisible(false);
	player2Text = findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("P2");
	player2Text.setVisible(false);
	player3Text = findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("P3");
	player3Text.setVisible(false);
	player4Text = findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("P4");
	player4Text.setVisible(false);


	player1Panel = findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("P1Background");
	player1Panel.setVisible(false);
	player2Panel = findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("P2Background");
	player2Panel.setVisible(false);
	player3Panel = findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("P3Background");
	player3Panel.setVisible(false);
	player4Panel = findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("P4Background");
	player4Panel.setVisible(false);
	
	manager = GameManager::GetInstance();

	reposition(manager->getTotalPlayers());
	initStatistics(manager->getTotalPlayers());

}

void ScoreMenu::initStatistics(int numOfPlayers)
{
	int paddles = numOfPlayers;

	for (int i = 0; i < paddles; i++)
	{
		setNumOfGoals(i + 1);
		setNumOfHits(i + 1);
		setTimeAlive(i + 1);
		setPositionOnLeaderBoard(i + 1);
		setNumOfSelfGoals(i + 1);
	}
}

void ScoreMenu::reposition(int numOfPlayers)
{
	float size=(1-0.2)/numOfPlayers;
	float iTextPos = 0.15;
	float iPanelPos = 0.1;

	int paddles = numOfPlayers;

	for (int i = 0; i < paddles; i++)
	{
		float textPos = iTextPos + size * i;
		float panelPos = iPanelPos + size * i;
		if (i == 0)
		{
			player1Text.setVisible(true);
			player1Text.setPosition(textPos, 0.1);
			player1Panel.setVisible(true);
			player1Panel.setPosition(panelPos, 0.2);
			player1Panel.setSize(size, 0.4);
		}
		if (i == 1)
		{
			player2Text.setVisible(true);
			player2Text.setPosition(textPos, 0.1);
			player2Panel.setVisible(true);
			player2Panel.setPosition(panelPos, 0.2);
			player2Panel.setSize(size, 0.4);
		}
		if (i == 2)
		{
			player3Text.setVisible(true);
			player3Text.setPosition(textPos, 0.1);
			player3Panel.setVisible(true);
			player3Panel.setPosition(panelPos, 0.2);
			player3Panel.setSize(size, 0.4);
		}
		if (i == 3)
		{
			player4Text.setVisible(true);
			player4Text.setPosition(textPos, 0.1);
			player4Panel.setVisible(true);
			player4Panel.setPosition(panelPos, 0.2);
			player4Panel.setSize(size, 0.4);
		}

	}
}

void ScoreMenu::setNumOfHits(int playerIndex)
{
	Score* score = manager->getScore();
	std::string name = "P" + std::to_string(playerIndex);
	name = name + "NumOfHits";
	
	if (playerIndex == 1)
		player1Panel.getChild(name).setText("Balls hit: " + std::to_string(score->getNumOfBallsHit(playerIndex)));
	else if (playerIndex == 2)
			player2Panel.getChild(name).setText("Balls hit: " + std::to_string(score->getNumOfBallsHit(playerIndex)));
	else if (playerIndex == 3)
			player3Panel.getChild(name).setText("Balls hit: " + std::to_string(score->getNumOfBallsHit(playerIndex)));
	else
			player4Panel.getChild(name).setText("Balls hit: " + std::to_string(score->getNumOfBallsHit(playerIndex)));
}

void ScoreMenu::setNumOfGoals(int playerIndex)
{
	Score* score = manager->getScore();
	std::string name = "P" + std::to_string(playerIndex);
	name = name + "NumOfGoals";

	if (playerIndex == 1)
		player1Panel.getChild(name).setText("Goals: " + std::to_string(score->getNumOfGoals(playerIndex)));
	else if (playerIndex == 2)
		player2Panel.getChild(name).setText("Goals: " + std::to_string(score->getNumOfGoals(playerIndex)));
	else if (playerIndex == 3)
		player3Panel.getChild(name).setText("Goals: " + std::to_string(score->getNumOfGoals(playerIndex)));
	else
		player4Panel.getChild(name).setText("Goals: " + std::to_string(score->getNumOfGoals(playerIndex)));
}

void ScoreMenu::setNumOfSelfGoals(int playerIndex)
{
	Score* score = manager->getScore();
	std::string name = "P" + std::to_string(playerIndex);
	name = name + "NumOfSelfGoals";

	if (playerIndex == 1)
		player1Panel.getChild(name).setText("Own goals: " + std::to_string(score->getNumOfSelfGoals(playerIndex)));
	else if (playerIndex == 2)
		player2Panel.getChild(name).setText("Own goals: " + std::to_string(score->getNumOfSelfGoals(playerIndex)));
	else if (playerIndex == 3)
		player3Panel.getChild(name).setText("Own goals: " + std::to_string(score->getNumOfSelfGoals(playerIndex)));
	else
		player4Panel.getChild(name).setText("Own goals: " + std::to_string(score->getNumOfSelfGoals(playerIndex)));
}

void ScoreMenu::setTimeAlive(int playerIndex)
{
	Score* score = manager->getScore();
	std::string name = "P" + std::to_string(playerIndex);
	name = name + "TimeAlive";

	if (playerIndex == 1)
		player1Panel.getChild(name).setText("Time alive: " + std::to_string(score->getTimeAlive(playerIndex)));
	else if (playerIndex == 2)
		player2Panel.getChild(name).setText("Time alive: " + std::to_string(score->getTimeAlive(playerIndex)));
	else if (playerIndex == 3)
		player3Panel.getChild(name).setText("Time alive: " + std::to_string(score->getTimeAlive(playerIndex)));
	else
		player4Panel.getChild(name).setText("Time alive: " + std::to_string(score->getTimeAlive(playerIndex)));
}

void ScoreMenu::setPositionOnLeaderBoard(int playerIndex)
{
	Score* score = manager->getScore();
	std::string name = "P" + std::to_string(playerIndex);
	name = name + "PositionLeaderBoard";

	if (playerIndex == 1)
		player1Panel.getChild(name).setText("Position: " + std::to_string(score->getPositionOnLeaderBoard(playerIndex)));
	else if (playerIndex == 2)
		player2Panel.getChild(name).setText("Position: " + std::to_string(score->getPositionOnLeaderBoard(playerIndex)));
	else if (playerIndex == 3)
		player3Panel.getChild(name).setText("Position: " + std::to_string(score->getPositionOnLeaderBoard(playerIndex)));
	else
		player4Panel.getChild(name).setText("Position: " + std::to_string(score->getPositionOnLeaderBoard(playerIndex)));
}


