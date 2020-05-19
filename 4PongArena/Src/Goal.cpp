#include "Goal.h"
#include <GameObject.h>
#include <MeshRenderer.h>
#include <MathUtils.h>
#include <Trail.h>

#include"ParticleManager.h"
#include "Health.h"
#include "GameManager.h"
#include "Score.h"
#include "PlayerIndex.h"
#include "Ball.h"
#include "CameraEffects.h"
#include "Camera.h"
#include "Scene.h"
#include <ComponentRegister.h>


REGISTER_FACTORY(Goal);

Goal::Goal(GameObject* gameObject) : UserComponent(gameObject), health(nullptr),cameraEffects(nullptr), score(0)
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
	Camera* cam = gameObject->getScene()->getMainCamera();
	if (cam != nullptr) cameraEffects = cam->gameObject->getComponent<CameraEffects>();

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

			if (!health->isAlive())cameraEffects->shake(Vector3(1, 0, 1));

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
				double Z= other->transform->getPosition().z - gameObject->transform->getPosition().z;
				double X = (other->transform->getPosition().x - gameObject->transform->getPosition().x);

				Vector3 x;
				
				x.x = (other->transform->getPosition().y - gameObject->transform->getPosition().y);
				x.y = (X/ gameObject->transform->getScale().y)*cos( gameObject->transform->getRotation().x * DEG_TO_RAD) + (Z / gameObject->transform->getScale().y) * sin(gameObject->transform->getRotation().x * DEG_TO_RAD);
				x.z = Z * cos(gameObject->transform->getRotation().x * DEG_TO_RAD) + X * -sin(gameObject->transform->getRotation().x * DEG_TO_RAD);

				other->getComponent<Trail>()->stop();

				
				particleManager->playParticles(0.6,x);
			}
		}
		other->transform->setPosition({ 0,-10,0 });
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