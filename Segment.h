#pragma once

#include "SFML\Graphics.hpp"

struct Segment {
	Segment() {};
	Segment(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
	}

	float x;
	float y;
	float z;

	float curve;
	sf::Color color;
};