#version 330 core
	out vec4 FragColor;
	in vec3 color;
	in vec2 texCoordinates;

	uniform sampler2D tex0;

	vec3 colors[4] = vec3[](
		vec3(1.0 , 0.0 , 0.0),
		vec3(0.0 , 1.0 , 0.0),
		vec3(0.0 , 1.0 , 1.0),
		vec3(1.0 , 0.0 , 1.0));



	void main()
	{
		//FragColor = vec4(color, 1.0f) * texture(tex0, texCoordinates);
	   //FragColor = vec4(color, 1.0f);
	   FragColor = vec4(colors[gl_PrimitiveID % 4], 1.0);
	}