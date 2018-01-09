
#include "../include/Play.hpp"

/// <summary>
/// initialises play scne and calls for setup 
/// </summary>
/// <returns></returns>
Play::Play(Game* game ,int level) : m_level(level)
{
	this->game = game;	
	setup();	
} 

/// <summary>
/// renders all in play scene objects
/// </summary>
/// <returns></returns>
void Play::draw()
{


	game->window.clear();
	
			
	sf::RectangleShape tile;
	tile.setPosition(sf::Vector2f(0, 0));
	tile.setSize(sf::Vector2f(10, 10));
	tile.setOutlineColor(sf::Color(0, 0, 0));
	tile.setOutlineThickness(0.5f);
			
	for (int y = m_renderStartY; y < m_renderEndY; y++)
	{
		for (int x = m_renderStartX; x < m_renderEndX; x++)
		{
			tile.setFillColor(m_grid[y][x].getColour());
			tile.setPosition(m_grid[y][x].getPosition());
			game->window.draw((tile));
		}
	}
	game->window.draw(m_ai->getRect());
	game->window.draw(m_algorithmNameText);
	game->window.draw(m_timeTakenText);
	game->window.draw(m_optimalNodeCountText);
	game->window.draw(m_nodesvisitedText);
	game->window.draw(m_pathLengthText);
	
	return;
}
///// <summary>
///calls for the load Level function. Sets all ui strings and posiitons , creates all algorithm objects
/// </summary>
/// <returns></returns>
void  Play::setup()
{
	m_dStarLiteString = "D* Lite";
	m_alteredDstarLiteString = "Altered D* Lite";
	m_aStarString = "A*";

	m_ai = new AI(sf::Vector2f(0, 0), 10, m_grid);

	//create camera
	m_camera = new  Camera(sf::Vector2f(200, 200), sf::Vector2f(400, 400));
	
	//load a font
	m_font.loadFromFile("../Dependencies/font.ttf");

	//create a formatted text string
	m_text.setFont(m_font);


	//setup Grid
	m_tileSize = 10;
	m_gridSize = 0;

	m_xEnd = 0;
	m_yEnd = 0;

	loadLevel(m_level, m_grid, m_gridSize);
	
	//create instances of algorithm objects
	m_dStar = new DStarLite(m_grid);
	m_ai = new  AI(m_grid[5][5].getPosition(), m_tileSize, m_grid);
	m_aStar = new AStar(m_grid);		
	
	m_Event = new sf::Event();

	//setup ui properties
	m_algorithmNameText.setFont(m_font);
	m_algorithmNameText.setString("Algorithm :");
	m_algorithmNameText.setCharacterSize(13);
	m_algorithmNameText.setFillColor(sf::Color::Green);
	m_algorithmNameText.setStyle(sf::Text::Italic);
	m_algorithmNameText.setOutlineColor(sf::Color::Red);
	m_algorithmNameText.setOutlineThickness(1);
	m_algorithmNameText.setOrigin(sf::Vector2f(m_algorithmNameText.getGlobalBounds().left + m_algorithmNameText.getGlobalBounds().width / 2, m_algorithmNameText.getGlobalBounds().top + m_algorithmNameText.getGlobalBounds().height / 2));

	m_timeTakenText.setFont(m_font);
	m_timeTakenText.setString("Time Taken : ");
	m_timeTakenText.setCharacterSize(13);
	m_timeTakenText.setFillColor(sf::Color::Green);
	m_timeTakenText.setStyle(sf::Text::Italic);
	m_timeTakenText.setOutlineColor(sf::Color::Red);
	m_timeTakenText.setOutlineThickness(1);
	m_timeTakenText.setOrigin(sf::Vector2f(m_timeTakenText.getGlobalBounds().left + m_timeTakenText.getGlobalBounds().width / 2, m_timeTakenText.getGlobalBounds().top + m_timeTakenText.getGlobalBounds().height / 2));

	m_optimalNodeCountText.setFont(m_font);
	m_optimalNodeCountText.setString("Optimal node count: " +  to_string(m_optimalNodeCount));
	m_optimalNodeCountText.setCharacterSize(13);
	m_optimalNodeCountText.setStyle(sf::Text::Italic);
	m_optimalNodeCountText.setFillColor(sf::Color::Green);
	m_optimalNodeCountText.setOutlineColor(sf::Color::Red);
	m_optimalNodeCountText.setOutlineThickness(1);
	m_optimalNodeCountText.setOrigin(sf::Vector2f(m_optimalNodeCountText.getGlobalBounds().left + m_optimalNodeCountText.getGlobalBounds().width / 2, m_optimalNodeCountText.getGlobalBounds().top + m_optimalNodeCountText.getGlobalBounds().height / 2));

	m_nodesvisitedText.setFont(m_font);
	m_nodesvisitedText.setString("Nodes visited : " + to_string(m_nodesVisited));
	m_nodesvisitedText.setCharacterSize(13);
	m_nodesvisitedText.setStyle(sf::Text::Italic);
	m_nodesvisitedText.setFillColor(sf::Color::Green);
	m_nodesvisitedText.setOutlineColor(sf::Color::Red);
	m_nodesvisitedText.setOutlineThickness(1);
	m_nodesvisitedText.setOrigin(sf::Vector2f(m_nodesvisitedText.getGlobalBounds().left + m_nodesvisitedText.getGlobalBounds().width / 2, m_nodesvisitedText.getGlobalBounds().top + m_nodesvisitedText.getGlobalBounds().height / 2));

	m_pathLengthText.setFont(m_font);
	m_pathLengthText.setString("Path Length : " + to_string(m_pathLength));
	m_pathLengthText.setCharacterSize(13);
	m_pathLengthText.setStyle(sf::Text::Italic);
	m_pathLengthText.setFillColor(sf::Color::Green);
	m_pathLengthText.setOutlineColor(sf::Color::Red);
	m_pathLengthText.setOutlineThickness(1);
	m_pathLengthText.setOrigin(sf::Vector2f(m_pathLengthText.getGlobalBounds().left + m_pathLengthText.getGlobalBounds().width / 2, m_pathLengthText.getGlobalBounds().top + m_pathLengthText.getGlobalBounds().height / 2));

	SoundManager::getInstance()->init();
}

