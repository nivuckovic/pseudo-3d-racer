#include "Segment.h"
#include "SideObject.h"
#include "Player.h"
#include "Map.h"

#include "global.h"
#include "Car.h"

Map::Map()
{
	if (!m_background.loadFromFile("background.png")) {
		printf("background");
	}
	m_spriteBackground.setTexture(m_background);
	m_spriteBackground.setScale(2.5, 1.25);
	m_spriteBackground.setOrigin((float)m_background.getSize().x / 2, (float)m_background.getSize().y / 1.95f);

	if (!m_towerTexture.loadFromFile("tower.png")) {
		printf("ERROR\n");
	}

	if (!m_treeTexture.loadFromFile("tree.png")) {
		printf("ERROR\n");
	}

	if (!m_rockTexture.loadFromFile("rock.png")) {
		printf("ERROR\n");
	}

	if (!m_sideCarTexture.loadFromFile("sideCar.png")) {
		printf("ERROR\n");
	}

	createRoad();
	createSideobjects();
}

Map::~Map()
{
	m_player = nullptr;
	for (unsigned int i = 0; i < sideObjects.size(); ++i) {
		for (std::vector<SideObject*>::iterator it = sideObjects[i].begin(); it != sideObjects[i].end(); ) {
			delete *it;
			*it = nullptr;

			it = sideObjects[i].erase(it);
		}
	}

}

void Map::update()
{
	m_spriteBackground.setPosition(getPlayer()->getPlayerX() * 0.05f, 0);
	for (unsigned int i = 0; i < sideCars.size(); ++i) {
		sideCars[i]->update();
	}
}

void Map::render(sf::RenderWindow & m_mainWindow)
{
	m_mainWindow.draw(m_spriteBackground);

	float dx = 0;
	basePercent = 1 - abs(getPlayer()->getPosition() - (segments[baseSegment + 1].z)) / segmentLength;
	dx = -(segments[baseSegment + 1].curve * basePercent);

	dy = segments[(baseSegment + 1) % segments.size()].y - segments[(baseSegment ) % segments.size()].y;

	if (baseSegment != oldSegment) {
		cameraHeight += dy;
		oldSegment = baseSegment;
	}

	cameraY = (float)cameraHeight + dy * (basePercent);

	std::vector<sf::VertexArray> v_roadSegments;
	std::vector<sf::VertexArray> v_grassSegments;
	std::vector<sf::VertexArray> v_centralLineSegments;
	std::vector<sf::VertexArray> v_outLineSegments;

	for (int i = 0; i < drawDistance * 2; i += 2) {
		int segment = (baseSegment + i) % (segments.size());

		double lap = floor((baseSegment + i) / (segments.size()));

		projectSegment(segment, lap, dx, segmentWidth / 2, segments, v_roadSegments);
		projectSegment(segment, lap, dx, grassWidth, grassSegments, v_grassSegments);
		projectSegment(segment, lap, dx, segmentWidth * 0.02 / 2, centralLineSegments, v_centralLineSegments);
		projectSegment(segment, lap, dx, segmentWidth * 1.1 / 2, outLineSegments, v_outLineSegments);

		for (unsigned int j = 0; j < sideObjects[segment / 2].size(); ++j) {
			sideObjects[segment / 2][j]->projectSideObject(lap, dx, this, getPlayer());
		}

		dx += segments[segment + 1].curve;
	}

	for (int i = v_roadSegments.size() - 1; i >= 0; --i) {
		m_mainWindow.draw(v_grassSegments[i]);
		m_mainWindow.draw(v_outLineSegments[i]);
		m_mainWindow.draw(v_roadSegments[i]);
		m_mainWindow.draw(v_centralLineSegments[i]);

		int k = (baseSegment / 2 + i) % (sideObjects.size());
		for (unsigned int j = 0; j < sideObjects[k].size(); ++j) {
			sideObjects[k][j]->render(m_mainWindow);
		}

	}

}

int Map::findSegment(float position)
{
	if (segments.size() == 0) {
		return 0;
	}
	
	return ((int)floor(position / segmentLength) % (segments.size() / 2)) * 2;
}

float Map::lastY() {
	if (segments.size() == 0) {
		return 0;
	}

	return segments[segments.size() - 1].y;
}

