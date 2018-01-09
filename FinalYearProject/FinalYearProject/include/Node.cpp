#include "Node.h"
/// <summary>
/// returns the current coloumn of the node
/// </summary>
/// <returns></returns>
int Node::getCol() const
{
	return m_coloumn;
}
/// <summary>
/// returns the current row of the node
/// </summary>
/// <returns></returns>
int Node::getRow() const
{
	return m_row;
}
/// <summary>
/// returns the current key value  of the node
/// </summary>
/// <returns></returns>
std::pair<double, double> Node::getKeyValue() const
{
	return m_keyValue;
}

void Node::setRow(int value)
{
	m_row = value;
}
/// <summary>
/// sets the coloumn of the current  node
/// </summary>
/// <returns></returns>
void Node::setCol(int value)
{
	m_coloumn = value;
}
/// <summary>
/// sets the key value  of the current  node
/// </summary>
/// <returns></returns>
void Node::setKeyValue(pair<double, double> value)
{
	m_keyValue = value;
}
