////////////////////////////////////////////////////////////
// Headers:
// ctime for getting system time and
// cmath for sin and cos functions
////////////////////////////////////////////////////////////
#include "stdafx.h"
#define _USE_MATH_DEFINES
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include <cmath> 


const float RADIUS = 200;
const int screenWidth = 600;
const int screenHeight = 600;
const int clockCircleSize = 200;
const int clockCircleThickness = 2;
const int FIGURES_SIZE = 40;
const int FIGURE_DISTANE = 10;
const int LINES_COUNT = 60;
const int NUMBERS_COUNT = 12;
const sf::Vector2f CENTER = { 300.f, 300.f };

struct Time
{
	static const int MAX_HOURS = 12;
	static const int MAX_MINUTES = 60;
	static const int MAX_SECONDS = 60;

	static Time now();

	short hours;
	short min;
	short sec;
};

struct Clock
{
	sf::CircleShape circle;	
	sf::Sprite sprite;
	sf::ConvexShape arrowMinutes;
	sf::ConvexShape arrowSeconds;	
	sf::ConvexShape arrowHours;	
	sf::CircleShape hourDot[LINES_COUNT];
	sf::RectangleShape minutesLine[LINES_COUNT];

	sf::Font font;
	sf::Text numbers[LINES_COUNT];
	sf::Music clockTick;

	void createCircle(Clock & clock);
	void createDots(Clock & clock);
	void Clock::createNumbers(Clock & clock);
	bool setBackground(Clock & clock, sf::Texture &clockImage);
	void createArrows(Clock & clock);

	sf::ConvexShape newArrow(sf::Vector2<sf::Vector2f>, const int, const int, sf::Color);
	sf::Texture clockImage;
};

int main()
{	
	
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "SFML Analog Clock", sf::Style::Close, settings);

	Clock clock;
	clock.createCircle(clock);	

	if (!clock.font.loadFromFile("resources/arial.ttf"))
		return EXIT_FAILURE;	

	clock.setBackground(clock, clock.clockImage);
	clock.createDots(clock);

	while (window.isOpen())
	{			
		clock.createArrows(clock);

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color::White);
		window.draw(clock.circle);		

		for (int i = 0; i < LINES_COUNT; i++)
		{
			window.draw(clock.hourDot[i]);
			window.draw(clock.minutesLine[i]);			
		}

		for (int i = 0; i < NUMBERS_COUNT; i++)
		{			
			clock.createNumbers(clock);
			window.draw(clock.numbers[i]);
		}

		window.draw(clock.arrowHours);
		window.draw(clock.arrowMinutes);
		window.draw(clock.arrowSeconds);
		
		// Display things on screen
		window.display();
	}

	return EXIT_SUCCESS;
}

// Create clock background
bool Clock::setBackground(Clock & clock, sf::Texture &clockImage)
{	
	if (!clockImage.loadFromFile("resources/clock-image.jpg"))
		return EXIT_FAILURE;
	clock.circle.setTexture(&clockImage);
	clock.circle.setTextureRect(sf::IntRect(40, 0, 500, 500)); 
	return true;
}

// set current time
Time Time::now()
{
	time_t date = time(nullptr);
	tm currentTime;
	localtime_s(&currentTime, &date);
	Time newTime;
	newTime.sec = currentTime.tm_sec;
	newTime.min = currentTime.tm_min;
	newTime.hours = currentTime.tm_hour;
	return newTime;
}

sf::ConvexShape Clock::newArrow(sf::Vector2<sf::Vector2f> params, const int amount, const int current, sf::Color color = sf::Color::Black)
{
	sf::ConvexShape arrow;
	arrow.setPointCount(3);
	arrow.setPoint(0, sf::Vector2f(0, 0));
	arrow.setPoint(1, sf::Vector2f(params.x.x / 1.f, params.x.y / 1.f));
	arrow.setPoint(2, sf::Vector2f(0, params.x.y));
	arrow.setOrigin(0, params.x.y);
	arrow.setPosition(CENTER.x, CENTER.y);
	arrow.rotate(360.f / amount * current);
	arrow.setFillColor(color);
	return arrow;
}

