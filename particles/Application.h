#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "ParticleSystem.h"
#include "Renderer.h"
#include <memory>
#include <chrono>
#include "WindowData.h"




void framebuffer_size_callback(GLFWwindow* window, int width, int height);

class Application
{
public:
	Application();

	
	std::shared_ptr<WindowData> _window_data;

private:

	std::shared_ptr<ParticleSystem> _particleSystem;
	std::unique_ptr<Renderer> _renderer;

	void init_glfw();
	void init_particleSystem();
	void init_Renderer();
	void run();
};

