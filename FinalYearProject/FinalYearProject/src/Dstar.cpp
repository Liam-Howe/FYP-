#include "../include/Dstar.h"
#include <math.h>
#include <iostream>
/* void Dstar::Dstar()
 * --------------------------
 * Constructor sets constants.
 */
Dstar::Dstar(std::vector<std::vector<Grid>>& tile) : m_tiles(tile) {

  maxSteps = 8000;  // node expansions before we give up
  C1       = 1;      // cost of an unseen cell

}

/* float Dstar::keyHashCode(state u)
 * --------------------------
 * Returns the key hash code for the state u, this is used to compare
 * a state that have been updated
 */
float Dstar::keyHashCode(Grid* u) {

  return (float)(u->getKeyValue().first + 1193* u->getKeyValue().second);

}

/* bool Dstar::isValid(state u)
 * --------------------------
 * Returns true if state u is on the open list or not by checking if
 * it is in the hash table.
 */
bool Dstar::isValid(Grid* u) {

  ds_oh::iterator cur = openHash.find(u);
  if (cur == openHash.end())
  {
	  return false;
  }
  if (!close(keyHashCode(u), cur->second))
  {
	  return false;
  }
  return true;
  
}

/* void Dstar::getPath()
 * --------------------------
 * Returns the path created by replan()
 */
std::vector<Grid*> Dstar::getPath() {
  return path;
}

/* bool Dstar::occupied(state u)
 * --------------------------
 * returns true if the cell is occupied (non-traversable), false
 * otherwise. non-traversable are marked with a cost < 0.
 */
bool Dstar::occupied(Grid* u) {

  ds_ch::iterator cur = cellHash.find(u);

  if (cur == cellHash.end())
  {
	  return false;
  }
  else if (!cur->second->getTraversable())
  {
	  return true;
  }
 ///*  return (cur->second->getCost() < 0);*/

	//if (u->getTraversable())
	//{
	//	return false;
	//}
	//else
	//{
	//	return true;
	//}*/
}

/* void Dstar::init(int sX, int sY, int gX, int gY)
 * --------------------------
 * Init dstar with start and goal coordinates, rest is as per
 * [S. Koenig, 2002]
 */
void Dstar::init(Grid* start, Grid* end) {
 // m_count = 0;
  cellHash.clear();
  path.clear();
  openHash.clear();
  while(!openList.empty()) openList.pop();

  k_m = 0;

  s_start = start;
 
  s_goal = end;

  
  s_goal->setGCost(0);
  s_goal->setRHS(0);
  s_goal->setCost(C1);



  cellHash[s_goal] = s_goal;

  s_start->setGCost(heuristic(s_start, s_goal));
  s_start->setRHS(heuristic(s_start, s_goal));
  cellHash[s_start] = s_start;
  s_start = calculateKey(s_start);

  s_last = s_start;
  computeShortestPath();
 /* if (replan())
  {
	  return getPath();
  }
  else
  {
	  return vector<Grid*>();
  }*/
}


/* double Dstar::getG(state u)
 * --------------------------
 * Returns the G value for state u.
 */
double Dstar::getG(Grid* u) {

  if (cellHash.find(u) == cellHash.end())
    return heuristic(u,s_goal);
 // return cellHash[u].g;
  return cellHash[u]->getGCost();
}

/* double Dstar::getRHS(state u)
 * --------------------------
 * Returns the rhs value for state u.
 */
double Dstar::getRHS(Grid* u) {

  if (u == s_goal) return 0;

   
  
  if (cellHash.find(u) == cellHash.end())
    return heuristic(u,s_goal);
  return cellHash[u]->getRHS();
	// return u->getRHS();
}

/* void Dstar::setG(state u, double g)
 * --------------------------
 * Sets the G value for state u
 */
void Dstar::setG(Grid* u, double g) {

	u->setGCost(g);

  makeNewCell(u);
  cellHash[u]->setGCost(g);
}

/* void Dstar::setRHS(state u, double rhs)
 * --------------------------
 * Sets the rhs value for state u
 */
double Dstar::setRHS(Grid* u, double rhs) {

	u->setRHS(rhs);

  makeNewCell(u);
  cellHash[u]->setRHS(rhs);
  return rhs;
}

/* double Dstar::eightCondist(state a, state b)
 * --------------------------
 * Returns the 8-way distance between state a and state b.
 */
