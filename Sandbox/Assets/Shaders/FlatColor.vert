#version 450 core

layout(location = 0) in vec3 position;
layout(location = 0) out vec4 fragColor;

layout(set = 0, binding = 0, std140) uniform UniformBuffer 
{
	vec4 color;
};

layout(set = 1, binding = 1, std140) uniform ViewMatrixBuffer 
{
	mat4 viewProjection;
	mat4 transform;
};

//'non-opaque uniforms outside a block' : not allowed when using GLSL for Vulkan
//layout(location = 1) uniform vec4 color; 

void main() 
{
	//gl_Position = vec4(position, 1.0);
	gl_Position =  viewProjection * transform * vec4(position, 1.0);
	fragColor = color;
}