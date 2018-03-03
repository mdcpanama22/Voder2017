#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 in_color;

//out vec3 pos;
out vec4 color;
//out vec3 light_pos;

uniform vec2 win_size;
uniform vec2 mousePos;
uniform vec4 col;

uniform mat4 projMat;
uniform mat4 viewMat;
uniform mat4 rotMat;


void main() {

	//light_pos = vec3(((mousePos.x - (win_size.x * 0.5)) / (win_size.x * 0.5)) * 2, -((mousePos.y - (win_size.y * 0.5)) / (win_size.y * 0.5)), -1.0);
	//pos = vec3(position);
	color = in_color;
	gl_Position = position;

}

#shader fragment
#version 330 core

layout (location = 0) out vec4 out_color;

//in vec3 pos;
in vec4 color;
//in vec3 light_pos;

void main() {

	//float intensity = 1.0 / length(pos - light_pos) * 0.25;
	//out_color = color * intensity;
	out_color = vec4(1.0, 1.0, 1.0, 1.0);

}

#shader geometry
#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 60) out;

uniform mat4 projMat;
uniform mat4 rotMat;

mat4 rotationMatrix(in vec3 axis, in float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

void createTri(in vec4 pos, in mat4 rotation) {
	gl_Position = projMat * rotation * (gl_in[0].gl_Position + pos);
	EmitVertex();

	gl_Position = projMat * rotation * (gl_in[1].gl_Position + pos);
	EmitVertex();

	gl_Position = projMat * rotation * (gl_in[2].gl_Position + pos);
	EmitVertex();

	EndPrimitive();
}

void main() {


	/*

	Notes:
		width  = 0.150 (per key)
		height = 0.600 (per key)
		space  = 0.001 (between keys)
		total  = 0.151 (from one vert to the next)

		      Original Points
		    X        Y       Z
		-0.075f, -0.300f, -1.000f,
		 0.075f, -0.300f, -1.000f,
		 0.075f,  0.300f, -1.000f,
		-0.075f,  0.300f, -1.000f,

	*/

	createTri( vec4(0.0, 0.0, 0.0, 0.0), rotationMatrix(vec3(0.0, 0.0, 1.0), radians(60)) );

}
