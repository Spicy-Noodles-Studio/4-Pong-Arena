#include "Goal.h"
#include <GameObject.h>
#include <MeshRenderer.h>

#include "Health.h"
#include "GameManager.h"
#include "Score.h"
#include "PlayerController.h"
#include "IAPaddle.h"
#include "Ball.h"
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
		if (health != nullptr)
		{
			int id = -1;
			if (health->gameObject->getComponent<PlayerController>() != nullptr)
				id = health->gameObject->getComponent<PlayerController>()->getPlayer().id;
			else if (health->gameObject->getComponent<IAPaddle>() != nullptr)
				id = health->gameObject->getComponent<IAPaddle>()->getId();
			
				
			if (other->getComponent<Ball>()->getIdPlayerHit() != -1 && other->getComponent<Ball>()->getIdPlayerHit() != id)
				GameManager::GetInstance()->getScore()->goalMade(other->getComponent<Ball>()->getIdPlayerHit());
		}

		other->setActive(false);
		other->getComponent<MeshRenderer>()->setVisible(false);
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