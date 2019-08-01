#pragma once

#include <SFML\Graphics.hpp>

class Map;

class Player {
public:
	Player();

	void update();
	void render(sf::RenderWindow & m_mainWindow);

	float getPosition();
	float getPlayerX();
	float getWidth();

	void setMap(Map * map);
	Map* getMap();

	float getSpeed();

	~Player();
private:
	Map * m_map;

	sf::Texture m_textureCar;
	sf::Sprite m_spriteCar;

	float scale = 6.5f;

	sf::Texture m_textureCarLeft;
	sf::Texture m_textureCarRight;

	float position = 0;
	float playerX = 0;
	float rotation = 0;

	float centrifugal = 0.35f;
	float dz = 0;

	bool keys[4] = { 0, 0, 0, 0 }; //UP, DOWN; LEFT, RIGHT
};

