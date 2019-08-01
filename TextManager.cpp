#include "TextManager.h"

#include "global.h"
#include "Player.h"
#include "Map.h"

TextManager::TextManager()
{
	if (!font.loadFromFile("SFAlienEncounters.ttf")) {
		printf("font_error");
	}

	m_timeText.setFont(font);
	m_timeText.setString("0");
	m_timeText.setCharacterSize(72);
	m_timeText.setFillColor(sf::Color::Yellow);


	m_bestTimeText.setFont(font);
	m_bestTimeText.setString("0.000000");
	m_bestTimeText.setCharacterSize(72);
	m_bestTimeText.setFillColor(sf::Color::Yellow);
	m_bestTimeText.setPosition(WINDOW_WIDTH - 425, 0);


	m_lapText.setFont(font);
	m_lapText.setString("LAP : 0");
	m_lapText.setCharacterSize(100);
	m_lapText.setFillColor(sf::Color::Yellow);
	m_lapText.setPosition(WINDOW_WIDTH / 2 - 225, 0);


	m_speedText.setFont(font);
	m_speedText.setString("SPEED : 0");
	m_speedText.setCharacterSize(48);
	m_speedText.setFillColor(sf::Color::Yellow);
	m_speedText.setPosition(0, WINDOW_HEIGHT - 50);
}

void TextManager::update()
{
	if (m_map->getBaseSegment() == m_map->getStartSegment() - 4) {
		if (start) {
			if (m_timeClock.getElapsedTime().asSeconds() > 1.f) {
				if (bestTime > m_timeClock.getElapsedTime().asSeconds()) {
					bestTime = m_timeClock.getElapsedTime().asSeconds();
					m_bestTimeText.setString(std::to_string(bestTime));
				}

				++lap;
				m_lapText.setString("LAP : " + std::to_string(lap));
			}

			m_timeClock.restart();
		}
		else {
			m_timeClock.restart();
			start = true;
		}
	}

	m_timeText.setString((start) ? std::to_string(m_timeClock.getElapsedTime().asSeconds()) : "0.000000");
	m_speedText.setString("SPEED : " + std::to_string(m_player->getSpeed()));
}

void TextManager::render(sf::RenderWindow & m_mainWindow)
{
	m_mainWindow.draw(m_timeText);
	m_mainWindow.draw(m_bestTimeText);
	m_mainWindow.draw(m_lapText);
	m_mainWindow.draw(m_speedText);
}

void TextManager::setMap(Map * map)
{
	m_map = map;
}

void TextManager::setPlayer(Player * player)
{
	m_player = player;
}


TextManager::~TextManager()
{
	m_map = nullptr;
	m_player = nullptr;
}
