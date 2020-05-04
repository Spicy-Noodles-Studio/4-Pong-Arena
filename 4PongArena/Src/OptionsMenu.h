#pragma once
#ifndef OPTIONS_MENU_H
#define OPTIONS_MENU_H

#include <UserComponent.h>
#include <UIElement.h>

const int MAX_VOLUME = 100;
const int MAX_GAMMA = 100;



class InterfaceSystem;
class WindowManager;
class RenderSystem;
class SoundSystem;
class OptionsMenu : public UserComponent
{
protected:
	InterfaceSystem* interfaceSystem;
	WindowManager* windowManager;
	RenderSystem* renderSystem;
	SoundSystem* soundSystem;

	UIElement resolutionButton;
	UIElement volumeScroll;
	UIElement musicScroll;
	UIElement gammaScroll;

	UIElement resolutionText;
	UIElement volumeText;
	UIElement musicText;
	UIElement checkbox;
	UIElement gammaText;

	UIElement root;

	float musicVolume;
	float soundsVolume;
	float gamma;

	bool fullscreen;
	int resolution;
	int currResolution;

	std::vector<std::string> resolutionNames;
	std::vector<std::pair<int, int>> resolutions;
	bool resolutionButtonClick();
	bool backButtonClick();
	bool resetConfigButtonClick();

	bool changeResolution(int value);
	bool changeFullscreen(bool value);

	bool changeSoundVolume();
	bool changeMusicVolume();
	bool changeGamma();

public:
	OptionsMenu(GameObject* gameObject);
	virtual ~OptionsMenu();

	virtual void start();
};

#endif