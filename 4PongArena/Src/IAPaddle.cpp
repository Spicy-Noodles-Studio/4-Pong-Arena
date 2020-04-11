#include "IAPaddle.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <RigidBody.h>
#include <MathUtils.h>

#include "Movement.h"

REGISTER_FACTORY(IAPaddle);

IAPaddle::IAPaddle(GameObject* gameObject) :	UserComponent(gameObject), currentState(State::MOVE), targetBall(nullptr), movement(nullptr), 
												timeToChange(0.0f), timerToChange(0.0f)
{

}

IAPaddle::~IAPaddle()
{

}

void IAPaddle::start()
{
	movement = gameObject->getComponent<Movement>();
	timeToChange = 1.0f;
}

void IAPaddle::update(float deltaTime)
{
	timerToChange += deltaTime;
	if (timerToChange >= timeToChange) { 
		timerToChange = 0.0f; 
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

void IAPaddle::processChooseTargetState()
{
	targetBall = nullptr;
	balls = findGameObjectsWithTag("ball");

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
	// TODO: mejorar esto (si averiguo como)
	Vector3 normal = movement->getNormal();
	Vector3 diff = targetBall->transform->getPosition() - gameObject->transform->getPosition();
	float tolerance = 0.2f;

	if (normal == Vector3::RIGHT || normal == Vector3::NEGATIVE_RIGHT) // mira la z
	{
		if (abs(diff.z) < tolerance) { movement->stop(); return; }
		if (diff.z < 0.0f)
			normal == Vector3::RIGHT ? movement->moveLeft() : movement->moveRight();
		else if (diff.z > 0.0f)
			normal == Vector3::RIGHT ? movement->moveRight() : movement->moveLeft();
	}
	else if (normal == Vector3::FORWARD || normal == Vector3::NEGATIVE_FORWARD) // mira la x
	{
		if (abs(diff.x) < tolerance) { movement->stop(); return; }
		if (diff.x < tolerance)
			normal == Vector3::FORWARD ? movement->moveRight() : movement->moveLeft();
		else if(diff.x > tolerance)
			normal == Vector3::FORWARD ? movement->moveLeft() : movement->moveRight();
	}
}

void IAPaddle::takeDecision()
{
	//If it has not target, then choose one
	if (targetBall == nullptr) { processChooseTargetState(); return; }

	//If it has, then see if its posible or choose another one
	if (!canReachToTarget())
		processChooseTargetState();
	else
		currentState = State::MOVE;

	/*if (!canReachToTarget()) {
		//Segun inteligencia
		float intelligence = 1.0f;
		if(random() <= intelligence)
			processChooseTargetState();
	}*/
}

bool IAPaddle::canReachToTarget()
{
	// De momento random, he probado otras formas no funcionan mejor que un random (lo cual es raro)
	return !rand();
}

bool IAPaddle::isBallBehind(const Vector3& ballPosition)
{
	Vector3 normal = movement->getNormal();
	Vector3 direction = (ballPosition - gameObject->transform->getPosition()) * Vector3(abs(normal.x), 0.0, abs(normal.z));
	direction.normalize();

	return normal != direction && ballPosition.y < 0.0;
}

bool IAPaddle::isBallHeadingToMe(const Vector3& ballDirection)
{
	//Cambiar a Raycast mejor cuando se pueda
	Vector3 inverseNormal = movement->getNormal() * Vector3::NEGATIVE_IDENTITY;
	Vector3 rawDirection = (ballDirection * Vector3(abs(inverseNormal.x), abs(inverseNormal.y), abs(inverseNormal.z)));
	Vector3 ballNormal = rawDirection.normalized();
	if (inverseNormal + ballNormal == Vector3::ZERO) return false;

	float tolerance = 0.1f;
	return rawDirection.magnitude() > tolerance;
}