/// <summary>
/// creates a grid depending on the level number
/// </summary>
/// <returns></returns>
void Play::loadLevel(int level, vector<vector<Grid>>& m_grid , int& m_gridSize)
{
	
	int type = 0;
	int tileSize = 10;
	int maxRow = 0;
	int column = 0;
	

	if (level == 1)
	{
		m_gridSize = 40;
		for (int row = 0; row < m_gridSize; row++)
		{
			
			vector<Grid> tempVec;
			for (int col = 0; col < m_gridSize; col++)
			{
				if (col == 0 || row == 0 || col == m_gridSize - 1 || row == m_gridSize - 1)
				{
					type = 1;

				}
				else if ((col == 15 || col == 30) && row >= 5 &&  row < 25)
				{
					type = 1;
				}
				else
				{
					type = 0;
				}

				Grid  _temp = Grid(sf::Vector2f(0 + (tileSize*maxRow), 0 + tileSize*column), sf::Vector2f(tileSize, tileSize), static_cast<TileType>(type), row, col);
				
				maxRow = maxRow + 1;
				tempVec.push_back(_temp);

			}
			if (maxRow >= m_gridSize)
			{
				column = column + 1;
				maxRow = 0;
				m_grid.push_back(tempVec);
			}
		}
		m_grid[1][32].setType(TileType::WALL);
		m_walls.push_back(&m_grid[1][32]);
		m_grid[2][32].setType(TileType::WALL);
		m_walls.push_back(&m_grid[2][32]);
		m_grid[3][32].setType(TileType::WALL);
		m_walls.push_back(&m_grid[3][32]);
		m_grid[5][32].setType(TileType::WALL);
		m_walls.push_back(&m_grid[5][32]);
		m_grid[4][32].setType(TileType::WALL);
		m_walls.push_back(&m_grid[4][32]);
		m_grid[6][32].setType(TileType::WALL);
		m_walls.push_back(&m_grid[6][32]);
		m_grid[7][32].setType(TileType::WALL);
		m_walls.push_back(&m_grid[7][32]);
		m_grid[8][32].setType(TileType::WALL);
		m_walls.push_back(&m_grid[8][32]);
		m_grid[8][33].setType(TileType::WALL);
		m_walls.push_back(&m_grid[8][33]);
		m_grid[8][34].setType(TileType::WALL);
		m_walls.push_back(&m_grid[8][34]);
		m_grid[8][35].setType(TileType::WALL);
		m_walls.push_back(&m_grid[8][35]);
		m_grid[7][35].setType(TileType::WALL);
		m_walls.push_back(&m_grid[7][35]);

		m_grid[6][35].setType(TileType::WALL);
		m_walls.push_back(&m_grid[6][35]);
		m_grid[6][34].setType(TileType::WALL);
		m_walls.push_back(&m_grid[6][34]);
		m_grid[4][34].setType(TileType::WALL);
		m_walls.push_back(&m_grid[4][34]);
		m_grid[4][35].setType(TileType::WALL);
		m_walls.push_back(&m_grid[4][35]);
		m_grid[4][36].setType(TileType::WALL);
		m_walls.push_back(&m_grid[4][36]);
		m_grid[4][37].setType(TileType::WALL);
		m_walls.push_back(&m_grid[4][37]);
		m_grid[5][37].setType(TileType::WALL);
		m_walls.push_back(&m_grid[5][37]);
		m_grid[6][37].setType(TileType::WALL);
		m_walls.push_back(&m_grid[6][37]);
		m_grid[7][37].setType(TileType::WALL);
		m_walls.push_back(&m_grid[7][37]);
		m_grid[8][37].setType(TileType::WALL);
		m_walls.push_back(&m_grid[8][37]);
		m_grid[8][38].setType(TileType::WALL);
		m_walls.push_back(&m_grid[8][38]);
	}
	else if (level == 2)
	{
		m_gridSize = 100;
		for (int row = 0; row < m_gridSize; row++)
		{
			
			vector<Grid> tempVec;
			for (int col = 0; col < m_gridSize; col++)
			{
				if (col == 0 || row == 0 || col == m_gridSize - 1 || row == m_gridSize - 1)
				{
					type = 1;

				}
				else if ((col == 15 || col == 30 || col == 45 || col == 60 || col == 75) && row >= 15 && row < 75)
				{
					type = 1;
				}
				else
				{
					type = 0;
				}

				Grid  _temp = Grid(sf::Vector2f(0 + (tileSize*maxRow), 0 + tileSize*column), sf::Vector2f(tileSize, tileSize), static_cast<TileType>(type), row, col);

				maxRow = maxRow + 1;
				tempVec.push_back(_temp);

			}
			if (maxRow >= m_gridSize)
			{
				column = column + 1;
				maxRow = 0;
				m_grid.push_back(tempVec);
			}
		}
	}
	else if (level == 3)
	{
		m_gridSize = 500;
		for (int row = 0; row < m_gridSize; row++)
		{

			vector<Grid> tempVec;
			for (int col = 0; col < m_gridSize; col++)
			{
				Grid  _temp = Grid(sf::Vector2f(0 + (tileSize*maxRow), 0 + tileSize*column), sf::Vector2f(tileSize, tileSize), static_cast<TileType>(type), row, col);
				
				maxRow = maxRow + 1;
				tempVec.push_back(_temp);

			}
			if (maxRow >= m_gridSize)
			{
				column = column + 1;
				maxRow = 0;
				m_grid.push_back(tempVec);
			}
		}
	}
	//sets all node g ,h and f costs and adds all pre defined walls to the wallss vector
	for (int row = 0; row < m_gridSize; row++)
	{
		for (int col = 0; col < m_gridSize; col++)
		{
			m_grid[row][col].setGCost(1000);
			m_grid[row][col].setHCost(1000);
			m_grid[row][col].setFCost(1000);
			m_grid[row][col].setIsDiagonal(false);
			if (m_grid[row][col].getType() == TileType::WALL)
			{
				m_walls.push_back(&m_grid[row][col]);
			}
		}
	}

	

	

}

