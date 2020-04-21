#pragma once
#ifndef GAME_H
#define GAME_H

#include <UserComponent.h>
#include <UIElement.h>

const int MAX_PLAYERS = 4;

class GameManager;
class UILayout;
class InputSystem;

class Game : public UserComponent
{
private:
	GameManager* gameManager;

	UILayout* gameLayout;
	UIElement timeText;
	UIElement winnerPanel;
	UIElement winnerText;

	std::vector<GameObject*> paddles;
	std::vector<std::pair<Vector3, Vector3>> playerTransforms;
	std::vector<std::pair<Vector3, Vector3>> spawnerTransforms;
	std::vector<std::pair<Vector3, Vector3>> forceFieldTransforms;

	Vector3 wallScale;

	float gameTimer;
	float finishTimer; // Quitar cuando haya menu de final de partida

	int winner;
	bool win;

	void createLevel();
	void createPlayers();
	void createSpawners();
	void createForceField();

	void playSong();
	void chooseWinner();

public:
	Game(GameObject* gameObject);
	virtual ~Game();

	virtual void start();
	virtual void update(float deltaTime);
};

#endif