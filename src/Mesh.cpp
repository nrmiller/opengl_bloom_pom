#include "Mesh.h"

Mesh::Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	Initialize();
}

Mesh::~Mesh()
{
}

void Mesh::Initialize()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	//Vertex Positions.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

	//Vertex Normals.
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));

	//Vertex Texture Coordinates.
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoord));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::Draw(GLuint shader)
{
	//Obtain the textures for the mesh and make them available as uniform samplers.
	GLuint diffuseIndex = 1;
	GLuint specularIndex = 1;
	for (GLuint index = 0; index < textures.size(); index++)
	{
		glActiveTexture(GL_TEXTURE0 + index);

		stringstream ss;
		string number;
		string name = textures[index].type;
		if (name == "texture_diffuse")
		{
			ss << diffuseIndex++;
		}
		else if (name == "texture_specular")
		{
			ss << specularIndex++;
		}
		number = ss.str();

		glUniform1f(glGetUniformLocation(shader, ("material." + name + number).c_str()), index);
		glBindTexture(GL_TEXTURE_2D, textures[index].id);
	}
	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}