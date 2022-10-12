#version 450 core

layout(location = 0) in vec3 position;
layout(location = 0) out vec4 fragColor;

layout(binding = 0, std140) uniform UniformBuffer //set = 0, std140
{
	vec4 color;
};

//layout(location = 1) uniform vec4 color;

void main() 
{
	gl_Position = vec4(position, 1.0);
	fragColor = color;
}