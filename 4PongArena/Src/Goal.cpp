#include "Goal.h"
#include <ComponentRegister.h>
#include <GameObject.h>

#include "Health.h"

REGISTER_FACTORY(Goal);

Goal::Goal(GameObject* gameObject) : UserComponent(gameObject), score(0), health(nullptr)
{

}

Goal::~Goal()
{

}

void Goal::onObjectEnter(GameObject* other)
{
	if (other->getTag() == "ball") {
		score++;
		if (health != nullptr)
			health->receiveDamage(1);
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
	if (keeper == nullptr) return;
	health = keeper->getComponent<Health>();
}

GameObject* Goal::getKeeper()
{
	if (health == nullptr) return nullptr;
	return health->gameObject;
}
