#include "../include/AI.h"
#include <iostream>

/// <summary>
/// initialises AI properties 
/// </summary>
/// <returns></returns>
AI::AI(sf::Vector2f pos, int size, std::vector<std::vector<Grid>>& tiles) : m_tiles(tiles)
{
	m_position = sf::Vector2f(pos.x, pos.y);
	m_rect.setPosition(m_position);
	m_rect.setSize(sf::Vector2f(size, size));
	m_rect.setFillColor(sf::Color::Magenta);
	m_rect.setOutlineColor(sf::Color(0, 0, 0));
	m_rect.setOutlineThickness(0.5f);
	m_index = 0;
	m_row = 0;
	m_col = 0;

	m_isDStarLite = false;
	m_isAStar = false;
	m_count = 0;
	m_deleteCount = 0;
}

AI::~AI()
{
}
/// <summary>
/// updates AI position based on which algorithm was executed
/// </summary>
/// <returns></returns>
void AI::update(Grid * goalNode, AStar & aStar, DStarLite& dstar, int& pathFindingTime, sf::Text& pathfindingTimeText, sf::Int32& optuimalNodesCalculatedCount, sf::Text& optimalNodesCalculatedText,sf::Text& nodesVisitedText, int& nodesVisited, sf::Text& pathLengthText)
{
	//if the ai dhould move
	if (m_shouldMove)
	{
		//if a star is the currently executed algorithm
		if (m_isAStar)
		{
			//incremnt the timer
			m_time += m_clock.restart().asSeconds();
			if (m_time > 1)
			{
				
				for (int j = m_index; j >= 0;)
				{
					if (j - 1 > 0)
					{ //if the next index in the path is un traversable
						if (m_path[j - 1]->getTraversable() == false)
						{
							for (int i = 0; i < m_path.size(); i++)
							{
								if (m_path[i]->getType() != TileType::START
									&& m_path[i]->getType() != TileType::END
									&& m_path[i]->getType() != TileType::WALL)
								{
									m_path[i]->setType(TileType::OLDPATH);
								}						
							}
							//record the current time 
							t1 = high_resolution_clock::now();
							//replan 
							setPath(aStar.AStarAlgorithm(*m_path[j], *goalNode));
							//record the new current time
							t2 = high_resolution_clock::now();
							//update testing criteria text
							optuimalNodesCalculatedCount += m_path.size() - 1;
							optimalNodesCalculatedText.setString("Optimal node count:" + to_string(optuimalNodesCalculatedCount));
							pathFindingTime += duration_cast<milliseconds>(t2 - t1).count();
							pathfindingTimeText.setString("Time Taken : " + to_string(pathFindingTime));
							nodesVisited += aStar.getNodesVisited();
							nodesVisitedText.setString("Nodes visited : " + to_string(nodesVisited));
							//set new index size
							m_index = m_path.size() -1;
							
							j = m_path.size() - 1;
						}
					}
					//set ai position
					if (m_path.size() ==0)
					{
						m_isAStar = false;
						break;
					}
					m_rect.setPosition(m_path[j]->getPosition());
					m_pathLength++;
					m_time = 0;
					m_index--;
				
					break;
				}
			}
		}
		//if d star lite is teh currently executed algorithm
		else if (m_isDStarLite)
		{
			m_time += m_clock.restart().asSeconds();
			if (m_time > 1)
			{
				for (int j = m_index; j < m_path.size();)
				{
					if (j + 1 < m_path.size())
					{
						//if the next index in the path is non-traversable
						if (m_path[j + 1]->getTraversable() == false)
						{
							for (int i = 0; i < m_path.size(); i++)
							{
								if (m_path[i]->getType() == TileType::PATH)
								{
									m_path[i]->setType(TileType::OLDPATH);
								}
							}
							//re - plan and update testing criteria
							dstar.setNodesVisited(0);
							t1 = high_resolution_clock::now();
							dstar.updateStartNode(m_path[j]->getCol(), m_path[j]->getRow());
							setPath(dstar.replan());
							t2 = high_resolution_clock::now();
							optuimalNodesCalculatedCount += m_path.size() - 1;
							optimalNodesCalculatedText.setString("Optimal node count:" + to_string(optuimalNodesCalculatedCount));
							pathFindingTime += duration_cast<milliseconds>(t2 - t1).count();
							pathfindingTimeText.setString("Time Taken : " + to_string(pathFindingTime));
							nodesVisited += dstar.getNodesVisited();
							nodesVisitedText.setString("Nodes visited : " + to_string(nodesVisited));
							m_index = 0;
							m_time = 0;
							break;
						}
					}
					//update aI position
					if (m_path.size() == 0)
					{
						m_isDStarLite = false;
						break;
					}
					m_rect.setPosition(m_path[j]->getPosition());
					m_row = m_path[j]->getRow();
					m_col = m_path[j]->getCol();
					m_time = 0;
					m_pathLength++;
					m_index++;
					break;
				}
			}
		}
		//if altered d star ltie is the currently executed algorithm
		else if (m_isAlteredDStar)
		{
			
			m_time += m_clock.restart().asSeconds();
			if (m_time >1)
			{
				for (int j = m_index; j < m_alteredPath.size();)
				{
					if (j + 1 < m_alteredPath.size())
					{
						//if the next index in the path is now untraversable
						if (m_alteredPath[j + 1]->getTraversable() == false)
						{
							for (int i = j + 1; j < m_alteredPath.size(); i++)
							{
								//get the next traversab;e mode in the path
								if (m_alteredPath[i]->getTraversable() == true)
								{
									m_nextTraversable = i;
									break;
								}
							}

							for (int i = j + 1; i < m_nextTraversable; i++)
							{//delte all the untraversable nodes form the path
								if (m_alteredPath[i]->getTraversable() == false)
								{
									m_alteredPath.erase(m_alteredPath.begin() + i);
									i--;
									m_deleteCount++;
								}

							}
							//get the index of the next traversable object in the path
							for (int i = j + 1; i < m_alteredPath.size(); i++)
							{
								m_nextTraversable = i;
								break;
							}
							//set old path
							for (int i = 0; i < m_alteredPath.size(); i++)
							{
								if (m_alteredPath[i]->getType() == TileType::PATH)
								{
									m_alteredPath[i]->setType(TileType::OLDPATH);
								}
							}
							//rplan to the next traversable node in the list and update the testing criteria
							dstar.setNodesVisited(0);
							t1 = high_resolution_clock::now();
							dstar.updateStartNode(m_alteredPath[m_index]->getCol(), m_alteredPath[m_index]->getRow());
							dstar.updateGoalNode(m_alteredPath[m_nextTraversable]->getCol(), m_alteredPath[m_nextTraversable]->getRow());
						    setPath(dstar.replan());
							t2 = high_resolution_clock::now();
							pathFindingTime += duration_cast<milliseconds> (t2 - t1).count();
							pathfindingTimeText.setString("Time Taken : " + to_string(pathFindingTime));
							
							for (int t = 0; t < m_path.size(); t++)
							{
								m_alteredPath.insert(m_alteredPath.begin() + m_index + t + 1, m_path[t]);
							}
							optuimalNodesCalculatedCount = m_alteredPath.size() - 1;
							optuimalNodesCalculatedCount += m_deleteCount;
							optimalNodesCalculatedText.setString("Optimal node count:" + to_string(optuimalNodesCalculatedCount));
							nodesVisited += dstar.getNodesVisited();
							nodesVisitedText.setString("Nodes visited : " + to_string(nodesVisited));
							m_time = 0;
							break;
						}
					}
					//update the ais postiion
					
					m_rect.setPosition(m_alteredPath[j]->getPosition());
					m_row = m_alteredPath[j]->getRow();
					m_col = m_alteredPath[j]->getCol();
					m_time = 0;
					m_pathLength++;
					m_index++;
					break;
				}
			}
		}
	}
	//if a star is is executed and the ai has reached the goal resets and display the path length
	if (m_isAStar && m_shouldMove && m_index == -1)
	{
		m_shouldMove = false;
		m_path.clear();
		m_index = 0;
		m_count = 0;
		pathLengthText.setString("Path Length : " + to_string(m_pathLength));
	}
	//if D star Lite is is executed and the ai has reached the goal resets and display the path length
	if (m_isAlteredDStar && m_shouldMove && m_index == m_alteredPath.size())
	{
		m_shouldMove = false;
		m_path.clear();
		m_index = 0;
		m_count = 0;
		pathLengthText.setString("Path Length : " + to_string(m_pathLength));
	}
	//if altered D star Lite is executed and the ai has reached the goal resets and display the path length
	else if (!m_isAlteredDStar && m_shouldMove && m_index == m_path.size() )
	{
		
		m_shouldMove = false;
		m_path.clear();
		m_index = 0;
		m_count = 0;
		pathLengthText.setString("Path Length : " + to_string(m_pathLength));
	}
	
}
/// <summary>
/// returns wether or not the AI should move
/// </summary>
/// <returns></returns>
bool AI::getShouldMove()
{
	return m_shouldMove;
}
/// <summary>
/// returns wether or not D Star Ltie is currently active
/// </summary>
/// <returns></returns>
bool AI::getIsDStarLite()
{
	return m_isDStarLite;
}
/// <summary>
/// returns wether or not altered D Star Ltie is currently active
/// </summary>
/// <returns></returns>
bool AI::getIsAlteredDstarLite()
{
	return m_isAlteredDStar;
}
/// <summary>
/// returns wether or not A Star is currently active
/// </summary>
/// <returns></returns>
bool AI::getIsAStar()
{
	return m_isAStar;
}
/// <summary>
/// returns the AI's rectangle
/// </summary>
/// <returns></returns>
sf::RectangleShape& AI::getRect()
{
	return m_rect;
}
/// <summary>
/// returns the optimal path
/// </summary>
/// <returns></returns>
std::vector<Grid*> AI::getPath()
{
	return m_path;
}
/// <summary>
/// returns the optimal altered path
/// </summary>
/// <returns></returns>
std::vector<Grid*> AI::getAlteredPath()
{
	return m_alteredPath;
}
int AI::getPathLength()
{
	return m_pathLength;
}
/// <summary>
/// sets the value of the path length
/// </summary>
/// <returns></returns>
void AI::setPathLength(int value)
{
	m_pathLength = value;
}
/// <summary>
/// returns the current row the AI is on
/// </summary>
/// <returns></returns>
int AI::getRow()
{
	return m_row;
}
/// <summary>
/// returns the current coloumn the AI is on
/// </summary>
/// <returns></returns>
int AI::getCol()
{
	return m_col;
}

