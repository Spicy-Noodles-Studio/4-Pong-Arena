#pragma once
#ifndef PLAYER_INDEX_H
#define PLAYER_INDEX_H

#include <UserComponent.h>

class PlayerIndex : public UserComponent
{
private:
	int id;
public:
	PlayerIndex(GameObject* gameObject);
	virtual ~PlayerIndex();

	virtual void handleData(ComponentData* data);

	int getId();
	void setId(int ind);
};
#endif

