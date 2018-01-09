#pragma once
#include <math.h>
#include <stack>
#include <queue>
#include <list>
#include <stdio.h>
#include <unordered_map>
#include <functional>
#include "Grid.h"
#include "NodeSearchCostComparer.h"
#include "KeyCostComparer.h"
using namespace std;


typedef priority_queue<std::pair<std::pair<int, int>, std::pair<float, float>>, vector<std::pair<std::pair<int, int>, std::pair<float, float>>>, KeyCostComparer> ds_pq;

typedef unordered_map<Grid*,Grid*> ds_ch;
typedef unordered_map<Grid*, float> ds_oh;



class Dstar {

 public:

  Dstar(std::vector<std::vector<Grid>>&);
  void  init(Grid*, Grid*);
  void   updateCell(Grid*, double val);
  void   updateStart(Grid*);
  void   updateGoal(Grid*);
  std::vector<Grid*>   replan(bool);
  void reset();
  std::vector<Grid*> getPath();

 private:

  std::vector<Grid*> path;

  double C1;
  double k_m;
  Grid* s_start;
  Grid* s_goal;
  Grid* s_last;
  int maxSteps;

  ds_pq openList;
  ds_ch cellHash;
  ds_oh openHash;

  bool   close(double x, double y);
  void   makeNewCell(Grid* u);
  double getG(Grid* u);
  double getRHS(Grid* u);
  void   setG(Grid* u, double g);
  double setRHS(Grid* u, double rhs);
  double eightCondist(Grid* a, Grid* b);
  int    computeShortestPath();
  void   updateVertex(Grid* u);
  void   insert(Grid* u);
  void   remove(Grid* u);
  double trueDist(Grid* a, Grid* b);
  double heuristic(Grid* a, Grid* b);
  Grid*  calculateKey(Grid* u);
  void   getSucc(Grid* u, list<Grid*> &s);
  void   getPred(Grid* u, list<Grid*> &s);
  double cost(Grid* a, Grid* b);
  bool   occupied(Grid* u);
  bool   isValid(Grid* u);
  float  keyHashCode(Grid* u);
  float M_SQRT22 =   1.41421356237309504880f;
  std::vector<std::vector<Grid>>& m_tiles;
  std::vector<Grid*> m_openNodes;
 // std::priority_queue< std::pair<std::pair<float, float>, float>, vector<std::pair<std::pair<float, float>, float>>, NodeSearchCostComparer> m_openQueue;
  bool keyCompare(const pair<float, float>& p1, const pair<float, float>& p2) const;
  int m_count;
};

