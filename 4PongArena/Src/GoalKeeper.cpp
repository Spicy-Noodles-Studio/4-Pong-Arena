#include "GoalKeeper.h"
#include <GameObject.h>
#include <sstream>

#include "Goal.h"

#include <ComponentRegister.h>

REGISTER_FACTORY(GoalKeeper);

GoalKeeper::GoalKeeper(GameObject* gameObject) : UserComponent(gameObject), goal(nullptr), offset(0.0f)
{

}

GoalKeeper::~GoalKeeper()
{

}

void GoalKeeper::start()
{
	Vector3 position = gameObject->transform->getPosition();
	Vector3 normal = Vector3::ZERO - position;

	normal *= Vector3(1.0, 0.0, 1.0); 
	normal.normalize();

	goal = instantiate("Goal", position - normal * offset);

	goal->transform->setRotation(gameObject->transform->getRotation());
	goal->getComponent<Goal>()->setKeeper(gameObject);
}

void GoalKeeper::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "offset")
		{
			if (!(ss >> offset))
				LOG("GOAL KEEPER: Invalid value for property with name \"%s\"", prop.first.c_str());
		}
		else 
			LOG("GOAL KEEPER: Invalid property with name \"%s\"", prop.first.c_str());
	}
}

GameObject* GoalKeeper::getGoal() const
{
	return goal;
}