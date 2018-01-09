#pragma once

#include "SFML/Graphics.hpp"
#include "../include/TileType.h"
#include "SFML\Graphics\Rect.hpp"
#include <functional>
#include <iostream>
using namespace std;
class Grid
{
	public : 

		

		Grid(sf::Vector2f _pos, sf::Vector2f _size, TileType _type,int row , int col);
		~Grid();

		void updateType();

		sf::IntRect& getRectangle();
		sf::Vector2f getPosition();
		TileType getType();
		int getRow();
		int getCol();
		float getGCost();
		float getHCost();
		float getFCost();
	
		bool getTraversable();
		bool getMarked();
		Grid* getPrevious();

		void setType(TileType _value);
		void setGCost(float);
		void setHCost(float);
		void setFCost(float);
		
		void setMarked(bool);
		void setPrevious(Grid*);
		void setOpen(bool);
		void setClosed(bool);
		bool getOpen();
		bool getClosed();
		sf::Color getColour();
		bool getIsDiagonal();
		void setIsDiagonal(bool);

		void reset();

	private :

		sf::Color m_colour;
		sf::IntRect m_Rect;
		TileType m_type;
		int m_row;
		int m_col;
		float m_gCost;
		float m_Hcost;
		float m_Fcost;
		
		bool m_traversable;
		bool m_isMarked;
		bool m_open;
		bool m_closed;
		bool m_isDiagonal;
		Grid * m_previous;
	
};