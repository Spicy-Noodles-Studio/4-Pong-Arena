#include "Score.h"
#include <ComponentRegister.h>
#include <GameObject.h>

#include "Health.h"

REGISTER_FACTORY(Score);

Score::Score(GameObject* gameObject) : UserComponent(gameObject), score(0), health(nullptr)
{

}

Score::~Score()
{

}

void Score::onObjectEnter(GameObject* other)
{
	if (other->getTag() == "ball") {
		score++;
		if (health != nullptr)
			health->receiveDamage(1);
	}
}

void Score::setScore(int score)
{
	this->score = score;
}

int Score::getScore() const
{
	return score;
}

void Score::setKeeper(GameObject* keeper)
{
	if (keeper == nullptr) return;
	health = keeper->getComponent<Health>();
}

GameObject* Score::getKeeper()
{
	if (health == nullptr) return nullptr;
	return health->gameObject;
}
