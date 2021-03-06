#pragma once
#ifndef PLAYER_INDEX_H
#define PLAYER_INDEX_H

#include <UserComponent.h>

class PlayerIndex : public UserComponent
{
private:
	int id;
	int pos;
public:
	PlayerIndex(GameObject* gameObject);
	virtual ~PlayerIndex();
	int getId() const;
	void setId(int ind);

	int getPosVector() const;
	void setPos(int posVector);

protected:
	virtual void handleData(ComponentData* data);
};
#endif

