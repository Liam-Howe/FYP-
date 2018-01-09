#pragma once


#include <SFML/Graphics.hpp>
#include "game_state.hpp"
#include "game.hpp"
#include "Play.hpp"
#include "Instructions.h"
#include "Level.h"
class Menu : public GameState
{
private:
	//postion of mouse on screen and in the window
	sf::Vector2i windowPosition;
	sf::Vector2i m_Mouseposition;

	//menu scene textures
	sf::Texture m_demoButtonTexture;
	sf::Texture m_quitButtonTexture;
	sf::Texture m_instructionsButtonTexture;
	sf::Texture m_background;
	//menu scene Sprites
	sf::Sprite m_backgroundSprite;
	sf::Sprite m_instructionsSprite;
	sf::Sprite m_quitSprite;
	sf::Sprite m_demoSprite;
public:
	//inherited functions 
	virtual void draw();
	virtual void update();
	virtual void handleInput();
	//checks to see if the button has been clicked
	bool checkClicked(sf::Sprite sprite, sf::Vector2i pos);
	~Menu();
	Menu(Game* game);
};

