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

Countdown::Countdown(GameObject* gameObject) : UserComponent(gameObject), panel(NULL), players(), time(0), startCounting(false), countingDown(false)
{

}

Countdown::~Countdown()
{

}

void Countdown::start()
{
	GameObject* mainCamera = findGameObjectWithName("MainCamera");

	if (mainCamera != nullptr)
	{
		UILayout* cameraLayout = mainCamera->getComponent<UILayout>();
		if (cameraLayout != nullptr)
			panel = cameraLayout->getRoot().getChild("CountdownBackground");
	}

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
		panel.setVisible(true);
		panel.setAlwaysOnTop(true);

		last = std::chrono::steady_clock::now();
	}

	if (countingDown)
	{
		time -= deltaTime;

		if (time >= 1)
			panel.getChild("Countdown").setText(std::to_string((int)time));
		else
			panel.getChild("Countdown").setText("SURVIVE!");

		if (time < 0)
		{
			for (int i = 0; i < players.size(); i++)
			{
				if (players[i]->getComponent<PlayerController>() != nullptr)
					players[i]->getComponent<PlayerController>()->setActive(true);
				else if (players[i]->getComponent<IAPaddle>() != nullptr)
					players[i]->getComponent<IAPaddle>()->setActive(true);
			}

			countingDown = false;
			panel.setVisible(false);
			panel.setAlwaysOnTop(false);
		}

		std::chrono::steady_clock::time_point current = std::chrono::steady_clock::now();
		std::chrono::duration<float> elapsed = std::chrono::duration_cast<std::chrono::duration<float>>(current - last);

		last = current;
		time -= elapsed.count();
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