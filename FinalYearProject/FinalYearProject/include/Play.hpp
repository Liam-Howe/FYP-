#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "game_state.hpp"
#include "Camera.h"
#include "Grid.h"
#include "DStarLite.h"
#include "AI.h"
#include "AStar.h"
#include "SoundManager.h"

#include <chrono>

using namespace std;
using namespace std::chrono;
class Play : public GameState
{
private:
	//grid container
	vector<vector<Grid>> m_grid;

	//poistion of mouse in pixels
	sf::Vector2i m_mousePixelPos;
	//position of mouse in world co-ordinates
	sf::Vector2f m_mouseWorldPos;
	
	//camera pointer
	Camera*  m_camera;
	//AI pointer
	AI* m_ai;
	//D star lite pointer
	DStarLite* m_dStar;
	// A star pointer
	AStar* m_aStar;

	//size of a node
	int m_tileSize;
	//size of the grdi's coloumn and row
	int m_gridSize;
	
	//start node x pos
	int m_xStart;
	//start nodes y pos
	int m_yStart;
	//goal nodes x pos
	int m_xEnd;
	//goal nodes y pos
	int m_yEnd;
	sf::Event *m_Event;

	//camera tiles to render
	int m_renderStartX;
	int m_renderEndX;
	int m_renderStartY;
	int m_renderEndY;

	//Text
	sf::Text m_text;
	sf::Font m_font;

	//name of the current Algorithm text
	sf::Text m_algorithmNameText;
	//time taken text
	sf::Text m_timeTakenText;
	//number of optimal nodes calulated text
	sf::Text m_optimalNodeCountText;
	//length of the path text
	sf::Text m_pathLengthText;
	//number of nodes visited text
	sf::Text m_nodesvisitedText;
	//d star lite string
	sf::String m_dStarLiteString;
	//Altered d star lite string
	sf::String m_alteredDstarLiteString;
	//A star string
	sf::String m_aStarString;
	//number of optimal nodes calulated
	int m_optimalNodeCount;
	//timer for algorithms to execute
	sf::Int32 m_timer;
	//duration of the time taken for the alogrithms to execute
	int m_pathfindingDuration;

	//vector of added non traversable nodes
	std::vector<Grid*> m_walls;

	int m_level;

	//number of nodes visited thorughout the AI travelling
	int m_nodesVisited;
	//legth of the path the ai traversed
	int m_pathLength;
public:
	//initialises start and end nodes
	void  setup();
	//creates the grid
	void loadLevel(int level, vector<vector<Grid>>& m_grid, int& m_gridSize);
	//updates the game loop
	virtual void update();

	//checks for user input
	virtual void handleInput();

	virtual void draw();
	~Play();
	
	Play(Game* game,int);
};

