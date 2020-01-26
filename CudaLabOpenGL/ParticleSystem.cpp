#include "ParticleSystem.h"

#include <iostream>
#include <ctime>
#include <thread>
#include <chrono>

int previousTime = 0;
int refreshTime = 10;
int particlesCount = 100;

ParticleSystem::ParticleSystem()
{
	std::srand(unsigned(std::time(0)));

	auto rand = [](double from, double to)
	{
		return from + std::rand() % (int)(to - from);
	};

	for (int i = 0; i < particlesCount; ++i)
	{
		particles.push_back(
			{
				{
					rand(border.first.x, border.third.x),
					rand(border.first.y, border.third.y),
					rand(border.first.z, border.third.z)
				},
				{
					rand(border.first.x, border.third.x),
					rand(border.first.y, border.third.y),
					rand(border.first.z, border.third.z)
				},
				0.05,
				0.5f
			});
	}
	std::cout << "Inited" << std::endl;
}

void ParticleSystem::MoveParticles()
{
	for (int i = 0; i < particles.size(); ++i)
	{
		for (int j = i + 1; j < particles.size(); ++j)
		{
			particles[i].Collide(particles[j]);
		}
	}
	for (auto& particle : particles)
	{
		//std::cout << "Moved by " << particle.Move(border) << " points\n" << std::endl;
		particle.Move(border);
		//std::cout << particle.position.x << " " << particle.position.y << " " << particle.position.z << std::endl;
	}
}

void ParticleSystem::MoveParticlesByDeltaTime()
{
	while (true)
	{
		previousTime = time(0);
		MoveParticles();
		auto deltaTime = refreshTime - (time(0) - previousTime);
		std::this_thread::sleep_for(std::chrono::milliseconds(deltaTime));
		std::cout << "Refreshed after " << deltaTime << std::endl;
	}
}

void ParticleSystem::MoveParticlesByDeltaTimeAsync()
{
	thrd = std::async(std::launch::async, std::bind(&ParticleSystem::MoveParticlesByDeltaTime, this));
}