double Dstar::eightCondist(Grid* a, Grid* b) {
 /* double temp =0;
  double min = fabs(a->getCol() - b->getCol());
  double max = fabs(a->getRow() - b->getRow());
  if (min > max) {
    double temp = min;
    min = max;
    max = temp;
  }
  return ((M_SQRT22-1.0)*min + max);*/

	float goalXPos = b->getCol();
	float goalYPos = b->getRow();
	float distanceX = abs(goalXPos - a->getCol());
	float distanceY = abs(goalYPos - a->getRow());
	float distance = distanceX + distanceY;//
	return distance;
}

/* int Dstar::computeShortestPath()
 * --------------------------
 * As per [S. Koenig, 2002] except for 2 main modifications:
 * 1. We stop planning after a number of steps, 'maxsteps' we do this
 *    because this algorithm can plan forever if the start is
 *    surrounded by obstacles.
 * 2. We lazily remove states from the open list so we never have to
 *    iterate through it.
 */
int Dstar::computeShortestPath() {

  list<Grid*> s;
  list<Grid*>::iterator i;

  if (openList.empty()) return 1;

  int k=0;
  while ((!openList.empty()) &&
         (keyCompare(openList.top().second,calculateKey(s_start)->getKeyValue())) ||
         (getRHS(s_start) != getG(s_start))) {

    if (k++ > maxSteps) {
      fprintf(stderr, "At maxsteps\n");
      return -1;
    }


	Grid* u;

    bool test = (getRHS(s_start) != getG(s_start));

    // lazy remove
    while(1) {
      if (openList.empty()) return 1;
	  int row = openList.top().first.first;
	  int col = openList.top().first.second;
	  u = &m_tiles[row][col];
      openList.pop();

	  if (!isValid(u))
	  {
		  continue;
	  }
	  if (!(keyCompare(u->getKeyValue(), s_start->getKeyValue())) && (!test))
	  {
		  return 2;
	  }
      break;
    }

    ds_oh::iterator cur = openHash.find(u);
    openHash.erase(cur);

	Grid* k_old = u;

    if (keyCompare(k_old->getKeyValue() , calculateKey(u)->getKeyValue())) { // u is out of date
      insert(u);
    } else if (getG(u) > getRHS(u)) { // needs update (got better)
      setG(u,getRHS(u));
      getPred(u,s);
      for (i=s.begin();i != s.end(); i++) {
        updateVertex(*i);
      }
    } else {   // g <= rhs, state has got worse
      setG(u,INFINITY);
      getPred(u,s);
      for (i=s.begin();i != s.end(); i++) {
        updateVertex(*i);
      }
      updateVertex(u);
    }
  }
  return 0;
}

/* bool Dstar::close(double x, double y)
 * --------------------------
 * Returns true if x and y are within 10E-5, false otherwise
 */
bool Dstar::close(double x, double y) {

	if (isinf(x) && isinf(y))
	{
		return true;
	}
  return (fabs(x-y) < 0.00001);

}

/* void Dstar::updateVertex(state u)
 * --------------------------
 * As per [S. Koenig, 2002]
 */
void Dstar::updateVertex(Grid* u) {

  list<Grid*> s;
  list<Grid*>::iterator i;

  if (u != s_goal) {
	  getSucc(u, s);
	  double tmp = INFINITY;
	  double tmp2;

	  for (i = s.begin(); i != s.end(); i++) {
		  tmp2 = getG(*i) + cost(u, *i);
		  if (tmp2 < tmp)
		  {
			  tmp = tmp2;
		  }
	  }
	  if (!close(getRHS(u), tmp))
	  {
		  setRHS(u, tmp);
	  }
  }

  if (!close(getG(u), getRHS(u)))
  {
	  insert(u);
  }
}

/* void Dstar::insert(state u)
 * --------------------------
 * Inserts state u into openList and openHash.
 */
void Dstar::insert(Grid* u) {

  ds_oh::iterator cur;
  float csum;

  u    = calculateKey(u);
  cur  = openHash.find(u);
  csum = keyHashCode(u);
  // return if cell is already in list. TODO: this should be
  // uncommented except it introduces a bug, I suspect that there is a
  // bug somewhere else and having duplicates in the openList queue
  // hides the problem...
  //if ((cur != openHash.end()) && (close(csum,cur->second))) return;

  openHash[u] = csum;
  openList.push(std::make_pair(std::make_pair(u->getRow(), u->getCol()), std::make_pair(u->getKeyValue().first, u->getKeyValue().second)));
}
void Dstar::makeNewCell(Grid* u) {

	if (cellHash.find(u) != cellHash.end()) return;

	u->setGCost(heuristic(u, s_goal));
	u->setRHS(heuristic(u, s_goal));
	u->setCost(C1);
	
	
	/*cellInfo tmp;
	tmp.g = tmp.rhs = heuristic(u, s_goal);
	tmp.cost = C1;
	cellHash[u] = tmp;*/
	cellHash[u] = u;
}
/* void Dstar::remove(state u)
 * --------------------------
 * Removes state u from openHash. The state is removed from the
 * openList lazilily (in replan) to save computation.
 */
