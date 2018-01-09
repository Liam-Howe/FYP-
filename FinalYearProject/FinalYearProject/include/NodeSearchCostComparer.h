#pragma once
#include "Grid.h"
class NodeSearchCostComparer {
public: 
	
	bool operator()(std::pair<std::pair<float, float>, float>& n1, std::pair<std::pair<float, float>, float>& n2) {

		return n1.second > n2.second;
	}
};


