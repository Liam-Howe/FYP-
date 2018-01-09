#pragma once
#include "Node.h"

class KeyCostComparer {
public:
	bool operator()(Node& key1, Node& key2) 
	{//compares the key value of two nodes
		return key1.getKeyValue() > key2.getKeyValue();
	}
	
};

class NodeComparer {

public:
	bool operator() (const Node &s,const  Node &s2)const
	{//checks if pne node has the same row and col values as another node
		return ((s.getCol() == s2.getCol()) && (s.getRow() == s2.getRow()));
	}
};




