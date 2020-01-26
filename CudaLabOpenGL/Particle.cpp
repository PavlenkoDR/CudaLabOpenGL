#include "Particle.h"

#include <cmath>
#include <algorithm>
#include <iostream>
#include <limits>

double Particle::Move()
{
	auto deltaX = speed * direction.x / sqrt(pow(direction.x, 2) + pow(direction.y, 2));
	auto deltaY = sqrt(pow(speed, 2) - pow(deltaX, 2));
	position.x += deltaX;
	position.y += deltaY;

	return sqrt(pow(deltaX, 2) + pow(deltaY, 2));
}

double Particle::Move(const Border& border)
{
	auto sumDistance = 0.;
	/*while (sumDistance < speed)
	{
		auto localDistance = sumDistance - speed;
		auto cosA = direction.x / sqrt(pow(direction.x, 2) + pow(direction.y, 2));
		auto deltaX = localDistance * cosA;
		auto deltaY = direction.y * sqrt(pow(localDistance, 2) - pow(deltaX, 2)) / abs(direction.y);
		if ((border.first.x > position.x + deltaX) && (border.first.x - position.x > 0.))
		{
			deltaX = border.first.x - position.x;
			deltaY = deltaX * sqrt(1 / pow(cosA, 2) - 1);
			direction.x = -direction.x;
			std::cout << "Boom left" << std::endl;
		}
		if ((position.x + deltaX > border.second.x) && (border.second.x - position.x > 0.))
		{
			deltaX = border.second.x - position.x;
			deltaY = deltaX * sqrt(1 / pow(cosA, 2) - 1);
			direction.x = -direction.x;
			std::cout << "Boom right" << std::endl;
		}
		if ((border.first.y > position.y + deltaY) && (border.first.y - position.y > 0.))
		{
			deltaY = border.first.y - position.y;
			deltaX = deltaY * sqrt(1 / pow(cosA, 2) - 1);
			direction.y = -direction.y;
			std::cout << "Boom bottom" << std::endl;
		}
		if ((position.y + deltaY > border.second.y) && (border.second.y - position.y > 0.))
		{
			deltaY = border.second.y - position.y;
			deltaX = deltaY * sqrt(1 / pow(cosA, 2) - 1);
			direction.y = -direction.y;
			std::cout << "Boom top" << std::endl;
		}
		position.x += deltaX;
		position.y += deltaY;
		sumDistance += sqrt(pow(deltaX, 2) + pow(deltaY, 2));
	}*/


	while (sumDistance < speed)
	{
		auto localDistance = speed - sumDistance;
		auto cosA = direction.x / sqrt(pow(direction.x, 2) + pow(direction.y, 2) + pow(direction.z, 2));
		auto cosB = direction.y / sqrt(pow(direction.x, 2) + pow(direction.y, 2) + pow(direction.z, 2));
		auto hypotenuseXY = localDistance * cosB;
		auto deltaZ = direction.z * hypotenuseXY * cosA / abs(direction.z);
		auto deltaY = direction.y * sqrt(pow(hypotenuseXY, 2) - pow(deltaZ, 2)) / abs(direction.y);
		auto deltaX = direction.x * sqrt(pow(localDistance, 2) - pow(hypotenuseXY, 2)) / abs(direction.x);
		
		auto fromDistance = sqrt(pow(deltaX, 2) + pow(deltaY, 2) + pow(deltaZ, 2));
		/*std::cout << "deltaX   " << deltaX << std::endl;
		std::cout << "deltaY   " << deltaY << std::endl;
		std::cout << "deltaZ   " << deltaZ << std::endl;
		std::cout << "distance " << fromDistance << std::endl;*/

		if ((border.first.x > position.x - radius + deltaX) && (border.first.x - (position.x - radius) > 0.))
		{
			direction.x = -direction.x;
			auto oldDeltaX = deltaX;
			deltaX = border.first.x - position.x + radius;
			deltaY *= deltaX / oldDeltaX;
			deltaZ *= deltaX / oldDeltaX;
		}
		if ((position.x + deltaX + radius > border.second.x) && (border.second.x - (position.x + radius) > 0.))
		{
			direction.x = -direction.x;
			auto oldDeltaX = deltaX;
			deltaX = border.second.x - position.x - radius;
			deltaY *= deltaX / oldDeltaX;
			deltaZ *= deltaX / oldDeltaX;
		}
		if ((border.first.y > position.y - radius + deltaY) && (border.first.y - (position.y - radius) > 0.))
		{
			direction.y = -direction.y;
			auto oldDeltaY = deltaY;
			deltaY = border.first.y - position.y + radius;
			deltaX *= deltaY / oldDeltaY;
			deltaZ *= deltaY / oldDeltaY;
		}
		if ((position.y + deltaY + radius > border.second.y) && (border.second.y - (position.y + radius) > 0.))
		{
			direction.y = -direction.y;
			auto oldDeltaY = deltaY;
			deltaY = border.second.y - position.y - radius;
			deltaX *= deltaY / oldDeltaY;
			deltaZ *= deltaY / oldDeltaY;
		}
		if ((border.first.z > position.z - radius + deltaZ) && (border.first.z - (position.z - radius) > 0.))
		{
			direction.z = -direction.z;
			auto oldDeltaZ = deltaZ;
			deltaZ = border.first.z - position.z + radius;
			deltaX *= deltaZ / oldDeltaZ;
			deltaY *= deltaZ / oldDeltaZ;
		}
		if ((position.z + deltaZ + radius > border.third.z) && (border.third.z - (position.z + radius) > 0.))
		{
			direction.z = -direction.z;
			auto oldDeltaZ = deltaZ;
			deltaZ = border.third.z - position.z - radius;
			deltaX *= deltaZ / oldDeltaZ;
			deltaY *= deltaZ / oldDeltaZ;
		}
		sumDistance += sqrt(pow(deltaX, 2) + pow(deltaY, 2) + pow(deltaZ, 2));
		/*std::cout << "new deltaX   " << deltaX << std::endl;
		std::cout << "new deltaY   " << deltaY << std::endl;
		std::cout << "new deltaZ   " << deltaZ << std::endl;
		std::cout << "new distance " << sumDistance << std::endl;
		std::cout << std::endl;*/
		position.x += deltaX;
		position.y += deltaY;
		position.z += deltaZ;
	}

	return sumDistance;
}

