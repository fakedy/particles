#include "Renderer.h"
#include "Shader.h"
#include <iostream>


Renderer::Renderer(std::shared_ptr<ParticleSystem> particleSystem, std::shared_ptr<WindowData> windowData)
{
	_particleSystem = particleSystem;
	_window_data = windowData;
	init();
}

void Renderer::render()
{

	auto& drawBuffer = _particleSystem->_readBuffer;

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	/*
	glBufferSubData(GL_ARRAY_BUFFER,
		0,
		drawBuffer.size() * sizeof(Particle),
		drawBuffer.data());
	*/
	glBufferData(GL_ARRAY_BUFFER,
		drawBuffer.size() * sizeof(Particle),
		drawBuffer.data(),
		GL_DYNAMIC_DRAW);

	proj = glm::ortho(0.0f, (float)_window_data->width, 0.0f, (float)_window_data->height);
	unsigned int projLoc = glGetUniformLocation(shader.ID, "proj");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, drawBuffer.size());

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void Renderer::init()
{
	auto& drawBuffer = _particleSystem->_particleBuffers[0];
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER,
		drawBuffer.size() * sizeof(Particle),
		nullptr,
		GL_DYNAMIC_DRAW);



	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, position));
	glVertexAttribDivisor(0, 1);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, size));
	glVertexAttribDivisor(1, 1);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, color));
	glVertexAttribDivisor(2, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	shader.create("Shaders/vertexShader.glsl", "Shaders/fragmentShader.glsl");
	shader.use();

	proj = glm::ortho(0.0f, (float)_window_data->width, 0.0f, (float)_window_data->height);

	unsigned int projLoc = glGetUniformLocation(shader.ID, "proj");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
}
