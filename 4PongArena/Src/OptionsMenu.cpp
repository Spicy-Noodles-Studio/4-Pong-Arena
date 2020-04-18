#include "OptionsMenu.h"
#include <InterfaceSystem.h>
#include <RenderSystem.h>
#include <SoundSystem.h>
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
	return false;
}

bool OptionsMenu::resolutionButtonClick()
{
	if (currResolution != resolution)
	{
		if (fullscreen)
		{
			RenderSystem::GetInstance()->setFullscreen(false);
			RenderSystem::GetInstance()->windowResize(RESOLUTIONS[resolution].first, RESOLUTIONS[resolution].second);
			RenderSystem::GetInstance()->setFullscreen(true);
			currResolution = resolution;
		}
		else
		{
			RenderSystem::GetInstance()->windowResize(RESOLUTIONS[resolution].first, RESOLUTIONS[resolution].second);
			currResolution = resolution;
		}
	}
	
	return false;
}

bool OptionsMenu::changeResolution(int value)
{
	resolution += value;

	if (resolution < 0) resolution = 0;
	if (resolution > resolutionNames.size() - 1) resolution = resolutionNames.size() - 1;

	findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("Resolution").setText(resolutionNames[resolution]);

	return false;
}

bool OptionsMenu::changeFullscreen(bool value)
{
	fullscreen = value;
	RenderSystem::GetInstance()->setFullscreen(fullscreen);

	return false;
}

bool OptionsMenu::changeSoundVolume()
{
	findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("SoundVolume").setText(std::to_string((int)(volumeScroll.getScrollPositionScrollBar() *MAX_VOLUME+0.5)));
	SoundSystem::GetInstance()->setSoundEffectsVolume(volumeScroll.getScrollPositionScrollBar());
	
	return false;
}

bool OptionsMenu::changeMusicVolume()
{
	findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("MusicVolume").setText(std::to_string((int)(musicScroll.getScrollPositionScrollBar() * MAX_VOLUME+0.5)));
	SoundSystem::GetInstance()->setMusicVolume(musicScroll.getScrollPositionScrollBar());

	return false;
}

bool OptionsMenu::changeGamma()
{
	findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("Gamma").setText(std::to_string((int)(gammaScroll.getScrollPositionScrollBar() * MAX_GAMMA +0.5)));
	RenderSystem::GetInstance()->changeParamOfShader("LuminancePS", "brigh", gammaScroll.getScrollPositionScrollBar());

	return false;
}

OptionsMenu::OptionsMenu(GameObject* gameObject) : UserComponent(gameObject), resolutionButton(NULL), volumeScroll(NULL),musicScroll(NULL),gammaScroll(NULL)
{
	InterfaceSystem::GetInstance()->registerEvent("-resolutionButtonClick", UIEvent("ButtonClicked", [this]() {return changeResolution(-1); }));
	InterfaceSystem::GetInstance()->registerEvent("+resolutionButtonClick", UIEvent("ButtonClicked", [this]() {return changeResolution(+1); }));

	InterfaceSystem::GetInstance()->registerEvent("fullscreenYesButtonClick", UIEvent("ToggleClicked", [this]() {return changeFullscreen(!fullscreen); }));

	InterfaceSystem::GetInstance()->registerEvent("volumeScrollChange", UIEvent("ScrollChange", [this]() {return changeSoundVolume(); }));
	InterfaceSystem::GetInstance()->registerEvent("volumeMusicScrollChange", UIEvent("ScrollChange", [this]() {return changeMusicVolume(); }));
	InterfaceSystem::GetInstance()->registerEvent("gammaScrollChange", UIEvent("ScrollChange", [this]() {return changeGamma(); }));

	InterfaceSystem::GetInstance()->registerEvent("resolutionApplyButtonClick", UIEvent("ButtonClicked", [this]() {return resolutionButtonClick(); }));
	InterfaceSystem::GetInstance()->registerEvent("backButtonClick", UIEvent("ButtonClicked", [this]() {return backButtonClick(); }));
	currResolution = -1;
}

OptionsMenu::~OptionsMenu()
{
	InterfaceSystem::GetInstance()->unregisterEvent("-resolutionButtonClick");
	InterfaceSystem::GetInstance()->unregisterEvent("+resolutionButtonClick");

	InterfaceSystem::GetInstance()->unregisterEvent("fullscreenYesButtonClick");

	InterfaceSystem::GetInstance()->unregisterEvent("volumeScrollChange");
	InterfaceSystem::GetInstance()->unregisterEvent("volumeMusicScrollChange");
	InterfaceSystem::GetInstance()->unregisterEvent("gammaScrollChange");

	InterfaceSystem::GetInstance()->unregisterEvent("resolutionApplyButtonClick");
	InterfaceSystem::GetInstance()->unregisterEvent("backButtonClick");
}

void OptionsMenu::start()
{
	resolutionButton = findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("ResolutionApplyButton");
	
	volumeScroll = findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("SoundScroll");
	musicScroll= findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("MusicScroll");
	gammaScroll= findGameObjectWithName("MainCamera")->getComponent<UILayout>()->getRoot().getChild("GammaScroll");

	musicVolume = 100;
	soundsVolume = 100;
	gamma = 100;

	resolution = 6;
	fullscreen = false;
}