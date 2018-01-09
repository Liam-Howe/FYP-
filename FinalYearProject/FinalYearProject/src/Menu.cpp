#include "../include/game.hpp"
#include "../include/Play.hpp"
#include "../include/Menu.hpp"
Menu::Menu(Game* game)
{
	this->game = game;
	sf::Vector2f pos = sf::Vector2f(this->game->window.getSize());
	m_background.loadFromFile("assets/intro.png");
	m_demoButtonTexture.loadFromFile("assets/demo.png");
	m_instructionsButtonTexture.loadFromFile("assets/instructions.png");
	m_quitButtonTexture.loadFromFile("assets/quit.png");
	m_instructionsSprite.setTexture(m_instructionsButtonTexture);
	
	m_demoSprite.setTexture(m_demoButtonTexture);
	m_quitSprite.setTexture(m_quitButtonTexture);
	m_backgroundSprite.setTexture(m_background);
	m_backgroundSprite.setPosition(sf::Vector2f(0, 0));
	m_instructionsSprite.setPosition(sf::Vector2f(pos.x / 2, pos.y / 2));
	m_demoSprite.setPosition(sf::Vector2f(pos.x / 2, pos.y / 1.5));
	m_quitSprite.setPosition(sf::Vector2f(pos.x / 2, pos.y / 1.2));
	m_instructionsSprite.setOrigin(m_instructionsSprite.getGlobalBounds().width / 2, m_instructionsSprite.getGlobalBounds().height / 2);
	m_demoSprite.setOrigin(m_demoSprite.getGlobalBounds().width / 2, m_demoSprite.getGlobalBounds().height / 2);
	m_quitSprite.setOrigin(m_quitSprite.getGlobalBounds().width / 2, m_quitSprite.getGlobalBounds().height / 2);
}
void Menu::draw()
{
	return;
}

void Menu::update()
{
	game->window.draw(m_backgroundSprite);
	game->window.draw(m_instructionsSprite);
	game->window.draw(m_demoSprite);
	game->window.draw(m_quitSprite);
	m_Mouseposition = sf::Mouse::getPosition(game->window);
	game->window.display();
	
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) == true)
	{
		if (checkClicked(m_demoSprite, m_Mouseposition) == true)
		{
			game->changeState(new Level(this->game));
		}

		else if (checkClicked(m_instructionsSprite, m_Mouseposition)==true)
		{
			game->changeState(new Instructions(this->game));
		}
		else if (checkClicked(m_quitSprite, m_Mouseposition) == true)
		{
			game->window.close();
		}
	}



	return;	
}

Menu::~Menu()
{

}

void Menu::handleInput()
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

bool Menu::checkClicked(sf::Sprite sprite, sf::Vector2i position)
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
