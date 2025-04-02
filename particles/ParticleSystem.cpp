#include "ParticleSystem.h"
#include "iostream"
ParticleSystem::ParticleSystem()
{


	_particleBuffers[0].resize(_particleCount);
	_particleBuffers[1].resize(_particleCount);

	auto& drawingBuffer = _particleBuffers[0];

	float gap = 5.0f;
	float yOffset = 2560 / 2.0f;
	int maxwidth = 80.0;

	float size = Particle().size;

	float ballsTotalWidth = (maxwidth * size) + ((maxwidth - 1) * gap);

	float xOffset = (3840 - ballsTotalWidth)/2.0f;

	int y = yOffset;
	int c = 0;

	// spawn particles
	for (unsigned int i = 0; i < _particleCount; i++)
	{

		Particle particle;
		
		
		if (i % maxwidth == 0) {
			y = y + particle.size+gap;
			c = 0;
		}

		int x = xOffset + c * (particle.size + gap);
		c++;


		particle.position = glm::vec2(x,y);
		drawingBuffer[i] = particle;
	}

}

void ParticleSystem::update()
{

	calcAccumilatedTime();


	while (_accumilatedTime >= _timestep) {

		auto& currentBuffer = _particleBuffers[0];

		// reset acceleration
		for (Particle& currentParticle : currentBuffer) {
			glm::vec2 force = glm::vec2(0);
			currentParticle.acceleration = force;
		}

		// calc all gravity forces
		calculateGravity();


		for (int i = 0; i < currentBuffer.size(); i++) {
			Particle& particle = currentBuffer[i];
			float colorMomentum = glm::length(glm::fvec2(particle.velocity));
			float scaledMomentum = (colorMomentum / 100);
			particle.color = glm::vec3(scaledMomentum, 0.0f, 1 - scaledMomentum);

			particle.velocity += particle.acceleration * _timestep;
			particle.position += particle.velocity * _timestep;

		}


		for (int i = 0; i < currentBuffer.size(); i++) {
			Particle& particle = currentBuffer[i];
			particleCollisionTest(particle, i);
			wallCollisionTest(particle);

		}


		_readBuffer = currentBuffer;

		_accumilatedTime -= _timestep;
	}

	if (glfwGetTime() > 5 && !once) {
		auto& drawingBuffer = _particleBuffers[0];
		drawingBuffer[15].weight = 10000.0f;
		drawingBuffer[15].size = 120.0f;
		drawingBuffer[15].position.x = 300;
		drawingBuffer[15].position.y = 900;
		drawingBuffer[15].velocity.y = 0;
		drawingBuffer[15].velocity.x = 50;
		once = true;
	}


}

void ParticleSystem::calculateGravity()
{
	auto& currentBuffer = _particleBuffers[0];
	int i = 0;
	for (Particle& currentParticle : currentBuffer) {
		
		for (int x = i + 1; x < _particleCount; x++)
		{
			Particle& particle = currentBuffer[x];

			// do gravity pull calculation
			glm::fvec2 dirVec = glm::normalize(particle.position - currentParticle.position);
			float dist = glm::distance(currentParticle.position, particle.position);

			glm::vec2 force = dirVec * gravity * (currentParticle.weight * particle.weight) / (dist * dist);

			currentParticle.acceleration += force / currentParticle.weight;
			particle.acceleration -= force / particle.weight;
		}

		glm::vec2 gravityPoint = glm::vec2(3840 / 2.0f, 2160 / 2.0f);
		glm::vec2 gravityPointDirection = glm::normalize(gravityPoint - currentParticle.position);

		glm::vec2 force = gravityPointDirection * gravity;
		currentParticle.acceleration += force;

		i++;
	}
}

void ParticleSystem::particleCollisionTest(Particle& currentParticle, unsigned int i)
{
	// fools algorithm

	auto& currentBuffer = _particleBuffers[0];

	// start at i because we've already handled the other particles
	for (int x = i + 1; x < _particleCount; x++)
	{
		Particle& particle = currentBuffer[x];

		float dist = glm::distance(currentParticle.position, particle.position);

		float radiusSum = (currentParticle.size / 2) + (particle.size / 2);

		if (dist <= radiusSum) {
			float overlap = radiusSum - dist;
			handleParticleCollision(currentParticle, particle, overlap);
		}
	}

}

void ParticleSystem::handleParticleCollision(Particle& particleA, Particle& particleB, float correction)
{
	float restitution = 0.90f;
	glm::fvec2 normal = glm::normalize(particleA.position - particleB.position);


	particleA.position += normal * correction*0.5f;
	particleB.position -= normal * correction*0.5f;

	// calculate the velocity in the normals direction
	glm::vec2 relVel = particleA.velocity - particleB.velocity;
	float velAlongNormal = glm::dot(relVel, normal);


	if (velAlongNormal > 0) {
		return;
	}

	float invMassA = 1.0f / particleA.weight;
	float invMassB = 1.0f / particleB.weight;

	float j = -((1 + restitution) * velAlongNormal)/ (invMassA + invMassB);


	particleA.velocity += j * normal * invMassA;
	particleB.velocity -= j * normal * invMassB;

}

void ParticleSystem::wallCollisionTest(Particle &particle)
{
		float restitution = 0.9f;

	if (particle.position.y-(particle.size/2) < bottomWall.y) {
		particle.velocity = glm::reflect(particle.velocity* restitution, glm::vec2(0, 1));
		particle.position.y = bottomWall.y + (particle.size / 2) + 0.5f;
	}

	if (particle.position.y + (particle.size/2) > topWall.y) {
		particle.velocity = glm::reflect(particle.velocity* restitution, glm::vec2(0, -1));
		particle.position.y = topWall.y - (particle.size / 2) - 0.5f;
	}

	if (particle.position.x - (particle.size / 2) < leftWall.x) {
		particle.velocity = glm::reflect(particle.velocity* restitution, glm::vec2(1, 0));
		particle.position.x = leftWall.x + (particle.size / 2) +0.5f;
	}

	if (particle.position.x + (particle.size / 2) > rightWall.x) {
		particle.velocity = glm::reflect(particle.velocity* restitution, glm::vec2(-1, 0));
		particle.position.x = rightWall.x - (particle.size / 2) - 0.5f;
	}

}

void ParticleSystem::calcAccumilatedTime()
{
	_currentTime = glfwGetTime();

	_frameTime = _currentTime - _previousTime;

	_previousTime = _currentTime;

	_accumilatedTime += _frameTime * _timeScale;
	
}
