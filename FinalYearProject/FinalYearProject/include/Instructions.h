#pragma once
#include <SFML/Graphics.hpp>
#include "game_state.hpp"
#include "game.hpp"
#include "Menu.hpp"


class Instructions : public GameState
{
private:
	//position of mouse in window and pixels
	sf::Vector2i windowPosition;
	sf::Vector2i m_Mouseposition;
	//background texture
	sf::Texture m_background;
	//background sprite
	sf::Sprite m_backgroundSprite;
	//back button texture
	sf::Texture m_backButtonTexture;
	sf::Sprite m_backSprite;
	
public:
	//draws all textures for the instrucions scene
	virtual void draw();
	virtual void update();
	virtual void handleInput();
	//checks to see if the button has been clicked
	bool checkClicked(sf::Sprite sprite, sf::Vector2i pos);
	~Instructions();
	Instructions(Game* game);
};
