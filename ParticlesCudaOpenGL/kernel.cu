
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "kernel.h"

#include <stdio.h>

__global__ void MoveParticlesKernel(Particle* particles, Border* border)
{
	//particles[threadIdx.x].Move(*border);
	auto sumDistance = 0.;
	while (sumDistance < particles[threadIdx.x].speed)
	{
		auto localDistance = particles[threadIdx.x].speed - sumDistance;
		auto cosA = particles[threadIdx.x].direction.x / sqrt(pow(particles[threadIdx.x].direction.x, 2) + pow(particles[threadIdx.x].direction.y, 2) + pow(particles[threadIdx.x].direction.z, 2));
		auto cosB = particles[threadIdx.x].direction.y / sqrt(pow(particles[threadIdx.x].direction.x, 2) + pow(particles[threadIdx.x].direction.y, 2) + pow(particles[threadIdx.x].direction.z, 2));
		auto hypotenuseXY = localDistance * cosB;
		auto deltaZ = particles[threadIdx.x].direction.z * hypotenuseXY * cosA / abs(particles[threadIdx.x].direction.z);
		auto deltaY = particles[threadIdx.x].direction.y * sqrt(pow(hypotenuseXY, 2) - pow(deltaZ, 2)) / abs(particles[threadIdx.x].direction.y);
		auto deltaX = particles[threadIdx.x].direction.x * sqrt(pow(localDistance, 2) - pow(hypotenuseXY, 2)) / abs(particles[threadIdx.x].direction.x);

		auto fromDistance = sqrt(pow(deltaX, 2) + pow(deltaY, 2) + pow(deltaZ, 2));
		/*std::cout << "deltaX   " << deltaX << std::endl;
		std::cout << "deltaY   " << deltaY << std::endl;
		std::cout << "deltaZ   " << deltaZ << std::endl;
		std::cout << "distance " << fromDistance << std::endl;*/

		if ((border->first.x > particles[threadIdx.x].position.x - particles[threadIdx.x].radius + deltaX) && (border->first.x - (particles[threadIdx.x].position.x - particles[threadIdx.x].radius) > 0.))
		{
			particles[threadIdx.x].direction.x = -particles[threadIdx.x].direction.x;
			auto oldDeltaX = deltaX;
			deltaX = border->first.x - particles[threadIdx.x].position.x + particles[threadIdx.x].radius;
			deltaY *= deltaX / oldDeltaX;
			deltaZ *= deltaX / oldDeltaX;
		}
		if ((particles[threadIdx.x].position.x + deltaX + particles[threadIdx.x].radius > border->second.x) && (border->second.x - (particles[threadIdx.x].position.x + particles[threadIdx.x].radius) > 0.))
		{
			particles[threadIdx.x].direction.x = -particles[threadIdx.x].direction.x;
			auto oldDeltaX = deltaX;
			deltaX = border->second.x - particles[threadIdx.x].position.x - particles[threadIdx.x].radius;
			deltaY *= deltaX / oldDeltaX;
			deltaZ *= deltaX / oldDeltaX;
		}
		if ((border->first.y > particles[threadIdx.x].position.y - particles[threadIdx.x].radius + deltaY) && (border->first.y - (particles[threadIdx.x].position.y - particles[threadIdx.x].radius) > 0.))
		{
			particles[threadIdx.x].direction.y = -particles[threadIdx.x].direction.y;
			auto oldDeltaY = deltaY;
			deltaY = border->first.y - particles[threadIdx.x].position.y + particles[threadIdx.x].radius;
			deltaX *= deltaY / oldDeltaY;
			deltaZ *= deltaY / oldDeltaY;
		}
		if ((particles[threadIdx.x].position.y + deltaY + particles[threadIdx.x].radius > border->second.y) && (border->second.y - (particles[threadIdx.x].position.y + particles[threadIdx.x].radius) > 0.))
		{
			particles[threadIdx.x].direction.y = -particles[threadIdx.x].direction.y;
			auto oldDeltaY = deltaY;
			deltaY = border->second.y - particles[threadIdx.x].position.y - particles[threadIdx.x].radius;
			deltaX *= deltaY / oldDeltaY;
			deltaZ *= deltaY / oldDeltaY;
		}
		if ((border->first.z > particles[threadIdx.x].position.z - particles[threadIdx.x].radius + deltaZ) && (border->first.z - (particles[threadIdx.x].position.z - particles[threadIdx.x].radius) > 0.))
		{
			particles[threadIdx.x].direction.z = -particles[threadIdx.x].direction.z;
			auto oldDeltaZ = deltaZ;
			deltaZ = border->first.z - particles[threadIdx.x].position.z + particles[threadIdx.x].radius;
			deltaX *= deltaZ / oldDeltaZ;
			deltaY *= deltaZ / oldDeltaZ;
		}
		if ((particles[threadIdx.x].position.z + deltaZ + particles[threadIdx.x].radius > border->third.z) && (border->third.z - (particles[threadIdx.x].position.z + particles[threadIdx.x].radius) > 0.))
		{
			particles[threadIdx.x].direction.z = -particles[threadIdx.x].direction.z;
			auto oldDeltaZ = deltaZ;
			deltaZ = border->third.z - particles[threadIdx.x].position.z - particles[threadIdx.x].radius;
			deltaX *= deltaZ / oldDeltaZ;
			deltaY *= deltaZ / oldDeltaZ;
		}
		sumDistance += sqrt(pow(deltaX, 2) + pow(deltaY, 2) + pow(deltaZ, 2));
		/*std::cout << "new deltaX   " << deltaX << std::endl;
		std::cout << "new deltaY   " << deltaY << std::endl;
		std::cout << "new deltaZ   " << deltaZ << std::endl;
		std::cout << "new distance " << sumDistance << std::endl;
		std::cout << std::endl;*/
		particles[threadIdx.x].position.x += deltaX;
		particles[threadIdx.x].position.y += deltaY;
		particles[threadIdx.x].position.z += deltaZ;
	}
}

