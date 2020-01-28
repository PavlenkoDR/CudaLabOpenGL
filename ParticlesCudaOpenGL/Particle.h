#pragma once

class Point
{
public:
	double x;
	double y;
	double z;
};

class Border
{
public:
	Point first;
	Point second;
	Point third;
};

class Particle
{
public:
	double Move();
	double Move(const Border& border);
	void Collide(Particle& particle);

	Point position;
	Point direction;
	double speed;
	double radius;
};

