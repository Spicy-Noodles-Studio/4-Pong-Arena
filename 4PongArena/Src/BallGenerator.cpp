#include "BallGenerator.h"
#include <GameObject.h>
#include <RigidBody.h>

#include "GameManager.h"

#include <ComponentRegister.h>

REGISTER_FACTORY(BallGenerator);

BallGenerator::BallGenerator(GameObject* gameObject) : UserComponent(gameObject), time(0.0f), generationTime(6.0f), minimumTime(1.0f), velocity(3.0f)
{

}

BallGenerator::~BallGenerator()
{

}

void BallGenerator::start()
{
	time = generationTime;
}

void BallGenerator::update(float deltaTime)
{
	if (time > 0)
		time -= deltaTime;
	else
	{
		generateBall();
		time = generationTime;
	}
}

void BallGenerator::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "generationTime")
		{
			if (!(ss >> generationTime))
				LOG("BALL GENERATOR: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "minimumTime")
		{
			if (!(ss >> minimumTime))
				LOG("BALL GENERATOR: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else if (prop.first == "velocity")
		{
			if (!(ss >> velocity))
				LOG("BALL GENERATOR: Invalid property with name \"%s\"", prop.first.c_str());
		}
		else
			LOG("BALL GENERATOR: Invalid property name \"%s\"", prop.first.c_str());
	}
}

void BallGenerator::generateBall()
{
	int i = 0;
	bool found = false;
	GameObject* ball = nullptr;
	std::vector<GameObject*> balls = GameManager::GetInstance()->getBalls();

	/*while (i < balls.size() && !found)
	{
		if (!balls[i]->isActive())
		{
			found = true;
			ball = balls[i];
			ball->setActive(true);
		}
		else
			i++;
	}

	if (i > balls.size())
	{
		ball = instantiate("Ball", gameObject->transform->getPosition());
		balls.push_back(ball);
	}*/

	ball = instantiate("Ball", gameObject->transform->getPosition());
	balls.push_back(ball);

	GameManager::GetInstance()->setBalls(balls);

	if (ball != nullptr)
		ball->getComponent<RigidBody>()->setLinearVelocity(Vector3(-gameObject->transform->getPosition().x, 0, -gameObject->transform->getPosition().z).normalized() * velocity);
}

void BallGenerator::setGenerationTime(float generationTime)
{
	this->generationTime = generationTime;
}

float BallGenerator::getGenerationTime() const
{
	return generationTime;
}

float BallGenerator::getMinimumTime() const
{
	return minimumTime;
}