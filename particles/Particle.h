#pragma once
#include <glm/glm.hpp>


struct Particle {

	glm::fvec2 position{0, 0};
	float size{ 15.0f };
	glm::fvec3 color{1.0, 1.0, 1.0};
	glm::fvec2 velocity{0.0, 0.0};
	glm::fvec2 acceleration{ 0.0, 0.0 };
	float weight{ 1.0f };

};