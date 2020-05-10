#include "OptionsMenu.h"
#include <InterfaceSystem.h>
#include <RenderSystem.h>
#include <SoundSystem.h>
#include <WindowManager.h>
#include <SceneManager.h>
#include <GameObject.h>
#include <UILayout.h>
#include <UIElement.h>
#include <Window.h>
#include <Camera.h>

#include "ComponentRegister.h"

REGISTER_FACTORY(OptionsMenu);

bool OptionsMenu::backButtonClick()
{
	SceneManager::GetInstance()->changeScene("MainMenu");
	buttonClick(backSound);
	return false;
}

bool OptionsMenu::resetConfigButtonClick()
{
	fullscreen = false;
	resolution = 1;
	soundsVolume = 100;
	musicVolume = 100;
	gamma = 100;


	changeResolution(0);
	musicScroll.setScrollPositionScrollBar(musicVolume);
	volumeScroll.setScrollPositionScrollBar(soundsVolume);
	gammaScroll.setScrollPositionScrollBar(gamma);
	checkbox.setCheckBoxState(false);
	changeFullscreen(false);

	buttonClick(buttonSound);
	return false;
}

bool OptionsMenu::resolutionButtonClick()
{
	if (currResolution != resolution)
	{
		if (fullscreen)
		{
			windowManager->setFullscreen(false);
			windowManager->windowResize(resolutions[resolution].first, resolutions[resolution].second);
			windowManager->setFullscreen(true);
			currResolution = resolution;
			windowManager->setActualResolutionId(currResolution);
		}
		else
		{
			windowManager->windowResize(resolutions[resolution].first, resolutions[resolution].second);
			currResolution = resolution;
			windowManager->setActualResolutionId(currResolution);
		}
	}
	buttonClick(buttonSound);
	return false;
}

bool OptionsMenu::changeResolution(int value)
{
	resolution += value;

	if (resolution < 1) resolution = 1;
	if (resolution > resolutionNames.size() - 1) resolution = resolutionNames.size() - 1;

	resolutionText.setText(resolutionNames[resolution]);

	buttonClick(buttonSound);
	return false;
}

bool OptionsMenu::changeFullscreen(bool value)
{
	fullscreen = value;
	windowManager->setFullscreen(fullscreen);
	buttonClick(buttonSound);
	return false;
}

bool OptionsMenu::changeSoundVolume()
{
	volumeText.setText(std::to_string((int)(volumeScroll.getScrollPositionScrollBar() * MAX_VOLUME + 0.5)));
	soundSystem->setSoundEffectsVolume(volumeScroll.getScrollPositionScrollBar());
	buttonClick(buttonSound);
	return false;
}

bool OptionsMenu::changeMusicVolume()
{
	musicText.setText(std::to_string((int)(musicScroll.getScrollPositionScrollBar() * MAX_VOLUME + 0.5)));
	soundSystem->setMusicVolume(musicScroll.getScrollPositionScrollBar());
	buttonClick(buttonSound);
	return false;
}

bool OptionsMenu::changeGamma()
{
	gammaText.setText(std::to_string((int)(gammaScroll.getScrollPositionScrollBar() * MAX_GAMMA + 0.5)));
	renderSystem->changeParamOfShader("LuminancePS", "brigh", gammaScroll.getScrollPositionScrollBar());
	windowManager->setBrightness(gammaScroll.getScrollPositionScrollBar());
	buttonClick(buttonSound);
	return false;
}

OptionsMenu::OptionsMenu(GameObject* gameObject) : Menu(gameObject), resolutionButton(NULL), volumeScroll(NULL), musicScroll(NULL), gammaScroll(NULL), interfaceSystem(nullptr), windowManager(nullptr), renderSystem(nullptr), soundSystem(nullptr),
resolutionText(NULL), volumeText(NULL), musicText(NULL), gammaText(NULL), root(NULL),checkbox(NULL)
{
	interfaceSystem = InterfaceSystem::GetInstance();
	interfaceSystem->registerEvent("-resolutionButtonClick", UIEvent("ButtonClicked", [this]() {return changeResolution(-1); }));
	interfaceSystem->registerEvent("+resolutionButtonClick", UIEvent("ButtonClicked", [this]() {return changeResolution(+1); }));

	interfaceSystem->registerEvent("fullscreenYesButtonClick", UIEvent("ToggleClicked", [this]() {return changeFullscreen(!fullscreen); }));

	interfaceSystem->registerEvent("volumeScrollChange", UIEvent("ScrollChange", [this]() {return changeSoundVolume(); }));
	interfaceSystem->registerEvent("volumeMusicScrollChange", UIEvent("ScrollChange", [this]() {return changeMusicVolume(); }));
	interfaceSystem->registerEvent("gammaScrollChange", UIEvent("ScrollChange", [this]() {return changeGamma(); }));

	interfaceSystem->registerEvent("resolutionApplyButtonClick", UIEvent("ButtonClicked", [this]() {return resolutionButtonClick(); }));
	interfaceSystem->registerEvent("resetConfigurationsButtonClick", UIEvent("ButtonClicked", [this]() {return resetConfigButtonClick(); }));
	interfaceSystem->registerEvent("backButtonClick", UIEvent("ButtonClicked", [this]() {return backButtonClick(); }));
	currResolution = -1;

	windowManager = WindowManager::GetInstance();
	renderSystem = RenderSystem::GetInstance();
	soundSystem = SoundSystem::GetInstance();

	resolutionNames = windowManager->getAvailableResolutionsStrings();
	resolutions = windowManager->getAvailableResolutionsForWindow();
}

OptionsMenu::~OptionsMenu()
{
	interfaceSystem->unregisterEvent("-resolutionButtonClick");
	interfaceSystem->unregisterEvent("+resolutionButtonClick");

	interfaceSystem->unregisterEvent("fullscreenYesButtonClick");

	interfaceSystem->unregisterEvent("volumeScrollChange");
	interfaceSystem->unregisterEvent("volumeMusicScrollChange");
	interfaceSystem->unregisterEvent("gammaScrollChange");

	interfaceSystem->unregisterEvent("resolutionApplyButtonClick");
	interfaceSystem->unregisterEvent("resetConfigurationsButtonClick");
	interfaceSystem->unregisterEvent("backButtonClick");
}

void OptionsMenu::start()
{
	Menu::start();
	root = findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("OptionsBackground");
	resolutionButton = root.getChild("ResolutionApplyButton");

	volumeScroll = root.getChild("SoundScroll");
	musicScroll = root.getChild("MusicScroll");
	gammaScroll = root.getChild("GammaScroll");
	resolutionText = root.getChild("Resolution");
	volumeText = root.getChild("SoundVolume");
	musicText = root.getChild("MusicVolume");
	gammaText = root.getChild("Gamma");
	checkbox = root.getChild("FullscreenYesButton");


	musicVolume = soundSystem->getMusicVolume();
	soundsVolume = soundSystem->getSoundVolume();
	gamma = windowManager->getBrightness();

	
	resolution = windowManager->getActualResolutionId();
	if (resolution == 0)
		resolution = 1;

	changeResolution(0);
	musicScroll.setScrollPositionScrollBar(musicVolume);
	volumeScroll.setScrollPositionScrollBar(soundsVolume);
	gammaScroll.setScrollPositionScrollBar(gamma);
	checkbox.setCheckBoxState(windowManager->getFullscreen());
}