#include "ImagePNG.h"

#include <string.h>
#include "./Core/include/Log.h"
#include "./Data/include/OPfile.h"
 #include <png.h>
 #include <cstdio>
 #include <string>
#include "./Core/include/DynamicMemory.h"
#include "./Human/Utilities/LodePNG.h"

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

#if defined(OPIFEX_OPENGL_ES_2)
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
	#define OP_GL_COMPRESSED_RGBA_S3TC_DXT1_EXT GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
	#define OP_GL_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83F2 // This is for Ouya, extension is supported in Tegra 3
#else
	#include <GL/glew.h>
	#include <GL/glfw.h>
	#define OP_GL_COMPRESSED_RGBA_S3TC_DXT1_EXT GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
	#define OP_GL_COMPRESSED_RGBA_S3TC_DXT3_EXT GL_COMPRESSED_RGBA_S3TC_DXT3_EXT
	#define OP_GL_COMPRESSED_RGBA_S3TC_DXT5_EXT GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
#endif

ImagePNG::ImagePNG(FILE* fp)
{
    png_byte header[8];

    // read the header
    fread(header, 1, 8, fp);

    if (png_sig_cmp(header, 0, 8))
    {
        fprintf(stderr, "error: is not a PNG.\n");
        fclose(fp);
        return;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
    {
        fprintf(stderr, "error: png_create_read_struct returned 0.\n");
        fclose(fp);
        return;
    }

    // create png info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        fprintf(stderr, "error: png_create_info_struct returned 0.\n");
        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        fclose(fp);
        return;
    }

    // create png info struct
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info)
    {
        fprintf(stderr, "error: png_create_info_struct returned 0.\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
        fclose(fp);
        return;
    }

    // the code in this if statement gets called if libpng encounters an error
    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "error from libpng\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return;
    }

    // init png reading
    png_init_io(png_ptr, fp);

    // let libpng know you already read the first 8 bytes
    png_set_sig_bytes(png_ptr, 8);

    // read all the info up to the image data
    png_read_info(png_ptr, info_ptr);

    // variables to pass to get info
    i32 bit_depth, color_type;

    // get info about png
    png_get_IHDR(png_ptr, info_ptr, &_width, &_height, &bit_depth, &color_type,
        NULL, NULL, NULL);

    //if (width){ *width = temp_width; }
    //if (height){ *height = temp_height; }

    // Update the png info struct.
    png_read_update_info(png_ptr, info_ptr);

    // Row size in bytes.
    ui32 rowbytes = png_get_rowbytes(png_ptr, info_ptr);

    // glTexImage2d requires rows to be 4-byte aligned
    rowbytes += 3 - ((rowbytes-1) % 4);

    // Allocate the image_data as a big block, to be given to opengl
    //png_byte * image_data;
	_buffer = (ui8*)OPalloc(rowbytes * _height * sizeof(ui8) + 15);
    if (_buffer == NULL)
    {
        fprintf(stderr, "error: could not allocate memory for PNG image data\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fclose(fp);
        return;
    }

    // row_pointers is for pointing to image_data for reading the png with libpng
    ui8** row_pointers = (ui8**)OPalloc(_height * sizeof(ui8*));
    if (row_pointers == NULL)
    {
        fprintf(stderr, "error: could not allocate memory for PNG row pointers\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        OPfree(_buffer);
        fclose(fp);
        return;
    }

    // set the individual row_pointers to point at the correct offsets of image_data
    ui32 i;
    for (i = 0; i < _height; i++)
    {
        row_pointers[_height - 1 - i] = _buffer + i * rowbytes;
    }

    // read the png into image_data through row_pointers
    png_read_image(png_ptr, row_pointers);

    // clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    //OPfree(image_data);
    OPfree(row_pointers);

	//_buffer = image_data;
	_mipMapCount = 1;
	_format = GL_RGBA;
	_compressed = false;
	_blocksize = 0;
}

ImagePNG* ImagePNG::FromFile(const char* file) {
	FileInformation fileInfo = OPreadFileInformation(file);
	return new ImagePNG(fileInfo.file);
}

Texture2D* ImagePNG::TextureFromFile(const char* file) {
	ImagePNG* imageData = ImagePNG::FromFile(file);
	Texture2D* texture = new Texture2D(imageData);
	delete imageData;
	return texture;
}