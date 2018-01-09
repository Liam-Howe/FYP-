#pragma once
#include "SFML/Graphics.hpp"

class Camera
{
public:

	Camera(sf::Vector2f _pos, sf::Vector2f _size);
	~Camera();
	/// updates the center of the camera
	void updateCamera();
	//alters the position of the camera
	void moveCamera(sf::Vector2f);
	//moves the camera within the bounds of the grid
	void checkBounds(sf::Event *Event , int gridSize, int tileSize, int level);
	//returns the current cmaera view
	sf::View getView();
	//returns the current position of the camera
	sf::Vector2f getposition();
private:
	//camera view
	sf::View m_View;
	//camera position
	sf::Vector2f m_cameraPosition;
	//size of the camera view
	sf::Vector2f m_Size;
	//speed at whic the camera scrolls
	int m_speed;
};