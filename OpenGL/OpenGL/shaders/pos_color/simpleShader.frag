#version 330 core

in vec4 frag_color;
in vec2 frag_texcoord;

out vec4 outColor;

void main()
{
	outColor = frag_color;
}