void Particle::Collide(Particle& particle)
{
	/*if ((abs(position.x - particle.position.x) > (radius * particle.radius) * 0.5) || (abs(position.y - particle.position.y) > (radius * particle.radius) * 0.5))
	{
		return;
	}*/
	if (sqrt(pow(position.x - particle.position.x, 2) +
		pow(position.y - particle.position.y, 2) +
		pow(position.z - particle.position.z, 2)) > radius + particle.radius)
	{
		return;
	}
	Point delta { position.x - particle.position.x, position.y - particle.position.y, position.z - particle.position.z };
	if (delta.x != 0 || delta.y != 0 || delta.z != 0)
	{
		/*auto len1 = sqrt(pow(direction.x, 2) + pow(direction.y, 2) + pow(direction.z, 2));
		auto len2 = sqrt(pow(particle.direction.x, 2) + pow(particle.direction.y, 2) + pow(particle.direction.z, 2));
		auto cosA = (direction.x * particle.direction.x + direction.y * particle.direction.y + direction.z * particle.direction.z) / (len1 * len2);
		*/
		/*direction.x = -direction.x;
		direction.y = -direction.y;
		direction.z = -direction.z;
		particle.direction.x = -particle.direction.x;
		particle.direction.y = -particle.direction.y;
		particle.direction.z = -particle.direction.z;*/
	}
	else
	{
		std::swap(direction, particle.direction);
	}
	/*speed = 0;
	particle.speed = 0;
	std::swap(speed, particle.speed);
	std::swap(direction, particle.direction);*/
	/*direction.x =- direction.x;
	direction.y =- direction.y;
	direction.z =- direction.z;
	particle.direction.x =- particle.direction.x;
	particle.direction.y =- particle.direction.y;
	particle.direction.z =- particle.direction.z;*/
}
