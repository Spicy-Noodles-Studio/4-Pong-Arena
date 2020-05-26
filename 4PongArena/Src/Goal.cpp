#include "Goal.h"

#include <GameObject.h>
#include <MeshRenderer.h>
#include <MathUtils.h>
#include <Trail.h>
#include <ComponentRegister.h>

#include "ParticleManager.h"
#include "Health.h"
#include "GameManager.h"
#include "Score.h"
#include "PlayerIndex.h"
#include "Ball.h"
#include "CameraEffects.h"
#include "Camera.h"
#include "Scene.h"


REGISTER_FACTORY(Goal);

Goal::Goal(GameObject* gameObject) : UserComponent(gameObject), keeperHealth(nullptr), cameraEffects(nullptr), gameManager(nullptr), particleManager(nullptr), scoreManager(nullptr), id(0), score(0)
{

}

Goal::~Goal()
{
	keeperHealth = nullptr;
	scoreManager = nullptr;
	gameManager = nullptr;
	particleManager = nullptr;
	cameraEffects = nullptr;
}

void Goal::start()
{

	gameManager = GameManager::GetInstance();
	if (notNull(gameManager)) scoreManager = gameManager->getScore();

	if (notNull(keeper))
	{
		id = -1;
		PlayerIndex* playerId = keeper->getComponent<PlayerIndex>();
		if (notNull(playerId)) id = playerId->getPosVector();
	}

	checkNullAndBreak(gameObject);
	particleManager = gameObject->getComponent<ParticleManager>();

	GameObject* mainCamera = findGameObjectWithName("MainCamera");
	if (notNull(mainCamera)) cameraEffects = mainCamera->getComponent<CameraEffects>();
}

void Goal::onObjectEnter(GameObject* other)
{
	if (notNull(other) && other->getTag() == "ball")
	{
		score++;
		Ball* ball = other->getComponent<Ball>();

		if (notNull(keeperHealth))
		{
			keeperHealth->receiveDamage(1);
			if (!keeperHealth->isAlive() && notNull(cameraEffects)) cameraEffects->shake(Vector3(1, 0, 1));
		}

		if (notNull(ball) && notNull(scoreManager))
		{
			int aux = ball->getIdPlayerHit();
			if (aux != -1 && aux != id)
				scoreManager->goalMade(aux);
			else if (aux != -1 && aux == id)
				scoreManager->goalSelfMade(aux);
		}

		checkNullAndBreak(gameObject);
		checkNullAndBreak(gameObject->transform);

		if (notNull(other->transform)) {
			Vector3 thisPos = gameObject->transform->getPosition();
			Vector3 otherPos = other->transform->getPosition();
			Vector3 midPos = otherPos - thisPos;

			Vector3 scale = gameObject->transform->getScale();
			Vector3 rotation = gameObject->transform->getRotation();

			float cosAngle = cos(rotation.x * DEG_TO_RAD);
			float sinAngle = sin(rotation.x * DEG_TO_RAD);
			Vector3 finalPos = { midPos.y, (midPos.x / scale.y) * cosAngle + (midPos.z / scale.y) * sinAngle, midPos.z * cosAngle + midPos.x * -sinAngle };

			if (notNull(particleManager)) particleManager->playParticles(0.6, finalPos);

			other->transform->setPosition({ 0,-10,0 });
		}

		other->setActive(false);

		Trail* otherTrail = other->getComponent<Trail>();
		if (notNull(otherTrail)) otherTrail->stop();

		MeshRenderer* otherRenderer = other->getComponent<MeshRenderer>();
		if (notNull(otherRenderer)) otherRenderer->setVisible(false);
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
	checkNullAndBreak(keeper);

	this->keeper = keeper;
	keeperHealth = keeper->getComponent<Health>();
}

GameObject* Goal::getKeeper()
{
	return keeper;
}