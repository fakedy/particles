#pragma once
#include <vector>
#include "Particle.h"
#include "glm/glm.hpp"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"


class ParticleSystem
{



public:

	// I intend on implementing several different methods and benchmarking them all


	ParticleSystem();
	// all our particles in the scene


	std::vector<Particle> _particleBuffers[2];

	std::vector<Particle> _readBuffer;

	int currentBufferIndex{ 0 };

	unsigned int _particleCount{200};

	void update();

private:


	double _currentTime{};
	double _previousTime{};
	double _frameTime{};
	const float _timestep{ 1.0f / 80.0f };
	float _accumilatedTime{ 0 };
	float _timeScale{ 2.0f };
	float gravity{ 9.82f };

	bool once{ false };

	glm::fvec2 bottomWall = glm::fvec2(0, 0);
	glm::fvec2 rightWall = glm::fvec2(1920, 0);
	glm::fvec2 leftWall = glm::fvec2(0, 0);
	glm::fvec2 topWall = glm::fvec2(0, 1080);

	void calculateGravity();
	void particleCollisionTest(Particle& particle, unsigned int i);
	void handleParticleCollision(Particle& particleA, Particle& particleB, float correction);
	void wallCollisionTest(Particle& particle);
	void calcAccumilatedTime();

};

