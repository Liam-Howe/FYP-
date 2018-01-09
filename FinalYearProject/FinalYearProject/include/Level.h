#pragma once
#include <SFML/Graphics.hpp>
#include "game_state.hpp"
#include "game.hpp"
#include "Play.hpp"


class Level : public GameState
{
private:
	//postion of mouse on screen and window
	sf::Vector2i windowPosition;
	sf::Vector2i m_Mouseposition;
	//background texture
	sf::Texture m_background;
	
	//level textures
	sf::Texture m_leveloneTexture;
	sf::Texture m_levelTwoTexture;
	sf::Texture m_levelThreeTexture;
	//level sprites
	sf::Sprite m_leveloneSprite;
	sf::Sprite m_levelTwoSprite;
	sf::Sprite	m_levelThreeSprite;

public:
	//inherited functions 
	virtual void draw();
	virtual void update();
	virtual void handleInput();
	//checks to see if the button has been clicked
	bool checkClicked(sf::Sprite sprite, sf::Vector2i pos);
	~Level();
	Level(Game* game);
};
