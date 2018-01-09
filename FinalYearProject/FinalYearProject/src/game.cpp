#include "../include/game_state.hpp"

void Game::pushState(GameState* state)
{
	this->states.push(state);
	return;
}


void Game::popState()
{
	delete this->states.top();
	this->states.pop();
	return;
}

void Game::changeState(GameState* state)
{
	if (!this->states.empty())
		popState();
	pushState(state);
	return;
}

GameState* Game::peekState()
{
	if (this->states.empty()) return nullptr;
	return this->states.top();
}

void Game::gameLoop()
{
	sf::Clock clock;
	while (this->window.isOpen())
	{
		sf::Time elapsed = clock.restart();
		if (peekState() == nullptr) continue;
		peekState()->handleInput();
		peekState()->update();
		peekState()->draw();
	}
}

Game::Game()
{
	this->window.create(sf::VideoMode(800, 600), "FYP");//, sf::Style::Fullscreen);
}

Game::~Game()
{
	while (!this->states.empty()) popState();
}