#pragma once
#include "Particle.h"
#include <vector>
#include <cuda_runtime_api.h>
#include <memory>


class KernelCUDA
{
public:
	KernelCUDA(std::vector<Particle>* particles = nullptr, Border* border = nullptr);
	~KernelCUDA();
	int MoveParticlesCUDA();
private:
	cudaError_t _MoveParticlesCUDA();
	cudaError_t cudaStatus;
	Particle* particlesDevice;
	Border* borderDevice;
	std::vector<Particle>* particles = nullptr;
	Border* border = nullptr;
};

class KernelCUDAInstanse
{
public:
	static std::shared_ptr<KernelCUDA> getInstanse(std::vector<Particle>* particles = nullptr, Border* border = nullptr);
private:
	static std::shared_ptr<KernelCUDA> instance;
};