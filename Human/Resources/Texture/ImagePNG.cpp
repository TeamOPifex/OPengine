#include "ImagePNG.h"

#include <string.h>
#include "./Core/include/Log.h"
#include "./Data/include/OPfile.h"
 #include <png.h>
 #include <cstdio>
 #include <string>
#include "./Core/include/DynamicMemory.h"
#include "./Human/Utilities/LodePNG.h"

#if defined(OPIFEX_OPENGL_ES_2)
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
#else
	#include <GL/glew.h>
	#include <GL/glfw.h>
#endif

ImagePNG::ImagePNG(const char* file)
{
	std::vector<ui8> image;
	ui32 width, height;
	unsigned error = lodepng::decode(image, width, height, file);

	// Ensure power of 2
	size_t u2 = 1; while(u2 < width) u2 *= 2;
    size_t v2 = 1; while(v2 < height) v2 *= 2;

	_buffer = (ui8*)OPalloc(u2 * v2 * 4);
	for(size_t y = 0; y < height; y++)
		for(size_t x = 0; x < width; x++)
			for(size_t c = 0; c < 4; c++)
			{
				_buffer[4 * u2 * y + 4 * x + c] = image[4 * width * y + 4 * x + c];
			}

	_width = width;
	_height = height;
	_format = GL_RGBA;
	_blocksize = 0;
	_mipMapCount = 1;
	_compressed = false;
}

ImagePNG* ImagePNG::FromFile(const char* file) {
	//FileInformation fileInfo = OPreadFileInformation(file);
	return new ImagePNG(file);
}

Texture2D* ImagePNG::TextureFromFile(const char* file) {
	ImagePNG* imageData = ImagePNG::FromFile(file);
	Texture2D* texture = new Texture2D(imageData);
	//delete imageData;
	return texture;
}