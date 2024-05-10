#include "ShaderManager.h"
#include "DebugHelper.h"

ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{
}

GLuint ShaderManager::CompileShader(LPCTSTR sourceFile, GLenum type)
{
	HANDLE hFile = CreateFile(sourceFile, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD fileSize = GetFileSize(hFile, NULL);

	LPVOID buffer = malloc(fileSize + 1);
	DWORD bytesRead = 0;
	if (!ReadFile(hFile, buffer, fileSize, &bytesRead, NULL))
	{		
		//Error reading file.
		DebugHelper::PrintFormat(_T("Error: %d\n"), GetLastError());
		return 0;
	}
	if (bytesRead != fileSize)
	{
		//Not entire file was read.
		return 0;
	}
	CloseHandle(hFile);

	char* source = static_cast<char*>(buffer);
	source[fileSize] = 0;

	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);
	free(buffer);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		//The compilation failed, print the error messages.
		GLint logLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		GLchar* infoLog = new GLchar[logLength];
		glGetShaderInfoLog(shader, logLength, NULL, infoLog);

		DebugHelper::PrintFormat(_T("Compilation failed in shader: \"%s\"\n"), sourceFile);
		OutputDebugStringA(infoLog);

		delete[] infoLog;
		return 0;
	}
	return shader;
}

GLuint ShaderManager::LinkShader(GLuint vertexShader, GLuint fragmentShader)
{
	GLuint shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	GLint success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (!success)
	{
		//The link failed, print the error messages.
		GLint logLength = 0;
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLength);
		GLchar* infoLog = new GLchar[logLength];
		glGetProgramInfoLog(shaderProgram, logLength, NULL, infoLog);
		DebugHelper::PrintFormat(_T("Linked failed in shader program: \"Program %d\"\n"), shaderProgram);
		OutputDebugStringA(infoLog);

		delete[] infoLog;
		return 0; //Indicate an error in linking.
	}

	return shaderProgram;
}