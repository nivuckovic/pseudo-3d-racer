#pragma once

#include <SFML\Graphics.hpp>
#include "SideObject.h"

class Map;

class Car : public SideObject
{
public:
	Car(float _x, float _y, float _z, float _curve, float _scaleFactor, sf::Texture * _m_texture);

	void update() override;

	void calculateScale(float distance, Map * map, Player * player) override;
	void setMap(Map * map);
	void setOldSegment(int segment);
	Map * getMap();

	void subtractLap();

	virtual ~Car();
private:
	Map * m_map;

	int tmpTimer;
	int timerCooldown;
	bool timer;

	int lap;

	int oldSegment;
	float m_speed; 
};

