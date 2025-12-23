#shader vertex
#version 450 core

layout(location = 0) in vec4 position;


uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * position;
}

#shader fragment
#version 450 core

layout(location = 0) out vec4 color;

uniform float u_Oppacity = 1.0;
uniform vec3 u_Color;

void main()
{
	color = vec4(u_Color, u_Oppacity);
}