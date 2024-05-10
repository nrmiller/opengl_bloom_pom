#include "Sphere.h"
#include <tchar.h>
#include <strsafe.h>

Sphere::Sphere(const GLfloat radius, const GLint slices, const GLint stacks)
{
	this->radius = radius;
	this->slices = slices;
	this->stacks = stacks;
	glGenVertexArrays(1, &vao);
	glGenBuffers(3, vbo);
	glGenBuffers(1, &ebo);
	GenerateVertexAttributes();
	GenerateIndices();
}

Sphere::~Sphere()
{
	positions.clear();
	colors.clear();
	normals.clear();
	indices.clear();
}

void Sphere::GenerateVertexAttributes()
{
	const int vertexCount = stacks * slices + 2;
	positions.resize(vertexCount);
	colors.resize(vertexCount);
	normals.resize(vertexCount);
	
	//Bottom vertex.
	positions[0] = glm::vec3(0.0f, -radius, 0.0f);
	colors[0] = glm::vec3(1.0f, 0.0f, 1.0f);
	normals[0] = glm::vec3(0.0f, -1.0f, 0.0f);
	
	//Middle vertices.
	GLfloat theta = 0;
	for (GLint sliceIndex = 0; sliceIndex < slices; sliceIndex++)
	{
		GLfloat phi = (GLfloat)(-M_PI / 2.0f + M_PI / (stacks + 1));
		for (GLint stackIndex = 0; stackIndex < stacks; stackIndex++)
		{
			GLfloat x = radius * cos(phi) * cos(theta);
			GLfloat y = radius * sin(phi);
			GLfloat z = radius * cos(phi) * sin(theta);
			positions[1 + sliceIndex * stacks + stackIndex] = glm::vec3(x, y, z);
			colors[1 + sliceIndex * stacks + stackIndex] = glm::vec3(1.0f, 0.0f, 1.0f);
			normals[1 + sliceIndex * stacks + stackIndex] = glm::vec3(x / radius, y / radius, z / radius);
			phi += (GLfloat)(M_PI / (stacks + 1));
		}
		theta += (GLfloat)(2.0f * M_PI / slices);
	}

	//Top vertex.
	positions[vertexCount - 1] = glm::vec3(0.0f, radius, 0.0f);
	colors[vertexCount - 1] = glm::vec3(1.0f, 0.0f, 1.0f);
	normals[vertexCount - 1] = glm::vec3(0.0f, 1.0f, 0.0f);

	glBindVertexArray(vao);

	//Write to the VERTEX_ARRAY buffer.
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(glm::vec3), &positions[0], GL_STATIC_DRAW);

	//Enable vertex attribute 0 and specify the data comes from the VERTEX_ARRAY (positions).
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);


	//Write to the COLOR_ARRAY buffer.
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(glm::vec3), &colors[0], GL_STATIC_DRAW);

	//Enable vertex attribute 1 and specify the data comes from the COLOR_ARRAY.
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);


	//Write to the NORMAL_ARRAY buffer.
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

	//Enable vertex attribute 2 and specify the data comes from the COLOR_ARRAY.
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Sphere::GenerateIndices()
{
	const GLuint trianglesPerSlice = ((stacks - 1) * 2) + 2;
	const GLuint numTriangles = trianglesPerSlice * slices;
	const int indexCount = numTriangles * 3;
	indices.resize(indexCount);

	//Middle vertices.
	for (GLint sliceIndex = 0; sliceIndex < slices; sliceIndex++)
	{
		indices[sliceIndex * trianglesPerSlice * 3] = 0;
		indices[sliceIndex * trianglesPerSlice * 3 + 1] = 1 + ((sliceIndex + 1) % slices) * stacks;
		indices[sliceIndex * trianglesPerSlice * 3 + 2] = 1 + sliceIndex * stacks;

		for (GLint stackIndex = 0; stackIndex < stacks - 1; stackIndex++)
		{
			indices[sliceIndex * trianglesPerSlice * 3 + 3 + stackIndex * 6] = (stackIndex + 1) + sliceIndex * stacks;
			indices[sliceIndex * trianglesPerSlice * 3 + 3 + stackIndex * 6 + 1] = (stackIndex + 1) + ((sliceIndex + 1) % slices) * stacks;
			indices[sliceIndex * trianglesPerSlice * 3 + 3 + stackIndex * 6 + 2] = (stackIndex + 2) + ((sliceIndex + 1) % slices) * stacks;

			indices[sliceIndex * trianglesPerSlice * 3 + 3 + stackIndex * 6 + 3] = (stackIndex + 1) + sliceIndex * stacks;
			indices[sliceIndex * trianglesPerSlice * 3 + 3 + stackIndex * 6 + 4] = (stackIndex + 2) + ((sliceIndex + 1) % slices) * stacks;
			indices[sliceIndex * trianglesPerSlice * 3 + 3 + stackIndex * 6 + 5] = (stackIndex + 2) + sliceIndex * stacks;
		}

		indices[sliceIndex * trianglesPerSlice * 3 + 3 + (stacks - 1) * 6] = (sliceIndex + 1) * stacks;
		indices[sliceIndex * trianglesPerSlice * 3 + 3 + (stacks - 1) * 6 + 1] = ((sliceIndex + 1) % slices) * stacks + stacks;
		indices[sliceIndex * trianglesPerSlice * 3 + 3 + (stacks - 1) * 6 + 2] = slices * stacks + 1;
	}

	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
	glBindVertexArray(0);
}

void Sphere::Draw()
{
	glBindVertexArray(vao);

	//Send vertices to the vertex stream using indexed rendering.
	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, (GLvoid*)0);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
