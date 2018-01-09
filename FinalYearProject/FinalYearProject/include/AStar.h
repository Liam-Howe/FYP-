#pragma once
#include "SFML/Graphics.hpp"
#include "Grid.h"
#include "NodeSearchCostComparer.h"
#include <queue>
class AStar
{
public:
	AStar(std::vector<std::vector<Grid>>& tiles);
	~AStar();
	//A star algorithm function
	std::vector<Grid*>	AStarAlgorithm(Grid& startNode,Grid& endNode);
	//stores the surrounding node in relation to the current node
	void calculateSurroundingNodes();
	//calculates the eucildean distance from the current node to the goal node
	float calculateHeurisitc(Grid * node);
	//creates and optimal path
	void AStar::formPath(Grid* firstNode, Grid* endNode);
	//resetss previosuly calulated infromation
	void reset(Grid&);
	//resets the type of tiles
	void resetType();
	//resets the amount of nodes the algortihm investigated
	void resetExpandednNodesCount();
	//gets the amount of nodes the algortihm investigatede
	int getNodesVisited();
	//sets the amount of nodes the algortihm investigatede
	void setNodesVisited(int);

private:
	
	// vector containing the nodes that have been expanded by the A star algorithm
	std::vector<Grid*> m_openNodes;
	
	std::vector<Grid*> m_closedNodes;
	// vector containing the nodes chosen by the algorithm that represent the most optimal path
	std::vector<Grid*> m_path;
	//pointers to the current 
	Grid * currentNode;
	
	//pointer to the goal node
	Grid * goalNode;
	//refference to the grid
	std::vector<std::vector<Grid>>& m_grid;
	int m_count;
	//vector coantining the surrounding nodes
	std::vector<Grid*> m_neighbourNodes;
	//priority queue which orders by f cost
	std::priority_queue< std::pair<std::pair<float, float>, float>, vector<std::pair<std::pair<float, float>, float>>, NodeSearchCostComparer> m_openQueue;
	
};

