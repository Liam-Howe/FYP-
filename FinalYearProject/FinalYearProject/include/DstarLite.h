#pragma once
#include <queue>
#include <list>
#include <unordered_map>
#include <functional>
#include "Grid.h"
#include "KeyCostComparer.h"
using namespace std;


struct NodeValue {
  //cost to move from one node to another
  double m_costTraversalValue;
  //cost for mthe goal node to the current node
  double m_gValue;
  //lookahed value
  double m_rhsValue;


};

class node_hash {
 public:
  size_t operator()(const Node &s)const 
  {
    return s.getCol() + 3157*s.getRow();
  }
};


class DStarLite {

 public:

	DStarLite(std::vector<std::vector<Grid>>& tiles);
	//initialises the start and endn nodes
	void   init(int sX, int sY, int gX, int gY);
	//returns the most optinmal path
	std::vector<Grid*>   replan();

	//updates the status of the specfied node
	void   updateNode(int x, int y, double val);
	//updates the status of the start node
	void   updateStartNode(int x, int y);
	//updates the status of the goal node
	void   updateGoalNode(int x, int y);
	
	//returns the amount of nodes vsited
	int getNodesVisited();
	//sets the amount of nodes vsited
	void setNodesVisited(int);
	//resets previous data calulated on nodes
	void  reset();
 

private:


	//holds all of the inconsistent nodes
	priority_queue<Node, vector<Node>, KeyCostComparer> m_openList;

	unordered_map<Node, NodeValue, node_hash, NodeComparer > m_cellHash;

	unordered_map<Node, float, node_hash, NodeComparer > m_openHash;

	//stores all of the optimal nodes
	std::vector<Grid*> m_path;
	//stores all of the nodes investigated by the algortithm
	std::vector<Grid*> m_openNodes;
	//cost of mocing to an unknown node
	double m_unseenCellScalar;
	//value which is added to all keyswhen a change occurs
	double m_incrementalHeuristicK;

	//start,goal and last ndoes
	Node m_start;
	Node m_goal;
	Node m_last;
	//maximum number of nodes before the search terminate
	int m_maxSteps;

	float m_sqrt2 = 1.41421356237309504880f;
	//rewfference to the grid
	std::vector<std::vector<Grid>>& m_tiles;

	int m_count;

	// returns true if the parameters are within a specific range of one another
	bool   close(double x, double y);
	//calculates properties for the node and inserts it into the cell hash
	void   newNode(Node currentNode);
	//gets the g avlue of the specfiied node
	double getGValue(Node currentNode);
	//gets the rhs of the specified node
	double getRHSValue(Node currentNode);
	//sets the g value of the specified node
	void   setGValue(Node currentNode, double gValue);
	//sets the rhs of the specfified node
	double setRHSValue(Node currentNode, double rhsValue);
	//returns the eight way distance ovf the specified ndoe
	double eightCondist(Node nodeA, Node nodeB);
	//updates all inconsistent nodes
	bool    computeShortestPath();
	void   updateVertex(Node currentNode);

	//removes/inserts a node from/to the open hash
	void   insert(Node currentNode);
	void   remove(Node currentNode);
	
	//calculates the heuristic value for the specified node
	double calculateHeuristic(Node nodeA, Node nodeB);
	//calculates the key value for the specified node
	Node  calculateKey(Node currentNode);
	//stores all the successors of the specified node
	void   getSuccssors(Node currentNode, list<Node> &s);
	//stores all the predecessors of the specified node
	void   getPredecessors(Node currentNode, list<Node> &s);
	//returns the cost to move from one node to another
	double cost(Node nodeA, Node nodeB);
	//returns wether or not a node is traversable
	bool   isUnTraversable(Node currentNode);
	//returns wether or not a node is already in the open list
	bool   isOpen(Node currentNode);

	//returns the hash code for a node
	float  getKeyHashCode(Node currentNode);

	//Compares the key values of nodes in the priority queue
	bool keyCompare(const Node& p1, const Node& p2) const;
};

