#include "BallGenerator.h"

#include "GameObject.h"

#include "RigidBody.h"

#include "ComponentRegister.h"

REGISTER_FACTORY(BallGenerator);

BallGenerator::BallGenerator(GameObject* gameObject) : UserComponent(gameObject)
{

}

BallGenerator::~BallGenerator()
{

}

void BallGenerator::start()
{
	lastGen = -1;
	balls = 0;
	players = 4;

	initTime = 6.0f;
	minTime = 1.0f;
	timeGen = 6.0f;


	time = 0.0f;

	vel = 3.0f;
}


void BallGenerator::update(float deltaTime)
{
	time += deltaTime;

	if (balls == 0 || time >= timeGen) generateBall();
	
}

void BallGenerator::playerDie()
{
	players--;

	timeGen /= 2.0f;

	if (timeGen < minTime) timeGen = minTime;
}

void BallGenerator::gol()
{
	balls--;
}

void BallGenerator::generateBall()
{
	int i = lastGen;

	while (i == lastGen) {
	i = rand() % 4;
	
	}


	float rand1 = rand() % 200;
	rand1 = (rand1 - 100.0f) / 100;

	float rand2 = rand() % 200;
	rand2 = (rand2 - 100.0f) / 100;

	GameObject* ball = instantiate("Ball", positions[i]);
	ball->getComponent<RigidBody>()->setLinearVelocity(Vector3(dirs[i].x + rand1,0.0, dirs[i].z + rand2).normalized() * vel);

	balls++;
	time = 0.0f;
	lastGen = i;
}