void Dstar::remove(Grid* u) {

  ds_oh::iterator cur = openHash.find(u);
  if (cur == openHash.end()) return;
  openHash.erase(cur);
}


/* double Dstar::trueDist(state a, state b)
 * --------------------------
 * Euclidean cost between state a and state b.
 */
double Dstar::trueDist(Grid* a, Grid* b) {

  float x = a->getCol()-b->getCol();
  float y = a->getRow()-b->getRow();
  return sqrt(x*x + y*y);

}

/* double Dstar::heuristic(state a, state b)
 * --------------------------
 * Pretty self explanitory, the heristic we use is the 8-way distance
 * scaled by a constant C1 (should be set to <= min cost).
 */
double Dstar::heuristic(Grid* a, Grid* b) {
  return eightCondist(a,b)*C1;
}

/* state Dstar::calculateKey(state u)
 * --------------------------
 * As per [S. Koenig, 2002]
 */
Grid* Dstar::calculateKey(Grid* u) {

  double val = fmin(getRHS(u),getG(u));

  u->setKeyValue(std::make_pair(val + heuristic(u, s_start) + k_m, val));

  return u;

}

/* double Dstar::cost(state a, state b)
 * --------------------------
 * Returns the cost of moving from state a to state b. This could be
 * either the cost of moving off state a or onto state b, we went with
 * the former. This is also the 8-way cost.
 */
double Dstar::cost(Grid* a, Grid* b) {

  int xd = fabs(a->getCol() - b->getCol());
  int yd = fabs(a->getRow() -b->getRow());
  double scale = 1;

  if (xd+yd>1) scale = M_SQRT22;

  if (cellHash.count(a) == 0) return scale*C1;
   return scale*cellHash[a]->getCost();
 // return scale*a->getCost();

}
/* void Dstar::updateCell(int x, int y, double val)
 * --------------------------
 * As per [S. Koenig, 2002]
 */
void Dstar::updateCell(Grid* node, double val) {

	Grid* u;

  u = node;
 

  if ((u == s_start) || (u == s_goal)) return;

  makeNewCell(u);
  cellHash[u]->setCost(val);

  updateVertex(u);
}

/* void Dstar::getSucc(state u,list<state> &s)
 * --------------------------
 * Returns a list of successor states for state u, since this is an
 * 8-way graph this list contains all of a cells neighbours. Unless
 * the cell is occupied in which case it has no successors.
 */
