#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 in_color;

out vec3 pos;
out vec4 color;
out vec3 light_pos;

uniform vec2 win_size;
uniform vec2 mousePos;
uniform vec4 col;

uniform mat4 projMat;
uniform mat4 viewMat;
uniform mat4 rotMat;


void main() {

	light_pos = vec3(((mousePos.x - (win_size.x * 0.5)) / (win_size.x * 0.5)) * 2, -((mousePos.y - (win_size.y * 0.5)) / (win_size.y * 0.5)), -1.0);
	pos = vec3(position);
	color = in_color;
	gl_Position = projMat * viewMat * position;

};

#shader fragment
#version 330 core

layout (location = 0) out vec4 out_color;

in vec3 pos;
in vec4 color;
in vec3 light_pos;

void main() {

	float intensity = 1.0 / length(pos - light_pos) * 0.25;
	out_color = color * intensity;

};