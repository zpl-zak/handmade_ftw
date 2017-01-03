#version 330

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 color;

uniform mat4 MVP;

out vec3 color0;

void main()
{
	gl_Position = MVP * vec4(vertex, 1);
	color0 = color;
}