/// <summary>
///updates the ai and mouse positions and ui text. Also updates the camera
/// </summary>
/// <returns></returns>
void Play::update()
{

	m_algorithmNameText.setPosition(sf::Vector2f(m_camera->getView().getCenter().x , m_camera->getView().getCenter().y - m_camera->getView().getSize().y / 1.9));
	m_timeTakenText.setPosition(sf::Vector2f(m_camera->getView().getCenter().x , m_camera->getView().getCenter().y -m_camera->getView().getSize().y/2.2));
	m_optimalNodeCountText.setPosition(sf::Vector2f(m_camera->getView().getCenter().x , m_camera->getView().getCenter().y - m_camera->getView().getSize().y / 2.4));
	m_nodesvisitedText.setPosition(sf::Vector2f(m_camera->getView().getCenter().x , m_camera->getView().getCenter().y - m_camera->getView().getSize().y / 2.6));
	m_pathLengthText.setPosition(sf::Vector2f(m_camera->getView().getCenter().x, m_camera->getView().getCenter().y - m_camera->getView().getSize().y / 2.9));
	
	m_renderStartX = (m_camera->getView().getCenter().x - (m_camera->getView().getSize().x / 2)) / m_tileSize;
	m_renderEndX = (m_camera->getView().getCenter().x + (m_camera->getView().getSize().x / 2)) / m_tileSize;
	m_renderStartY = (m_camera->getView().getCenter().y - (m_camera->getView().getSize().y / 2)) / m_tileSize;
	m_renderEndY = (m_camera->getView().getCenter().y + (m_camera->getView().getSize().y / 2)) / m_tileSize;

	game->window.setView(m_camera->getView());
	
	
	m_mousePixelPos = sf::Mouse::getPosition(game->window);
	m_mouseWorldPos = game->window.mapPixelToCoords(m_mousePixelPos);
	
	if (m_xEnd != NULL && m_yEnd != NULL)
	{
		m_ai->update(&m_grid[m_xEnd][m_yEnd], *m_aStar, *m_dStar, m_pathfindingDuration,m_timeTakenText,m_optimalNodeCount,m_optimalNodeCountText,m_nodesvisitedText,m_nodesVisited,m_pathLengthText);
	}

	// Process events 

	m_camera->updateCamera();


	game->window.display();
	
	return;
}




