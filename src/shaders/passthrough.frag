#version 330 core

in vec2 tex_coords;

uniform sampler2D tex;

out vec4 frag_color;

void main()
{
	// frag_color = vec4(1.0, 0.0, 0.0, 1.0);
	frag_color = texture(tex, tex_coords);
	// frag_color = vec4(tex_coords, 0.0, 1.0);
}
