#pragma once

#include "Includes.h"
#include "Mesh.h"
#include <vector>
//#include <SOIL.h>
//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>

using namespace std;

class Model
{
public:
	Model(string path);
	~Model();
	void Draw(GLuint shader);
private:
	vector<Mesh> meshes;
	string directory;
	vector<Texture> loadedTextures;

	void LoadModel(string path);
//	void ProcessNode(aiNode* node, const aiScene* scene);
//	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
//	vector<Texture> LoadMaterialTextures(aiMaterial* material, aiTextureType type, string typeName);
//	GLuint TextureFromFile(const char* fileName, string directory);
};

