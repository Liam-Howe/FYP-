#include "../include/AStar.h"
#include <iostream>


/// <summary>
/// AStar class consturctior assigns m_grid with a refference to the demo's grid
/// </summary>
/// <returns></returns>
AStar::AStar(std::vector<std::vector<Grid>>& tiles) : m_grid(tiles)
{
}

AStar::~AStar()
{
}

/// <summary>
/// Calculates the most optimal path from a start node to a goal node via the A star algorithm
/// </summary>
/// <param name="firstNode">Start Node</param>
/// <param name="endNode">The goal node</param>
/// <returns></returns>
std::vector<Grid*> AStar::AStarAlgorithm(Grid& firstNode, Grid& endNode)
{//reset previous search calulations
	m_count = 0;
	reset(firstNode);
	//set the current node to the start node
	currentNode = &firstNode;
	currentNode->setGCost(0);
	
	goalNode = &endNode;
	//calculate the current nodes heuristic
	calculateHeurisitc(currentNode);
	//calculate the current nodes F cost
	currentNode->setFCost(currentNode->getGCost() + currentNode->getHCost());
	m_openQueue.push(std::make_pair(std::make_pair(currentNode->getRow(), currentNode->getCol()), currentNode->getFCost()));
	currentNode->setOpen(true);
	m_openNodes.push_back(currentNode);

	while (m_openQueue.size() != 0)
	{
		int row = m_openQueue.top().first.first;
		int col = m_openQueue.top().first.second;
		//set the current node to be the lowest f csot form the priority queue
		currentNode = &m_grid[row][col];
		m_openQueue.pop();

		//if at the goal node from the path
		if (currentNode == goalNode)
		{
			formPath(&firstNode, &endNode);
			return m_path;	
		}
		//swap the node to be on the closed list
		currentNode->setOpen(false);
		currentNode->setClosed(true);
		//calculate all surrounding nodes of the current node
		calculateSurroundingNodes();

		for (int  i = 0; i <  m_neighbourNodes.size(); i++)
		{
			//increment visited node count
			m_count++;
			//if not moving horizontally add 1 to the g cost
			float tentGCost = 0;
			if (m_neighbourNodes[i]->getIsDiagonal() == false)
			{
			 tentGCost = currentNode->getGCost() + 1;
			}
			//if  moving horizontally add 1.4 to the g cost
			else if (m_neighbourNodes[i]->getIsDiagonal() == true)
			{
				tentGCost = currentNode->getGCost() + 1.4f;
			} 
			//compare this g cost with the successor nodes g cost
			if (tentGCost <= m_neighbourNodes[i]->getGCost())
			{
				//if this path ism ore optimal update the previous node and re calculate the g and f costs
				m_neighbourNodes[i]->setPrevious(currentNode);
				m_neighbourNodes[i]->setGCost(tentGCost);
				m_neighbourNodes[i]->setFCost(m_neighbourNodes[i]->getGCost() + calculateHeurisitc(m_neighbourNodes[i]));
			}
			//if not yet been investigated add it to the priority queue
			if (m_neighbourNodes[i]->getOpen() == false)
			{
				
				m_neighbourNodes[i]->setOpen(true);
				if (m_neighbourNodes[i]->getType() != TileType::END &&
					m_neighbourNodes[i]->getType() != TileType::OLDPATH)
				{
					m_neighbourNodes[i]->setType(TileType::OPEN);
				}
				m_openQueue.push(std::make_pair(std::make_pair(m_neighbourNodes[i]->getRow(), m_neighbourNodes[i]->getCol()), m_neighbourNodes[i]->getFCost()));
				m_openNodes.push_back(m_neighbourNodes[i]);
			}
		}
		m_neighbourNodes.clear();

	}

	if (m_openQueue.size() ==0)
	{
		std::cout << "couldnt find a path" << std::endl;
		return std::vector<Grid*>();
	}
	


	return m_closedNodes;
}
/// <summary>
/// Caluclates the euciladean distance between nodes and the goal node
/// </summary>
/// <returns></returns>
float  AStar::calculateHeurisitc(Grid * node)
{
	
	//calcualte the eiciladean diistance
	float goalXPos = goalNode->getCol();
	float goalYPos = goalNode->getRow();
	
	float dx = abs(node->getCol() - goalXPos);
	float dy = abs(node->getRow() - goalYPos);
	float distance = (dx * dx + dy * dy);
	node->setHCost(distance);
	return distance;
}
/// <summary>
/// Calculates the surrounding nodes in relation to the current node and adds them back to the open list, if 
/// it is not a wall or it is not already added to the open list
/// </summary>
/// <param name="currentNode">Represents the current node</param>
/// <param name="endNode">The goal node</param>
/// <returns></returns>
void AStar::calculateSurroundingNodes()
{
	//if within the grids bounds and the node is traversable and its not already on the closed list or the open list add it to the neighbours list
	if (currentNode->getRow() <m_grid.size() - 1)
	{
		if (m_grid[currentNode->getRow() + 1][currentNode->getCol()].getTraversable() &&
			m_grid[currentNode->getRow() + 1][currentNode->getCol()].getClosed() == false &&
			m_grid[currentNode->getRow() + 1][currentNode->getCol() ].getOpen() == false &&
			&m_grid[currentNode->getRow() + 1][currentNode->getCol()] != currentNode->getPrevious())
		{
			m_neighbourNodes.push_back(&m_grid[currentNode->getRow() + 1][currentNode->getCol()]);
		}
	}
	if (currentNode->getRow() > 0)
	{
		if (m_grid[currentNode->getRow() - 1][currentNode->getCol()].getTraversable() &&
			m_grid[currentNode->getRow() - 1][currentNode->getCol()].getClosed() == false &&
			m_grid[currentNode->getRow() - 1][currentNode->getCol()].getOpen() == false &&
			&m_grid[currentNode->getRow() - 1][currentNode->getCol()] != currentNode->getPrevious())
		{
			m_neighbourNodes.push_back(&m_grid[currentNode->getRow() - 1][currentNode->getCol()]);
		}
	}
	if (currentNode->getCol() > 0)
	{
		if (m_grid[currentNode->getRow()][currentNode->getCol() - 1].getTraversable() &&
			m_grid[currentNode->getRow()][currentNode->getCol() - 1].getClosed() == false &&
			m_grid[currentNode->getRow()][currentNode->getCol() - 1].getOpen() == false &&
			&m_grid[currentNode->getRow()][currentNode->getCol() - 1] != currentNode->getPrevious())
		{
			m_neighbourNodes.push_back(&m_grid[currentNode->getRow()][currentNode->getCol() - 1]);
		}
	}
	if (currentNode->getCol() < m_grid.size() - 1)
	{
		if (m_grid[currentNode->getRow()][currentNode->getCol() + 1].getTraversable() &&
			m_grid[currentNode->getRow()][currentNode->getCol() + 1].getClosed() == false &&
			m_grid[currentNode->getRow()][currentNode->getCol() + 1].getOpen() == false &&
			&m_grid[currentNode->getRow()][currentNode->getCol() + 1] != currentNode->getPrevious())
		{
			m_neighbourNodes.push_back(&m_grid[currentNode->getRow()][currentNode->getCol() + 1]);
		}
	}
	if (currentNode->getRow() < m_grid.size() - 1 && currentNode->getCol() < m_grid.size() - 1)
	{
		if (m_grid[currentNode->getRow() + 1][currentNode->getCol() + 1].getTraversable() &&
			m_grid[currentNode->getRow() + 1][currentNode->getCol() + 1].getClosed() == false &&
			m_grid[currentNode->getRow() + 1][currentNode->getCol() + 1].getOpen() == false &&
			&m_grid[currentNode->getRow() + 1][currentNode->getCol() + 1] != currentNode->getPrevious())
		{
			m_neighbourNodes.push_back(&m_grid[currentNode->getRow() + 1][currentNode->getCol() + 1]);
			m_grid[currentNode->getRow() + 1][currentNode->getCol() + 1].setIsDiagonal(true);
		}
	}

	if (currentNode->getRow() > 0 && currentNode->getCol() > 0)
	{
		if (m_grid[currentNode->getRow() - 1][currentNode->getCol() - 1].getTraversable() &&
			m_grid[currentNode->getRow() - 1][currentNode->getCol() - 1].getClosed() == false &&
			m_grid[currentNode->getRow() - 1][currentNode->getCol() - 1].getOpen() == false &&
			&m_grid[currentNode->getRow() - 1][currentNode->getCol() - 1] != currentNode->getPrevious())
		{
			m_neighbourNodes.push_back(&m_grid[currentNode->getRow() - 1][currentNode->getCol() - 1]);
			m_grid[currentNode->getRow() - 1][currentNode->getCol() - 1].setIsDiagonal(true);
		}
	}
	if (currentNode->getRow() < m_grid.size() - 1 && currentNode->getCol() > 0)
	{
		if (m_grid[currentNode->getRow() + 1][currentNode->getCol() - 1].getTraversable() &&
			m_grid[currentNode->getRow() + 1][currentNode->getCol() - 1].getClosed() == false &&
			m_grid[currentNode->getRow() + 1][currentNode->getCol() - 1].getOpen() == false &&
			&m_grid[currentNode->getRow() + 1][currentNode->getCol() - 1] != currentNode->getPrevious())
		{
			m_neighbourNodes.push_back(&m_grid[currentNode->getRow() + 1][currentNode->getCol() - 1]);
			m_grid[currentNode->getRow() + 1][currentNode->getCol() - 1].setIsDiagonal(true);
		}
	}
	if (currentNode->getCol() < m_grid.size() - 1 && currentNode->getRow() > 0)
	{
		if (m_grid[currentNode->getRow() - 1][currentNode->getCol() + 1].getTraversable() &&
			m_grid[currentNode->getRow() - 1][currentNode->getCol() + 1].getClosed() == false &&
			m_grid[currentNode->getRow() - 1][currentNode->getCol() + 1].getOpen() == false &&
			&m_grid[currentNode->getRow() - 1][currentNode->getCol() + 1] != currentNode->getPrevious())
		{
			m_neighbourNodes.push_back(&m_grid[currentNode->getRow() - 1][currentNode->getCol() + 1]);
			m_grid[currentNode->getRow() - 1][currentNode->getCol() + 1].setIsDiagonal(true);
		}
	}
}

