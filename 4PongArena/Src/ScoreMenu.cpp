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

ScoreMenu::ScoreMenu(GameObject* gameObject) : Menu(gameObject), gameManager(nullptr), texts(), panels()
{

}

ScoreMenu::~ScoreMenu()
{

}

void ScoreMenu::start()
{
	Menu::start();
	
	gameManager = GameManager::GetInstance();

	GameObject* camera = findGameObjectWithName("MainCamera");
	UILayout* layout = nullptr;
	UIElement root = NULL;
	Score* score=nullptr;
	if (camera != nullptr)
	{
		layout = camera->getComponent<UILayout>();

		if (layout != nullptr)
			root = layout->getRoot();
	}
	if (gameManager != nullptr) {
		score = gameManager->getScore();
	}
	int lastUsed = -1;
	for (int i = 0; i < 4; i++)
	{
		if (score != nullptr) {
			std::string name;
			int ranking;
			if (score->getPlayerId(i) != -1) {
				name = "P" + std::to_string(score->getPlayerId(i));
				ranking = score->getPlayerId(i);
			}
			else {
				name = "P" + std::to_string(i + 1);
				ranking = i + 1;
			}
			

			texts.push_back(root.getChild(name));
			if (gameManager != nullptr && lastUsed != ranking)
			{
				root.getChild(name).setText(name + ": " + std::to_string(gameManager->getPlayerRanking(i+1)));
				lastUsed = ranking;
			}

			name = name + "Background";
			panels.push_back(root.getChild(name));
		}
	}


	if (gameManager != nullptr && score!=nullptr && gameManager->getWinner() != -1)
		root.getChild("Result").setText("WINNER: PLAYER " + std::to_string(score->getPlayerId(gameManager->getWinner()-1)));
	else
		root.getChild("Result").setText("TIE");

	if (gameManager != nullptr) {
		reposition(gameManager->getInitialPlayers());
		initStatistics(gameManager->getInitialPlayers());
	}
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

	if (gameManager == nullptr) return;

	Score* score = gameManager->getScore();

	float size = (1 - 0.2) / numOfPlayers;
	float iTextPos = 0.15;
	float iPanelPos = 0.1;

	for (int i = 0; i < numOfPlayers; i++)
	{
		float textPos = iTextPos + size * i;
		float panelPos = iPanelPos + size * i;


		int id=score->getPlayerId(i);
		texts.at(id-1).setVisible(true);
		texts.at(id - 1).setPosition(textPos, 0.2);

		panels.at(id - 1).setVisible(true);
		panels.at(id - 1).setPosition(panelPos, 0.3);
		panels.at(id - 1).setSize(size, 0.4);
	}
}

void ScoreMenu::initStatistics(int numOfPlayers)
{
	for (int i = 0; i < numOfPlayers; i++)
	{
		setNumOfHits(i );

		setNumOfGoals(i );
		setNumOfSelfGoals(i );

		setTimeAlive(i);
	}
}

void ScoreMenu::setNumOfHits(int playerIndex)
{
	if (gameManager == nullptr) return;

	Score* score = gameManager->getScore();
	std::string name = "P" + std::to_string(score->getPlayerId(playerIndex));
	name = name + "NumOfHits";

	if (playerIndex >= 0)
		panels.at(score->getPlayerId(playerIndex)-1).getChild(name).setText("Balls hit: " + std::to_string(score->getNumOfBallsHit(playerIndex)));
}

void ScoreMenu::setNumOfGoals(int playerIndex)
{
	if (gameManager == nullptr) return;

	Score* score = gameManager->getScore();
	std::string name = "P" + std::to_string(score->getPlayerId(playerIndex));
	name = name + "NumOfGoals";

	if (playerIndex >= 0)
		panels.at(score->getPlayerId(playerIndex) - 1).getChild(name).setText("Goals: " + std::to_string(score->getNumOfGoals(playerIndex)));
}

void ScoreMenu::setNumOfSelfGoals(int playerIndex)
{
	if (gameManager == nullptr) return;

	Score* score = gameManager->getScore();
	std::string name = "P" + std::to_string(score->getPlayerId(playerIndex));
	name = name + "NumOfSelfGoals";

	if (playerIndex >= 0)
		panels.at(score->getPlayerId(playerIndex) - 1).getChild(name).setText("Own goals: " + std::to_string(score->getNumOfSelfGoals(playerIndex)));
}

void ScoreMenu::setTimeAlive(int playerIndex)
{
	if (gameManager == nullptr) return;

	Score* score = gameManager->getScore();
	std::string name = "P" + std::to_string(score->getPlayerId(playerIndex));
	name = name + "TimeAlive";

	if (playerIndex >= 0)
		panels.at(score->getPlayerId(playerIndex) - 1).getChild(name).setText("Time alive: " + std::to_string(score->getTimeAlive(playerIndex)));
}