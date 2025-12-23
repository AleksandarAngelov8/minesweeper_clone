#shader vertex
#version 450 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in float texID;

out vec2 v_TexCoord;
out float TexID;

uniform mat4 u_MVP;

void main()
{
	TexID = texID;
	gl_Position = u_MVP * position;
	v_TexCoord = texCoord;
}

#shader fragment
#version 450 core


layout(location = 0) out vec4 color;

uniform float u_Oppacity = 1.0;

in vec2 v_TexCoord;
in float TexID;

uniform float u_TexID = -1.0;
uniform sampler2D u_Texture[32];
void main()
{
	int index = int(TexID);
	if (u_TexID != -1.0){
		index = int(u_TexID);
	}
	color = vec4(1.0, 1.0, 1.0, u_Oppacity);
	color *= texture(u_Texture[index], v_TexCoord);
}