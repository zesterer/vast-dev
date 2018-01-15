#version 330 core

uniform float uni_time;
uniform mat4 uni_mmat;
uniform mat4 uni_vmat;
uniform mat4 uni_pmat;

in vec3 vert_pos;
in vec3 vert_color;
in vec3 vert_norm;
in vec2 vert_uv;

out vec3 frag_color;

void main()
{
	frag_color = vec3(1);

	gl_Position = uni_pmat * uni_vmat * uni_mmat * vec4(vert_pos, 1.0);
	gl_Position.z = -(1 / (-gl_Position.z - 1) + 1);
}
