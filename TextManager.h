#pragma once

#include <SFML\Graphics.hpp>

class Map;
class Player;

class TextManager
{
public:
	TextManager();

	void update();
	void render(sf::RenderWindow& m_mainWindow);

	void setMap(Map * map);
	void setPlayer(Player * player);

	~TextManager();
private:
	Map * m_map;
	Player * m_player;

	sf::Font font;

	sf::Text m_timeText;
	sf::Text m_bestTimeText;
	sf::Text m_lapText;
	sf::Text m_speedText;

	sf::Clock m_timeClock;
	bool start = false;
	float bestTime = 10000;
	int lap = 0;
};