namespace ARROW_SIZE
{
	const sf::Vector2<sf::Vector2f> SEC = { { 9, RADIUS - 18 },{ 8, 30 } };
	const sf::Vector2<sf::Vector2f> MIN = { { 11, RADIUS - 26 },{ 12, 20 } };
	const sf::Vector2<sf::Vector2f> HOUR = { { 13, RADIUS - 60 },{ 16, 20 } };
};

void Clock::createArrows(Clock & clock)
{	
	Time currentTime = Time::now();
	clock.arrowHours = newArrow(ARROW_SIZE::HOUR, currentTime.MAX_HOURS, currentTime.hours, sf::Color(45, 45, 45));
	clock.arrowMinutes = newArrow(ARROW_SIZE::MIN, currentTime.MAX_MINUTES, currentTime.min);
	clock.arrowSeconds = newArrow(ARROW_SIZE::SEC, currentTime.MAX_SECONDS, currentTime.sec, sf::Color::Red);
}

// Create dots and place them to very right positions
void Clock::createDots(Clock & clock)
{
	float angleLines = ((2 * (float)(M_PI)) / LINES_COUNT);
	int dotsOffset = 10;

	for (int i = 0; i < LINES_COUNT; i++)
	{
		const float angle = (float(i) / LINES_COUNT) * 2 * float(M_PI);
		const float radius = clockCircleSize - dotsOffset;
		const sf::Vector2f offset = {
			roundf(radius * cos(angle)),
			roundf(radius * sin(angle))
		};

		if (i % 5 == 0)
		{
			hourDot[i] = sf::CircleShape(3);
		}
		else
		{
			minutesLine[i] = sf::RectangleShape(sf::Vector2f(1, 12));
		}

		minutesLine[i].rotate(angleLines);
		hourDot[i].setFillColor(sf::Color::Black);
		minutesLine[i].setFillColor(sf::Color::Black);
		hourDot[i].setOrigin(hourDot[i].getGlobalBounds().width / 2, hourDot[i].getGlobalBounds().height / 2);
		hourDot[i].setPosition(CENTER + offset);
		minutesLine[i].setOrigin(minutesLine[i].getGlobalBounds().width / 2, minutesLine[i].getGlobalBounds().height / 2);
		minutesLine[i].setPosition(CENTER + offset);
		minutesLine[i].setRotation(float(i * 6 + 90));
	}	
}

void Clock::createNumbers(Clock & clock)
{
	for (int i = 0; i < NUMBERS_COUNT; i++)
	{
		clock.numbers[i].setFont(font);
		clock.numbers[i].setColor(sf::Color::Black);
		clock.numbers[i].setStyle(sf::Text::Bold);
		clock.numbers[i].setCharacterSize(FIGURES_SIZE);
		clock.numbers[i].setString(std::to_string(i + 1));
		sf::FloatRect textRect = clock.numbers[i].getLocalBounds();
		clock.numbers[i].setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
		clock.numbers[i].setPosition(float(-sin(-M_PI * (i + 1) / 6) * (RADIUS - FIGURES_SIZE - FIGURE_DISTANE) + CENTER.x), float(-cos(-M_PI * (i + 1) / 6) * (RADIUS - FIGURES_SIZE - FIGURE_DISTANE) + CENTER.y));
	}	
}

void Clock::createCircle(Clock & clock)
{
	sf::CircleShape circle(RADIUS);
	circle.setOrigin(RADIUS, RADIUS);
	circle.setPosition(CENTER);
	circle.setOutlineThickness(2);
	circle.setOutlineColor(sf::Color::Black);
	circle.setPointCount(100);
	clock.circle = circle;
}