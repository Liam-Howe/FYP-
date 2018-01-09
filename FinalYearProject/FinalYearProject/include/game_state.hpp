#pragma once

#include "game.hpp"

class GameState
{
//pure bastract class which contains necessary methods for inheriting classes to inherit
public:
	//Game instance
	Game* game;
	
	virtual void draw() = 0;
	virtual void update() = 0;
	virtual void handleInput() = 0;
};

