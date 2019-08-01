#pragma once

#include "SFML\Graphics.hpp"

class Player;
class Map;

class SideObject
{
public:
	SideObject() {};

	virtual void update();
	virtual void render(sf::RenderWindow & m_mainWindow);

	SideObject(float _x, float _y, float _z, float _curve, float _scaleFactor, sf::Texture * _m_texture );
	void setPosition(float _x, float _y, float _z);
	void setY(float _y);
	sf::Vector3f getPosition();

	float getWidth();
	float getCurve();
	float getScaleFactor();
	sf::Sprite* getSprite();

	void setSpritePosition(float X, float Y);
	void projectSideObject(double lap, float dx, Map * map, Player * player);
	virtual void calculateScale(float distance, Map * map, Player * player);

	~SideObject();
protected:
	float x;
	float y;
	float z;

	float curve;
	float scaleFactor;
	float scale;

	sf::Sprite m_sprite;
private:
	
};
