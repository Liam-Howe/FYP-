

#include "../include/DStarLite.h"
#include <iostream>

DStarLite::DStarLite(std::vector<std::vector<Grid>>& tiles)
	: m_tiles(tiles)
{

  m_maxSteps = 80000; 
  m_unseenCellScalar = 1;    

}

/// <summary>
/// returns the hash code for a node
/// </summary>
/// <returns></returns>
float DStarLite::getKeyHashCode(Node currentNode)
{
  return (float)(currentNode.getKeyValue().first + 993* currentNode.getKeyValue().second);
}
/// <summary>
/// checks if the current node is in the open list or not
/// </summary>
/// <returns></returns>
bool DStarLite::isOpen(Node currentNode)
{

  auto cur = m_openHash.find(currentNode);
  //if the curent node is not in the open hash return false
  if (cur == m_openHash.end())
  {
	  return false;
  }
  //if the key has code is not of a similar value to the current node // return false
  if (!close(getKeyHashCode(currentNode), cur->second))
  {
	  return false;
  }
  return true;

}
/// <summary>
/// gets the amount of nodes that the algorithm has visited
/// </summary>
/// <returns></returns>
int DStarLite::getNodesVisited()
{
	return m_count;
}
/// <summary>
/// sets the amount of nodes that the algorithm has visited
/// </summary>
/// <returns></returns>
void DStarLite::setNodesVisited(int value)
{
	m_count = value;
}
/// <summary>
/// returns wether or not a node is traversable or not
/// </summary>
/// <returns></returns>
bool DStarLite::isUnTraversable(Node currentNode)
{

  auto cur = m_cellHash.find(currentNode);

  if (cur == m_cellHash.end())
  {
	  return false;
  }
  //if the current nodes cost is set as untraversable return true
  return (cur->second.m_costTraversalValue < 0);
}

/// <summary>
/// initialises the start and goal node values
/// </summary>
/// <returns></returns>
void DStarLite::init(int startCol, int startRow, int goalCol, int goalRow)
{
	//clear containers
  m_openHash.clear();
  m_cellHash.clear();
 
  while (!m_openList.empty())
  {
	  m_openList.pop();
  }
  m_incrementalHeuristicK = 0;
  //set start node position
  m_start.setCol(startCol);
  m_start.setRow(startRow);
  m_goal.setCol(goalCol);
  m_goal.setRow(goalRow);

  //set goal node calculation properties
  NodeValue tmp;
  tmp.m_gValue = 0;
  tmp.m_rhsValue =  0;
  tmp.m_costTraversalValue = m_unseenCellScalar;

  m_cellHash[m_goal] = tmp;

  //calculate the start node , g ,h nd trversalCost values
  tmp.m_gValue = calculateHeuristic(m_start, m_goal);
  tmp.m_rhsValue = calculateHeuristic(m_start,m_goal);
  tmp.m_costTraversalValue = m_unseenCellScalar;
  m_cellHash[m_start] = tmp;
  m_start = calculateKey(m_start);

  //update the status of the last node
  m_last = m_start;

}
/// <summary>
/// creates the values for a new node depending on if it has already been added or not
/// </summary>
/// <returns></returns>
void DStarLite::newNode(Node currentNode)
{
	//if the node is already stored return
	if (m_cellHash.find(currentNode) != m_cellHash.end())
	{
		return;
	}
	//otherwise calculate the propeties for the node
  NodeValue tmp;
  tmp.m_gValue   = calculateHeuristic(currentNode, m_goal);
   tmp.m_rhsValue = calculateHeuristic(currentNode, m_goal);
  tmp.m_costTraversalValue    = m_unseenCellScalar;
  m_cellHash[currentNode] = tmp;

}
/// <summary>
/// returns the g value of the specified node unless the node does not exist it caluclate it and returns it
/// </summary>
/// <returns></returns>
double DStarLite::getGValue(Node currentNode)
{

	if (m_cellHash.find(currentNode) == m_cellHash.end())
	{
		return calculateHeuristic(currentNode, m_goal);
	}
	//return the nodes g cost
	return m_cellHash[currentNode].m_gValue;

}
/// <summary>
/// returns the rhs value of the specified node unless the node does not exist it caluclate it and returns it
/// </summary>
/// <returns></returns>
double DStarLite::getRHSValue(Node currentNode)
{

   if (currentNode.getCol() == m_goal.getCol() &&currentNode.getRow() == m_goal.getRow())
   {
	  return 0;
   }
   //if the rhs value has not already been calulated , claulate it 
  if (m_cellHash.find(currentNode) == m_cellHash.end())
  {
	  return calculateHeuristic(currentNode, m_goal);
  }
  else
  {
	  //if the node is already added , return its rhs value
	  return m_cellHash[currentNode].m_rhsValue;
  }
}

