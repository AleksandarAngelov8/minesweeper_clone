#shader vertex
#version 450 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 color;
out vec3 Color;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * position;
	Color = color;
}

#shader fragment
#version 450 core

layout(location = 0) out vec4 color;

in vec3 Color;

uniform float u_Oppacity = 1.0;

void main()
{
	color = vec4(Color, u_Oppacity);
}