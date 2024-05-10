#include "Square.h"

Square::Square(const GLfloat xSize, const GLfloat ySize)
{
	this->xSize = xSize;
	this->ySize = ySize;
	glGenVertexArrays(1, &vao);
	glGenBuffers(3, vbo);
	glGenBuffers(1, &ebo);
	GenerateVertexAttributes();
	GenerateIndices();
}

Square::~Square()
{
	vertices.clear();
	indices.clear();
}

void Square::GenerateVertexAttributes()
{
	//Fill the vertex with 4 instances of the Vertex struct
	//These will be filled in below to specify the squares's model parameters.
	for (int index = 0; index < 4; index++)
	{
		vertices.push_back(Vertex());
	}
	tangents.resize(4);
	bitangents.resize(4);

	//Front vertices.
	vertices[0].Position = glm::vec3(-xSize / 2.0f, -ySize / 2.0f,0.0f);
	vertices[1].Position = glm::vec3(xSize / 2.0f, -ySize / 2.0f, 0.0f);
	vertices[2].Position = glm::vec3(-xSize / 2.0f, ySize / 2.0f, 0.0f);
	vertices[3].Position = glm::vec3(xSize / 2.0f, ySize / 2.0f, 0.0f);

	//============================

	//Front colors.
	vertices[0].Color = glm::vec3(1.0f, 0.0f, 0.0f);
	vertices[1].Color = glm::vec3(1.0f, 0.0f, 0.0f);
	vertices[2].Color = glm::vec3(1.0f, 0.0f, 0.0f);
	vertices[3].Color = glm::vec3(1.0f, 0.0f, 0.0f);

	//===========================

	//Front normals.
	vertices[0].Normal = glm::vec3(0.0f, 0.0f, 1.0f);
	vertices[1].Normal = glm::vec3(0.0f, 0.0f, 1.0f);
	vertices[2].Normal = glm::vec3(0.0f, 0.0f, 1.0f);
	vertices[3].Normal = glm::vec3(0.0f, 0.0f, 1.0f);
	
	//==========================

	//Front tex coords.
	vertices[0].TexCoord = glm::vec2(0.0f, 0.0f);
	vertices[1].TexCoord = glm::vec2(1.0f, 0.0f);
	vertices[2].TexCoord = glm::vec2(0.0f, 1.0f);
	vertices[3].TexCoord = glm::vec2(1.0f, 1.0f);


	//Tangent & Bitagents
	for (int index = 0; index < 4; index++)
	{
		ComputeTangentAndBitangent(index);
		//DebugHelper::PrintFormat(_T("Square_Vertex[%d]: T=<%f, %f, %f>, B=<%f, %f, %f>\n"), index, tangents[index].x, tangents[index].y, tangents[index].z, bitangents[index].x, bitangents[index].y, bitangents[index].z);
	}

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	//Enable vertex attribute 0 and associate it with position.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Position));

	//Enable vertex attribute 1 and associate it with color.
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Color));

	//Enable vertex attribute 2 and associate it with normals.
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));

	//Enable vertex attribute 3 and associate it with tex coords.
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoord));


	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &tangents[0], GL_STATIC_DRAW);

	//Enable vertex attribute 4 and associate it with tangent vector.
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &bitangents[0], GL_STATIC_DRAW);

	//Enable vertex attribute 5 and associate it with bitangent vector.
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
}

void Square::ComputeTangentAndBitangent(int vertex)
{
	int offset1 = 1;
	int offset2 = 2;
	if (vertex % 4 == 2)
	{
		//Second to last vertex.
		offset1 = -1;
		offset2 = 1;
	}
	else if (vertex % 4 == 3)
	{
		//Last vertex
		offset1 = -2;
		offset2 = -1;
	}

	glm::vec3 edge1 = vertices[vertex + offset1].Position - vertices[vertex].Position;
	glm::vec3 edge2 = vertices[vertex + offset2].Position - vertices[vertex].Position;
	glm::vec2 deltaUV1 = vertices[vertex + offset1].TexCoord - vertices[vertex].TexCoord;
	glm::vec2 deltaUV2 = vertices[vertex + offset2].TexCoord - vertices[vertex].TexCoord;

	float determinate = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
	tangents[vertex].x = determinate * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangents[vertex].y = determinate * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangents[vertex].z = determinate * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	tangents[vertex] = glm::normalize(tangents[vertex]);

	bitangents[vertex].x = determinate * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangents[vertex].y = determinate * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangents[vertex].z = determinate * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
	bitangents[vertex] = glm::normalize(bitangents[vertex]);
}

void Square::GenerateIndices()
{
	indices.resize(6);

	//Front faces.
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	indices[3] = 2;
	indices[4] = 1;
	indices[5] = 3;

	//Select VAO to specify indices for.
	glBindVertexArray(vao);
	//Send indices to the element buffer object.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
	//Deselect the VAO (this also deselects the element array buffer).
	glBindVertexArray(0);
}

void Square::Draw()
{
	glBindVertexArray(vao);

	//Send vertices to the vertex stream using indexed rendering.
	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}