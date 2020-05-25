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

Goal::Goal(GameObject* gameObject) : UserComponent(gameObject), health(nullptr),cameraEffects(nullptr), gameManager(nullptr), particleManager(nullptr),scoreManager(nullptr), id(0), score(0)
{

}

Goal::~Goal()
{

}

void Goal::start()
{
	if (gameObject != nullptr) {
		gameManager = GameManager::GetInstance();
		if (gameManager != nullptr)
			scoreManager = gameManager->getScore();
		
		if (health != nullptr && health->gameObject != nullptr)
		{
			PlayerIndex* playerId = health->gameObject->getComponent<PlayerIndex>();

			id = -1;
			if (playerId != nullptr)
			{
				id = playerId->getPosVector();
			}
		}
		particleManager = gameObject->getComponent<ParticleManager>();
		
		GameObject* cam = findGameObjectWithName("MainCamera");
		if (cam != nullptr) cameraEffects = cam->getComponent<CameraEffects>();
	}

}

void Goal::onObjectEnter(GameObject* other)
{
	if (other != nullptr && other->getTag() == "ball")
	{
		score++;
		Ball* ball = other->getComponent<Ball>();

		if (health != nullptr)
		{
			health->receiveDamage(1);

			if (!health->isAlive() && cameraEffects != nullptr) cameraEffects->shake(Vector3(1, 0, 1));

			if (ball != nullptr)
			{
				if (scoreManager != nullptr)
				{
					int aux = ball->getIdPlayerHit();
					if (aux != -1 && aux != id)
						scoreManager->goalMade(aux);
					else if (aux != -1 && aux == id)
					{
						scoreManager->goalSelfMade(aux);
					}
				}
			}
			if (particleManager != nullptr)
			{
				Vector3 thisPos, otherPos;
				if (gameObject != nullptr && gameObject->transform != nullptr) thisPos = gameObject->transform->getPosition();
				if (other->transform != nullptr) otherPos = other->transform->getPosition();
				double midZ= otherPos.z - thisPos.z;
				double midX = (otherPos.x - thisPos.x);

				if (gameObject != nullptr && gameObject->transform != nullptr)
				{
					Vector3 finalPos, scale, rotation;
					scale = gameObject->transform->getScale();
					rotation = gameObject->transform->getRotation();

					finalPos.x = (otherPos.y - thisPos.y);
					finalPos.y = (midX / scale.y) * cos(rotation.x * DEG_TO_RAD) + (midZ / scale.y) * sin(rotation.x * DEG_TO_RAD);
					finalPos.z = midZ * cos(rotation.x * DEG_TO_RAD) + midX * -sin(rotation.x * DEG_TO_RAD);

					if (other->getComponent<Trail>() != nullptr) other->getComponent<Trail>()->stop();

					particleManager->playParticles(0.6, finalPos);
				}
			}
		}
		if (other->transform != nullptr) other->transform->setPosition({ 0,-10,0 });
		other->setActive(false);
		other->getComponent<MeshRenderer>()->setVisible(false);
	}
}

void Goal::setScore(int score)
{
	if (this != nullptr) this->score = score;
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