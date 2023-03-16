#version 330 core
	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec2 aTex;
	out vec3 color;
	out vec2 texCoordinates;

	uniform float scale;

	uniform mat4 view;
	uniform mat4 model;
	uniform mat4 proj;
	uniform vec3 inColor;

	void main()
	{
		gl_Position = view * model * proj * vec4(aPos * scale, 1.0);
		color = inColor;
		texCoordinates = aTex;
	}