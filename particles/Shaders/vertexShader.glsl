#version 450 core
layout (location = 0) in vec2 pos;
layout (location = 1) in float size;
layout (location = 2) in vec3 color;

out vec3 FragColor;
out float Size;
out vec2 actualPos;

uniform mat4 proj;

const vec2 quad[4] = {
    vec2(-0.5, -0.5),
    vec2( 0.5, -0.5),
    vec2( 0.5,  0.5),
    vec2(-0.5,  0.5)
};


const int indices[6] = int[6](0,2,1,2,0,3);

void main(){

	FragColor = color;
	Size = size;
	actualPos = pos;
	int i = indices[gl_VertexID];

	vec2 scaledQuad = quad[i]*size;

	vec2 screenPos = scaledQuad + pos;

	
	gl_Position = proj * vec4(screenPos, 0.0, 1.0);
	
}