#include "IAPaddle.h"
#include <ComponentRegister.h>
#include <GameObject.h>

#include "Movement.h"
// TODO: quitar
#include <RigidBody.h>
#include <InputSystem.h>

REGISTER_FACTORY(IAPaddle);

IAPaddle::IAPaddle(GameObject* gameObject) :	UserComponent(gameObject), currentState(State::IDLE), targetBall(nullptr), movement(nullptr), 
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
	if (timerToChange >= timeToChange) timerToChange = 0.0f; //Maybe change state

	// Simple State Machine Logic
	switch (currentState)
	{
	case State::IDLE:
		processIdleState(deltaTime);
		break;
	case State::CHOOSE_TARGET:
		processChooseTargetState(deltaTime);
		break;
	case State::MOVE:
		processMoveState(deltaTime);
		break;
	default:
		processIdleState(deltaTime);
		break;
	}
	// DEBUG PURPOSES TODO: quitar
	if (InputSystem::GetInstance()->getKeyPress("Space")) {
		GameObject* ball = instantiate("Ball");
		ball->getComponent<RigidBody>()->setLinearVelocity(Vector3(0.5, 0.0, 0.75).normalized() * 5.0);
	}
}

void IAPaddle::processIdleState(float deltaTime)
{
	movement->stop();
	if (targetBall == nullptr)
		currentState = State::CHOOSE_TARGET;
}

void IAPaddle::processChooseTargetState(float deltaTime)
{
	// De momento va a ser una bola random
	// TODO: cambiar esto
	balls = findGameObjectsWithTag("ball");
	if (!balls.size()) {
		currentState = State::IDLE;
		return;
	}
	// TODO: escoger la bola que mejor venga
	int ballIndex = rand() % balls.size();
	if (ballIndex >= 0 && ballIndex < balls.size())
		targetBall = balls[ballIndex];
	if (targetBall != nullptr)
		currentState = State::MOVE;
	else
		currentState = State::IDLE;
}

void IAPaddle::processMoveState(float deltaTime)
{
	if (targetBall == nullptr) { currentState == State::CHOOSE_TARGET; return; }
	// TODO: mejorar esto (si averiguo como)
	Vector3 normal = movement->getNormal();
	if (normal == Vector3::RIGHT || normal == Vector3::NEGATIVE_RIGHT) // mira la z
	{
		if (targetBall->transform->getPosition().z < gameObject->transform->getPosition().z)
			normal == Vector3::RIGHT ? movement->moveLeft() : movement->moveRight();
		else
			normal == Vector3::RIGHT ? movement->moveRight() : movement->moveLeft();
	}
	else if (normal == Vector3::FORWARD || normal == Vector3::NEGATIVE_FORWARD) // mira la x
	{
		if (targetBall->transform->getPosition().x < gameObject->transform->getPosition().x)
			normal == Vector3::FORWARD ? movement->moveRight() : movement->moveLeft();
		else
			normal == Vector3::FORWARD ? movement->moveLeft() : movement->moveRight();
	}
}
