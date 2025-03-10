#version 450 core

in vec3 FragColor;
in float Size;
in vec2 actualPos;
in vec4 gl_FragCoord;

out vec4 outColor;


void main(){
	
	if(distance(actualPos, gl_FragCoord.xy) > Size/2.0f)
		discard;

	outColor = vec4(FragColor, 1.0);

}