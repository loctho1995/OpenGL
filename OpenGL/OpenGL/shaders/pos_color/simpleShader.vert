#version 330 core

in vec3 position;
in vec4 color;
in vec2 texcoord;

out vec4 frag_color;
out vec2 frag_texcoord;

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(position, 1.0);
	frag_color = color;
	frag_texcoord = texcoord;
}