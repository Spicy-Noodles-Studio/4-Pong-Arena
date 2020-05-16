#include "Goal.h"
#include <GameObject.h>
#include <MeshRenderer.h>

#include"ParticleManager.h"
#include "Health.h"
#include "GameManager.h"
#include "Score.h"
#include "PlayerIndex.h"
#include "Ball.h"
#include <ComponentRegister.h>

REGISTER_FACTORY(Goal);

Goal::Goal(GameObject* gameObject) : UserComponent(gameObject), health(nullptr), score(0)
{

}

Goal::~Goal()
{

}

void Goal::start()
{
	manager = GameManager::GetInstance();
	if (manager != nullptr)
		scores = manager->getScore();

	PlayerIndex* playerId = this->gameObject->getComponent<PlayerIndex>();
	particleManager = gameObject->getComponent<ParticleManager>();
	id = -1;
	if (playerId != nullptr)
	{
		id = playerId->getId();
	}
}

void Goal::onObjectEnter(GameObject* other)
{
	if (other->getTag() == "ball")
	{
		score++;
		Ball* ball = other->getComponent<Ball>();

		if (health != nullptr)
		{
			health->receiveDamage(1);
			if (ball != nullptr)
			{
				if (ball->getIdPlayerHit() != -1 && ball->getIdPlayerHit() != id)
					scores->goalMade(ball->getIdPlayerHit());
				else if (ball->getIdPlayerHit() != -1 && ball->getIdPlayerHit() == id)
				{
					scores->goalSelfMade(ball->getIdPlayerHit());
				}
			}
			if (particleManager != nullptr)
			{
				particleManager->playParticles(0.6);
			}
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