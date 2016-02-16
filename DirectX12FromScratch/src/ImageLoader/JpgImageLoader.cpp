#include <iostream>
#include "ImageLoader/JpgImageLoader.h"

JpgImageLoader::JpgImageLoader() : ImageLoader(EImageFormat::Type::JPG) {}

JpgImageLoader::~JpgImageLoader() {}

bool								JpgImageLoader::isJpg(FILE *file)
{
	unsigned char					buffer[2];
	int								rd;

	memset(buffer, 0, 2);

	rd = fread(buffer, sizeof(unsigned char), 2, file);
	rewind(file);

	if (rd != 2)
		return (false);

	return (buffer[0] == 0xff && buffer[1] == 0xd8);
}

bool								JpgImageLoader::loadFromFile(FILE *file)
{
	struct jpeg_decompress_struct	cinfo;
	struct jpeg_error_mgr			jerr;
	jmp_buf							setjmp_buffer;
	unsigned char					*buffer;
	int								row_stride;
	unsigned long					counter;

	cinfo.err = jpeg_std_error(&jerr);
	
	jpeg_create_decompress(&cinfo);

	if (setjmp(setjmp_buffer))
	{
		std::cerr << "Error in jpeg file" << std::endl;
		jpeg_destroy_decompress(&cinfo);
		return (false);
	}

	jpeg_stdio_src(&cinfo, file);


	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);

	_width = cinfo.output_width;
	_height = cinfo.output_height;
	_rgbFormat = cinfo.num_components == 4 ? ERGBFormat::Type::RGBA : ERGBFormat::Type::RGB;
	_bitDepth = 8;
	_length = _width * _height * 4;
	buffer = new unsigned char[_width * _height * cinfo.num_components];
	_data = new unsigned char[_width * _height * 4];

	int j = 0;
	counter = 0;
	row_stride = _width * cinfo.num_components;
	JSAMPROW ptr = buffer;
	while (cinfo.output_scanline < cinfo.output_height)
	{
		jpeg_read_scanlines(&cinfo, &ptr, 1);
		for (int i = 0; i < row_stride; i++) {
			if (cinfo.num_components == 3 && (i + 1) % 3 == 0) {
				((unsigned char*)_data)[j++] = ptr[i];
				((unsigned char*)_data)[j++] = 1;
			}
			else
				((unsigned char*)_data)[j++] = ptr[i];
		}
		ptr += row_stride;
	}

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	delete buffer;

	return (true);
}


