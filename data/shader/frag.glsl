#version 330 core

uniform float uni_time;

in vec3 frag_color;

out vec4 color;

void main()
{
	color = vec4(frag_color, 1.0);
}
