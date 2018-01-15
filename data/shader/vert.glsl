#version 330 core

uniform float uni_time;

in vec3 vert_pos;
in vec3 vert_color;
in vec3 vert_norm;
in vec2 vert_uv;

out vec3 frag_color;

void main()
{
	frag_color = vert_color;
	gl_Position = vec4(vert_pos, 1.0);
	gl_Position.x += 0.2f * sin(uni_time / 10.0f + gl_Position.y);
}
