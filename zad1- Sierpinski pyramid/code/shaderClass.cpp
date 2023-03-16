#include "shaderClass.h"

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!do ogarniecia!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
std::string getShaderContents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
}

//Konstruktor laczacy 2 shader-y
Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	//pobranie shader-ow i wrzucenie ich do string-a
	std::string vertexContents = getShaderContents(vertexFile);
	std::string fragmentContents = getShaderContents(fragmentFile);
	const char* vertexSource = vertexContents.c_str();
	const char* fragmentSource = fragmentContents.c_str();

	//podpinanie szhader-ow
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); //tworzenie shader-a
	glShaderSource(vertexShader, 1, &vertexSource, NULL); //podpiêcie zrodla do obiektu
	glCompileShader(vertexShader); //przetrawienie oobiektu na kod maszynowy
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	//podpinanie shader-ow do programu shader-owego i podpiecie go do sceny (czy zlinkowanie sheder-ow ze soba (?))
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//czyszczenie shader-ow z kodu
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

// Activates the Shader Program
void Shader::Activate()
{
	glUseProgram(shaderProgram);
}

// Deletes the Shader Program
void Shader::Delete()
{
	glDeleteProgram(shaderProgram);
}