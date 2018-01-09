#pragma once

#include "SFML/Graphics.hpp"
#include "Grid.h"
#include <math.h>
#include "SFML\Graphics\PrimitiveType.hpp"
#include "AStar.h"
#include "Dstar.h"
#include <chrono>
#include "../include/DStarLite.h"
using namespace std::chrono;
class AI
{
public:

	AI(sf::Vector2f pos,int size, std::vector<std::vector<Grid>>&);
	~AI();

	//updates the position of the AI
	 //void update(DstarLite& dStarLite, Grid * goalNode, AStar & aStar, Dstar& dStar, int, sf::Text& pathfindingTimeText,sf::Int32,sf::Text&);
	void update(Grid * goalNode, AStar & aStar, DStarLite& dStar, int&, sf::Text& pathfindingTimeText, sf::Int32&, sf::Text&, sf::Text& nodesVisitedText, int& nodesVisited ,sf::Text& pathLengthText);
	
	 //set methods
	//sets wehter or not the ai should be moving
	 void setShouldMove(bool value);
	 //sets the current index of the ai along the [ath
	 void setIndex(int val);
	 // sets if A star is the currently executed algorithm
	 void setIsAStar(bool);
	 // sets if D star Lite is the currently executed algorithm
	 void setIsDStarLite(bool);
	 // sets if Altered D star is the currently executed algorithm
	 void setIsAlteredDStarLite(bool);
	 // // sets the path for the AI to traverse along
	 void setPath(std::vector<Grid*>);
	 // // sets the  altered path for the AI to traverse along
	 void setAlteredPath(std::vector<Grid*>);
	 // // sets the position of the AI
	 void setPosition(sf::Vector2f);
	
	 //get methods
	 bool getIsAStar();
	 bool getIsDStarLite();
	 bool getIsAlteredDstarLite();
	 int getCol();
	 int getRow();
	 bool getShouldMove();
	 sf::RectangleShape& getRect();
	 std::vector<Grid*> getPath();
	 std::vector<Grid*> getAlteredPath();
	 int getPathLength();
	 void setPathLength(int);
private:
	//AI rectagnle shape
	sf::RectangleShape m_rect;
	//position of the the AI
	sf::Vector2f m_position;
	//path returned by executing the pathfinding algorithms
	std::vector<Grid*> m_path;
	std::vector<Grid*> m_alteredPath;
	//wether or not the AI should move
	bool m_shouldMove;
	sf::Clock m_clock;
	float m_time;
	//current index of the ai along the gnereated path
	int m_index;
	//row at which the ai is at on the grid
	int m_row;
	//coloumn at which the ai is at on the grid
	int m_col;
	//wether or not the folowing algorithms are active
	bool m_isDStarLite;
	bool m_isAStar;
	bool m_isAlteredDStar;
	//refference to he grid
	std::vector<std::vector<Grid>>& m_tiles;
	
	int m_count;
	int m_nextTraversable;
	//timers fro calulating the time of path generation
	high_resolution_clock::time_point t1;
	high_resolution_clock::time_point t2; 
	int m_deleteCount;
	int m_pathLength;
};

