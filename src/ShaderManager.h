#pragma once
#include "Includes.h"

class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();
	GLuint CompileShader(LPCTSTR sourceFile, GLenum type);
	GLuint LinkShader(GLuint vertexShader, GLuint fragmentShader);
};

