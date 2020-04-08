#include "Score.h"
#include <ComponentRegister.h>
#include <GameObject.h>

REGISTER_FACTORY(Score);

Score::Score(GameObject* gameObject) : UserComponent(gameObject), score(0)
{

}

Score::~Score()
{
	
}

void Score::onObjectEnter(GameObject* other)
{
	if (other->getTag() == "ball")
		score++;
}

void Score::setScore(int score)
{
	this->score = score;
}

int Score::getScore() const
{
	return score;
}
