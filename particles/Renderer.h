#pragma once
#include "Shader.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ParticleSystem.h"
#include "memory"
#include "glad/glad.h"
#include "WindowData.h"

class Renderer
{

public:
	Renderer(std::shared_ptr<ParticleSystem> particleSystem, std::shared_ptr<WindowData> windowData);
	void render();

private:

	std::shared_ptr<ParticleSystem> _particleSystem;
	std::shared_ptr<WindowData> _window_data;
	Shader shader;
	glm::mat4 proj;

	unsigned int VBO;
	unsigned int VAO;
	
	void init();


};

