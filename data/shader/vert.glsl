#version 330 core

uniform float uni_time;
uniform mat4 uni_mmat;
uniform mat4 uni_vmat;
uniform mat4 uni_pmat;
uniform sampler2D uni_tex;

in vec3 vert_pos;
in vec4 vert_color;
in vec3 vert_norm;
in vec2 vert_uv;

out vec3 frag_pos;
out vec4 frag_color;
out vec3 frag_norm;
out vec2 frag_uv;

void main()
{
	frag_pos = vert_pos;
	frag_color = vert_color;
	frag_norm = vert_norm;
	frag_uv = vert_uv;

	gl_Position = uni_pmat * uni_vmat * uni_mmat * vec4(vert_pos, 1.0);

	//if (gl_Position.z < 0)
	//	frag_color = vec4(1, 0, 0, 1);

	gl_Position.z = 2 / (max(gl_Position.z, 0) + 2) - 1;

	//gl_Position.z = gl_Position.z;
	//if (gl_Position.z < 0)
	//gl_Position.z = 2 / (2 - gl_Position.z) - 1;
	//gl_Position.z = -1 / (-gl_Position.z - 1) - 1;
	//gl_Position.z = gl_Position.z / 1.0f;
}
