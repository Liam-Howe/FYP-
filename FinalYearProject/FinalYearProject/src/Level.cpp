#include "../include/game.hpp"
#include "../include/Level.h"

Level::Level(Game* game)
{
	this->game = game;
	sf::Vector2f pos = sf::Vector2f(this->game->window.getSize());

	//sets up tersture position sizes and loads them in
	m_background.loadFromFile("assets/levelbackground.png");
	m_leveloneTexture.loadFromFile("assets/level1.png");
	m_levelTwoTexture.loadFromFile("assets/level2.png");
	m_levelThreeTexture.loadFromFile("assets/level3.png");
	//asigns the textures to the sprites
	m_leveloneSprite.setTexture(m_leveloneTexture);
	m_levelTwoSprite.setTexture(m_levelTwoTexture);
	m_levelThreeSprite.setTexture(m_levelThreeTexture);

	m_leveloneSprite.setPosition(sf::Vector2f(pos.x / 8, pos.y / 4));
	m_levelTwoSprite.setPosition(sf::Vector2f(pos.x / 8, pos.y / 3));
	m_levelThreeSprite.setPosition(sf::Vector2f(pos.x / 8, pos.y / 2));

	m_leveloneSprite.setOrigin(m_leveloneSprite.getGlobalBounds().width / 2, m_leveloneSprite.getGlobalBounds().height / 2);
	m_levelTwoSprite.setOrigin(m_levelTwoSprite.getGlobalBounds().width / 2, m_levelTwoSprite.getGlobalBounds().height / 2);
	m_levelThreeSprite.setOrigin(m_levelThreeSprite.getGlobalBounds().width / 2, m_levelThreeSprite.getGlobalBounds().height / 2);
}
void Level::draw()
{
	return;
}

void Level::update()
{
	game->window.clear();
	game->window.draw(m_leveloneSprite);
	game->window.draw(m_levelTwoSprite);
	game->window.draw(m_levelThreeSprite);

	m_Mouseposition = sf::Mouse::getPosition(game->window);
	game->window.display();
	//if the user clicks the left button whilst hovering over the level1,2,3  transition to the appropriate scene
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) == true)
	{
		if (checkClicked(m_leveloneSprite, m_Mouseposition) == true)
		{
			game->changeState(new Play(this->game,1));
		}
		else if (checkClicked(m_levelTwoSprite, m_Mouseposition) == true)
		{
			game->changeState(new Play(this->game,2));
		}
		else if (checkClicked(m_levelThreeSprite, m_Mouseposition) == true)
		{
			game->changeState(new Play(this->game,3));
		}
	}



	return;
}

Level::~Level()
{

	//while (!this->states.empty()) popState();
}

void Level::handleInput()
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
			if (event.key.code == sf::Keyboard::Escape) this->game->window.close();
			{

			}
			break;
		}
		default: break;
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		this->game->changeState(new Play(this->game,1));
	}

	return;
}

bool Level::checkClicked(sf::Sprite sprite, sf::Vector2i position)
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
