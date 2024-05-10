#include "TGALoader.h"

using namespace std;

Texture TGALoader::Load(string path)
{
	Texture texture;
	texture.id = 0; //from OpenGL (later)
	texture.path = path;
	
	ifstream instream(path.c_str(), ios::binary);

	char* buf = new char[18];
	instream.read(buf, 18);

	if (instream.eof())
	{
		//Error, haven't finished reading TGA.
		instream.close();
	}

	int imageIDLength = buf[0];
	int colorMapType = buf[1];
	int imageType = buf[2];
	//DebugHelper::PrintFormat(_T("Image ID Length: %d\n"), imageIDLength);
	//DebugHelper::PrintFormat(_T("Color Map Type: %d\n"), colorMapType);
	//DebugHelper::PrintFormat(_T("Image Type: %d\n"), imageType);
	bool rle = (((imageType >> 3) & 0x01) == 0x01);
	if (rle)
	{
		//DebugHelper::PrintFormat(_T("RLE Compressed Image\n"));
	}


	int colorMapFirstIndex = (buf[4] << 8) | buf[3];
	int colorMapLength = (buf[6] << 8) | buf[5];
	int colorMapBitsPerPixel = buf[7];
	//DebugHelper::PrintFormat(_T("Color Map Length: %d\n"), colorMapLength);
	//DebugHelper::PrintFormat(_T("Color Map Entry Size: %d bits/pixel\n"), colorMapBitsPerPixel);

	ImageSpecification imageSpec;
	imageSpec.xOrigin = (buf[9] << 8) | buf[8];
	imageSpec.yOrigin = (buf[11] << 8) | buf[10];
	imageSpec.width = (buf[13] << 8) | buf[12];
	imageSpec.height = (buf[15] << 8) | buf[14];
	imageSpec.pixelDepth = buf[16];
	imageSpec.imageDescriptor = buf[17];

	//DebugHelper::PrintFormat(_T("Image %d × %d px (x=%d, y=%d)\n"), imageSpec.width, imageSpec.height, imageSpec.xOrigin, imageSpec.yOrigin);
	//DebugHelper::PrintFormat(_T("Pixel Depth: %d\n"), imageSpec.pixelDepth);
	//DebugHelper::PrintFormat(_T("Image Descriptor: %d\n"), imageSpec.imageDescriptor);
	
	int attributeBitsPerPixel = (imageSpec.imageDescriptor & 0x0F);
	bool rightToLeft = (((imageSpec.imageDescriptor >> 4) & 0x01) == 0x01);
	bool topToBottom = (((imageSpec.imageDescriptor >> 5) & 0x01) == 0x01);
	if (rightToLeft || topToBottom)
	{
		//Error, only bottom-left origin is supported.
	}

	//DebugHelper::PrintFormat(_T("Attribute Bits per Pixel: %d\n"), attributeBitsPerPixel);
	//DebugHelper::PrintFormat(_T("leftToRight = %d, topToBottom = %d\n"), rightToLeft, topToBottom);
	
	delete[] buf;

	//Done reading header, now read the image ID.

	buf = new char[imageIDLength];
	instream.read(buf, imageIDLength);

	if (instream.eof())
	{
		//Error, haven't finished reading TGA.
		instream.close();
	}

	delete[] buf;

	//Done reading imageID, now read the color map.

	buf = new char[colorMapLength];
	instream.read(buf, colorMapLength);

	if (instream.eof())
	{
		//Error, haven't finished reading TGA.
		instream.close();
	}

	delete[] buf;

	//Done reading color map, now read the image data.

	int bytesPerPixel = imageSpec.pixelDepth / 8;
	int pixels = imageSpec.width * imageSpec.height;
	
	char* imageData = new char[pixels * bytesPerPixel];
	if (rle)
	{
		LoadCompressedTGA(&instream, imageType, &imageSpec, imageData);
	}

	/*for (int row = 0; row < imageSpec.height; row++)
	{
		for (int col = 0; col < imageSpec.width; col++)
		{
			DebugHelper::PrintFormat(_T("R=%02X G=%02X B=%02X A=%02X "),
				(unsigned char)imageData[(row * imageSpec.width + col) * bytesPerPixel],
				(unsigned char)imageData[(row * imageSpec.width + col) * bytesPerPixel + 1],
				(unsigned char)imageData[(row * imageSpec.width + col) * bytesPerPixel + 2],
				(unsigned char)imageData[(row * imageSpec.width + col) * bytesPerPixel + 3]);
		}
		DebugHelper::PrintFormat(_T("\n"));
	}*/

	if (instream.eof())
	{
		//Error, haven't finished reading TGA.
		instream.close();
	}

	//Create OpenGL texture object.
	glGenTextures(1, &texture.id);
	glBindTexture(GL_TEXTURE_2D, texture.id);
	
	//Apply filtering parameters.
	//When a pixel spans multiple texels, use a linear computation for the pixel.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//When a pixel is completely inside a texel, use the color at the spot (nearest).
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Send image data to texture object.
	//Note: OpenGL expects texture to be built starting from bottom-left.
	if (attributeBitsPerPixel > 0)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageSpec.width, imageSpec.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageSpec.width, imageSpec.height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
	}

	//Unbind texture.
	glBindTexture(GL_TEXTURE_2D, 0);

	//Cleanup.
	delete[] imageData;
	instream.close();

	//DebugHelper::PrintFormat(_T("TextureID: %d\n"), texture.id);
	return texture;
}

