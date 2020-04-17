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
	std::vector<std::pair<Vector3, Vector3>> playerTransforms;

	float gameTimer;
	float finishTimer; // Quitar cuando haya menu de final de partida

	int winner;

	void createLevel();
	void createPlayers();

	void playSong();
	void chooseWinner();

public:
	Game(GameObject* gameObject);
	virtual ~Game();

	virtual void start();
	virtual void update(float deltaTime);
};

#endif