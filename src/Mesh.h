#pragma once

#include "Includes.h"
#include <vector>
#include <sstream>

using namespace std;

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoord;
};

struct Texture
{
	GLuint id;
	string type;
	string path;
};

class Mesh
{
public:
	Mesh(vector<Vertex> vertices, vector<GLuint> indicies, vector<Texture> textures);
	~Mesh();
	void Draw(GLuint shader);
private:
	GLuint vao;
	GLuint vbo; //Contains interleaved data (Position + Normal + TexCoord).
	GLuint ebo;

	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;

	void Initialize();
};

