#pragma once
#include "Includes.h"
#include "Texture.h"

using namespace std;

typedef struct
{
	unsigned short xOrigin;
	unsigned short yOrigin;
	unsigned short width;
	unsigned short height;
	unsigned char pixelDepth;
	unsigned char imageDescriptor;
} ImageSpecification;

class TGALoader
{
public:
	static Texture Load(string path);
private:
	static bool LoadCompressedTGA(ifstream* tgaFile, int imageType, ImageSpecification* imageSpec, char* imageData);
};