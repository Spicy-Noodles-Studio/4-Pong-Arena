#include "Goal.h"
#include <GameObject.h>

#include "Health.h"

#include <ComponentRegister.h>

REGISTER_FACTORY(Goal);

Goal::Goal(GameObject* gameObject) : UserComponent(gameObject), health(nullptr), score(0)
{

}

Goal::~Goal()
{

}

void Goal::onObjectEnter(GameObject* other)
{
	if (other->getTag() == "ball")
	{
		score++;

		if (health != nullptr)
			health->receiveDamage(1);

		other->setActive(false);
	}
}

void Goal::setScore(int score)
{
	this->score = score;
}

int Goal::getScore() const
{
	return score;
}

void Goal::setKeeper(GameObject* keeper)
{
	if (keeper != nullptr)
		health = keeper->getComponent<Health>();
}

GameObject* Goal::getKeeper()
{
	if (health != nullptr)
		return health->gameObject;
}