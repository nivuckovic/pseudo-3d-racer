#include "Car.h"

#include "global.h"
#include "Map.h"
#include "Player.h"

Car::Car(float _x, float _y, float _z, float _curve, float _scaleFactor, sf::Texture * _m_texture) : SideObject(_x, _y, _z, _curve, _scaleFactor, _m_texture)
{
	m_sprite.setScale(scale, scale);
	m_speed = 35;

	curve = 0;
	lap = 0;

	timerCooldown = 5000;
	timer = false;
}

void Car::update()
{
	if (getMap()->collision(this)) {
		x *= -1;
		
		tmpTimer = 0;
		timer = true;
	}

	z += m_speed;

	//seljacki timer promjenit!!
	tmpTimer += m_speed;
	if (tmpTimer >= timerCooldown && timer) {
		x *= -1;
		timer = false;
	}

	int segment = getMap()->findSegment(z) / 2;

	if (oldSegment != segment) {
		getMap()->moveCar(this, oldSegment);
		oldSegment = segment;
	}

	if (z >= getMap()->getTrackLength()) {
		z -= getMap()->getTrackLength();
		++lap;
	}

	y += getMap()->getDY(segment * 2) /  (getMap()->getSegmentLength() / m_speed);
}

void Car::calculateScale(float distance, Map * map, Player * player)
{
	scale = (float)calculateScaleSideCar((float)(distance + 725) / ((float)map->getDrawDistance() * (float)map->getSegmentLength()));
	m_sprite.setScale(scale * getScaleFactor(), scale * getScaleFactor());
}

void Car::setMap(Map * map)
{
	m_map = map;
}

void Car::setOldSegment(int segment)
{
	oldSegment = segment;
}

Map * Car::getMap()
{
	return m_map;
}

void Car::subtractLap()
{
	z -= getMap()->getTrackLength() * lap;
}

Car::~Car()
{
	m_map = nullptr;
}
