#version 330 core

flat in int id;

out vec4 frag_color;

void main()
{
	if (id % 2 == 0)
	{
		frag_color = vec4(0.0, 0.0, 0.0, 1.0);
	}
	else
	{
		frag_color = vec4(1.0, 0.0, 0.0, 1.0);
	}
	// frag_color = texture(tex, tex_coords);
}
