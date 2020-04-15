#include "IAPaddle.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <RigidBody.h>
#include <MathUtils.h>
#include <sstream>

#include "Movement.h"

REGISTER_FACTORY(IAPaddle);

IAPaddle::IAPaddle(GameObject* gameObject) :	UserComponent(gameObject), currentState(State::MOVE), targetBall(nullptr), movement(nullptr), 
												decisionTime(1.0f), decisionTimer(0.0f)
{

}

IAPaddle::~IAPaddle()
{

}

void IAPaddle::start()
{
	movement = gameObject->getComponent<Movement>();
}

void IAPaddle::update(float deltaTime)
{
	decisionTimer += deltaTime;
	if (decisionTimer >= decisionTime) { 
		decisionTimer = 0.0f; 
		takeDecision(); 
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
	for (auto prop : data->getProperties()) {
		std::stringstream ss(prop.second);
		if (prop.first == "decisionTime") {
			if (!(ss >> decisionTime))
				LOG("IA PADDLE: Invalid value for property with name \"%s\"", prop.first.c_str());
		}
		else
			LOG("IA PADDLE: Invalid property with name \"%s\"", prop.first.c_str());
	}
}

void IAPaddle::processChooseTargetState()
{
	targetBall = nullptr;
	balls = findGameObjectsWithTag("ball"); // TODO: el vector deberia de ser una referencia a un vector ya creado (mas eficiente)

	//Escogemos una bola que venga hacia nosotros aleatoria
	std::vector<GameObject*> validBalls;
	for (GameObject* ball : balls) {
		Vector3 direction = ball->getComponent<RigidBody>()->getLinearVelocity().normalized();
		if (!isBallBehind(ball->transform->getPosition()) && isBallHeadingToMe(direction))
			validBalls.push_back(ball);
	}
	if (!validBalls.size()) return;

	//De todas las que se dirigen a mi, elijo una random
	targetBall = validBalls[rand() % validBalls.size()];
	currentState = State::MOVE;
}

void IAPaddle::processMoveState()
{
	if (targetBall == nullptr) { 
		movement->stop(); 
		currentState = State::CHOOSE_TARGET;
		return; 
	}

	Vector3 normal = movement->getNormal();
	Vector3 motionDirection(-normal.z, 0.0, normal.x); // Perpendicular vector (right)
	Vector3 directionMask = Vector3(abs(-normal.z), 0.0, abs(normal.x)).normalized();
	Vector3 diff = (targetBall->transform->getPosition() - gameObject->transform->getPosition()) * directionMask; //Extract raw differnce (no signe modification)
	float tolerance = 0.2f;

	if(diff.magnitude() < tolerance) // We have arrived
		movement->stop();
	else if (diff.normalized() + motionDirection.normalized() == Vector3::ZERO) // Different direction, go to my left
		movement->moveLeft();
	else
		movement->moveRight();
}

void IAPaddle::takeDecision()
{
	// Choose randomly if it should choose another target or if it has not target, then choose one
	if (rand() || targetBall == nullptr)
		processChooseTargetState();
}

bool IAPaddle::isBallBehind(const Vector3& ballPosition)
{
	Vector3 normal = movement->getNormal();
	Vector3 direction = (ballPosition - gameObject->transform->getPosition()) * Vector3(abs(normal.x), 0.0, abs(normal.z));
	direction.normalize();

	return normal != direction && ballPosition.y < 0.0; // TODO: quitar segunda condicion (cuando la gestion de bolas este hecha)
}

bool IAPaddle::isBallHeadingToMe(const Vector3& ballDirection)
{
	Vector3 inverseNormal = movement->getNormal() * Vector3::NEGATIVE_IDENTITY;
	Vector3 rawDirection = (ballDirection * Vector3(abs(inverseNormal.x), abs(inverseNormal.y), abs(inverseNormal.z)));
	Vector3 ballNormal = rawDirection.normalized();
	if (inverseNormal + ballNormal == Vector3::ZERO) return false;

	float tolerance = 0.1f;
	return rawDirection.magnitude() > tolerance;
}