/// <summary>
/// sets the index the ai is currently traversing in the paths vector
/// </summary>
/// <returns></returns>
void AI::setIndex(int val)
{
	m_index = val;
}
/// <summary>
/// sets if A star is the currently executed algorithm
/// </summary>
/// <returns></returns>
void AI::setIsAStar(bool value)
{
	m_isAStar = value;
}
/// <summary>
/// sets if D star Lite is the currently executed algorithm
/// </summary>
/// <returns></returns>
void AI::setIsDStarLite(bool value)
{
	m_isDStarLite = value;
}
/// <summary>
/// sets if Ai should move
/// </summary>
/// <returns></returns>
void AI::setShouldMove(bool value)
{
	m_shouldMove = value;	
}
/// <summary>
/// sets if altered D star Lite is the currently executed algorithm
/// </summary>
/// <returns></returns>
void AI::setIsAlteredDStarLite(bool value)
{
	m_isAlteredDStar = value;
}
/// <summary>
/// sets the current optimal path
/// </summary>
/// <returns></returns>
void AI::setPath(std::vector<Grid*> path)
{
	m_count += path.size() -1;
	m_path = path;
	std::cout << m_count << std::endl;
}
/// <summary>
/// sets the current optimal altered path
/// </summary>
/// <returns></returns>
void AI::setAlteredPath(std::vector<Grid*> path)
{
	m_alteredPath = path;
}
/// <summary>
/// sets the position of the AI
/// </summary>
/// <returns></returns>
void AI::setPosition(sf::Vector2f pos)
{
	m_position = pos;
}


