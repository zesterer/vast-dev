#version 330 core

uniform float uni_time;
uniform mat4 uni_mmat;
uniform mat4 uni_vmat;
uniform mat4 uni_pmat;

in vec3 frag_color;

out vec4 color;

void main()
{
	color = vec4(frag_color, 1.0);

	color = floor(color * 10.0) / 10.0;
}