/// <summary>
/// sets the g value of the specified node
/// </summary>
/// <returns></returns>
void DStarLite::setGValue(Node currentNode, double g)
{
	//check if the node has already been added or not
  newNode(currentNode);
  //set the nodes g value
  m_cellHash[currentNode].m_gValue = g;
}

/// <summary>
/// sets the rhs value of the specified node
/// </summary>
/// <returns></returns>
double DStarLite::setRHSValue(Node currentNode, double rhs)
{
	//check if the node has already been added or not
  newNode(currentNode);
  //set the nodes rhs value
  m_cellHash[currentNode].m_rhsValue = rhs;
  return rhs;
}

/// <summary>
/// calculates the eight way distance between the specified nodes 
/// </summary>
/// <returns></returns>
double DStarLite::eightCondist(Node a, Node b)
{
	//calculate the eight way heurisitc betrween thew two specified nodes
	double temp = 0;
	double min = fabs(a.getCol() - b.getCol());
	double max = fabs(a.getRow() - b.getRow());
	if (min > max) 
	{
		double temp = min;
		min = max;
		max = temp;
	}
	return ((m_sqrt2 - 1.0)*min + max);
}

/// <summary>
/// updates all locally inconcistent nodes 
/// </summary>
/// <returns></returns>
bool DStarLite::computeShortestPath()
{

  list<Node> nodes;
  list<Node>::iterator nodeIterator;

 /* if (m_openList.empty())
  {
	  return false;
  }*/
  int m_steps=0;
  //while the opne list has nodes and the key values for the lowest inconsistent node is lesser than the start node and the start node is not consistent
  while ((!m_openList.empty()) && keyCompare(m_openList.top() , m_start = calculateKey(m_start)) || (getRHSValue(m_start) != getGValue(m_start)))
  {
	  //if the max steps has been reached, return thast there is not a shortest path
    if (m_steps++ >= m_maxSteps)
	{
		std::cout << "reached max attempts" << std::endl;
	  return false;
    }


    Node currentNode;
	//test wether the start node is consistent or not
    bool test = (getRHSValue(m_start) != getGValue(m_start));

  
	  currentNode = m_openList.top();
      m_openList.pop();

	  //if the current node is not already in the open list , continue
	  if (!isOpen(currentNode))
	  {
		  continue;
	  }
	  //if key value of the first node is greater and the start node is not consistent return that htere is no path
	  if (!keyCompare(currentNode, m_start) && (!test))
	  {
		  return false;
	  }
	  //erase the current node
    auto cur = m_openHash.find(currentNode);
	m_openHash.erase(cur);

    Node k_old = currentNode;
	//if the current node is under consistent  add it to the open list
    if (keyCompare(k_old , calculateKey(currentNode)))
	{
      insert(currentNode);
    } 
	//if the current node is over consistent  update its g values and investigate the predecessors
	else if (getGValue(currentNode) > getRHSValue(currentNode))
	{ 
      setGValue(currentNode,getRHSValue(currentNode));
      getPredecessors(currentNode, nodes);
      for (nodeIterator = nodes.begin(); nodeIterator != nodes.end(); nodeIterator++)
	  {
        updateVertex(*nodeIterator);
      }
    } 
	else
	{  
      setGValue(currentNode,INFINITY);
      getPredecessors(currentNode, nodes);
      for (nodeIterator = nodes.begin(); nodeIterator != nodes.end(); nodeIterator++)
	  {
        updateVertex(*nodeIterator);
      }
      updateVertex(currentNode);
    }
  }
  return true;
}

