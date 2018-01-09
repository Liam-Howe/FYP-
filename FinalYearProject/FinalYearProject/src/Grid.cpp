#include "../include/Grid.h"
/// <summary>
/// initialises grid position ,type and size
/// </summary>
/// <returns></returns>
Grid::Grid(sf::Vector2f _pos, sf::Vector2f _size, TileType _type,int row, int col) : m_type(_type),m_row(row),m_col(col)
{
	

	m_Rect.left = _pos.x;
	m_Rect.top = _pos.y;
	m_Rect.width = _size.x;
	m_Rect.height = _size.y;
	m_traversable = true;
	m_isMarked = false;

	if (m_type == TileType::TILE)
	{
		 m_colour = sf::Color(255, 255, 255);
	}
	else if (m_type == TileType::START)
	{
		m_colour = sf::Color(0, 255, 0);
	}
	else if (m_type == TileType::END)
	{
		m_colour = sf::Color(255, 0, 0);
	}
	else if (m_type == TileType::WALL)
	{
		m_colour = sf::Color(0, 0, 0);
		m_traversable = false;
	}
	else if (m_type == TileType::PATH)
	{
		m_colour = sf::Color(0, 0, 255);
	}
	else if (m_type == TileType::OLDPATH)
	{
		m_colour = sf::Color::Magenta;
	}
}

Grid::~Grid()
{


}
/// <summary>
/// sets the type of the node
/// </summary>
/// <returns></returns>
void Grid::setType(TileType _value)
{
	m_type = _value;
	updateType();
}
/// <summary>
/// update the colour of the grid based on its type
/// </summary>
/// <returns></returns>
void Grid::updateType()
{
	if (m_type == TileType::TILE)
	{
		m_colour = sf::Color(255, 255, 255);
		m_traversable = true;
	}
	else if (m_type == TileType::START)
	{
		m_colour = sf::Color(0, 255, 0);
	}
	else if (m_type == TileType::END)
	{
		m_colour = sf::Color(255, 0, 0);
	}
	else if (m_type == TileType::WALL)
	{
		m_colour = sf::Color(0, 0, 0);
		m_traversable = false;	
	}
	else if (m_type == TileType::PATH)
	{
		m_colour = sf::Color(0, 0, 255);
	}
	else if (m_type == TileType::OPEN)
	{
		m_colour = sf::Color::Yellow;
	}
	else if (m_type == TileType::OLDPATH)
	{
		m_colour = sf::Color::Cyan;
	}

}
/// <summary>
/// returns if a specifc node is traversable or not
/// </summary>
/// <returns></returns>
bool Grid::getTraversable()
{
	return m_traversable;
}
/// <summary>
/// returns if a specifc node is marked or not
/// </summary>
/// <returns></returns>
bool Grid::getMarked()
{
	return m_isMarked;
}
/// <summary>
/// returns the previous node to the current node
/// </summary>
/// <returns></returns>
Grid * Grid::getPrevious()
{
	return m_previous;
}
/// <summary>
/// sets if a node has been investigated or not
/// </summary>
/// <returns></returns>
void Grid::setMarked(bool value)
{
	m_isMarked = value;
}
/// <summary>
/// sets the previous node to the current node
/// </summary>
/// <returns></returns>
void Grid::setPrevious(Grid * previous)
{
  m_previous = previous;
}
/// <summary>
/// sets wether or not a node has been added to the open list or not
/// </summary>
/// <returns></returns>
void Grid::setOpen(bool value)
{
	m_open = value;
}
/// <summary>
/// sets wether or not a node has been added to the closed list or not
/// </summary>
/// <returns></returns>
void Grid::setClosed(bool value)
{
	m_closed = value;
}
/// <summary>
/// gets wether or not a node has been added to the open list or not
/// </summary>
/// <returns></returns>
bool Grid::getOpen()
{
	return m_open;
}
/// <summary>
/// gets wether or not a node has been added to the closed list or not
/// </summary>
/// <returns></returns>
bool Grid::getClosed()
{
	return m_closed;
}
/// <summary>
/// gets the colour of the node
/// </summary>
/// <returns></returns>
sf::Color Grid::getColour()
{
	return m_colour;
}
/// <summary>
/// gets wether or not a node has been added diagonally
/// </summary>
/// <returns></returns>
bool Grid::getIsDiagonal()
{
	return m_isDiagonal;
}
/// <summary>
/// sets wether or not a node has been added diagonally
/// </summary>
/// <returns></returns>
void Grid::setIsDiagonal(bool value)
{
	m_isDiagonal = value;
}
/// <summary>
/// resets the tile type
/// </summary>
/// <returns></returns>
void Grid::reset()
{
	m_type = TileType::TILE;
	updateType();
}
/// <summary>
/// gets the tile type
/// </summary>
/// <returns></returns>
TileType Grid::getType()
{
	return m_type;
}
/// <summary>
/// gets the row of the node
/// </summary>
/// <returns></returns>
int Grid::getRow()
{
	return m_row;
}
/// <summary>
/// gets the coloumn of the node
/// </summary>
/// <returns></returns>
int Grid::getCol()
{
	return m_col;
}
/// <summary>
/// gets the gcost  of the node
/// </summary>
/// <returns></returns>
float Grid::getGCost()
{
	return m_gCost;
}
/// <summary>
/// gets the hcost  of the node
/// </summary>
/// <returns></returns>
float Grid::getHCost()
{
	return m_Hcost;
}
/// <summary>
/// gets the fcost  of the node
/// </summary>
/// <returns></returns>
float Grid::getFCost()
{
	return m_Fcost;
}
/// <summary>
/// sets the gcost  of the node
/// </summary>
/// <returns></returns>
void Grid::setGCost(float value)
{
	m_gCost = value;
}
/// <summary>
/// sets the hcost  of the node
/// </summary>
/// <returns></returns>
void Grid::setHCost(float value)
{
	m_Hcost = value;

}
/// <summary>
/// sets the fcost  of the node
/// </summary>
/// <returns></returns>
void Grid::setFCost(float value)
{
	m_Fcost = value;
}

/// <summary>
/// returns the in rectangle of the node
/// </summary>
/// <returns></returns>
sf::IntRect& Grid::getRectangle()
{
	return m_Rect;
}
/// <summary>
/// returns  the position  of the node
/// </summary>
/// <returns></returns>
sf::Vector2f Grid::getPosition()
{
	return sf::Vector2f(m_Rect.left,m_Rect.top);
}