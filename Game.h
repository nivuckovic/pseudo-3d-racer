#pragma once

#include <SFML/Graphics.hpp>

class Game
{
public:
	Game();

	void run();
	void render(sf::RenderWindow & m_mainWindow);

	~Game();
private:
	sf::RenderWindow m_mainWindow;
	sf::RenderTexture m_renderTexture;
};

