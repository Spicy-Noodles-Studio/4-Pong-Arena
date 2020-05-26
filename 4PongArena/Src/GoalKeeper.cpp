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

GoalKeeper::GoalKeeper(GameObject* gameObject) : UserComponent(gameObject), goalObject(nullptr), health(nullptr), scores(nullptr), gameManager(nullptr), id(0), offset(0.0f)
{

}

GoalKeeper::~GoalKeeper()
{
	goalObject = nullptr;
	health = nullptr;
	scores = nullptr;
	gameManager = nullptr;
}

void GoalKeeper::start()
{
	checkNullAndBreak(gameObject);
	checkNullAndBreak(gameObject->transform);

	Vector3 position = gameObject->transform->getPosition();
	Vector3 normal = Vector3::ZERO - position;

	normal *= Vector3(1.0, 0.0, 1.0);
	normal.normalize();

	goalObject = instantiate("Goal", position - normal * offset);

	if (notNull(goalObject) && notNull(goalObject->transform)) {
		goalObject->transform->setRotation(gameObject->transform->getRotation());
		Goal* goal = goalObject->getComponent<Goal>();
		if (notNull(goal)) goal->setKeeper(gameObject);
	}

	gameManager = GameManager::GetInstance();
	if (notNull(gameManager)) scores = gameManager->getScore();

	health = gameObject->getComponent<Health>();
	PlayerIndex* playerId = gameObject->getComponent<PlayerIndex>();

	id = -1;
	if (notNull(playerId)) id = playerId->getPosVector();
}

void GoalKeeper::handleData(ComponentData* data)
{
	checkNullAndBreak(data);

	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "offset")
		{
			setFloat(offset);
		}
		else
			LOG("GOAL KEEPER: Invalid property with name \"%s\"", prop.first.c_str());
	}
}

void GoalKeeper::onCollisionEnter(GameObject* other)
{
	if (notNull(other) && other->getTag() == "ball") {
		if (notNull(health) && health->isAlive()) {
			Ball* ball = other->getComponent<Ball>();
			if (notNull(ball) && id != -1 && notNull(scores)) {
				ball->setIdPlayerHit(id);
				scores->ballHit(id);
			}
		}
	}
}

GameObject* GoalKeeper::getGoal() const
{
	return goalObject;
}