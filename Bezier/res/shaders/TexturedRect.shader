#shader vertex
#version 450 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;
uniform vec2 u_texCoordOffSet;

void main()
{
	gl_Position = u_MVP * position;
	v_TexCoord = texCoord + u_texCoordOffSet;
}

#shader fragment
#version 450 core


layout(location = 0) out vec4 color;

uniform float u_Oppacity = 1.0;

in vec2 v_TexCoord;

uniform sampler2D u_TexID;
uniform float u_Desaturation = 0.0;
uniform vec3 u_Color = vec3(1.0,1.0,1.0);

void main()
{
	color = vec4(u_Color, u_Oppacity);
	color -= vec4(u_Desaturation, u_Desaturation, u_Desaturation, 0.0);
	color *= texture(u_TexID, v_TexCoord);
}