/// <summary>
/// returns true if the parameters are within a specific range of one another
/// </summary>
/// <param name="x">value being compared</param>
/// <param name="y">value being compared</param>
/// <returns></returns>
bool DStarLite::close(double x, double y)
{
	//if the values are set to infinty return true , otherwise if the absolute ditance is less than   0.00001) return 
	if (x == INFINITY && y == INFINITY)
	{
		return true;
	}
	return (fabs(x-y) < 0.00001);
}

/// <summary>
/// updates a node , and inserts it depending on the status of its g and rhs values
/// </summary>
/// <param name="u">node being updated</param>
/// <returns></returns>
void DStarLite::updateVertex(Node currentNode)
{

  list<Node> successornode;
  list<Node>::iterator iterator;

  if (currentNode.getCol() != m_goal.getCol() || currentNode.getRow() != m_goal.getRow())
  {
	  //retrieve the successors of the current node
	  getSuccssors(currentNode, successornode);
	  double tmp = INFINITY;
	  double tmp2;

	  for (iterator = successornode.begin(); iterator != successornode.end(); iterator++)
	  {
		  //if the cost from the ucrrent node to its successor plus the g cost of the successor node is less than the lowest one update tmp to be the most optimal successpr
		  tmp2 = cost(currentNode, *iterator) + getGValue(*iterator);
		  if (tmp2 < tmp)
		  {
			  tmp = tmp2;
		  }
	  }
	  //if the vrhs value is within acertain range of the successors minimum rhs update the rhs value of the current node
	  if (!close(getRHSValue(currentNode), tmp))
	  {
		  setRHSValue(currentNode, tmp);
	  }
  }

  if (!close(getGValue(currentNode), getRHSValue(currentNode)))
  {
	  //insert the current node depending on tis g and rhs values
	  insert(currentNode);
  }
}
/// <summary>
/// inserts specified node from the open list and open hash
/// </summary>
/// <param name="u">node being inserted</param>
/// <returns></returns>
void DStarLite::insert(Node u)
{
  float csum;
  //update the nodes key value
  u    = calculateKey(u);
 
  csum = getKeyHashCode(u);
 //insert the nodes hash key
  m_openHash[u] = csum;
  //add the node to the open list
  m_openList.push(u);
}

/// <summary>
/// removes specified node from the open hash
/// </summary>
/// <param name="u">node being removed</param>
/// <returns></returns>
void DStarLite::remove(Node u)
{

  auto cur = m_openHash.find(u);
  if (cur == m_openHash.end())
  {
	  return;
  }
  //remove the current node
  m_openHash.erase(cur);
}


/// <summary>
/// 8-way distance heuristic scaled by a constant value
/// </summary>
/// <param name="a">node calculating from</param>
/// <param name="a">node calculating to</param>
/// <returns></returns>
double DStarLite::calculateHeuristic(Node a, Node b)
{
	return eightCondist(a, b)*m_unseenCellScalar;
}

/// <summary>
/// calculates the key value of the specified node
/// </summary>
/// <param name="currentNode">node moving from</param>
/// <returns></returns>
Node DStarLite::calculateKey(Node currentNode)
{
	//retrieve the lesser value between the rhs and g value
  double minimumProperty = fmin(getRHSValue(currentNode),getGValue(currentNode));

  currentNode.setKeyValue(make_pair(minimumProperty + calculateHeuristic(currentNode, m_start) + m_incrementalHeuristicK, minimumProperty));// .first = ;


  return currentNode;

}

/// <summary>
/// calculates the cost to move from one node to another
/// </summary>
/// <param name="a">node moving from</param>
/// <param name="b">node moving to</param>
/// <returns></returns>
double DStarLite::cost(Node a, Node b)
{
	double scalearValue = 1;
	int coloumnDiff = fabs(a.getCol() - b.getCol());
	int rowDiff = fabs(a.getRow() - b.getRow());
	

	if (coloumnDiff + rowDiff > 1)
	{
		scalearValue = m_sqrt2;
	}
	if (m_cellHash.count(a) == 0)
	{
		return scalearValue*m_unseenCellScalar;
	}
	else
	{
		return scalearValue*m_cellHash[a].m_costTraversalValue;
	}
}
/// <summary>
/// updates the cost of a node.
/// </summary>
/// <param name="col">col of the node to be updated</param>
/// <param name="row">row of the node to be updated</param>
/// <param name="row">value of the node to be updated</param>
/// <returns></returns>
void DStarLite::updateNode(int col, int row, double val)
{

   Node currentNode;

   currentNode.setCol(col);
   currentNode.setRow(row);

  if ((currentNode.getCol() == m_start.getCol() && currentNode.getRow() == m_start.getRow()) || (currentNode.getCol() == m_goal.getCol() && currentNode.getRow() == m_goal.getRow()))
  {
	  return;
  }
  newNode(currentNode);
  m_cellHash[currentNode].m_costTraversalValue = val;

  updateVertex(currentNode);
}

