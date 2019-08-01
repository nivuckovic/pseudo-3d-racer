#include "SideObject.h"

#include "global.h"
#include "Player.h"
#include "Map.h"

void SideObject::update()
{
}

void SideObject::render(sf::RenderWindow & m_mainWindow)
{
	m_mainWindow.draw(m_sprite);
}

SideObject::SideObject(float _x, float _y, float _z, float _curve, float _scaleFactor, sf::Texture * _m_texture)
{
	x = _x;
	y = _y;
	z = _z;
	
	curve = _curve;
	scaleFactor = _scaleFactor;

	m_sprite.setTexture(*_m_texture);
	m_sprite.setOrigin((float)_m_texture->getSize().x / 2, (float)_m_texture->getSize().y - 4);

}

void SideObject::setPosition(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}

void SideObject::setY(float _y)
{
	y = _y;
}

sf::Vector3f SideObject::getPosition()
{
	return sf::Vector3f(x, y, z);
}

float SideObject::getWidth()
{
	return m_sprite.getTexture()->getSize().x * scale;
}

float SideObject::getCurve()
{
	return curve;
}

float SideObject::getScaleFactor()
{
	return scaleFactor;
}

sf::Sprite * SideObject::getSprite()
{
	return &m_sprite;
}

void SideObject::setSpritePosition(float X, float Y)
{
	m_sprite.setPosition(X, Y);
}

void SideObject::projectSideObject(double lap, float dx, Map * map, Player * player)
{
	double distance = (z + lap * map->getTrackLength()) - player->getPosition();

	setSpritePosition(project(map->getCameraDistance(), x - player->getPlayerX(), distance, (float)WINDOW_WIDTH / 2, (dx + curve)), project(map->getCameraDistance(), map->getCameraY() - y, distance, (float)WINDOW_HEIGHT / 1.75f));

	calculateScale(distance, map, player);
}

void SideObject::calculateScale(float distance, Map * map, Player * player)
{
	scale = calculateScaleLN((float)distance / ((float)map->getDrawDistance() * (float)map->getSegmentLength()));
	m_sprite.setScale(scale * getScaleFactor(), scale * getScaleFactor());
}

SideObject::~SideObject()
{
	
}
