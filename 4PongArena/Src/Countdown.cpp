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

void Countdown::activateCountdown(bool activate)
{
	for (int i = 0; i < players.size(); i++)
	{
		if (notNull(players[i])) {
			PlayerController* playerController = players[i]->getComponent<PlayerController>();
			IAPaddle* AIController = players[i]->getComponent<IAPaddle>();
			if (playerController != nullptr)
				playerController->setActive(!activate);
			else if (AIController != nullptr)
				AIController->setActive(!activate);
		}
	}

	countingDown = activate;
	panel.setVisible(activate);
	panel.setAlwaysOnTop(activate);
}

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

	if (notNull(mainCamera))
	{
		UILayout* cameraLayout = mainCamera->getComponent<UILayout>();
		if (notNull(cameraLayout))
			panel = cameraLayout->getRoot().getChild("CountdownBackground");
	}

	if (notNull(game))
	{
		soundEmitter = game->getComponent<SoundEmitter>();
		if(notNull(soundEmitter))
			soundEmitter->setVolume(0.8);
	}

	GameManager* gameManager = GameManager::GetInstance();
	if (notNull(gameManager)) players = gameManager->getPaddles();
}

void Countdown::update(float deltaTime)
{
	if (!startCounting)
	{
		activateCountdown(true);
		startCounting = true;

		last = std::chrono::steady_clock::now();
	}

	if (countingDown)
	{
		UIElement countdownPanel = panel.getChild("Countdown");
		if (time + 1 >= 1)
		{
			countdownPanel.setText(std::to_string((int)time + 1));
			if (previousCount != std::to_string((int)time + 1))
			{
				previousCount = std::to_string((int)time + 1);
				if (notNull(soundEmitter)) soundEmitter->playSound("Countdown");
			}
		}
		else
		{
			countdownPanel.setText("SURVIVE!");
			if (previousCount != "SURVIVE!")
			{
				previousCount = "SURVIVE!";
				if (notNull(soundEmitter)) soundEmitter->playSound("Countdown_end");
			}
		}

		if (time + 1 < 0)
			activateCountdown(false);

		std::chrono::steady_clock::time_point current = std::chrono::steady_clock::now();
		std::chrono::duration<float> elapsed = std::chrono::duration_cast<std::chrono::duration<float>>(current - last);

		last = current;
		time -= elapsed.count();
	}
}

void Countdown::handleData(ComponentData* data)
{
	checkNullAndBreak(data);

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