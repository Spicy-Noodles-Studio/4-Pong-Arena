#include "Countdown.h"
#include <ComponentRegister.h>
#include <InterfaceSystem.h>
#include <RenderSystem.h>
#include <SceneManager.h>
#include <WindowManager.h>
#include <GameObject.h>
#include <UILayout.h>

#include "PlayerController.h"
#include "IAPaddle.h"
#include "GameManager.h"
#include "DebugUtils.h"

REGISTER_FACTORY(Countdown);

Countdown::Countdown(GameObject* gameObject) : UserComponent(gameObject), text(NULL), players(), time(0), startCounting(false), countingDown(false)
{

}

Countdown::~Countdown()
{

}

void Countdown::start()
{
	UILayout* cameraLayout = findGameObjectWithName("MainCamera")->getComponent<UILayout>();

	if (cameraLayout != nullptr)
		text = cameraLayout->getRoot().getChild("Countdown");

	players = GameManager::GetInstance()->getPaddles();
}

void Countdown::update(float deltaTime)
{
	if (!startCounting)
	{
		for (int i = 0; i < players.size(); i++)
		{
			if (players[i]->getComponent<PlayerController>() != nullptr)
				players[i]->getComponent<PlayerController>()->setActive(false);
			else if (players[i]->getComponent<IAPaddle>() != nullptr)
				players[i]->getComponent<IAPaddle>()->setActive(false);
		}

		startCounting = true;
		countingDown = true;
	}

	if (countingDown)
	{
		time -= deltaTime;

		if (time + 1 >= 1)
			text.setText(std::to_string((int)time));
		else
			text.setText("SURVIVE!");

		if (time + 1 <= 0)
		{
			text.setText("");

			for (int i = 0; i < players.size(); i++)
			{
				if (players[i]->getComponent<PlayerController>() != nullptr)
					players[i]->getComponent<PlayerController>()->setActive(true);
				else if (players[i]->getComponent<IAPaddle>() != nullptr)
					players[i]->getComponent<IAPaddle>()->setActive(true);
			}

			countingDown = false;
		}
	}
}

void Countdown::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties())
	{
		std::stringstream ss(prop.second);

		if (prop.first == "time")
		{
			setFloat(time);
		}
		else
			LOG("COUNTDOWN: Invalid property name %s", prop.first.c_str());
	}
}

bool Countdown::isCounting() const
{
	return countingDown;
}