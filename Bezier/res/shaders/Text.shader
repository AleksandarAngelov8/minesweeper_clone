#shader vertex
#version 450 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * position;
	v_TexCoord = texCoord;
}

#shader fragment
#version 450 core


layout(location = 0) out vec4 color;

uniform float u_Oppacity = 1.0;

in vec2 v_TexCoord;


uniform sampler2D u_TexID;
uniform vec3 u_Color = vec3(1.0,1.0,1.0);

void main()
{
	color = vec4(u_Color, u_Oppacity);
	color *= texture(u_TexID, v_TexCoord);
}