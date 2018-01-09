#pragma once
#include <utility>
using namespace std;
class Node
{
public :
	//returns the nodes current coloumn
	int getCol() const;
	//returns the nodes current row
	int getRow() const;
	//returns the nodes current key value
	pair<double, double> getKeyValue() const;
	//sets the nodes current row
	void setRow(int);
	//sets the nodes current coloumn
	void setCol(int);
	//sets the nodes current key value
	void setKeyValue(pair<double, double>);
private:
	//coloumn of the node
	int m_coloumn;
	//row of the node
	int m_row;
	//key value of the node
	pair<double, double> m_keyValue;
};