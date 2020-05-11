#include "Countdown.h"
#include <InterfaceSystem.h>
#include <RenderSystem.h>
#include <SceneManager.h>
#include <WindowManager.h>
#include <GameObject.h>
#include "UILayout.h"
#include "GameManager.h"

#include <ComponentRegister.h>
#include <SoundEmitter.h>

#include <ctime>

#include "DebugUtils.h"

REGISTER_FACTORY(Countdown);


Countdown::Countdown(GameObject* gameObject) : UserComponent(gameObject), text(NULL)
{

}

Countdown::~Countdown()
{
	
}

void Countdown::start()
{
	started = false;
	charged = false;
	paused = false;

	previousCount = "NO COUNT";

	UILayout* cameraLayout = findGameObjectWithName("MainCamera")->getComponent<UILayout>();

	if (cameraLayout != nullptr)
		text = cameraLayout->getRoot().getChild("Countdown");

	soundEmitter = findGameObjectWithName("MainCamera")->getComponent<SoundEmitter>();
	soundEmitter->setVolume(0.8);

}

void Countdown::preUpdate(float deltaTime)
{
	if (charged && !started) {
		pauseGame();
		started = true;
		paused = true;
		last = std::chrono::steady_clock::now();
	}
	if (paused) {
		

		std::chrono::steady_clock::time_point current = std::chrono::steady_clock::now();
		std::chrono::duration<float> elapsed = std::chrono::duration_cast<std::chrono::duration<float>>(current - last);
		deltaTime = elapsed.count();
		last = current;

		time -= deltaTime;
		if (time >= 1) {
			text.setText(std::to_string((int)time));
			if (previousCount != std::to_string((int)time)) {
				previousCount = std::to_string((int)time);
				soundEmitter->playSound("Countdown");
			}
		}
		else { 
			text.setPosition(0.3f, 0.25f);
			text.setText("FIGHT!");
			if (previousCount != "FIGHT!") {
				previousCount = "FIGHT!";
				soundEmitter->playSound("Countdown_end");
			}
		}

		if (time <= 0) { 
			startGame();
			paused = false;
			text.setText("");
			soundEmitter->setVolume(1.2);
			soundEmitter->playSound("Game_Start");
		}
		
	}
	
	if (!charged) charged = true;

	
}

void Countdown::handleData(ComponentData* data)
{
	for (auto prop : data->getProperties()) {
		std::stringstream ss(prop.second);

		if (prop.first == "time") {
			setFloat(time);
		}else
			LOG("DODGE: Invalid property name %s", prop.first.c_str());
	}
}


void Countdown::pauseGame()
{
	GameManager::GetInstance()->pause(true);
}

void Countdown::startGame()
{
	GameManager::GetInstance()->pause(false);
}
