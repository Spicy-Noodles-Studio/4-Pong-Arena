
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
	//manager->setLevel(manager->getLastLevel());
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

ScoreMenu::ScoreMenu(GameObject* gameObject) : UserComponent(gameObject)
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
	texts.clear();
	panels.clear();
}

void ScoreMenu::start()
{
	UIElement root = findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot();
	for (int i = 0; i < 4; i++)
	{
		std::string name = "P" + std::to_string(i+1);
		texts.push_back(root.getChild(name));
		name = name + "Background";
		panels.push_back(root.getChild(name));
	}
	
	manager = GameManager::GetInstance();

	reposition(manager->getTotalPlayers());
	initStatistics(manager->getTotalPlayers());

}

void ScoreMenu::initStatistics(int numOfPlayers)
{
	for (int i = 0; i < numOfPlayers; i++)
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

	for (int i = 0; i < numOfPlayers; i++)
	{
		float textPos = iTextPos + size * i;
		float panelPos = iPanelPos + size * i;
		
		texts.at(i).setVisible(true);
		texts.at(i).setPosition(textPos, 0.1);
		panels.at(i).setVisible(true);
		panels.at(i).setPosition(panelPos, 0.2);
		panels.at(i).setSize(size, 0.4);

	}
}

void ScoreMenu::setNumOfHits(int playerIndex)
{
	Score* score = manager->getScore();
	std::string name = "P" + std::to_string(playerIndex);
	name = name + "NumOfHits";
	panels.at(playerIndex-1).getChild(name).setText("Balls hit: " + std::to_string(score->getNumOfBallsHit(playerIndex)));

}

void ScoreMenu::setNumOfGoals(int playerIndex)
{
	Score* score = manager->getScore();
	std::string name = "P" + std::to_string(playerIndex);
	name = name + "NumOfGoals";
	panels.at(playerIndex-1).getChild(name).setText("Goals: " + std::to_string(score->getNumOfGoals(playerIndex)));
}

void ScoreMenu::setNumOfSelfGoals(int playerIndex)
{
	Score* score = manager->getScore();
	std::string name = "P" + std::to_string(playerIndex);
	name = name + "NumOfSelfGoals";
	panels.at(playerIndex-1).getChild(name).setText("Own goals: " + std::to_string(score->getNumOfSelfGoals(playerIndex)));
}

void ScoreMenu::setTimeAlive(int playerIndex)
{
	Score* score = manager->getScore();
	std::string name = "P" + std::to_string(playerIndex);
	name = name + "TimeAlive";
	panels.at(playerIndex-1).getChild(name).setText("Time alive: " + std::to_string(score->getTimeAlive(playerIndex)));
}

void ScoreMenu::setPositionOnLeaderBoard(int playerIndex)
{
	Score* score = manager->getScore();
	std::string name = "P" + std::to_string(playerIndex);
	name = name + "PositionLeaderBoard";
	panels.at(playerIndex-1).getChild(name).setText("Position: " + std::to_string(score->getPositionOnLeaderBoard(playerIndex)));
}


