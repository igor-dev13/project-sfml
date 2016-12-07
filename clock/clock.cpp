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


const float RADIUS = 170;
const int screenWidth = 800;
const int screenHeight = 600;
const float PI = 3.1415927;
const int clockCircleSize = 250;
const int clockCircleThickness = 2;
const int FIGURES_SIZE = 40;

int x, y, timeX, timeY;
float angle = 0.0;
float angletime = 11.53;
float angleLines = ((2 * PI) / 60);

const int LINES_COUNT = 60;
const int NUMBERS_COUNT = 12;
const sf::Vector2f CENTER = { 400.f, 300.f };


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
	
	bool createSoundEffect(Clock & clock);
	bool setBackground(Clock & clock);
	void createArrows(Clock & clock);

	sf::ConvexShape newArrow(sf::Vector2<sf::Vector2f>, const int, const int, sf::Color);
};

namespace ARROW_SIZE
{
	const sf::Vector2<sf::Vector2f> SEC = { { 8, RADIUS - 20 },{ 8, 30 } };
	const sf::Vector2<sf::Vector2f> MIN = { { 10, RADIUS - 30 },{ 10, 20 } };
	const sf::Vector2<sf::Vector2f> HOUR = { { 14, RADIUS - 70 },{ 14,20 } };
};

struct Time
{
	short hours;
	short min;
	short sec;
	const int MAX_HOURS = 12;
	const int MAX_MINUTES = 60;
	const int MAX_SECONDS = 60;
	Time * newTime();
};

int main()
{	
	// Set multisampling level
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	// Create the window of the application
	sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "SFML Analog Clock", sf::Style::Close, settings);
	// Define windowCenter which gets the center of the window here, right after creating window
	//sf::Vector2f windowCenter = sf::Vector2f(screenWidth / 2.0f, screenHeight / 2.0f);

	Clock clock;
	
	//clock.createCircleCenter(clock);

	clock.createCircle(clock);
	clock.setBackground(clock);
	clock.createDots(clock);

	if (!clock.font.loadFromFile("resources/arial.ttf"))
		return EXIT_FAILURE;	



	//// Create outline of the clock
	//sf::CircleShape clockCircle(clockCircleSize);

	//clockCircle.setPointCount(100);
	//clockCircle.setOutlineThickness(clockCircleThickness);
	//clockCircle.setOutlineColor(sf::Color::Black);
	//clockCircle.setOrigin(clockCircle.getGlobalBounds().width / 2, clockCircle.getGlobalBounds().height / 2);
	//clockCircle.setPosition(window.getSize().x / 2 + clockCircleThickness, window.getSize().y / 2 + clockCircleThickness);


	// Create clock background
	sf::Texture clockImage;
	if (!clockImage.loadFromFile("resources/clock-image.jpg"))
	{
		return EXIT_FAILURE;
	}

	clock.circle.setTexture(&clockImage);
	clock.circle.setTextureRect(sf::IntRect(40, 0, 500, 500));



	while (window.isOpen())
	{			
		clock.createArrows(clock);
		//clock.createSoundEffect(clock);
		//

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
			window.draw(clock.numbers[i]);
		}

		//window.draw(clockBrandSprite);
		window.draw(clock.arrowHours);
		window.draw(clock.arrowMinutes);
		window.draw(clock.arrowSeconds);
		//window.draw(clock.sprite);
		// inside the main loop, between window.clear() and window.display()	

		// Display things on screen
		//window.draw(clock.circle);
		window.display();
	}

	return EXIT_SUCCESS;
}

// Create clock background
bool Clock::setBackground(Clock & clock)
{
	sf::Texture clockImage;
	if (!clockImage.loadFromFile("resources/clock-image.jpg"))
		return EXIT_FAILURE;
	clock.circle.setTexture(&clockImage);
	clock.circle.setTextureRect(sf::IntRect(40, 0, 500, 500)); 
	return true;
}


// Create sound effect
bool Clock::createSoundEffect(Clock & clock)
{	
	if (!clock.clockTick.openFromFile("resources/clock-1.wav"))
		return EXIT_FAILURE;
	clock.clockTick.setLoop(true);
	clock.clockTick.play();
	return true;
}