bool TGALoader::LoadCompressedTGA(ifstream* tgaFile, int imageType, ImageSpecification* imageSpec, char* imageData)
{
	if (imageType == 9)
	{
		OutputDebugString(_T("Run-Length Encoded, Color-Mapped Images not supported!\n"));
		return false;
	}
	if (imageType == 11)
	{
		OutputDebugString(_T("Run-Length Encoded, Black-and-White Images not supported!\n"));
		return false;
	}

	//Image globals.
	int bytesPerPixel = imageSpec->pixelDepth / 8;
	int totalPixels = imageSpec->width * imageSpec->height;
	int attributeBitDepth = (imageSpec->imageDescriptor & 0x0F);
	int rgbBitDepth = imageSpec->pixelDepth - attributeBitDepth;
	int pixelIndex = 0;
	
	//Current color values.
	int a;
	int r;
	int g;
	int b;

	//Iterate while there are still pixels to read.
	while (pixelIndex < totalPixels)
	{
		//Read packet header.
		char* header = new char[1];
		tgaFile->read(header, 1);

		if (tgaFile->eof())
		{
			tgaFile->close();
			return false; //Failed because of error reading image file.
		}

		//Decode header.
		bool isRunLengthPacket = (((header[0] >> 7) & 0x01) == 0x01);
		int numPixels = (header[0] & 0x7F) + 1;

		delete[] header;

		if (isRunLengthPacket)
		{
			//Run-Length Packet

			//Read pixel and store into the result image data n times.
			if (imageType == 10)
			{
				//If True-Color pixels, read in ARGB format (little-endian).
				//Only 24-bit and 32-bit pixels are supported.
				char* color = new char[bytesPerPixel];
				tgaFile->read(color, bytesPerPixel);

				if (tgaFile->eof())
				{
					tgaFile->close();
					return false; //Failed because of error reading image file.
				}

				b = color[0];
				g = color[1];
				r = color[2];
				if (attributeBitDepth > 0)
				{
					a = color[3];
				}
				delete[] color;

				//Store as RGB or RGBA
				//Iterate through the entire run.
				for (int runIndex = 0; runIndex < numPixels; runIndex++)
				{
					imageData[pixelIndex * bytesPerPixel] = r;
					imageData[pixelIndex * bytesPerPixel + 1] = g;
					imageData[pixelIndex * bytesPerPixel + 2] = b;
					if (attributeBitDepth > 0)
					{
						imageData[pixelIndex * bytesPerPixel + 3] = a;
					}
					pixelIndex++;
				}
			}
		}
		else
		{
			//Raw Packet

			//Read n pixels
			if (imageType == 10)
			{
				for (int rawIndex = 0; rawIndex < numPixels; rawIndex++)
				{
					//If True-Color pixels, read in ARGB format (little-endian).
					//Only 24-bit and 32-bit pixels are supported.
					char* color = new char[bytesPerPixel];
					tgaFile->read(color, bytesPerPixel);

					if (tgaFile->eof())
					{
						tgaFile->close();
						return false; //Failed because of error reading image file.
					}

					b = color[0];
					g = color[1];
					r = color[2];
					if (attributeBitDepth > 0)
					{
						a = color[3];
					}
					delete[] color;

					//Store as RGB or RGBA
					imageData[pixelIndex * bytesPerPixel] = r;
					imageData[pixelIndex * bytesPerPixel + 1] = g;
					imageData[pixelIndex * bytesPerPixel + 2] = b;
					if (attributeBitDepth > 0)
					{
						imageData[pixelIndex * bytesPerPixel + 3] = a;
					}
					pixelIndex++;
				}
			}
		}
	}
	return true;
}
