#pragma once
#ifndef GAME_H
#define GAME_H

#include <UserComponent.h>
#include <UIElement.h>

const int MAX_PLAYERS = 4;

class GameManager;
class SoundEmitter;
class UILayout;
class Countdown;
class CameraEffects;

class Game : public UserComponent
{
private:
	GameManager* gameManager;
	SoundEmitter* soundEmitter;

	UILayout* gameLayout;
	UIElement timePanel;

	Countdown* countdown;
	bool fadeIn;
	bool darkness;
	bool end;

	CameraEffects* cameraEffects;

	std::vector<GameObject*> paddles;
	std::vector<Vector3> playerColours;
	std::vector<std::vector<Vector3>> levelColours;

	std::pair<Vector3, Vector3> baseColour;
	std::pair<Vector3, Vector3> neonColour;

	std::vector<std::pair<Vector3, Vector3>> playerTransforms;
	std::vector<std::pair<Vector3, Vector3>> spawnerTransforms;
	std::vector<std::pair<Vector3, Vector3>> forceFieldTransforms;
	std::vector<std::pair<Vector3, Vector3>> obstacleTransforms;

	Vector3 wallScale;

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
	void endgameHandleSound();

	std::pair<std::string, std::string> timeToText();

public:
	Game(GameObject* gameObject);
	virtual ~Game();

protected:
	virtual void start();
	virtual void update(float deltaTime);
};

#endif