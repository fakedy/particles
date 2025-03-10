#include "Application.h"
#include <iostream>


Application::Application() {
	init_glfw();

	init_particleSystem();

	init_Renderer();

	run();
}

void Application::init_glfw()
{
	if (!glfwInit()) {

		std::cout << "GLFW FAILED TO INITIALIZE\n";
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	_window_data = std::make_shared<WindowData>();


	_window_data->_window = glfwCreateWindow(_window_data->width, _window_data->height, "particles :3", nullptr, nullptr);
	if (_window_data->_window == nullptr) {
		std::cout << "WINDOW CREATION FAILED\n";
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(_window_data->_window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwDestroyWindow(_window_data->_window);
		glfwTerminate();
		return;
	}

	glfwSetWindowUserPointer(_window_data->_window, reinterpret_cast<void*>(this));
	glfwSetFramebufferSizeCallback(_window_data->_window, framebuffer_size_callback);
	glViewport(0, 0, _window_data->width, _window_data->height);
}

void Application::init_particleSystem()
{
	_particleSystem = std::make_shared<ParticleSystem>();

}

void Application::init_Renderer()
{
	_renderer = std::make_unique<Renderer>(_particleSystem, _window_data);
}

void Application::run()
{

	while (!glfwWindowShouldClose(_window_data->_window)){

		// implement time tracking loop

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		// lets start here for now

		_particleSystem->update();

		// upload data to gpu for rendering

		_renderer->render();

	
		glfwSwapBuffers(_window_data->_window);
		glfwPollEvents();
	}

	glfwDestroyWindow(_window_data->_window);
	glfwTerminate();

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	Application* app = reinterpret_cast<Application*> (glfwGetWindowUserPointer(window));
	app->_window_data->width = width;
	app->_window_data->height = height;

	glViewport(0, 0, width, height);

}