void Dstar::getSucc(Grid* currentNode,list<Grid*> &s) {

  s.clear();
  currentNode->setKeyValue(std::make_pair(-1, -1));

  if (occupied(currentNode))
  {
	  return;
  }
 
 /* s.push_front(&m_tiles[currentNode->getRow() + 1][currentNode->getCol()]);
  s.push_front(&m_tiles[currentNode->getRow() - 1][currentNode->getCol()]);
  s.push_front(&m_tiles[currentNode->getRow() ][currentNode->getCol() + 1]);
  s.push_front(&m_tiles[currentNode->getRow()][currentNode->getCol() - 1]);
  s.push_front(&m_tiles[currentNode->getRow() -1][currentNode->getCol() - 1]);
  s.push_front(&m_tiles[currentNode->getRow() + 1][currentNode->getCol() - 1]);
  s.push_front(&m_tiles[currentNode->getRow() + 1][currentNode->getCol() + 1]);
  s.push_front(&m_tiles[currentNode->getRow() - 1][currentNode->getCol() + 1]);*/
  
  
  
  /*u.x += 1;
  s.push_front(u);
  u.y += 1;
  s.push_front(u);
  u.x -= 1;
  s.push_front(u);
  u.x -= 1;
  s.push_front(u);
  u.y -= 1;
  s.push_front(u);
  u.y -= 1;
  s.push_front(u);
  u.x += 1;
  s.push_front(u);
  u.x += 1;
  s.push_front(u);*/

  

  if (currentNode->getRow() <m_tiles.size() - 1)
  {
	  if (m_tiles[currentNode->getRow() + 1][currentNode->getCol()].getTraversable() &&
		  m_tiles[currentNode->getRow() + 1][currentNode->getCol()].getClosed() == false &&
		  m_tiles[currentNode->getRow() + 1][currentNode->getCol()].getOpen() == false &&
		  &m_tiles[currentNode->getRow() + 1][currentNode->getCol()] != currentNode->getPrevious())
	  {
		  s.push_front(&m_tiles[currentNode->getRow() + 1][currentNode->getCol()]);
	  }
  }
  if (currentNode->getRow() > 0)
  {
	  if (m_tiles[currentNode->getRow() - 1][currentNode->getCol()].getTraversable() &&
		  m_tiles[currentNode->getRow() - 1][currentNode->getCol()].getClosed() == false &&
		  m_tiles[currentNode->getRow() - 1][currentNode->getCol()].getOpen() == false &&
		  &m_tiles[currentNode->getRow() - 1][currentNode->getCol()] != currentNode->getPrevious())
	  {
		  s.push_front(&m_tiles[currentNode->getRow() - 1][currentNode->getCol()]);
	  }
  }
  if (currentNode->getCol() > 0)
  {
	  if (m_tiles[currentNode->getRow()][currentNode->getCol() - 1].getTraversable() &&
		  m_tiles[currentNode->getRow()][currentNode->getCol() - 1].getClosed() == false &&
		  m_tiles[currentNode->getRow()][currentNode->getCol() - 1].getOpen() == false &&
		  &m_tiles[currentNode->getRow()][currentNode->getCol() - 1] != currentNode->getPrevious())
	  {
		  s.push_front(&m_tiles[currentNode->getRow()][currentNode->getCol() - 1]);
	  }
  }
  if (currentNode->getCol() < m_tiles.size() - 1)
  {
	  if (m_tiles[currentNode->getRow()][currentNode->getCol() + 1].getTraversable() &&
		  m_tiles[currentNode->getRow()][currentNode->getCol() + 1].getClosed() == false &&
		  m_tiles[currentNode->getRow()][currentNode->getCol() + 1].getOpen() == false &&
		  &m_tiles[currentNode->getRow()][currentNode->getCol() + 1] != currentNode->getPrevious())
	  {
		  s.push_front(&m_tiles[currentNode->getRow()][currentNode->getCol() + 1]);
	  }
  }
  if (currentNode->getRow() <m_tiles.size() - 1 && currentNode->getCol() <m_tiles.size() - 1)
  {
	  if (m_tiles[currentNode->getRow() + 1][currentNode->getCol() + 1].getTraversable() &&
		  m_tiles[currentNode->getRow() + 1][currentNode->getCol() + 1].getClosed() == false &&
		  m_tiles[currentNode->getRow() + 1][currentNode->getCol() + 1].getOpen() == false &&
		  &m_tiles[currentNode->getRow() + 1][currentNode->getCol() + 1] != currentNode->getPrevious())
	  {
		  s.push_front(&m_tiles[currentNode->getRow() + 1][currentNode->getCol() + 1]);
	  }
  }

  if (currentNode->getRow() > 0 && currentNode->getCol() > 0)
  {
	  if (m_tiles[currentNode->getRow() - 1][currentNode->getCol() - 1].getTraversable() &&
		  m_tiles[currentNode->getRow() - 1][currentNode->getCol() - 1].getClosed() == false &&
		  m_tiles[currentNode->getRow() - 1][currentNode->getCol() - 1].getOpen() == false &&
		  &m_tiles[currentNode->getRow() - 1][currentNode->getCol() - 1] != currentNode->getPrevious())
	  {
		  s.push_front(&m_tiles[currentNode->getRow() - 1][currentNode->getCol() - 1]);
	  }
  }
  if (currentNode->getRow() <m_tiles.size() - 1 && currentNode->getCol() > 0)
  {
	  if (m_tiles[currentNode->getRow() + 1][currentNode->getCol() - 1].getTraversable() &&
		  m_tiles[currentNode->getRow() + 1][currentNode->getCol() - 1].getClosed() == false &&
		  m_tiles[currentNode->getRow() + 1][currentNode->getCol() - 1].getOpen() == false &&
		  &m_tiles[currentNode->getRow() + 1][currentNode->getCol() - 1] != currentNode->getPrevious())
	  {
		  s.push_front(&m_tiles[currentNode->getRow() + 1][currentNode->getCol() - 1]);
	  }
  }
  if (currentNode->getCol() <m_tiles.size() - 1 && currentNode->getRow() > 0)
  {
	  if (m_tiles[currentNode->getRow() - 1][currentNode->getCol() + 1].getTraversable() &&
		  m_tiles[currentNode->getRow() - 1][currentNode->getCol() + 1].getClosed() == false &&
		  m_tiles[currentNode->getRow() - 1][currentNode->getCol() + 1].getOpen() == false &&
		  &m_tiles[currentNode->getRow() - 1][currentNode->getCol() + 1] != currentNode->getPrevious())
	  {
		  s.push_front(&m_tiles[currentNode->getRow() - 1][currentNode->getCol() + 1]);
	  }
  }
}

