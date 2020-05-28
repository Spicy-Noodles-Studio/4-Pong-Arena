#pragma once
#ifndef CURSOR_GAME_H
#define CURSOR_GAME_H

#include <UserComponent.h>

class InputSystem;
class Cursor;
class GameManager;

class CursorGame : public UserComponent
{
public:
	CursorGame(GameObject* gameObject);
	virtual ~CursorGame();

private:
	bool isPaused() const;

	bool mouseUsed() const;
	bool keyboardUsed() const;
	bool controllerUsed() const;

	void hideCursor();
	void showCursor();

private:
	InputSystem* inputSystem;
	Cursor* cursor;
	GameManager* gameManager;

protected:
	void start();
	void preUpdate(float deltaTime);


};
#endif
