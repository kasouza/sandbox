#version 330 core

layout (location = 0) in vec3 a_pos;

uniform float scale;
uniform float x_pos;
uniform float y_pos;
uniform float aspect_ratio;

out vec2 tex_coords;

void main()
{
    gl_Position = vec4(vec2((a_pos.x + x_pos) / aspect_ratio, a_pos.y - y_pos) * scale, 0.0, 1.0);
	tex_coords = vec2((a_pos.x + 1) / 2, (a_pos.y + 1) / 2);
}
