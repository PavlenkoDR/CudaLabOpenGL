#pragma once

#include "Particle.h"

#include <vector>
#include <future>

class ParticleSystem
{
public:
	ParticleSystem();
	std::vector<Particle> particles =
	{
		/*{{0, 0, 0}, {3, 7, 2}, 1.f, 0.5f}*/
	};
	Border border = { {-20, -20, -20}, {20, 20, -20} , {20, 20, 20} };
	std::future<void> thrd;

	void MoveParticles();
	void MoveParticlesByDeltaTime();
	void MoveParticlesByDeltaTimeAsync();
};