/* void Dstar::getPred(state u,list<state> &s)
 * --------------------------
 * Returns a list of all the predecessor states for state u. Since
 * this is for an 8-way connected graph the list contails all the
 * neighbours for state u. Occupied neighbours are not added to the
 * list.
 */
void Dstar::getPred(Grid* currentNode,list<Grid*> &s) {

  s.clear();
  currentNode->setKeyValue(std::make_pair(-1, -1));

  if (currentNode->getRow() <m_tiles.size() - 1)
  {
	  if (!occupied(&m_tiles[currentNode->getRow() + 1][currentNode->getCol()]))
	  {
		  s.push_front(&m_tiles[currentNode->getRow() + 1][currentNode->getCol()]);
	  }
  }
  if (currentNode->getRow() > 0)
  {
	  if (!occupied(&m_tiles[currentNode->getRow() - 1][currentNode->getCol()]))
	  {
		  s.push_front(&m_tiles[currentNode->getRow() - 1][currentNode->getCol()]);
	  }
  }
  if (currentNode->getCol() > 0)
  {
	  if (!occupied(&m_tiles[currentNode->getRow()][currentNode->getCol() - 1]))
	  {
		  s.push_front(&m_tiles[currentNode->getRow()][currentNode->getCol() - 1]);
	  }
  }
  if (currentNode->getCol() < m_tiles.size() - 1)
  {
	  if (!occupied(&m_tiles[currentNode->getRow()][currentNode->getCol() + 1]))
	  {
		  s.push_front(&m_tiles[currentNode->getRow()][currentNode->getCol() + 1]);
	  }
  }
  if (currentNode->getRow() <m_tiles.size() - 1 && currentNode->getCol() <m_tiles.size() - 1)
  {
	  if (!occupied(&m_tiles[currentNode->getRow() + 1][currentNode->getCol() + 1]))
	  {
		  s.push_front(&m_tiles[currentNode->getRow() + 1][currentNode->getCol() + 1]);
	  }
  }

  if (currentNode->getRow() > 0 && currentNode->getCol() > 0)
  {
	  if (!occupied(&m_tiles[currentNode->getRow() - 1][currentNode->getCol() - 1]))
	  {
		  s.push_front(&m_tiles[currentNode->getRow() - 1][currentNode->getCol() - 1]);
	  }
  }
  if (currentNode->getRow() <m_tiles.size() - 1 && currentNode->getCol() > 0)
  {
	  if (!occupied(&m_tiles[currentNode->getRow() + 1][currentNode->getCol() - 1]))
	  {
		  s.push_front(&m_tiles[currentNode->getRow() + 1][currentNode->getCol() - 1]);
	  }
  }
  if (currentNode->getCol() <m_tiles.size() - 1 && currentNode->getRow() > 0)
  {
	  if (!occupied(&m_tiles[currentNode->getRow() - 1][currentNode->getCol() + 1]))
	  {
		  s.push_front(&m_tiles[currentNode->getRow() - 1][currentNode->getCol() + 1]);
	  }
  }



  //if (!occupied(currentNode))
  //{
	 // s.push_front(&m_tiles[currentNode->getRow() + 1][currentNode->getCol()]);
  //}
  //if (!occupied(currentNode))
  //{
	 // s.push_front(&m_tiles[currentNode->getRow() - 1][currentNode->getCol()]);
  //}
  //if (!occupied(currentNode))
  //{
	 // s.push_front(&m_tiles[currentNode->getRow()][currentNode->getCol() + 1]);
  //}
  //if (!occupied(currentNode))
  //{
	 // s.push_front(&m_tiles[currentNode->getRow()][currentNode->getCol() - 1]);
  //}
  //if (!occupied(currentNode))
  //{
	 // s.push_front(&m_tiles[currentNode->getRow() - 1][currentNode->getCol() - 1]);
  //}
  //if (!occupied(currentNode))
  //{
	 // s.push_front(&m_tiles[currentNode->getRow() + 1][currentNode->getCol() - 1]);
  //}
  //if (!occupied(currentNode))
  //{
	 // s.push_front(&m_tiles[currentNode->getRow() + 1][currentNode->getCol() + 1]);
  //}
  //if (!occupied(currentNode))
  //{
	 // s.push_front(&m_tiles[currentNode->getRow() + 1][currentNode->getCol() - 1]);
  //}

  //u.x += 1;
  //if (!occupied(u)) s.push_front(u);
  //u.y += 1;
  //if (!occupied(u)) s.push_front(u);
  //u.x -= 1;
  //if (!occupied(u)) s.push_front(u);
  //u.x -= 1;
  //if (!occupied(u)) s.push_front(u);
  //u.y -= 1;
  //if (!occupied(u)) s.push_front(u);
  //u.y -= 1;
  //if (!occupied(u)) s.push_front(u);
  //u.x += 1;
  //if (!occupied(u)) s.push_front(u);
  //u.x += 1;
  //if (!occupied(u)) s.push_front(u);


  //if (currentNode->getRow() <m_tiles.size() - 1)
  //{
	 // if (m_tiles[currentNode->getRow() + 1][currentNode->getCol()].getTraversable())
	 // {
		//  s.push_front(&m_tiles[currentNode->getRow() + 1][currentNode->getCol()]);
		//  //m_grid[currentNode->getRow() + 1][currentNode->getCol()].setIsDiagonal(false);
	 // }
  //}
  //if (currentNode->getRow() > 0)
  //{
	 // if (m_tiles[currentNode->getRow() - 1][currentNode->getCol()].getTraversable() )
	 // {
		//  s.push_front(&m_tiles[currentNode->getRow() - 1][currentNode->getCol()]);
		//  //m_grid[currentNode->getRow() - 1][currentNode->getCol()].setIsDiagonal(false);
	 // }
  //}
  //if (currentNode->getCol() > 0)
  //{
	 // if (m_tiles[currentNode->getRow()][currentNode->getCol() - 1].getTraversable() )
	 // {
		//  s.push_front(&m_tiles[currentNode->getRow()][currentNode->getCol() - 1]);
		//  //m_grid[currentNode->getRow() ][currentNode->getCol() -1].setIsDiagonal(false);
	 // }
  //}
  //if (currentNode->getCol() < m_tiles.size() - 1)
  //{
	 // if (m_tiles[currentNode->getRow()][currentNode->getCol() + 1].getTraversable() )
	 // {
		//  s.push_front(&m_tiles[currentNode->getRow()][currentNode->getCol() + 1]);
		//  //m_grid[currentNode->getRow() ][currentNode->getCol() + 1].setIsDiagonal(false);
	 // }
  //}
  //if (currentNode->getRow() <m_tiles.size() - 1 && currentNode->getCol() <m_tiles.size() - 1)
  //{
	 // if (m_tiles[currentNode->getRow() + 1][currentNode->getCol() + 1].getTraversable())
	 // {
		//  s.push_front(&m_tiles[currentNode->getRow() + 1][currentNode->getCol() + 1]);
		//  m_tiles[currentNode->getRow() + 1][currentNode->getCol() + 1].setIsDiagonal(true);
	 // }
  //}

  //if (currentNode->getRow() > 0 && currentNode->getCol() > 0)
  //{
	 // if (m_tiles[currentNode->getRow() - 1][currentNode->getCol() - 1].getTraversable())
	 // {
		//  s.push_front(&m_tiles[currentNode->getRow() - 1][currentNode->getCol() - 1]);
		//  m_tiles[currentNode->getRow() - 1][currentNode->getCol() - 1].setIsDiagonal(true);
	 // }
  //}
  //if (currentNode->getRow() <m_tiles.size() - 1 && currentNode->getCol() > 0)
  //{
	 // if (m_tiles[currentNode->getRow() + 1][currentNode->getCol() - 1].getTraversable())
	 // {
		//  s.push_front(&m_tiles[currentNode->getRow() + 1][currentNode->getCol() - 1]);
		//  m_tiles[currentNode->getRow() + 1][currentNode->getCol() - 1].setIsDiagonal(true);
	 // }
  //}
  //if (currentNode->getCol() <m_tiles.size() - 1 && currentNode->getRow() > 0)
  //{
	 // if (m_tiles[currentNode->getRow() - 1][currentNode->getCol() + 1].getTraversable() )
	 // {
		//  s.push_front(&m_tiles[currentNode->getRow() - 1][currentNode->getCol() + 1]);
		//  m_tiles[currentNode->getRow() - 1][currentNode->getCol() + 1].setIsDiagonal(true);
	 // }
  //}



}

