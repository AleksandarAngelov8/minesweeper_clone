#shader vertex
#version 450 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 color;

out vec3 Color;

uniform mat4 u_MVP;

void main()
{
	Color = color;
	gl_Position = u_MVP * position;
}

#shader fragment
#version 450 core

in vec3 Color;
layout(location = 0) out vec4 color;

uniform float u_Oppacity = 1.0;
uniform int u_Inverter = 0;

void main()
{
	if (u_Inverter == 1) color = vec4(Color*0.6 + Color, u_Oppacity);
	else color = vec4(Color, u_Oppacity);
}