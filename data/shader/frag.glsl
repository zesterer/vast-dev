#version 330 core

uniform float uni_time;
uniform mat4 uni_mmat;
uniform mat4 uni_vmat;
uniform mat4 uni_pmat;

in vec3 frag_pos;
in vec3 frag_color;
in vec3 frag_norm;
in vec2 frag_uv;

out vec4 tgt_color;

const vec3 sun_dir = normalize(vec3(-2, -1, -1));
const vec3 sun_color = vec3(1, 1, 0.9);
const float sun_factor = 50;
const float sun_shine = 0;

const float surf_diffuse = 0.5;
const float surf_ambiance = 0.2;

void main()
{
	// Geometry
	vec3 world_norm = normalize((uni_mmat * vec4(frag_norm, 0)).xyz);
	vec3 cam_rpos = (uni_vmat * vec4(frag_pos, 1)).xyz;

	// Ambiance
	vec3 ambience = surf_ambiance * sun_color;

	// Diffuse
	vec3 diffuse = surf_diffuse * sun_color * max(0, dot(world_norm, -sun_dir));

	// Specular
	vec3 reflect = (uni_vmat * vec4(reflect(sun_dir, world_norm), 0)).xyz;
	float surf_specular = clamp(dot(-normalize(cam_rpos), reflect) + sun_shine, 0, 1);
	vec3 specular = sun_color * pow(surf_specular, sun_factor);

	// Total light
	vec3 light = ambience + diffuse + specular;

	// Target color
	tgt_color = vec4(frag_color * light, 1.0);
}
