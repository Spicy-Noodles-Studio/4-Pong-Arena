
#include <iostream>
#include "GaiaCore.h"

#include <ComponentManager.h>
#include "PlayerController.h"
#include "ForceField.h"
#include "WallManager.h"

#ifdef _DEBUG
int main()
#else
#include <Windows.h>
int WINAPI
WinMain(HINSTANCE hinstance, HINSTANCE prevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif
{
	ComponentManager::GetInstance()->registerComponent<PlayerController>("PlayerController");
	ComponentManager::GetInstance()->registerComponent<Health>("Health");
	ComponentManager::GetInstance()->registerComponent<WallManager>("WallManager");

	GaiaCore* g = new GaiaCore();
	g->init();
	g->run();
	g->close();
	
	delete g;

	return 0;
}