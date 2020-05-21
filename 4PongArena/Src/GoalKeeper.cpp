#include "GoalKeeper.h"
#include <GameObject.h>
#include <sstream>

#include "Goal.h"
#include "Ball.h"
#include "GameManager.h"
#include "Score.h"
#include "PlayerIndex.h"
#include "Health.h"
#include <ComponentRegister.h>

REGISTER_FACTORY(GoalKeeper);

GoalKeeper::GoalKeeper(GameObject* gameObject) : UserComponent(gameObject), goal(nullptr), offset(0.0f)
{

}

GoalKeeper::~GoalKeeper()
{

}

void GoalKeeper::start()
{
	if (gameObject != nullptr) {
		Vector3 position = gameObject->transform->getPosition();
		Vector3 normal = Vector3::ZERO - position;

		normal *= Vector3(1.0, 0.0, 1.0);
		normal.normalize();

		goal = instantiate("Goal", position - normal * offset);

		if (goal != nullptr) {
			goal->transform->setRotation(gameObject->transform->getRotation());
			goal->getComponent<Goal>()->setKeeper(gameObject);
		}

		manager = GameManager::GetInstance();
		if (manager != nullptr)
			scores = manager->getScore();

		health = this->gameObject->getComponent<Health>();
		PlayerIndex* playerId = this->gameObject->getComponent<PlayerIndex>();

		id = -1;
		if (playerId != nullptr)
		{
			id = playerId->getId();
		}
	}
}

void GoalKeeper::handleData(ComponentData* data)
{
	if (data == nullptr) return;

	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "offset")
		{
			if (!(ss >> offset))
				LOG("GOAL KEEPER: Invalid value for property with name \"%s\"", prop.first.c_str());
		}
		else 
			LOG("GOAL KEEPER: Invalid property with name \"%s\"", prop.first.c_str());
	}
}

void GoalKeeper::onCollisionEnter(GameObject* other)
{
	if (other != nullptr && other->getTag() == "ball")
	{
		if (health != nullptr)
		{
			if (health->isAlive()) {
				Ball* ball;
				ball=other->getComponent<Ball>();
				if (ball != nullptr) {
					ball->setIdPlayerHit(id);
					if (id != -1)
					{
						scores->ballHit(id);
					}
				}
			}
		}
	}
}

GameObject* GoalKeeper::getGoal() const
{
	return goal;
}