/* void Dstar::updateStart(int x, int y)
 * --------------------------
 * Update the position of the robot, this does not force a replan.
 */
void Dstar::updateStart(Grid* start) {

	s_start = start;
	if (s_last != NULL)
	{
		k_m += heuristic(s_last, s_start);
	}

  s_start = calculateKey(s_start);
  s_last  = s_start;

}

/* void Dstar::updateGoal(int x, int y)
 * --------------------------
 * This is somewhat of a hack, to change the position of the goal we
 * first save all of the non-empty on the map, clear the map, move the
 * goal, and re-add all of non-empty cells. Since most of these cells
 * are not between the start and goal this does not seem to hurt
 * performance too much. Also it free's up a good deal of memory we
 * likely no longer use.
 */
void Dstar::updateGoal(Grid* goal) {

 /* list< pair<ipoint2, double> > toAdd;
  pair<ipoint2, double> tp;
	s_goal = goal;
  ds_ch::iterator i;
  list<pair<ipoint2, double> >::iterator kk;

  for(i=cellHash.begin(); i!=cellHash.end(); i++) {
    if (!close(i->second.cost, C1)) {
      tp.first.x = i->first.x;
      tp.first.y = i->first.y;
      tp.second = i->second.cost;
      toAdd.push_back(tp);
    }
  }

  cellHash.clear();
  openHash.clear();

  while(!openList.empty())
    openList.pop();

  k_m = 0;*/

  s_goal= goal;
  goal->setGCost(0);
  goal->setRHS(0);
  goal->setCost(C1);
  cellHash[goal] = goal;
  
  s_start->setGCost(0);
  s_start->setRHS(0);
  s_start->setCost(C1);
 
  cellHash[s_start] = s_start;
  s_start = calculateKey(s_start);

  s_last = s_start;

 /* for (kk=toAdd.begin(); kk != toAdd.end(); kk++) {
    updateCell(kk->first.x, kk->first.y, kk->second);
  }*/


}

