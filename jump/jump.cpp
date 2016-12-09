// jump.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
// Если используется stdafx.h, перенесите директиву define в начало stdafx.h, 

// This program takes max jump height from input and prints
// jump height for every time point with step 0.1 seconds.
// Program should print all time points when height is min and max.
//
// TODO: Fix all warnings on high warning level (/W4, -Wall -Wextra).
// TODO: Rename variables and split to several functions,
// see also https://ps-group.github.io/sfml/coding_conventions.html
// TODO: fix negative height values, fix heigh values higher than max height.

/*
Программа вычисляет на какой высоте находится объект
в процессе прыжка с интервалом 0.1 секунда

Исходное состояние системы:
- <начальная высота> = 0
- Формула расчёта высоты h(t):
h(t) == v0 * t - 0.5 * g * t * t, где:
g = 9.8 м/с^2 (ускорение свободного падения)
t - момент времени с начала прыжка
v0 - скорость в начальный момент времени
v0 = g * T
T - время достижения максимальной высоты

На входе: неотрицательное целое число (максимальная высота прыжка в метрах)
На выходе:
1) время достижения максимальной высоты (в сек.)
2) пары <момент времени> <высота нахождения объекта>
(все выходные данные с точностью 6 знаков после запятой)
Обработка ошибок:
в случае неверных входных данных выводится сообщение об ошибке
*/

// Программа вычисляет на какой высоте находится объект
// в процессе прыжка с интервалом 0.1 секунда

// Псевдокод работы программы:
//
// Задаём максимальную высоту прыжка
// Расчёт и вывод времени достижения максимальной высоты Т
// Вычисляем начальную скорость v0
// Вывод пар <момент времени> <высота нахождения объекта> 
// с шагом 0.1 сек. пока длится прыжок
//      * Дополнительно выводим пару с максимальной высотой
// Вывод пары <момент приземления> <высота 0>

using namespace std;

const double GRAVITY = 9.8f;
const double DISTANCE = 0.1f;

double findObjectHeight(double const & startSpeed, double const & time)
{
	return startSpeed * time - 0.5 * GRAVITY * time * time;
}

int main(int, char *[])
{	
	int maxJumpHeight = 0;

	cout << "Enter the maximum jumping height:" << endl;
	cout << "> ";

	// Считываем высоту из input
	if ((cin >> maxJumpHeight) && (maxJumpHeight <= 0))
	{
		cout << "Expected non - negative integer number." << endl;
		return EXIT_FAILURE;
	}	

	double const timeToReachMaximumHeight = sqrt(maxJumpHeight * 2 / GRAVITY);
	double const stopTime = timeToReachMaximumHeight * 2;

	// Выводим время достижения максимальной высоты
	cout << "Time to reach the maximum height: " << timeToReachMaximumHeight << endl;

	// Вычисляем начальную скорость startSpeed
	double const startSpeed = GRAVITY * timeToReachMaximumHeight;

	// Вывод пар <момент времени> <высота нахождения объекта> с шагом 0.1 сек. пока длится прыжок	
	bool isMaxPointReached = false;

	for (float time = 0; time < stopTime; time += DISTANCE)
	{
		double const currentHeight = findObjectHeight(startSpeed, time);

		if (time > timeToReachMaximumHeight && !isMaxPointReached)
		{
			isMaxPointReached = true;
			cout << "Time to reach maximum height: " << timeToReachMaximumHeight << " Height: " << currentHeight << endl;
		}
		cout << "At time: " << time << " Height is: " << currentHeight << endl;
	}

	// Вывод пары <момент приземления> <высота>
	double const landingHeight = findObjectHeight(startSpeed, stopTime);
	cout << "Landing time: " << stopTime  << " Height is: " << landingHeight << endl;

	return EXIT_FAILURE;
}