/// <summary>
/// checks for user Input
/// </summary>
/// <returns></returns>
void Play::handleInput()
{
	
	while (game->window.pollEvent(*m_Event))
	{
		//if the user presses A , executes the A Star algorithm and records the testing criteria also updating their text.
		if ((m_Event->type == sf::Event::KeyPressed) && (m_Event->key.code == sf::Keyboard::A))
		{
			m_ai->setPathLength(0);
			m_dStar->reset();
			m_aStar->setNodesVisited(0);
			m_aStar->resetType();
			m_aStar->resetExpandednNodesCount();
			high_resolution_clock::time_point t1 = high_resolution_clock::now();
			if (m_xStart != 0 && m_yStart != 0)
			{
				m_ai->setPath(m_aStar->AStarAlgorithm(m_grid[m_xStart][m_yStart], m_grid[m_xEnd][m_yEnd]));
			}
			high_resolution_clock::time_point t2 = high_resolution_clock::now();
			

			m_pathfindingDuration = duration_cast<milliseconds>(t2 - t1).count();

			m_timeTakenText.setString("Time taken : " + to_string(m_pathfindingDuration));
			m_ai->setShouldMove(true);
			m_ai->setIndex(0);

			m_ai->setIsAlteredDStarLite(false);
			m_ai->setIsDStarLite(false);
			m_ai->setIsAStar(true);
			m_ai->setIndex(m_ai->getPath().size() - 1);
			m_optimalNodeCount = m_ai->getPath().size() - 1;
			m_algorithmNameText.setString(" Algorithm : " + m_aStarString);
			m_optimalNodeCountText.setString("Optimal node count:" + to_string(m_optimalNodeCount));
			m_nodesVisited = m_aStar->getNodesVisited();
			m_nodesvisitedText.setString("Nodes visited : " + to_string(m_nodesVisited));

			
			
			
			
		}
		//if the user presses D , executes the D Star Lite algorithm and records the testing criteria also updating their text.
		else if ((m_Event->type == sf::Event::KeyPressed) && (m_Event->key.code == sf::Keyboard::D))
		{
			m_ai->setPathLength(0);
			m_aStar->reset(m_grid[m_xStart][m_yStart]);
			m_dStar->reset();
			m_aStar->resetType();
			m_dStar->setNodesVisited(0);
		
			high_resolution_clock::time_point t1 = high_resolution_clock::now();
			if (m_xStart != 0 && m_yStart != 0)
			{
				m_dStar->init(m_yStart, m_xStart, m_yEnd, m_xEnd);
				for (int i = 0; i < m_walls.size(); i++)
				{
					m_dStar->updateNode(m_walls[i]->getCol(), m_walls[i]->getRow(), -1);
				}

				m_ai->setPath(m_dStar->replan());
			}
			high_resolution_clock::time_point t2 = high_resolution_clock::now();
			m_pathfindingDuration = duration_cast<milliseconds>(t2 - t1).count();
			m_timeTakenText.setString("Time taken : " + to_string(m_pathfindingDuration));
		
			

			m_ai->setShouldMove(true);
			m_ai->setIndex(0);
			m_ai->setIsAStar(false);
			m_ai->setIsAlteredDStarLite(false);
			m_ai->setIsDStarLite(true);
		
			m_algorithmNameText.setString("Algorithm :" + m_dStarLiteString);
			m_optimalNodeCount = m_ai->getPath().size() - 1;
			m_optimalNodeCountText.setString("Optimal node count:" + to_string(m_optimalNodeCount));
			m_nodesVisited = m_dStar->getNodesVisited();
			m_nodesvisitedText.setString("Nodes visited : " + to_string(m_nodesVisited));

			
			
		}
		//if the user presses Space , executes the D Star Lite algorithm and records the testing criteria also updating their text.
		else if ((m_Event->type == sf::Event::KeyPressed) && (m_Event->key.code == sf::Keyboard::Space))
		{
			m_ai->setPathLength(0);
			m_aStar->reset(m_grid[m_xStart][m_yStart]);
			m_dStar->setNodesVisited(0);
			m_dStar->reset();
			m_aStar->resetType();
			high_resolution_clock::time_point t1 = high_resolution_clock::now();
			if (m_xStart != 0 && m_yStart != 0)
			{
				m_dStar->init(m_yStart, m_xStart, m_yEnd, m_xEnd);
				for (int i = 0; i < m_walls.size(); i++)
				{
					m_dStar->updateNode(m_walls[i]->getCol(), m_walls[i]->getRow(), -1);
				}
				m_ai->setAlteredPath(m_dStar->replan());
			}
			high_resolution_clock::time_point t2 = high_resolution_clock::now();
			m_pathfindingDuration = duration_cast<milliseconds>(t2 - t1).count();
			m_timeTakenText.setString("Time taken : " + to_string(m_pathfindingDuration));

			

			

			m_ai->setShouldMove(true);
			m_ai->setIndex(0);
			m_ai->setIsDStarLite(false);
			m_ai->setIsAStar(false);
			m_ai->setIsAlteredDStarLite(true); 
			m_algorithmNameText.setString("Algorithm :" + m_alteredDstarLiteString);
			m_optimalNodeCount = m_ai->getAlteredPath().size() - 1;
			m_optimalNodeCountText.setString("Optimal node count:" + to_string(m_optimalNodeCount));
			m_nodesVisited = m_dStar->getNodesVisited();
			m_nodesvisitedText.setString("Nodes visited : " + to_string(m_nodesVisited));
			m_pathLength = m_ai->getPathLength();

	
		}

		// Close window : exit 
		if (m_Event->type == sf::Event::Closed)
			game->window.close();

		// Escape key : exit 
		if ((m_Event->type == sf::Event::KeyPressed) && (m_Event->key.code == sf::Keyboard::Escape))
			game->window.close();

		m_camera->checkBounds(m_Event, m_gridSize, m_tileSize,m_level);



		//If the user presses the middle mouse button, it updates the node to be of type wall if it is of type tile , vice versa
		if (m_Event->type == sf::Event::MouseButtonPressed)
		{
			if (m_Event->mouseButton.button == sf::Mouse::Middle)
			{
				for (int row = m_renderStartY; row < m_renderEndY; row++)
				{
					for (int col = m_renderStartX; col < m_renderEndX; col++)
					{
						if (m_mouseWorldPos.x > m_grid[row][col].getPosition().x && m_mouseWorldPos.x < m_grid[row][col].getPosition().x + m_grid[row][col].getRectangle().width
							&& m_mouseWorldPos.y > m_grid[row][col].getPosition().y && m_mouseWorldPos.y < m_grid[row][col].getPosition().y + m_grid[row][col].getRectangle().height)
						{
							if (m_grid[row][col].getType() == TileType::WALL)
							{
								m_grid[row][col].setType(TileType::TILE);
								m_dStar->updateNode(m_grid[row][col].getCol(), m_grid[row][col].getRow(), 1);
							}
							else if ( m_grid[row][col].getType() != TileType::START
								&&m_grid[row][col].getType() != TileType::END)
							{
								m_grid[row][col].setType(TileType::WALL);
								if (m_ai->getIsAlteredDstarLite())
								{
									m_dStar->updateNode(m_grid[row][col].getCol(), m_grid[row][col].getRow(), -1);
									m_walls.push_back(&m_grid[row][col]);
								}
								if (m_ai->getIsDStarLite())
								{
									m_dStar->updateNode(m_grid[row][col].getCol(), m_grid[row][col].getRow(), -1);
									m_walls.push_back(&m_grid[row][col]);
								}
								else
								{
									m_walls.push_back(&m_grid[row][col]);
								}
								SoundManager::getInstance()->playSound(SoundType::WALL);
							}
						}
					}
				}
			}
			//If the user presses the left mouse button, it updates the node to be of type Start iunless the node is already of type wall
			else if (m_Event->mouseButton.button == sf::Mouse::Left)
			{
				for (int row = m_renderStartY; row < m_renderEndY; row++)
				{
					for (int col = m_renderStartX; col < m_renderEndX; col++)
					{
						if (m_mouseWorldPos.x > m_grid[row][col].getPosition().x && m_mouseWorldPos.x < m_grid[row][col].getPosition().x + m_grid[row][col].getRectangle().width
							&& m_mouseWorldPos.y > m_grid[row][col].getPosition().y && m_mouseWorldPos.y < m_grid[row][col].getPosition().y + m_grid[row][col].getRectangle().height)
						{
							if (m_grid[row][col].getType() != TileType::WALL)
							{
								if (m_xStart == NULL && m_yStart == NULL)
								{
									m_grid[row][col].setType(TileType::START);
									m_xStart = row;
									m_yStart = col;
									
									m_ai->setPosition(m_grid[row][col].getPosition());
									SoundManager::getInstance()->playSound(SoundType::START);
								}
								else
								{
									m_grid[row][col].setType(TileType::START);
									m_grid[m_xStart][m_yStart].setType(TileType::TILE);
									m_xStart = row;
									m_yStart = col;
									
									if (m_ai->getIsDStarLite())
									{
										m_dStar->updateStartNode(row,col);
									}
									m_ai->setPosition(m_grid[row][col].getPosition());
									SoundManager::getInstance()->playSound(SoundType::START);
								}
							}
						}
					}
				}
			}
			//If the user presses the right mouse button, it updates the node to be of type goal iunless the node is already of type wall
			else if (m_Event->mouseButton.button == sf::Mouse::Right)
			{
				for (int row = m_renderStartY; row < m_renderEndY; row++)
				{
					for (int col = m_renderStartX; col < m_renderEndX; col++)
					{
						if (m_mouseWorldPos.x > m_grid[row][col].getPosition().x && m_mouseWorldPos.x < m_grid[row][col].getPosition().x + m_grid[row][col].getRectangle().width
							&& m_mouseWorldPos.y > m_grid[row][col].getPosition().y && m_mouseWorldPos.y < m_grid[row][col].getPosition().y + m_grid[row][col].getRectangle().height)
						{
							if (m_grid[row][col].getType() != TileType::WALL)
							{
								if (m_xEnd == NULL && m_yEnd == NULL)
								{
									m_grid[row][col].setType(TileType::END);
									m_xEnd = row;
									m_yEnd = col;
									SoundManager::getInstance()->playSound(SoundType::GOAL);
								}
								else
								{
									m_grid[row][col].setType(TileType::END);
									m_grid[m_xEnd][m_yEnd].setType(TileType::TILE);
									m_xEnd = row;
									m_yEnd = col;
									
									if (m_ai->getIsDStarLite())
									{
										m_dStar->updateGoalNode(m_grid[row][col].getCol(), m_grid[row][col].getRow());
									}
									m_ai->setPosition(m_grid[row][col].getPosition());
									SoundManager::getInstance()->playSound(SoundType::GOAL);
								}
							}
						}
					}
				}
			}
		}
	}
}

Play::~Play()
{


}





