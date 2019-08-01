#pragma once

#include <vector>
#include <SFML\Graphics.hpp>

struct Segment;

class SideObject;
class Player;
class Car;

class Map {
public:
	Map();
	~Map();

	void update();
	void render(sf::RenderWindow & m_mainWindow);

	int findSegment(float position);

	float lastY();

	void addSegment(double curve, double y, sf::Color color);
	template <typename T>
	T* addSideObject(int segment, sf::Texture * m_texture, int _x, float scaleFactor = 1);
	void addRoad(int enter, int hold, int leave, float curve, float y, sf::Color color = sf::Color(0, 0, 0));
	void createRoad();
	void createSideobjects();

	void projectSegment(int segment, double lap, float dx, double segmentWidth, std::vector<Segment>& objectSegments, std::vector<sf::VertexArray>& v_objectSegments);

	float getBaseSegmentCurve();
	int getTrackLength();
	int getBaseSegment();
	int getStartSegment();
	float getHeight();
	int getCameraDistance();
	float getCameraY();
	int getDrawDistance();
	int getSegmentLength();
	float getBasePercent();
	float getDY(int segment);

	bool collisionAll();
	bool collision(SideObject * object);

	void moveCar(Car * car, int segment);
	void substractLapCars();

	void calculateBaseSegment(float position);

	void setPlayer(Player * player);
	Player* getPlayer();
private:
	Player * m_player;

	sf::Texture m_background;
	sf::Sprite m_spriteBackground;

	sf::Texture m_towerTexture;
	sf::Texture m_treeTexture;
	sf::Texture m_rockTexture;
	sf::Texture m_sideCarTexture;

	int d = 125;
	int fov = 100;
	int segmentLength = 100;
	int segmentWidth = 5000;
	int grassWidth = 200000;
	int numberOfSegments = 100;
	int trackLength;

	int baseSegment;
	int startSegment = 16;
	int oldSegment;
	float basePercent;

	std::vector<Segment> segments;
	std::vector<Segment> grassSegments;
	std::vector<Segment> centralLineSegments;
	std::vector<Segment> outLineSegments;

	std::vector<std::vector<SideObject*>> sideObjects;
	std::vector<Car*> sideCars;

	float cameraHeight = 1100;
	float cameraY = cameraHeight;
	float dy;

	int direction[4] = { 0, 1, 1, 0 };
	int sign[4] = { -1, -1, 1, 1 };

	int drawDistance = 175;
};
