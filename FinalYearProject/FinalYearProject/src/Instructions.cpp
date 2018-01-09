#include "../include/game.hpp"
#include "../include/Instructions.h"

Instructions::Instructions(Game* game)
{
	this->game = game;
	//sets texture and sprite positions
	sf::Vector2f pos = sf::Vector2f(this->game->window.getSize());
	m_background.loadFromFile("assets/instructionsbackground.png");
	
	m_backButtonTexture.loadFromFile("assets/back.png");

	m_backSprite.setTexture(m_backButtonTexture);
	m_backgroundSprite.setTexture(m_background);
	m_backgroundSprite.setPosition(sf::Vector2f(0, 0));
	
	m_backSprite.setPosition(sf::Vector2f(pos.x / 20, pos.y / 1.2));
}
void Instructions::draw()
{
	return;
}

void Instructions::update()
{
	game->window.clear();
	game->window.draw(m_backgroundSprite);

	game->window.draw(m_backSprite);
	m_Mouseposition = sf::Mouse::getPosition(game->window);
	game->window.display();

	//if the user clicks the left button whilst hovering over the back button return to the menu
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) == true)
	{
		if (checkClicked(m_backSprite, m_Mouseposition) == true)
		{
			game->changeState(new Menu(this->game));
		}
	}



	return;
}

Instructions::~Instructions()
{
}

void Instructions::handleInput()
{
	sf::Event event;

	while (this->game->window.pollEvent(event))
	{
		switch (event.type)
		{
			/* Close the window */
		case sf::Event::Closed:
		{
			game->window.close();
			break;
		}
		/* Resize the window */

		case sf::Event::KeyPressed:
		{
			if (event.key.code == sf::Keyboard::Escape) 
			{
				this->game->window.close();
			}
			break;
		}
		default: break;
		}
	}

	return;
}

bool Instructions::checkClicked(sf::Sprite sprite, sf::Vector2i position)
{
	//returns true if the mouse posiiton is within the bounds of the button
	if (position.x > sprite.getGlobalBounds().left
		&& position.x < (sprite.getGlobalBounds().left + sprite.getGlobalBounds().width)
		&& position.y > sprite.getGlobalBounds().top
		&& position.y < (sprite.getGlobalBounds().top + sprite.getGlobalBounds().height))
	{
		return true;
	}
	else
	{
		return false;
	}
}
