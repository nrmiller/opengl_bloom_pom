#pragma once
#include "Includes.h"

using namespace std;

class Sphere
{
public:
	Sphere(const GLfloat radius, const GLint slices, const GLint stacks);
	void Draw();
	~Sphere();
private:
	float radius;
	int slices;
	int stacks;

	GLuint vao;
	GLuint vbo[3];
	GLuint ebo;

	vector<glm::vec3> positions;
	vector<glm::vec3> colors;
	vector<glm::vec3> normals;
	vector<GLuint> indices;

	void GenerateVertexAttributes();
	void GenerateIndices();
};

