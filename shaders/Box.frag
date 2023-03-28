#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 localPos;

layout(location = 0) out vec4 outColor;

void main() 
{
	float distance = length(localPos);
	float rate = clamp(distance * distance + 0.2, 0, 1); 
	
    outColor = vec4(fragColor * rate, 1.0);
}
