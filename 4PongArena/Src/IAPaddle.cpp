#include "IAPaddle.h"
#include <GameObject.h>
#include <RigidBody.h>
#include <MathUtils.h>
#include <sstream>
#include <Trail.h>

#include "Movement.h"
#include "SpawnerManager.h"
#include "GameManager.h"

#include <ComponentRegister.h>
#include <SoundEmitter.h>

REGISTER_FACTORY(IAPaddle);

IAPaddle::IAPaddle(GameObject* gameObject) : UserComponent(gameObject), currentState(State::MOVE), soundEmitter(nullptr), targetBall(nullptr), movement(nullptr), decisionTime(1.0f), decisionTimer(0.0f), id(0), volume(0.0f)
{

}

IAPaddle::~IAPaddle()
{
	targetBall = nullptr;
	movement = nullptr;
	soundEmitter = nullptr;
}

void IAPaddle::start()
{
	checkNullAndBreak(gameObject);

	movement = gameObject->getComponent<Movement>();
	soundEmitter = gameObject->getComponent<SoundEmitter>();
	Trail* trail = gameObject->getComponent<Trail>();

	volume = 0.8;
	if (notNull(soundEmitter)) soundEmitter->setVolume(volume);
	if (notNull(trail)) trail->start();
}

void IAPaddle::update(float deltaTime)
{
	decisionTimer += deltaTime;
	if (decisionTimer >= decisionTime)
	{
		decisionTimer = 0.0f;
		takeDecision();
	}

	GameManager* gameManager = GameManager::GetInstance();
	if (volume > 0 && notNull(gameManager) && gameManager->isGameEnded()) {
		volume = 0;
		if (notNull(soundEmitter)) soundEmitter->setVolume(0);
	}

	// Simple State Machine Logic
	switch (currentState)
	{
	case State::CHOOSE_TARGET:
		processChooseTargetState();
		break;

	case State::MOVE:
		processMoveState();
		break;
	}
}

void IAPaddle::handleData(ComponentData* data)
{
	checkNullAndBreak(data);

	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "decisionTime")
		{
			setFloat(decisionTime);
		}
		else
			LOG("IA PADDLE: Invalid property with name \"%s\"", prop.first.c_str());
	}
}

void IAPaddle::setId(int id)
{
	this->id = id;
}

int IAPaddle::getId() const
{
	return id;
}

void IAPaddle::processChooseTargetState()
{
	targetBall = nullptr;
	std::vector<GameObject*> balls;

	GameObject* spawner = findGameObjectWithName("SpawnerManager");
	SpawnerManager* spawnerManager = (notNull(spawner)) ? spawner->getComponent<SpawnerManager>() : nullptr;
	if (notNull(spawnerManager)) balls = spawnerManager->getPool();

	//Escogemos una bola que venga hacia nosotros aleatoria
	std::vector<GameObject*> validBalls;
	for (GameObject* ball : balls)
	{
		if (notNull(ball) && ball->isActive())
		{
			Vector3 direction = Vector3::ZERO;
			RigidBody* ballRB = ball->getComponent<RigidBody>();
			if (notNull(ballRB)) direction = ballRB->getLinearVelocity().normalized();
			if (notNull(ball->transform) && !isBallBehind(ball->transform->getPosition()) && isBallHeadingToMe(direction))
				validBalls.push_back(ball);
		}
	}
	if (!validBalls.size()) return;

	//De todas las que se dirigen a mi, elijo una random
	targetBall = validBalls[rand() % validBalls.size()];
	currentState = State::MOVE;
}

void IAPaddle::processMoveState()
{
	checkNullAndBreak(movement);

	if (targetBall == nullptr)
	{
		movement->stop();
		currentState = State::CHOOSE_TARGET;
		return;
	}

	Vector3 normal = movement->getNormal();
	Vector3 motionDirection(-normal.z, 0.0, normal.x); // Perpendicular vector (right)
	Vector3 directionMask = Vector3(abs(-normal.z), 0.0, abs(normal.x)).normalized();
	Vector3 diff;
	if (notNull(targetBall) && notNull(targetBall->transform) && notNull(gameObject) && notNull(gameObject->transform))
		diff = (targetBall->transform->getPosition() - gameObject->transform->getPosition()) * directionMask; //Extract raw differnce (no signe modification)
	float tolerance = 0.2f;

	if (diff.magnitude() < tolerance) // We have arrived
		movement->stop();
	else if (diff.normalized() + motionDirection.normalized() == Vector3::ZERO) // Different direction, go to my left
		movement->moveLeft();
	else
		movement->moveRight();
}

void IAPaddle::takeDecision()
{
	// Choose randomly if it should choose another target or if it has no target, then choose one
	if (rand() || targetBall == nullptr)
		processChooseTargetState();
}

bool IAPaddle::isBallBehind(const Vector3& ballPosition)
{
	checkNullAndBreak(movement, false);

	Vector3 normal = movement->getNormal();
	Vector3 direction;

	if (notNull(gameObject) && notNull(gameObject->transform))
		direction = (ballPosition - gameObject->transform->getPosition()) * Vector3(abs(normal.x), 0.0, abs(normal.z));

	direction.normalize();

	return normal != direction && ballPosition.y < 0.0; // TODO: quitar segunda condicion (cuando la gestion de bolas este hecha)
}

bool IAPaddle::isBallHeadingToMe(const Vector3& ballDirection)
{
	checkNullAndBreak(movement, false);

	Vector3 inverseNormal = movement->getNormal() * Vector3::NEGATIVE_IDENTITY;
	Vector3 rawDirection = (ballDirection * Vector3(abs(inverseNormal.x), abs(inverseNormal.y), abs(inverseNormal.z)));
	Vector3 ballNormal = rawDirection.normalized();
	if (inverseNormal + ballNormal == Vector3::ZERO) return false;

	float tolerance = 0.1f;
	return rawDirection.magnitude() > tolerance;
}