/// <summary>
/// Stores the successors of the current node 
/// if the cell is not traversable it won't have successors
/// </summary>
/// <param name="u">current expanded node</param>
/// <param name="s">list of nodes</param>
/// <returns></returns>
void DStarLite::getSuccssors(Node u,list<Node> &s)
{

	s.clear();
	u.setKeyValue(make_pair(-1,-1));


	if (isUnTraversable(u))
	{
		return;
	}

	u.setCol(u.getCol()+ 1);
	s.push_front(u);
	u.setRow(u.getRow()+ 1);
	s.push_front(u);
	u.setCol(u.getCol() - 1);
	s.push_front(u);
	u.setCol(u.getCol() - 1);
	s.push_front(u);
	u.setRow(u.getRow() - 1);
	s.push_front(u);
	u.setRow(u.getRow() - 1);
	s.push_front(u);
	u.setCol(u.getCol() + 1);
	s.push_front(u);
	u.setCol(u.getCol() + 1);
	s.push_front(u);
}

/// <summary>
/// Stores the predecessors fo the current node 
/// does not store nodes which are un trvaersable
/// </summary>
/// <param name="u">current expanded node</param>
/// <param name="s">list of nodes</param>
/// <returns></returns>
void DStarLite::getPredecessors(Node u,list<Node> &s)
{

	s.clear();
	u.setKeyValue(make_pair(-1,-1));


	u.setCol(u.getCol() + 1);
	if (!isUnTraversable(u))
	{
		s.push_front(u);
	}
	u.setRow(u.getRow() + 1);
	if (!isUnTraversable(u))
	{
		s.push_front(u);
	}
	u.setCol(u.getCol() -1);
	if (!isUnTraversable(u))
	{
		s.push_front(u);
	}
	u.setCol(u.getCol() - 1);
	if (!isUnTraversable(u))
	{
		s.push_front(u);
	}
	u.setRow(u.getRow() - 1);
	if (!isUnTraversable(u))
	{
		s.push_front(u);
	}
	u.setRow(u.getRow() - 1);
	if (!isUnTraversable(u))
	{
		s.push_front(u);
	}
	u.setCol(u.getCol() + 1);
	if (!isUnTraversable(u))
	{
		s.push_front(u);

	}
	u.setCol(u.getCol() + 1);
	if (!isUnTraversable(u))
	{
		s.push_front(u);
	}
}

/// <summary>
/// Updates the start nodes x,y
/// Increments the incremental heuristic value
///up dates the key value for the start node and assigns the last node to the new start node
/// </summary>
/// <param name="col">col of the new start node</param>
/// <param name="row">row of the new start node</param>
/// <returns></returns>
void DStarLite::updateStartNode(int col, int row)
{

	m_start.setCol(col);
	m_start.setRow(row);

  m_incrementalHeuristicK += calculateHeuristic(m_last,m_start);

  m_start = calculateKey(m_start);
  m_last  = m_start;

}


void DStarLite::updateGoalNode(int col, int row)
{

	list<pair<pair<double, double>, double> > toAdd;
	pair<pair<double,double>, double> tp;
	//store all wall nodes
	std::iterator< const Node,  NodeValue> i;
	for (auto i = m_cellHash.begin(); i != m_cellHash.end(); i++)
	{
		if (!close(i->second.m_costTraversalValue, m_unseenCellScalar)) 
		{
			
			tp.first.first = i->first.getCol();
			tp.first.second = i->first.getRow();
			tp.second = i->second.m_costTraversalValue;
			toAdd.push_back(tp);
		}
	}
	//clear containers
	m_cellHash.clear();
	m_openHash.clear();

	while (!m_openList.empty())
	{
		m_openList.pop();
	}
	//update the goal node
	m_incrementalHeuristicK = 0;

	m_goal.setCol(col);
	m_goal.setRow(row);

	NodeValue tmp;
	tmp.m_gValue = tmp.m_rhsValue = 0;
	tmp.m_costTraversalValue = m_unseenCellScalar;

	m_cellHash[m_goal] = tmp;
	//calculate new start node values
	tmp.m_gValue =  calculateHeuristic(m_start, m_goal);
	tmp.m_rhsValue = calculateHeuristic(m_start, m_goal);
	tmp.m_costTraversalValue = m_unseenCellScalar;
	m_cellHash[m_start] = tmp;
	m_start = calculateKey(m_start);

	m_last = m_start;
	//re add all wall nodes
	for (auto kk = toAdd.begin(); kk != toAdd.end(); kk++)
	{
		updateNode(kk->first.first, kk->first.second, kk->second);
	}


}

