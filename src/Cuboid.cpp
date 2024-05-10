#include "Cuboid.h"


Cuboid::Cuboid(const GLfloat xSize, const GLfloat ySize, const GLfloat zSize)
{
	this->xSize = xSize;
	this->ySize = ySize;
	this->zSize = zSize;
	glGenVertexArrays(1, &vao);
	glGenBuffers(3, vbo);
	glGenBuffers(1, &ebo);
	GenerateVertexAttributes();
	GenerateIndices();
}

Cuboid::Cuboid(const GLfloat xSize, const GLfloat ySize, const GLfloat zSize, glm::vec3 solidColor)
{
	this->xSize = xSize;
	this->ySize = ySize;
	this->zSize = zSize;
	this->usingSolidColor = true;
	this->solidColor = solidColor;
	glGenVertexArrays(1, &vao);
	glGenBuffers(3, vbo);
	glGenBuffers(1, &ebo);
	GenerateVertexAttributes();
	GenerateIndices();
}

Cuboid::~Cuboid()
{
	vertices.clear();
	indices.clear();
}

void Cuboid::GenerateVertexAttributes()
{
	//Fill the vertex with 24 instances of the Vertex struct
	//These will be filled in below to specify the cuboid's model parameters.
	for (int index = 0; index < 24; index++)
	{
		vertices.push_back(Vertex());
	}
	tangents.resize(24);
	bitangents.resize(24);

	//Front vertices.
	vertices[0].Position = glm::vec3(-xSize / 2.0f, -ySize / 2.0f, zSize / 2.0f);
	vertices[1].Position = glm::vec3(xSize / 2.0f, -ySize / 2.0f, zSize / 2.0f);
	vertices[2].Position = glm::vec3(-xSize / 2.0f, ySize / 2.0f, zSize / 2.0f);
	vertices[3].Position = glm::vec3(xSize / 2.0f, ySize / 2.0f, zSize / 2.0f);
	
	//Right vertices.
	vertices[4].Position = glm::vec3(xSize / 2.0f, -ySize / 2.0f, zSize / 2.0f);
	vertices[5].Position = glm::vec3(xSize / 2.0f, -ySize / 2.0f, -zSize / 2.0f);
	vertices[6].Position = glm::vec3(xSize / 2.0f, ySize / 2.0f, zSize / 2.0f);
	vertices[7].Position = glm::vec3(xSize / 2.0f, ySize / 2.0f, -zSize / 2.0f);

	//Back vertices.
	vertices[8].Position = glm::vec3(xSize / 2.0f, -ySize / 2.0f, -zSize / 2.0f);
	vertices[9].Position = glm::vec3(-xSize / 2.0f, -ySize / 2.0f, -zSize / 2.0f);
	vertices[10].Position = glm::vec3(xSize / 2.0f, ySize / 2.0f, -zSize / 2.0f);
	vertices[11].Position = glm::vec3(-xSize / 2.0f, ySize / 2.0f, -zSize / 2.0f);
	
	//Left vertices.
	vertices[12].Position = glm::vec3(-xSize / 2.0f, -ySize / 2.0f, -zSize / 2.0f);
	vertices[13].Position = glm::vec3(-xSize / 2.0f, -ySize / 2.0f, zSize / 2.0f);
	vertices[14].Position = glm::vec3(-xSize / 2.0f, ySize / 2.0f, -zSize / 2.0f);
	vertices[15].Position = glm::vec3(-xSize / 2.0f, ySize / 2.0f, zSize / 2.0f);

	//Top vertices.
	vertices[16].Position = glm::vec3(-xSize / 2.0f, ySize / 2.0f, zSize / 2.0f);
	vertices[17].Position = glm::vec3(xSize / 2.0f, ySize / 2.0f, zSize / 2.0f);
	vertices[18].Position = glm::vec3(-xSize / 2.0f, ySize / 2.0f, -zSize / 2.0f);
	vertices[19].Position = glm::vec3(xSize / 2.0f, ySize / 2.0f, -zSize / 2.0f);

	//Bottom vertices.
	vertices[20].Position = glm::vec3(-xSize / 2.0f, -ySize / 2.0f, -zSize / 2.0f);
	vertices[21].Position = glm::vec3(xSize / 2.0f, -ySize / 2.0f, -zSize / 2.0f);
	vertices[22].Position = glm::vec3(-xSize / 2.0f, -ySize / 2.0f, zSize / 2.0f);
	vertices[23].Position = glm::vec3(xSize / 2.0f, -ySize / 2.0f, zSize / 2.0f);

	//============================

	if (!usingSolidColor)
	{
		//Front colors.
		vertices[0].Color = glm::vec3(1.0f, 0.0f, 0.0f);
		vertices[1].Color = glm::vec3(1.0f, 0.0f, 0.0f);
		vertices[2].Color = glm::vec3(1.0f, 0.0f, 0.0f);
		vertices[3].Color = glm::vec3(1.0f, 0.0f, 0.0f);

		//Right colors.
		vertices[4].Color = glm::vec3(0.0f, 1.0f, 0.0f);
		vertices[5].Color = glm::vec3(0.0f, 1.0f, 0.0f);
		vertices[6].Color = glm::vec3(0.0f, 1.0f, 0.0f);
		vertices[7].Color = glm::vec3(0.0f, 1.0f, 0.0f);

		//Back colors.
		vertices[8].Color = glm::vec3(1.0f, 1.0f, 0.0f);
		vertices[9].Color = glm::vec3(1.0f, 1.0f, 0.0f);
		vertices[10].Color = glm::vec3(1.0f, 1.0f, 0.0f);
		vertices[11].Color = glm::vec3(1.0f, 1.0f, 0.0f);

		//Left colors.
		vertices[12].Color = glm::vec3(0.0f, 0.0f, 1.0f);
		vertices[13].Color = glm::vec3(0.0f, 0.0f, 1.0f);
		vertices[14].Color = glm::vec3(0.0f, 0.0f, 1.0f);
		vertices[15].Color = glm::vec3(0.0f, 0.0f, 1.0f);

		//Top colors.
		vertices[16].Color = glm::vec3(1.0f, 0.0f, 1.0f);
		vertices[17].Color = glm::vec3(1.0f, 0.0f, 1.0f);
		vertices[18].Color = glm::vec3(1.0f, 0.0f, 1.0f);
		vertices[19].Color = glm::vec3(1.0f, 0.0f, 1.0f);

		//Bottom colors.
		vertices[20].Color = glm::vec3(0.0f, 1.0f, 1.0f);
		vertices[21].Color = glm::vec3(0.0f, 1.0f, 1.0f);
		vertices[22].Color = glm::vec3(0.0f, 1.0f, 1.0f);
		vertices[23].Color = glm::vec3(0.0f, 1.0f, 1.0f);
	}
	else
	{
		for (int index = 0; index < 24; index++)
		{
			vertices[index].Color = this->solidColor;
		}
	}

	//===========================

	//Front normals.
	vertices[0].Normal = glm::vec3(0.0f, 0.0f, 1.0f);
	vertices[1].Normal = glm::vec3(0.0f, 0.0f, 1.0f);
	vertices[2].Normal = glm::vec3(0.0f, 0.0f, 1.0f);
	vertices[3].Normal = glm::vec3(0.0f, 0.0f, 1.0f);

	//Right normals.
	vertices[4].Normal = glm::vec3(1.0f, 0.0f, 0.0f);
	vertices[5].Normal = glm::vec3(1.0f, 0.0f, 0.0f);
	vertices[6].Normal = glm::vec3(1.0f, 0.0f, 0.0f);
	vertices[7].Normal = glm::vec3(1.0f, 0.0f, 0.0f);

	//Back normals.
	vertices[8].Normal = glm::vec3(0.0f, 0.0f, -1.0f);
	vertices[9].Normal = glm::vec3(0.0f, 0.0f, -1.0f);
	vertices[10].Normal = glm::vec3(0.0f, 0.0f, -1.0f);
	vertices[11].Normal = glm::vec3(0.0f, 0.0f, -1.0f);

	//Left normals.
	vertices[12].Normal = glm::vec3(-1.0f, 0.0f, 0.0f);
	vertices[13].Normal = glm::vec3(-1.0f, 0.0f, 0.0f);
	vertices[14].Normal = glm::vec3(-1.0f, 0.0f, 0.0f);
	vertices[15].Normal = glm::vec3(-1.0f, 0.0f, 0.0f);

	//Top normals.
	vertices[16].Normal = glm::vec3(0.0f, 1.0f, 0.0f);
	vertices[17].Normal = glm::vec3(0.0f, 1.0f, 0.0f);
	vertices[18].Normal = glm::vec3(0.0f, 1.0f, 0.0f);
	vertices[19].Normal = glm::vec3(0.0f, 1.0f, 0.0f);

	//Bottom normals.
	vertices[20].Normal = glm::vec3(0.0f, -1.0f, 0.0f);
	vertices[21].Normal = glm::vec3(0.0f, -1.0f, 0.0f);
	vertices[22].Normal = glm::vec3(0.0f, -1.0f, 0.0f);
	vertices[23].Normal = glm::vec3(0.0f, -1.0f, 0.0f);

	//==========================

	//Front tex coords.
	vertices[0].TexCoord = glm::vec2(0.0f, 0.0f);
	vertices[1].TexCoord = glm::vec2(1.0f, 0.0f);
	vertices[2].TexCoord = glm::vec2(0.0f, 1.0f);
	vertices[3].TexCoord = glm::vec2(1.0f, 1.0f);

	//Right tex coords.
	vertices[4].TexCoord = glm::vec2(1.0f, 0.0f);
	vertices[5].TexCoord = glm::vec2(0.0f, 0.0f);
	vertices[6].TexCoord = glm::vec2(1.0f, 1.0f);
	vertices[7].TexCoord = glm::vec2(0.0f, 1.0f);

	//Back tex coords.
	vertices[8].TexCoord = glm::vec2(0.0f, 0.0f);
	vertices[9].TexCoord = glm::vec2(1.0f, 0.0f);
	vertices[10].TexCoord = glm::vec2(0.0f, 1.0f);
	vertices[11].TexCoord = glm::vec2(1.0f, 1.0f);

	//Left tex coords.
	vertices[12].TexCoord = glm::vec2(1.0f, 0.0f);
	vertices[13].TexCoord = glm::vec2(0.0f, 0.0f);
	vertices[14].TexCoord = glm::vec2(1.0f, 1.0f);
	vertices[15].TexCoord = glm::vec2(0.0f, 1.0f);

	//Top tex coords.
	vertices[16].TexCoord = glm::vec2(0.0f, 0.0f);
	vertices[17].TexCoord = glm::vec2(1.0f, 0.0f);
	vertices[18].TexCoord = glm::vec2(0.0f, 1.0f);
	vertices[19].TexCoord = glm::vec2(1.0f, 1.0f);

	//Bottom tex coords.
	vertices[20].TexCoord = glm::vec2(1.0f, 1.0f);
	vertices[21].TexCoord = glm::vec2(0.0f, 1.0f);
	vertices[22].TexCoord = glm::vec2(1.0f, 0.0f);
	vertices[23].TexCoord = glm::vec2(0.0f, 0.0f);

	//Tangent & Bitagents
	for (int index = 0; index < 24; index++)
	{
		ComputeTangentAndBitangent(index);
		//DebugHelper::PrintFormat(_T("T=<%f, %f, %f>, B=<%f, %f, %f>\n"), tangents[index].x, tangents[index].y, tangents[index].z, bitangents[index].x, bitangents[index].y, bitangents[index].z);
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

void Cuboid::ComputeTangentAndBitangent(int vertex)
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

void Cuboid::GenerateIndices()
{
	const int sides = 6;
	const int trianglesPerSide = 2;
	const int verticesPerTriangle = 3;
	indices.resize(sides * trianglesPerSide * verticesPerTriangle);

	//Front faces.
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	indices[3] = 2;
	indices[4] = 1;
	indices[5] = 3;

	//Right faces.
	indices[6] = 4;
	indices[7] = 5;
	indices[8] = 6;

	indices[9] = 6;
	indices[10] = 5;
	indices[11] = 7;

	//Back faces. 
	indices[12] = 8;
	indices[13] = 9;
	indices[14] = 10;

	indices[15] = 10;
	indices[16] = 9;
	indices[17] = 11;

	//Left faces.
	indices[18] = 12;
	indices[19] = 13;
	indices[20] = 14;

	indices[21] = 14;
	indices[22] = 13;
	indices[23] = 15;

	//Top faces.
	indices[24] = 16;
	indices[25] = 17;
	indices[26] = 18;

	indices[27] = 18;
	indices[28] = 17;
	indices[29] = 19;

	//Bottom faces.
	indices[30] = 20;
	indices[31] = 21;
	indices[32] = 22;

	indices[33] = 22;
	indices[34] = 21;
	indices[35] = 23;

	//Select VAO to specify indices for.
	glBindVertexArray(vao);
	//Send indices to the element buffer object.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
	//Deselect the VAO (this also deselects the element array buffer).
	glBindVertexArray(0);
}

void Cuboid::Draw()
{
	glBindVertexArray(vao);

	//Send vertices to the vertex stream using indexed rendering.
	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}