void Map::addSegment(double curve, double y, sf::Color color) {
	int size = segments.size() / 2;
	Segment p1(0, lastY(), (float)(size * segmentLength));
	Segment p2(0, y, (float)((size + 1) * segmentLength));

	p1.curve = 0;
	p2.curve = curve;

	p1.color = (segments.size() % 4 == 2) ? sf::Color(66, 66, 66) : sf::Color(69, 69, 69);

	segments.push_back(p1);
	segments.push_back(p2);

	p1.color = (segments.size() % 8 == 2) ? sf::Color(255, 255, 255) : p1.color;

	centralLineSegments.push_back(p1);
	centralLineSegments.push_back(p2);

	p1.color = (segments.size() % 4 == 2) ? sf::Color(55, 160, 29) : sf::Color(46, 142, 20);

	grassSegments.push_back(p1);
	grassSegments.push_back(p2);

	p1.color = (segments.size() % 4 == 2) ? sf::Color(255, 50, 0) : sf::Color(255, 255, 255);

	outLineSegments.push_back(p1);
	outLineSegments.push_back(p2);
}

template <typename T>
T* Map::addSideObject(int segment, sf::Texture * m_texture, int _x, float scaleFactor) {
	T* object = new T((float)_x, (segments[segment * 2 + 1].y + segments[segment * 2].y) / 2, (segments[segment * 2 + 1].z + segments[segment * 2].z) / 2, segments[segment * 2 + 1].curve, scaleFactor, m_texture);

	sideObjects[segment].push_back(object);

	return object;
}


void Map::addRoad(int enter, int hold, int leave, float curve, float y, sf::Color color) {
	float startY = lastY();
	float endY = startY + y;

	int total = enter + hold + leave;

	for (int i = 0; i < enter; ++i) {
		addSegment(easeIn(0, curve, (float)i / enter), easeInOut(startY, endY, (float)i / total), color);
	}

	for (int i = 0; i < hold; ++i) {
		addSegment(curve, easeInOut(startY, endY, (float)(enter + i) / total), color);
	}

	for (int i = 0; i < leave; ++i) {
		addSegment(easeInOut(curve, 0, (float)i / leave), easeInOut(startY, endY, (float)(enter + hold + i) / total), color);
	}
}

void Map::createRoad() {

	addRoad(0, 25, 0, 0, 0);
	addRoad(30, 10, 30, 0, 8000);
	addRoad(0, 10, 0, 0, 0);
	addRoad(20, 30, 10, 20, 0);
	addRoad(0, 10, 0, 0, 0);
	addRoad(10, 25, 10, -30, 0);
	addRoad(0, 10, 0, 0, 0);
	addRoad(15, 70, 15, 10, 0);
	addRoad(40, 10, 20, 0, -8000);
	addRoad(0, 8, 0, 0, 0);

	segments[startSegment].color = sf::Color::White;

	trackLength = segments.size() / 2 * segmentLength;
}

void Map::createSideobjects() {
	for (unsigned int i = 0; i < segments.size() / 2; ++i) {
		sideObjects.push_back(std::vector<SideObject*>());
	}

	for (unsigned int i = 0; i < sideObjects.size(); ++i) {
		if (i % 8 == 4) {

		}
		if (i % 16 == 8) {
			addSideObject<SideObject>(i, &m_towerTexture, (segmentWidth + 3000), randomNumber(0.8f, 1.f));
			addSideObject<SideObject>(i, &m_treeTexture, -(segmentWidth + 3000), randomNumber(0.6f, 0.9f));

			addSideObject<SideObject>(i, &m_rockTexture, (segmentWidth + 12000), 0.5f);
			addSideObject<SideObject>(i, &m_rockTexture, -(segmentWidth + 12000), 0.5f);
		}

		if (i % 32 == 8 || i % 32 == 24) {
			addSideObject<SideObject>(i, &m_treeTexture, (segmentWidth + 18000), randomNumber(0.3f, 0.5f));
			addSideObject<SideObject>(i, &m_treeTexture, -(segmentWidth + 18000), randomNumber(0.3f, 0.5f));

			addSideObject<SideObject>(i, &m_rockTexture, (segmentWidth + 21000), 0.25f);
			addSideObject<SideObject>(i, &m_rockTexture, -(segmentWidth + 21000), 0.25f);

			addSideObject<SideObject>(i, &m_treeTexture, (segmentWidth + 27000), 0.25f);
			addSideObject<SideObject>(i, &m_treeTexture, -(segmentWidth + 27000), 0.25f);
		}
	}

	for (int i = 0; i < 16; ++i) {
		sideCars.push_back(addSideObject<Car>(i * 25, &m_sideCarTexture, ((i % 2) ? -1 : 1) * segmentWidth / 4, 0.4f));
		sideCars.back()->setMap(this);
		sideCars.back()->setOldSegment(i * 25);
	}
}

