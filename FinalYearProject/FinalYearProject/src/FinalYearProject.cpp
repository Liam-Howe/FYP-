////////////////////////////////////////////////////////////
//// FinalYearProject.cpp : Defines the entry point for the console application.
//////////////////////////////////////////////////////////// 
#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 
#pragma comment(lib,"opengl32.lib") 
#pragma comment(lib,"glu32.lib") 

#include "SFML/Graphics.hpp" 
#include "SFML/OpenGL.hpp" 
#define _USE_MATH_DEFINES
#include "../include/Menu.hpp"
#include "../include/game.hpp"



int main()
{
	Game * game = new Game();
	game->changeState(new Menu(game));
	
	sf::Clock clock;
	//main update loop
	while (game->window.isOpen())
	{
		sf::Time elapsed = clock.restart();
	

		if (game->peekState() == nullptr)
		{
			continue;
		}
		game->peekState()->handleInput();
		game->peekState()->update();
		game->peekState()->draw();

	}
}

