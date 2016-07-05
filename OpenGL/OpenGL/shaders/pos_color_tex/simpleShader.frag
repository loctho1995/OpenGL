#version 330 core

in vec4 frag_color;
in vec2 frag_texcoord;

uniform sampler2D sampl;

out vec4 outColor;

void main()
{
	vec4 texColor = texture(sampl , frag_texcoord);

	outColor = frag_color * texColor;
}