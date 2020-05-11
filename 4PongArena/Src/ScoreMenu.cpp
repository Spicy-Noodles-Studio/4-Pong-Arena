#include "ScoreMenu.h"
#include <ComponentRegister.h>
#include <InputSystem.h>
#include <InterfaceSystem.h>
#include <SceneManager.h>
#include <GameObject.h>
#include <UILayout.h>
#include <UIElement.h>
#include <Camera.h>

#include "Score.h"
#include "GameManager.h"

REGISTER_FACTORY(ScoreMenu);

ScoreMenu::ScoreMenu(GameObject* gameObject) : UserComponent(gameObject), gameManager(nullptr), texts(), panels()
{

}

ScoreMenu::~ScoreMenu()
{

}

void ScoreMenu::start()
{
	gameManager = GameManager::GetInstance();

	GameObject* camera = findGameObjectWithName("MainCamera");
	UILayout* layout = nullptr;
	UIElement root = NULL;

	if (camera != nullptr)
	{
		layout = camera->getComponent<UILayout>();

		if (layout != nullptr)
			root = layout->getRoot();
	}

	for (int i = 0; i < 4; i++)
	{
		std::string name = "P" + std::to_string(i + 1);
		texts.push_back(root.getChild(name));
		root.getChild(name).setText(name + ": " + std::to_string(gameManager->getPlayerRanking(i + 1)) + "º");

		name = name + "Background";
		panels.push_back(root.getChild(name));
	}

	if (gameManager->getWinner() != -1)
		root.getChild("Result").setText("WINNER: PLAYER " + std::to_string(gameManager->getWinner()));
	else
		root.getChild("Result").setText("TIE");

	reposition(gameManager->getInitialPlayers());
	initStatistics(gameManager->getInitialPlayers());
}

void ScoreMenu::update(float deltaTime)
{
	if (InputSystem::GetInstance()->getKeyPress("ESCAPE") || checkControllersInput())
		SceneManager::GetInstance()->changeScene("ConfigurationMenu");
}

bool ScoreMenu::checkControllersInput()
{
	bool result = false;

	int i = 0;
	while (i < 4 && !result)
	{
		if (InputSystem::GetInstance()->getButtonPress(i, "B"))
			result = true;

		i++;
	}

	return result;
}

void ScoreMenu::reposition(int numOfPlayers)
{
	float size = (1 - 0.2) / numOfPlayers;
	float iTextPos = 0.15;
	float iPanelPos = 0.1;

	for (int i = 0; i < numOfPlayers; i++)
	{
		float textPos = iTextPos + size * i;
		float panelPos = iPanelPos + size * i;

		texts.at(i).setVisible(true);
		texts.at(i).setPosition(textPos, 0.2);

		panels.at(i).setVisible(true);
		panels.at(i).setPosition(panelPos, 0.3);
		panels.at(i).setSize(size, 0.4);
	}
}

void ScoreMenu::initStatistics(int numOfPlayers)
{
	for (int i = 0; i < numOfPlayers; i++)
	{
		setNumOfHits(i + 1);

		setNumOfGoals(i + 1);
		setNumOfSelfGoals(i + 1);

		setTimeAlive(i + 1);
	}
}

void ScoreMenu::setNumOfHits(int playerIndex)
{
	Score* score = gameManager->getScore();
	std::string name = "P" + std::to_string(playerIndex);
	name = name + "NumOfHits";

	if (playerIndex > 0)
		panels.at(playerIndex - 1).getChild(name).setText("Balls hit: " + std::to_string(score->getNumOfBallsHit(playerIndex)));
}

void ScoreMenu::setNumOfGoals(int playerIndex)
{
	Score* score = gameManager->getScore();
	std::string name = "P" + std::to_string(playerIndex);
	name = name + "NumOfGoals";

	if (playerIndex > 0)
		panels.at(playerIndex - 1).getChild(name).setText("Goals: " + std::to_string(score->getNumOfGoals(playerIndex)));
}

void ScoreMenu::setNumOfSelfGoals(int playerIndex)
{
	Score* score = gameManager->getScore();
	std::string name = "P" + std::to_string(playerIndex);
	name = name + "NumOfSelfGoals";

	if (playerIndex > 0)
		panels.at(playerIndex - 1).getChild(name).setText("Own goals: " + std::to_string(score->getNumOfSelfGoals(playerIndex)));
}

void ScoreMenu::setTimeAlive(int playerIndex)
{
	Score* score = gameManager->getScore();
	std::string name = "P" + std::to_string(playerIndex);
	name = name + "TimeAlive";

	if (playerIndex > 0)
		panels.at(playerIndex - 1).getChild(name).setText("Time alive: " + std::to_string(score->getTimeAlive(playerIndex)));
}