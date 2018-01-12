#version 150 core

in vec3 frag_color;

out vec4 target_color;

void main()
{
	target_color = vec4(frag_color, 1.0);
}
