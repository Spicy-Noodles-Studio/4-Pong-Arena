#include "CursorGame.h"
#include <InputSystem.h>
#include <GameObject.h>
#include <Cursor.h>

#include "GameManager.h"

#include <ComponentRegister.h>

REGISTER_FACTORY(CursorGame);

CursorGame::CursorGame(GameObject* gameObject) : UserComponent(gameObject), inputSystem(nullptr), cursor(nullptr), gameManager(nullptr)
{

}

CursorGame::~CursorGame()
{
	inputSystem = nullptr;
	cursor = nullptr;
	gameManager = nullptr;
}

void CursorGame::start()
{
	inputSystem = InputSystem::GetInstance();
	gameManager = GameManager::GetInstance();
	checkNull(inputSystem);
	checkNull(gameManager);

	if (notNull(gameObject)) cursor = gameObject->getComponent<Cursor>();

	checkNullAndBreak(cursor);
	cursor->setVisibleOnWindow(false);
	hideCursor();
}

void CursorGame::preUpdate(float deltaTime)
{
	if (isPaused()) {
		if (mouseUsed()) showCursor();
		else if (controllerUsed() || keyboardUsed()) hideCursor();
	}
	else
		hideCursor();
}

bool CursorGame::isPaused() const
{
	checkNullAndBreak(gameManager, false);
	return gameManager->isPaused();
}

bool CursorGame::mouseUsed() const
{
	checkNullAndBreak(inputSystem, false);

	return inputSystem->isMouseUsed();
}

bool CursorGame::keyboardUsed() const
{
	checkNullAndBreak(inputSystem, false);

	return inputSystem->isKeyboardUsed();
}

bool CursorGame::controllerUsed() const
{
	checkNullAndBreak(inputSystem, false);

	return inputSystem->isControllerUsed();
}

void CursorGame::hideCursor()
{
	checkNullAndBreak(cursor);
	cursor->setSpriteVisible(false);
}

void CursorGame::showCursor()
{
	checkNullAndBreak(cursor);
	cursor->setSpriteVisible(true);
}
