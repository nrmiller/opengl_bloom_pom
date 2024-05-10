#include "Model.h"

Model::Model(string path)
{
	LoadModel(path);
}

Model::~Model()
{
}

void Model::LoadModel(string path)
{
//	Assimp::Importer import;
//	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
//
//	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
//	{
//		const int errSize = 39 + 256 + 256 + 1;
//		TCHAR errMessage[errSize];
//		std::fill_n(errMessage, errSize, 0);
//		StringCchPrintf(errMessage, errSize, _T("Failed to load model: %s.\nERROR::ASSIMP::%s"), path, import.GetErrorString());
//		MessageBox(NULL, errMessage, _T("Window Error"), MB_OK | MB_ICONERROR);
//		return;
//	}
//
	directory = path.substr(0, path.find_last_of('/'));

//	ProcessNode(scene->mRootNode, scene);
}

//void Model::ProcessNode(aiNode* node, const aiScene* scene)
//{
//	//Process meshes.
//	for (int index = 0; index < node->mNumMeshes; index++)
//	{
//		//Iterate through all mesh indices inside the current node.
//		//Locate the actual mesh data inside the scene using these indices.
//		aiMesh* mesh = scene->mMeshes[node->mMeshes[index]];
//		meshes.push_back(ProcessMesh(mesh, scene));
//	}
//
//	//Process sub-nodes.
//	for (int index = 0; index < node->mNumChildren; index++)
//	{
//		ProcessNode(node->mChildren[index], scene);
//	}
//}
//
//Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
//{
//	vector<Vertex> vertices;
//	vector<GLuint> indices;
//	vector<Texture> textures;
//
//	//Convert aiMesh to our Mesh.
//	for (int index = 0; index < mesh->mNumVertices; index++)
//	{
//		Vertex vertex;
//		glm::vec3 vector;
//		vector.x = mesh->mVertices[index].x;
//		vector.y = mesh->mVertices[index].y;
//		vector.z = mesh->mVertices[index].z;
//		vertex.Position = vector;
//
//		vector.x = mesh->mNormals[index].x;
//		vector.y = mesh->mNormals[index].y;
//		vector.z = mesh->mNormals[index].z;
//		vertex.Normal = vector;
//
//		//If there are texture coordinates, copy them.
//		if (mesh->mTextureCoords[0])
//		{
//			glm::vec2 vec;
//			vec.x = mesh->mTextureCoords[0][index].x;
//			vec.y = mesh->mTextureCoords[0][index].y;
//			vertex.TexCoord = vec;
//		}
//		else
//		{
//			vertex.TexCoord = glm::vec2(0.0f, 0.0f);
//		}
//		vertices.push_back(vertex);
//	}
//
//	//Obtain indices from mesh.
//	for (int faceIndex = 0; faceIndex < mesh->mNumFaces; faceIndex++)
//	{
//		aiFace face = mesh->mFaces[faceIndex];
//		for (int index = 0; index < face.mNumIndices; index++)
//		{
//			indices.push_back(face.mIndices[index]);
//		}
//	}
//
//	//Check if mesh has a material.
//	if (mesh->mMaterialIndex >= 0)
//	{
//		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
//		//Add all textures to the material.
//		vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
//		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
//		vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
//		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
//	}
//
//	return Mesh(vertices, indices, textures);
//}
//
//void Model::Draw(GLuint shader)
//{
//	for (GLuint index = 0; index < meshes.size(); index++)
//	{
//		meshes[index].Draw(shader);
//	}
//}
//
//vector<Texture> Model::LoadMaterialTextures(aiMaterial* material, aiTextureType type, string typeName)
//{
//	vector<Texture> textures;
//	for (int index = 0; index < material->GetTextureCount(type); index++)
//	{
//		aiString str;
//		material->GetTexture(type, index, &str); //Get file path of the texture.
//
//		//Check if texture is already loaded.  If so, then use it.
//		bool alreadyLoaded = false;
//		for (int texIndex = 0; texIndex < loadedTextures.size(); texIndex++)
//		{
//			if (loadedTextures[texIndex].path == str)
//			{
//				textures.push_back(loadedTextures[texIndex]);
//				alreadyLoaded = true;
//				break;
//			}
//		}
//
//		if (!alreadyLoaded)
//		{
//			Texture texture;
//			texture.id = TextureFromFile(str.C_Str(), this->directory);
//			texture.type = typeName;
//			texture.path = str;
//			textures.push_back(texture);
//			loadedTextures.push_back(texture);
//		}
//	}
//	return textures;
//}
//
//GLuint Model::TextureFromFile(const char* fileName, string directory)
//{
//	string fullName = directory + '/' + string(fileName);
//	GLuint textureID;
//	glGenTextures(1, &textureID);
//	int width, height;
//	unsigned char* image = SOIL_load_image(fullName.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
//
//	//Store texture image in GPU.
//	glBindTexture(GL_TEXTURE_2D, textureID);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
//
//	//Configure texture settings.
//	glGenerateMipmap(GL_TEXTURE_2D);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//	glBindTexture(GL_TEXTURE_2D, 0);
//	SOIL_free_image_data(image);
//	return textureID;
//}