sf::ConvexShape Clock::newArrow(sf::Vector2<sf::Vector2f> params, const int amount, const int current, sf::Color color = sf::Color::Black)
{
	sf::ConvexShape arrow;
	arrow.setPointCount(3);
	arrow.setPoint(0, sf::Vector2f(0, 0));
	arrow.setPoint(1, sf::Vector2f(params.x.x / 2.f, params.x.y / 2.f));
	arrow.setPoint(2, sf::Vector2f(0, params.x.y));

	arrow.setOrigin(0, params.x.y);
	arrow.setPosition(CENTER.x, CENTER.y);
	arrow.rotate(360.f / amount * current);
	arrow.setFillColor(color);
	return arrow;
}

Time * newTime()
{
	time_t date = time(NULL);
	tm currentTime;
	localtime_s(&currentTime, &date);
	Time * newTime = new Time;
	newTime->sec = currentTime.tm_sec;
	newTime->min = currentTime.tm_min;
	newTime->hours = currentTime.tm_hour;
	return newTime;
}

void Clock::createArrows(Clock & clock)
{	
	// Get system time
	Time * currentTime = newTime();
	clock.arrowHours = newArrow(ARROW_SIZE::HOUR, currentTime->MAX_HOURS, currentTime->hours, sf::Color(69, 69, 69));;
	clock.arrowMinutes = newArrow(ARROW_SIZE::MIN, currentTime->MAX_MINUTES, currentTime->min);;
	clock.arrowSeconds = newArrow(ARROW_SIZE::SEC, currentTime->MAX_SECONDS, currentTime->sec, sf::Color::Red);
	delete currentTime;
}

// Create dots and place them to very right positions
void Clock::createDots(Clock & clock)
{
	for (int i = 0; i < LINES_COUNT; i++)
	{
		x = (int)((clockCircleSize - 10) * cos(angle));
		y = (int)((clockCircleSize - 10) * sin(angle));

		timeX = (int)((clockCircleSize - 50) * cos(angletime));
		timeY = (int)((clockCircleSize - 50) * sin(angletime));

		if (i % 5 == 0)
		{
			numbers[i].setFont(font);
			hourDot[i] = sf::CircleShape(3);
		}
		else
			minutesLine[i] = sf::RectangleShape(sf::Vector2f(1, 12));


		minutesLine[i].rotate(angleLines);

		hourDot[i].setFillColor(sf::Color::Black);
		minutesLine[i].setFillColor(sf::Color::Black);
		hourDot[i].setOrigin(hourDot[i].getGlobalBounds().width / 2, hourDot[i].getGlobalBounds().height / 2);
		hourDot[i].setPosition(x + (float)screenWidth / 2, y + (float)screenHeight / 2);

		minutesLine[i].setOrigin(minutesLine[i].getGlobalBounds().width / 2, minutesLine[i].getGlobalBounds().height / 2);
		minutesLine[i].setPosition(x + (float)screenWidth / 2, y + (float)screenHeight / 2);


		// Create numbers of the clock
		std::string hourStr = std::to_string(i / 5 + 1);
		numbers[i].setString(hourStr);
		numbers[i].setOrigin(numbers[i].getGlobalBounds().width / 1, numbers[i].getGlobalBounds().height / 1);
		numbers[i].setPosition(timeX + (float)screenWidth / 2, timeY + (float)screenHeight / 2);
		numbers[i].setFillColor(sf::Color(0, 0, 0));
		numbers[i].setCharacterSize(42);
		numbers[i].setStyle(sf::Text::Bold);

		//sf::FloatRect textRect = clock.numbers[i].getLocalBounds();
		//clock.numbers[i].setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
		//clock.numbers[i].setPosition(float(-sin(-M_PI * (i + 1) / 6) * (RADIUS - FIGURES_SIZE) + CENTER.x), float(-cos(-M_PI * (i + 1) / 6) * (RADIUS - FIGURES_SIZE) + CENTER.y));



		minutesLine[i].setRotation(float(i * 6 + 90));
		angle = angle + ((2 * PI) / 60);
		angletime = angletime + ((2 * PI) / 60);
	}
}

// Create outline of the clock
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