/* bool Dstar::replan()
 * --------------------------
 * Updates the costs for all cells and computes the shortest path to
 * goal. Returns true if a path is found, false otherwise. The path is
 * computed by doing a greedy search over the cost+g values in each
 * cells. In order to get around the problem of the robot taking a
 * path that is near a 45 degree angle to goal we break ties based on
 *  the metric euclidean(state, goal) + euclidean(state,start).
 */
std::vector<Grid*> Dstar::replan(bool replan) {
	

	path.clear();

	int res = computeShortestPath();
	//printf("res: %d ols: %d ohs: %d tk: [%f %f] sk: [%f %f] sgr: (%f,%f)\n",res,openList.size(),openHash.size(),openList.top().k.first,openList.top().k.second, s_start.k.first, s_start.k.second,getRHS(s_start),getG(s_start));
	if (res < 0) {
		fprintf(stderr, "NO PATH TO GOAL\n");
		//return false;
	}
	list<Grid*> n;
	list<Grid*>::iterator i;

	Grid* cur = s_start;

	if (isinf(getG(s_start))) {
		fprintf(stderr, "NO PATH TO GOAL\n");
		// return false;
	}

	// cur = s_goal;



	while (cur != s_goal) {


		/*if (replan)
		{
			if (cur != s_start)
			{
				path.push_back(cur);
			}

		}
		else
		{
			path.push_back(cur);
		}*/
		path.push_back(cur);
		
		
	//	cur->setClosed(true);
		getSucc(cur, n);
		
		if (n.empty()) {
			fprintf(stderr, "NO PATH TO GOAL\n");
			// return false;
		}

		double cmin = INFINITY;
		double tmin = 0;
		Grid* smin = nullptr;

		//std::cout << " Current node : " << " y : " << cur->getRow() << " x : " << cur->getCol() << std::endl;
		//std::cout << " -----------------------------------------------------------------------------" << std::endl;
		for (i = n.begin(); i != n.end(); i++) {
			if ((*i)->getType() != TileType::END &&
				(*i)->getType() != TileType::OLDPATH
				&& (*i)->getType() != TileType::PATH)
			{
				(*i)->setType(TileType::OPEN);
				
			}
			m_count++;
			m_openNodes.push_back(*i);
			//if (occupied(*i)) continue;
			double val = cost(cur, *i);
			double val2 = trueDist(*i, s_goal) + trueDist(s_start, *i); // (Euclidean) cost to goal + cost to pred
			val += getG(*i);
			
			//  std::cout << " X : "  << (*i)->getCol() << " Y :  "<< (*i)->getRow() << "cost to move " << cost(cur,*i) << " g value : " << (*i)->getGCost() << " h :"  << val2 << " total cost : " << val << std::endl;
			if (close(val, cmin)) {
				if (tmin > val2) {
					tmin = val2;
					cmin = val;
					smin = *i;
					//(*i)->setOpen(true);
					(*i)->setPrevious(cur);
					
					// (*i)->setType(TileType::OPEN);
					
				}
			}
			else if (val < cmin) {
				tmin = val2;
				cmin = val;
				smin = *i;
				//(*i)->setOpen(true);
			   (*i)->setPrevious(cur);
				//(*i)->setType(TileType::OPEN);
			}
		}
		//std::cout << " -----------------------------------------------------------------------------" << std::endl;
		n.clear();
		cur = smin;
	}




  //m_openQueue.push(std::make_pair(std::make_pair(cur->getRow(), cur->getCol()), cur->getFCost()));
  //cur->setOpen(true);
  //cur->setGCost(0);
  //while (cur != s_goal && m_openQueue.size() != 0)
  //{
	 // int row = m_openQueue.top().first.first;
	 // int col = m_openQueue.top().first.second;
	 // cur = &m_tiles[row][col];
	 // 
	 // m_openQueue.pop();


	 // if (cur == s_goal)
	 // {
		//  //formPath(&firstNode, &endNode);
		// // return path;
		//  break;
	 // }
	 // cur->setOpen(false);
	 // cur->setClosed(true);
	 // //path.push_back(cur);
	 // getSucc(cur,n);
	 // 
	 // for (i = n.begin(); i != n.end(); i++) {

		//  float tentGCost = 0;// = currentNode->getGCost() + 1;
		//  if ((*i)->getIsDiagonal() == false)
		//  {
		//	  tentGCost = cur->getGCost() + 1;
		//  }
		//  else if ((*i)->getIsDiagonal() == true)
		//  {
		//	  tentGCost = cur->getGCost() + 1.4f;
		//  }

		//  if (tentGCost <= (*i)->getGCost())
		//  {
		//	  (*i)->setPrevious(cur);
		//	  (*i)->setGCost(tentGCost);
		//	  (*i)->setFCost((*i)->getGCost() + cost(cur,*i));
		//  }

		//  if ((*i)->getOpen() == false)
		//  {
		//	  (*i)->setOpen(true);
		//	  (*i)->setType(TileType::OPEN);
		//	  m_openQueue.push(std::make_pair(std::make_pair((*i)->getRow(), (*i)->getCol()), (*i)->getFCost()));
		//  }
	 // }
	 // n.clear();



  //}
  
	if (replan)
	{
		if (cur != s_goal)
		{
			path.push_back(s_goal);
		}

	}
	else
	{
		path.push_back(s_goal);
	}
 
  std::vector<Grid*>::iterator j;
  //for ( j = path.begin(); j != path.end(); j++)
  //{
	 // if ((*j)->getType() != TileType::START &&
		//  (*j)->getType() != TileType::END)
	 // {
		//  (*j)->setType(TileType::PATH);
		////  std::cout << " x: " << (*j)->getCol() << " y: " << (*j)->getRow() << std::endl;
	 // }
	 // 
  //}


 // std::cout << "count " << m_count << std::endl;
  for (Grid* i = s_goal; i->getPrevious() != 0; i = i->getPrevious())
  {
	  if (i->getType() != TileType::START &&
		  i->getType() != TileType::END)
	  {
		 // path.push_back(i);
		  i->setType(TileType::PATH);
	  }
	 
  }

  for (int i = 0; i < path.size(); i++)
  {
	  path[i]->setOpen(false);
	  path[i]->setClosed(false);
	  path[i]->setPrevious(NULL);
  }
  return path;
}


void Dstar::reset()
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
	m_openNodes.clear();


}
bool Dstar::keyCompare(const pair<float, float>& p1, const pair<float, float>& p2) const
{
		if (p1.first + 0.000001 < p2.first) return true;
	else if (p1.first - 0.000001 > p2.first) return false;
	return p1.second < p2.second;
	/*else if (p1.second < p2.second)
	{
		return true;
	}
	else if (p1.second > p2.second)
	{
		return false;
	}
	return false;*/
}