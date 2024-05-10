#pragma once
#include "Includes.h"
#include "Vertex.h"

using namespace std;

class Square
{
public:
	Square(const GLfloat xSize, const GLfloat ySize);
	void Draw();
	~Square();
private:
	float xSize;
	float ySize;

	GLuint vao;
	GLuint vbo[3];
	GLuint ebo;

	vector<Vertex> vertices;
	vector<glm::vec3> tangents;
	vector<glm::vec3> bitangents;
	vector<GLuint> indices;

	void GenerateVertexAttributes();
	void GenerateIndices();
	void ComputeTangentAndBitangent(int vertex);
};

