#include "Player.h"

#include "global.h"
#include "Map.h"
#include <SFML\System.hpp>

enum Keys {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

Player::Player()
{
	if (!m_textureCar.loadFromFile("car.png")) {
		printf("ERROR\n");
	}

	m_spriteCar.setTexture(m_textureCar);
	m_spriteCar.setOrigin((float)m_textureCar.getSize().x / 2, (float)m_textureCar.getSize().y / 2);

	m_spriteCar.setScale(sf::Vector2f(scale, scale));
	m_spriteCar.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 80);

	if (!m_textureCarLeft.loadFromFile("car_left.png")) {
		printf("ERROR\n");
	}

	if (!m_textureCarRight.loadFromFile("car_right.png")) {
		printf("ERROR\n");
	}

}

void Player::update()
{

	for (int i = 0; i < 4; ++i) {
		keys[i] = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		keys[Keys::UP] = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		keys[LEFT] = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		keys[RIGHT] = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		keys[DOWN] = true;
	}

	bool outLeft = false, outRight = false;

	if (playerX < -4500) {
		outLeft = true;
	}

	if (playerX > 4500) {
		outRight = true;
	}

	float maxSpeed = 70;

	if (keys[UP]) {
		if (getMap()->collisionAll()) {
			dz -= 50;
			if (dz < 0) {
				dz = 0;
			}
		}
		dz += 1.f;
		if (dz > maxSpeed) {
			dz = maxSpeed;
		}
	}
	else if (keys[DOWN]) {
		dz -= 1.5f;
		if (dz < 0) {
			dz = 0;
		}
	}
	else {
		dz -= 0.8f;
		if (dz < 0) {
			dz = 0;
		}
	}
	
	float angle = 40;

	if (keys[RIGHT] && dz > 0 && !outRight) {
		m_spriteCar.setTexture(m_textureCarRight);
		playerX += angle;
	}

	if (keys[LEFT] && dz > 0 && !outLeft) {
		m_spriteCar.setTexture(m_textureCarLeft);
		playerX -= angle;
	}

	if (!(keys[LEFT] || keys[RIGHT])) {
		m_spriteCar.setTexture(m_textureCar);
	}

	position += dz;
	if (position >= getMap()->getTrackLength()) {
		position = position - getMap()->getTrackLength();
		//getMap()->substractLapCars();
	}

	if (playerX < -2800 || playerX > 2800) {
		if (dz > 20) {
			dz -= 2.2;
		}
	}

	if (!outLeft && !outRight) {
		playerX -= getMap()->getBaseSegmentCurve() * centrifugal * (dz / 4.5);
	}

	getMap()->calculateBaseSegment(getPosition());

	m_spriteCar.setScale(scale * calculateScale(getMap()->getHeight(), 1000), scale * calculateScale(getMap()->getHeight(), 1000));
}

void Player::render(sf::RenderWindow & m_mainWindow)
{
	m_mainWindow.draw(m_spriteCar);
}

float Player::getPosition()
{
	return position;
}

float Player::getPlayerX()
{
	return playerX;
}

float Player::getWidth()
{
	return m_textureCar.getSize().x * m_spriteCar.getScale().x;
}

void Player::setMap(Map * map)
{
	m_map = map;
}

Map * Player::getMap()
{
	return m_map;
}

float Player::getSpeed()
{
	return dz;
}

Player::~Player()
{
	m_map = nullptr;
}
