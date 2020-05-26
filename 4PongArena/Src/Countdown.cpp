#include "Countdown.h"
#include <ComponentRegister.h>
#include <GameObject.h>
#include <SoundEmitter.h>
#include <UILayout.h>

#include "GameManager.h"
#include "PlayerController.h"
#include "IAPaddle.h"
#include "GameManager.h"
#include "DebugUtils.h"

REGISTER_FACTORY(Countdown);

Countdown::Countdown(GameObject* gameObject) : UserComponent(gameObject), soundEmitter(nullptr), panel(NULL), players(), time(0), startCounting(false), countingDown(false)
{

}

Countdown::~Countdown()
{
	soundEmitter = nullptr;
	players.clear();
}

void Countdown::start()
{
	GameObject* mainCamera = findGameObjectWithName("MainCamera");
	GameObject* game = findGameObjectWithName("Game");

	if (mainCamera != nullptr)
	{
		UILayout* cameraLayout = mainCamera->getComponent<UILayout>();
		if (cameraLayout != nullptr)
			panel = cameraLayout->getRoot().getChild("CountdownBackground");
	}

	if (game != nullptr)
	{
		soundEmitter = game->getComponent<SoundEmitter>();
		if (soundEmitter != nullptr)
			soundEmitter->setVolume(0.8);
	}

	if (GameManager::GetInstance() != nullptr) players = GameManager::GetInstance()->getPaddles();
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
		if (time + 1 >= 1)
		{
			panel.getChild("Countdown").setText(std::to_string((int)time + 1));
			if (previousCount != std::to_string((int)time + 1))
			{
				previousCount = std::to_string((int)time + 1);
				if (soundEmitter != nullptr) soundEmitter->playSound("Countdown");
			}
		}
		else
		{
			panel.getChild("Countdown").setText("SURVIVE!");
			if (previousCount != "SURVIVE!")
			{
				previousCount = "SURVIVE!";
				if (soundEmitter != nullptr) soundEmitter->playSound("Countdown_end");
			}
		}

		if (time + 1 < 0)
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
	if (data == nullptr) return;

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

bool Countdown::hasStarted() const
{
	return startCounting;
}

bool Countdown::isCounting() const
{
	return countingDown;
}

float Countdown::getRemainingTime() const
{
	return time;
}