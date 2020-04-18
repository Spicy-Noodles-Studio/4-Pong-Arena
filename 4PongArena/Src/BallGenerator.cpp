#include "BallGenerator.h"

#include "GameObject.h"

#include "RigidBody.h"

#include "ComponentRegister.h"

#include "Ball.h"

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

	initTime = 5.0f;
	minTime = 2.0f;
	timeGen = 5.0f;


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


	float rand1 = rand() % 50;
	rand1 = (rand1 - 25.0f) / 25;

	float rand2 = rand() % 50;
	rand2 = (rand2 - 25.0f) / 25;

	GameObject* ball = instantiate("Ball", positions[i]);
	ball->getComponent<RigidBody>()->setLinearVelocity(Vector3(dirs[i].x + rand1,0.0, dirs[i].z + rand2).normalized() * vel);
	//Vector3 auxv =  ball->getComponent<RigidBody>()->getLinearVelocity();
	balls++;
	time = 0.0f;
	lastGen = i;
}



