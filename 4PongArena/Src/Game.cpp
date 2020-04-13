#include "Game.h"
#include <GaiaData.h>
#include <SceneManager.h>
#include <UILayout.h>
#include <GameObject.h>
#include <RigidBody.h>

#include "PlayerController.h"
#include "Health.h"
#include "ConfigurationMenu.h"
#include "GameManager.h"

#include <ComponentRegister.h>

REGISTER_FACTORY(Game);

void Game::createLevel()
{
	GaiaData levelData;
	levelData.load("./Assets/Levels/" + GameManager::GetInstance()->getLevel() + ".level");

	// player initial transforms
	GaiaData playerData = levelData.find("PlayerTransforms");
	for (int i = 0; i < playerData.size(); i++)
	{
		std::stringstream ss(playerData[i][0].getValue());
		double posX, posY, posZ;

		if (!(ss >> posX >> posY >> posZ))
		{
			LOG_ERROR("GAME", "invalid player position \"%s\"", playerData[i][0].getValue().c_str());
			continue;
		}

		ss = std::stringstream(playerData[i][1].getValue());
		double rotX, rotY, rotZ;

		if (!(ss >> rotX >> rotY >> rotZ))
		{
			LOG_ERROR("GAME", "invalid player rotation \"%s\"", playerData[i][1].getValue().c_str());
			continue;
		}

		playerTransforms.push_back({ { posX, posY, posZ }, { rotX, rotY, rotZ } });
	}
}

void Game::createPlayers()
{
	std::vector<Player> players = gameManager->getPlayers();

	int nPlayers = players.size();

	for (int i = 0; i < nPlayers; i++)
	{
		GameObject* paddle = instantiate("Paddle", playerTransforms[i].first);
		paddle->transform->setRotation(playerTransforms[i].second);

		paddle->getComponent<PlayerController>()->setPlayer(players[i].id, players[i].index);
		paddle->getComponent<Health>()->setHealth(gameManager->getHealth());

		paddles.push_back(paddle);
	}
}

void Game::playSong()
{
	//findGameObjectWithName("MainCamera")->getComponent<SoundEmitter>()->play(GameManager::GetInstance()->getSong());
}

void Game::chooseWinner()
{
	gameTimer = 0.0f;

	bool tie = false;
	int majorHealth = 0;
	int majorIndex = 0;

	for (int i = 0; i < paddles.size(); i++)
	{
		Health* health = paddles[i]->getComponent<Health>();
		if (health != nullptr && health->isAlive())
		{
			if (health->getHealth() > majorHealth)
			{
				majorHealth = health->getHealth();
				majorIndex = i;
			}
			else if (health->getHealth() == majorHealth)
				tie = true;
		}
	}

	/*if (gameLayout != nullptr)
	{
		winnerPanel.setVisible(true);

		if (tie)
			winnerText.setText("TIE");
		else
		{
			winner = majorIndex;
			winnerText.setText("Winner: P" + std::to_string(winner + 1));
		}
	}*/
}

Game::Game(GameObject* gameObject) : UserComponent(gameObject), gameManager(nullptr)/*, gameLayout(nullptr), timeText(NULL), winnerPanel(NULL), winnerText(NULL)*/
{

}

Game::~Game()
{

}

void Game::start()
{
	gameManager = GameManager::GetInstance();

	GameObject* mainCamera = findGameObjectWithName("MainCamera");

	/*if (mainCamera != nullptr)
		gameLayout = mainCamera->getComponent<UILayout>();

	if (gameLayout != nullptr)
	{
		timeText = gameLayout->getRoot().getChild("Time");

		winnerPanel = gameLayout->getRoot().getChild("WinnerBackground");
		winnerPanel.setVisible(false);

		winnerText = winnerPanel.getChild("Winner");
	}*/

	numPlayers = gameManager->getPlayers().size();

	createLevel();
	createPlayers();

	gameTimer = gameManager->getTime();
	finishTimer = 4.0f; // Placeholder

	winner = 0;
	ended = false;

	playSong();
}

void Game::update(float deltaTime)
{
	if (gameTimer > 0)
	{
		gameTimer -= deltaTime;

		if (gameTimer < 0.0f)
			gameTimer = 0.0f;

		/*if (gameLayout != nullptr)
			timeText.setText(std::to_string((int)gameTimer % 60));*/
	}
	else
	{
		ended = true;
		finishTimer -= deltaTime;

		if (finishTimer <= 0.0f)
			SceneManager::GetInstance()->changeScene("ConfigurationMenu"); // Placeholder
	}

	if (ended)
		chooseWinner();
}

void Game::updatePlayers()
{
	numPlayers--;

	if (numPlayers == 1)
		ended = true;
}