void Map::setPlayer(Player * player)
{
	m_player = player;
}

Player * Map::getPlayer()
{
	return m_player;
}

void Map::projectSegment(int segment, double lap, float dx, double segmentWidth, std::vector<Segment>& objectSegments, std::vector<sf::VertexArray>& v_objectSegments) {
	float screenX, screenY;

	sf::VertexArray objectSegment(sf::Quads, 4);

	for (int j = 0; j < 4; ++j) {
		double objectX = objectSegments[segment + direction[j]].x + sign[j] * segmentWidth;
		double objectY = objectSegments[segment + direction[j]].y;

		//screenX = d * dx / dz
		double distance = abs(getPlayer()->getPosition() - (objectSegments[segment + direction[j]].z + lap * trackLength));
		screenX = project((float)d, objectX - getPlayer()->getPlayerX(), distance, (float)WINDOW_WIDTH / 2, (dx + objectSegments[segment + direction[j]].curve));
		screenY = project((float)d, cameraY - objectY, distance, (float)WINDOW_HEIGHT / 1.75f);

		objectSegment[j].position = sf::Vector2f(screenX, screenY);
	}

	for (int j = 0; j < 4; ++j) {
		objectSegment[j].color = objectSegments[segment].color;
	}

	v_objectSegments.push_back(objectSegment);
}

float Map::getBaseSegmentCurve()
{
	return segments[baseSegment + 1].curve;
}

int Map::getTrackLength()
{
	return trackLength;
}

int Map::getBaseSegment()
{
	return baseSegment;
}

int Map::getStartSegment()
{
	return startSegment;
}

float Map::getHeight()
{
	return dy;
}

int Map::getCameraDistance()
{
	return d;
}

float Map::getCameraY()
{
	return cameraY;
}

int Map::getDrawDistance()
{
	return drawDistance;
}

int Map::getSegmentLength()
{
	return segmentLength;
}

float Map::getBasePercent()
{
	return basePercent;
}

float Map::getDY(int segment)
{
	return segments[(segment + 1) % segments.size()].y - segments[(segment) % segments.size()].y;
}

bool Map::collisionAll()
{
	for (unsigned int i = 0; i < sideCars.size(); ++i) {
		if ((abs(m_player->getPlayerX() - sideCars[i]->getPosition().x) < sideCars[i]->getWidth() / 2 + m_player->getWidth() / 2) && sideCars[i]->getPosition().z - m_player->getPosition() < 500 && (sideCars[i]->getPosition().z) - m_player->getPosition() > 0) {
			return true;
		}
	}

	return false;
}

bool Map::collision(SideObject * object)
{
	if ((abs(m_player->getPlayerX() - object->getPosition().x) < object->getWidth() / 2 + m_player->getWidth() / 2) && object->getPosition().z - m_player->getPosition() < 500 && m_player->getPosition() - object->getPosition().z > 0) {
		return true;
	}

	return false;
}

void Map::moveCar(Car * car, int segment)
{
	for (std::vector<SideObject*>::iterator it = sideObjects[segment].begin(); it != sideObjects[segment].end(); ++it) {
		if (*it == car) {
			sideObjects[segment].erase(it);
			break;
		}
	}
	
	sideObjects[(segment + 1) % sideObjects.size()].push_back(car);
}

void Map::substractLapCars()
{
	for (unsigned int i = 0; i < sideCars.size(); ++i) {
		sideCars[i]->subtractLap();
	}
}

void Map::calculateBaseSegment(float position)
{
	baseSegment = findSegment(m_player->getPosition());
}
