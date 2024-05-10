#pragma once
#include "Includes.h"
#include "Vertex.h"

using namespace std;

class Cuboid
{
public:
	Cuboid(const GLfloat xSize, const GLfloat ySize, const GLfloat zSize);
	Cuboid(const GLfloat xSize, const GLfloat ySize, const GLfloat zSize, glm::vec3 solidColor);
	void Draw();
	~Cuboid();
private:
	float xSize;
	float ySize;
	float zSize;
	bool usingSolidColor;
	glm::vec3 solidColor;

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

