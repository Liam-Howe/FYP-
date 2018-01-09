#include "../include/Camera.h"

/// <summary>
/// initialises the camera properties
/// </summary>
/// <returns></returns>
Camera::Camera(sf::Vector2f _pos, sf::Vector2f _size) : m_cameraPosition (_pos) , m_Size(_size)
{
	m_View.setCenter(m_cameraPosition);
	m_View.setSize(m_Size);
}
Camera::~Camera()
{

	
	
	
}
/// <summary>
/// updates the center of the camera
/// </summary>
/// <returns></returns>
void Camera::updateCamera()
{

	m_View.setCenter(m_cameraPosition);
}
/// <summary>
/// alters the cameras position
/// </summary>
/// <returns></returns>
void Camera::moveCamera(sf::Vector2f _tempVector)
{
	m_cameraPosition += _tempVector;
}
/// <summary>
/// checks that the camera has not moved beyond the bounds of the grid, also calls fro the camera to be moved depedning on user input
/// </summary>
/// <returns></returns>
void Camera::checkBounds(sf::Event * Event , int gridSize , int tileSize,int level)
{
	if (level == 1)
	{
		m_speed = 10;
	}
	else if (level == 2)
	{
		m_speed = 10;
	}
	else
	{
		m_speed = 40;
	}
	

	if ((Event->type == sf::Event::KeyPressed) && (Event->key.code == sf::Keyboard::Up))
	{
		if (m_View.getCenter().y - m_View.getSize().y / 2 > 0)
		{
			moveCamera(sf::Vector2f(0, -m_speed));
		}
	}
	else if ((Event->type == sf::Event::KeyPressed) && (Event->key.code == sf::Keyboard::Down))
	{
		if (m_View.getCenter().y + m_View.getSize().y / 2 < gridSize * tileSize)
		{
			moveCamera(sf::Vector2f(0, m_speed));
		}
	}
	else if ((Event->type == sf::Event::KeyPressed) && (Event->key.code == sf::Keyboard::Left))
	{

		if (m_View.getCenter().x - m_View.getSize().x/2 > 0)
		{
			moveCamera(sf::Vector2f(-m_speed, 0));
		}
		
	}
	else if ((Event->type == sf::Event::KeyPressed) && (Event->key.code == sf::Keyboard::Right))
	{
		if (m_View.getCenter().x + m_View.getSize().x / 2 < gridSize * tileSize)
		{
			moveCamera(sf::Vector2f(m_speed, 0));
		}
	}



}
/// <summary>
/// returns the piositon of the camera
/// </summary>
/// <returns></returns>
sf::Vector2f Camera::getposition()
{
	return m_cameraPosition;
}
/// <summary>
/// returns the current view  of the camera
/// </summary>
/// <returns></returns>
sf::View Camera::getView()
{
	return m_View;
}