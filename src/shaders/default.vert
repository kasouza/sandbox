#version 330 core

layout (location = 0) in vec3 a_pos;

uniform int size;

flat out int id;

void main()
{
	float x = float(gl_InstanceID % size) / size * 2 - 1;
	float y = float(gl_InstanceID / size) / size * 2 - 1;

    gl_Position = vec4(x, y, 0.0, 1.0);
	id = int(round(x));
}
