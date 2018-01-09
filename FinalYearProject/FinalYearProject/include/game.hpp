#pragma once

#include <stack>
#include <SFML/Graphics.hpp>

class GameState;

class Game
{
public:
	//vector of states
	std::stack<GameState*> states;

	sf::RenderWindow window;
	//ads a new state to this vector
	void pushState(GameState* state);
	//deletes a state from the vector
	void popState();
	//ability to change states
	void changeState(GameState* state);
	GameState* peekState();

	void gameLoop();

	Game();
	~Game();
};

