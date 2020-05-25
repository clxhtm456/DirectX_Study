#pragma once

#include "Unit.h"

class Player : public Unit
{
public:
	Player(ModelCreator* model);
	~Player();

	void Update();

private:
};