/// <summary>
/// checks costs of cells and updates them ,also computes the most optimal route to the goal , by traversing to the node with the minimal cost + g value
/// </summary>
/// <returns></returns>
std::vector<Grid*> DStarLite::replan()
{
  m_path.clear();
  

  bool res = computeShortestPath();

  if (res == false)
  {
	  return std::vector<Grid*>();
  }
  list<Node> neighbours;
  list<Node>::iterator iterator;

  Node currentNode = m_start;

  if (getGValue(m_start) == INFINITY)
  {
	  std::cout << " No Path to goal" << std::endl;
  }

  while(currentNode.getCol() != m_goal.getCol() ||currentNode.getRow() != m_goal.getRow())
  {
	m_path.push_back(&m_tiles[currentNode.getRow()][currentNode.getCol()]);
    //retreive successor nodes
    getSuccssors(currentNode, neighbours);

 

    double minimumCost = INFINITY;
    double tmin =0;
    Node minimumNode;
	//loop thorugh all successors at each iteration traversing to the successor whihc minimises the g cost plus the cost from the current node to the successor node
    for (iterator = neighbours.begin(); iterator != neighbours.end(); iterator++)
	{
	   m_openNodes.push_back(&m_tiles[iterator->getRow()][iterator->getCol()]);
	   m_count++;
      double successorValue  = cost(currentNode,*iterator);
      successorValue += getGValue(*iterator);

	  if (successorValue < minimumCost)
	  {
        minimumCost = successorValue;
        minimumNode = *iterator;
      }
    }


    neighbours.clear();
	//update the current node
    currentNode = minimumNode;
  }


  m_path.push_back(&m_tiles[m_goal.getRow()][m_goal.getCol()]);

  for (int i = 0; i < m_openNodes.size(); i++)
  {
	  if (m_openNodes[i]->getType() != TileType::START &&
		  m_openNodes[i]->getType() != TileType::END &&
		  m_openNodes[i]->getType() != TileType::WALL && 
		  m_openNodes[i]->getType() != TileType::OLDPATH)
	  {
		  m_openNodes[i]->setType(TileType::OPEN);
	  }

  }

  for (int i = 0; i < m_path.size(); i++)
  {
	  if (m_path[i]->getType() != TileType::START &&
		  m_path[i]->getType() !=TileType::END)
	  {
		  m_path[i]->setType(TileType::PATH);
	  }
	 
  }
 
 
  return m_path;
}

/// <summary>
/// Resets al tile types in the open and path vectors to be a tile
/// </summary>
/// <returns></returns>
void DStarLite::reset()
{
	for (int i = 0; i < m_openNodes.size(); i++)
	{
		if (m_openNodes[i]->getType() != TileType::WALL)
		{
			m_openNodes[i]->setType(TileType::TILE);
	   }		
	}

	for (int i = 0; i < m_path.size(); i++)
	{
		m_path[i]->setType(TileType::TILE);
	}
	m_openNodes.clear();
}

/// <summary>
/// Compares the key values of nodes in the priority queue
/// </summary>
/// <param name="p1">first node to be compared</param>
/// <param name="p2">second node to be compared</param>
/// <returns></returns>
bool DStarLite::keyCompare(const Node& p1, const Node& p2) const
{
	if (p1.getKeyValue().first< p2.getKeyValue().first)
	{
		return true;
	}
	else if (p1.getKeyValue().first > p2.getKeyValue().first)
	{
		return false;
	}
	return p1.getKeyValue().second < p2.getKeyValue().second;
}