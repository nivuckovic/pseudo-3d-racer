#include "Game.h"
#include <vector>
#include <algorithm>
#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>
#include <cstdlib>

#include "global.h"

#include "Map.h"
#include "Player.h"
#include "TextManager.h"

Game::Game()
{
	m_mainWindow.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Pseudo 3D Racer");
	m_renderTexture.create(WINDOW_WIDTH, WINDOW_HEIGHT);
}

void Game::run()
{
	m_mainWindow.setFramerateLimit(60);

	std::srand(std::time(nullptr));

	//loadFromFile("C:\Users\Korko\Documents\Visual Studio 2017\Projects\Pseudo3DRacer\car.png")

	Map m_map;
	Player m_player;
	TextManager m_textManager;

	m_map.setPlayer(&m_player);
	m_player.setMap(&m_map);
	m_textManager.setMap(&m_map);
	m_textManager.setPlayer(&m_player);

	while (m_mainWindow.isOpen())
	{
		sf::Event event;
		while (m_mainWindow.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				m_mainWindow.close();
			}
		}

		m_player.update();
		m_map.update();
		m_textManager.update();


		m_mainWindow.clear();	
		
		m_map.render(m_mainWindow);
		m_player.render(m_mainWindow);
		m_textManager.render(m_mainWindow);

		m_mainWindow.display();
	}
}

void Game::render(sf::RenderWindow & m_mainWindow)
{
	
}


Game::~Game()
{
}



