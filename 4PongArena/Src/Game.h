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

	std::vector<GameObject*> paddles;
	std::vector<Vector3> playerColours;

	std::vector<std::pair<Vector3, Vector3>> playerTransforms;
	std::vector<std::pair<Vector3, Vector3>> spawnerTransforms;
	std::vector<std::pair<Vector3, Vector3>> forceFieldTransforms;
	std::vector<std::pair<Vector3, Vector3>> obstacleTransforms;

	float gameTimer;

	int levelBase;
	int levelForces;
	int levelObstacles;

	void createLevel();
	void createPlayers();
	void createSpawners();
	void createForceField();
	void createObstacles();

	void configureLevelRender(const std::string& name);
	void configureLevelCollider(const std::string& name);

	void playSong();
	void chooseWinner();

public:
	Game(GameObject* gameObject);
	virtual ~Game();

	virtual void start();
	virtual void update(float deltaTime);
};

#endif