/// <summary>
/// stores all optimal nodes chosen by A star and stores them into the path by reccursvely traversing to each nodes previous node
/// </summary>
/// <returns></returns>
void AStar::formPath(Grid* firstNode, Grid* endNode)
{
	for (Grid* i = endNode; i->getPrevious() != 0; i = i->getPrevious())
	{
		m_path.push_back(i);
		if (i->getType() != TileType::END)
		{
			i->setType(TileType::PATH);
		}		
	}
	 m_path.push_back(firstNode);
}
/// <summary>
/// resets all preiviously calculated information by A star
/// </summary>
/// <returns></returns>
void AStar::reset(Grid & node)
{
	//reset all previously calcualted information
	m_path.clear();
	m_closedNodes.clear();
	
	m_neighbourNodes.clear();
	while (!m_openQueue.empty())
	{
		m_openQueue.pop();
	}
	node.setGCost(1000);
	node.setHCost(1000);
	node.setFCost(1000);
	node.setOpen(false);
	node.setClosed(false);
	node.setPrevious(NULL);

	for (int i = 0; i < m_openNodes.size(); i++)
	{

			m_openNodes[i]->setGCost(1000);
			m_openNodes[i]->setHCost(1000);
			m_openNodes[i]->setFCost(1000);
			m_openNodes[i]->setOpen(false);
			m_openNodes[i]->setClosed(false);
			m_openNodes[i]->setPrevious(NULL);
	}
}
/// <summary>
/// Resets all node types that were calculated by a star
/// </summary>
/// <returns></returns>
void AStar::resetType()
{
	for (int i = 0; i < m_openNodes.size(); i++)
	{
		if (m_openNodes[i]->getType() == TileType::OPEN
			|| m_openNodes[i]->getType() == TileType::PATH
			|| m_openNodes[i]->getType() == TileType::OLDPATH)
		{
			m_openNodes[i]->setType(TileType::TILE);
		}
	}
}
/// <summary>
/// resets the amount of nodes a star has expanded
/// </summary>
/// <returns></returns>
void AStar::resetExpandednNodesCount()
{
	m_count = 0;
}
/// <summary>
/// returns the amount of nodes visited by A star
/// </summary>
/// <returns></returns>
int AStar::getNodesVisited()
{
	return m_count;
}
/// <summary>
/// sets the amount of nodes a star has visited
/// </summary>
/// <returns></returns>
void AStar::setNodesVisited(int value)
{
	m_count = value;
}

 