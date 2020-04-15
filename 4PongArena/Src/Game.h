#pragma once
#ifndef GAME_H
#define GAME_H

#include <UserComponent.h>
#include <UIElement.h>

class GameManager;
class UILayout;
class InputSystem;

class Game : public UserComponent
{
private:
	GameManager* gameManager;
	//UILayout* gameLayout;

	//UIElement timeText;
	//UIElement winnerPanel;
	//UIElement winnerText;

	std::vector<GameObject*> paddles;

	int numPlayers;
	std::vector<std::pair<Vector3, Vector3>> playerTransforms;

	float gameTimer; // If time is -1, then infinite
	float finishTimer; // Time taken to send us back to MainMenu

	int winner;
	bool ended;

	void createLevel();
	void createPlayers();

	void playSong();

	void chooseWinner();

public:
	Game(GameObject* gameObject);
	virtual ~Game();

	virtual void start();
	virtual void update(float deltaTime);

	void updatePlayers();
};

#endif