std::shared_ptr<KernelCUDA> KernelCUDAInstanse::instance = nullptr;

std::shared_ptr<KernelCUDA> KernelCUDAInstanse::getInstanse(std::vector<Particle>* particles, Border* border)
{
	if (!instance)
	{
		instance = std::make_shared<KernelCUDA>(particles, border);
	}
	return instance;
}

cudaError_t KernelCUDA::_MoveParticlesCUDA()
{
	cudaError_t cudaStatus;
	MoveParticlesKernel << <1, particles->size() >> > (particlesDevice, borderDevice);
	cudaStatus = cudaGetLastError();
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "MoveParticlesKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
	}
	else
	{
		cudaStatus = cudaDeviceSynchronize();
	}
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching MoveParticlesKernel!\n", cudaStatus);
	}
	else
	{
		cudaStatus = cudaMemcpy(&(*particles)[0], particlesDevice, particles->size() * sizeof(Particle), cudaMemcpyDeviceToHost);
	}

	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMemcpy failed!");
	}

	return cudaStatus;
}

int KernelCUDA::MoveParticlesCUDA()
{
	cudaError_t cudaStatus = _MoveParticlesCUDA();
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "addWithCuda failed!");
		return 1;
	}

	return 0;
}

KernelCUDA::KernelCUDA(std::vector<Particle>* particles, Border* border) : particles(particles), border(border)
{
	cudaStatus = cudaSetDevice(0);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
	}
	else
	{
		cudaStatus = cudaMalloc((void**)&particlesDevice, particles->size() * sizeof(Particle));
	}

	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMalloc failed!");
	}
	else
	{
		cudaStatus = cudaMalloc((void**)&borderDevice, sizeof(Border));
	}

	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMalloc failed!");
	}
	else
	{
		cudaStatus = cudaMemcpy(borderDevice, border, sizeof(Border), cudaMemcpyHostToDevice);
	}

	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMemcpy failed!");
	}
	else
	{
		cudaStatus = cudaMemcpy(particlesDevice, &(*particles)[0], particles->size() * sizeof(Particle), cudaMemcpyHostToDevice);
	}

	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaMemcpy failed!");
	}
}

KernelCUDA::~KernelCUDA()
{
	cudaFree(borderDevice);
	cudaFree(particlesDevice);
	// cudaDeviceReset must be called before exiting in order for profiling and
	// tracing tools such as Nsight and Visual Profiler to show complete traces.
	cudaStatus = cudaDeviceReset();
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaDeviceReset failed!");
	}
}
