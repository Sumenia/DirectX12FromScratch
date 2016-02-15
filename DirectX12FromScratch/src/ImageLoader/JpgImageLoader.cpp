#include <iostream>
#include "ImageLoader/JpgImageLoader.h"

JpgImageLoader::JpgImageLoader() : ImageLoader(EImageFormat::Type::JPG) {}

JpgImageLoader::~JpgImageLoader() {}

bool								JpgImageLoader::loadFromFile(const std::string &filename)
{
	FILE							*file;
	errno_t							err;
	char							error[4096];
	struct jpeg_decompress_struct	cinfo;
	struct jpeg_error_mgr			jerr;
	jmp_buf							setjmp_buffer;
	unsigned char					*buffer;
	int								row_stride;
	unsigned long					counter;

	err = fopen_s(&file, filename.c_str(), "rb");
	if (err)
	{
		strerror_s(error, 4096, err);
		std::cerr << "Error opening file " << filename << " : " << error << std::endl;
		return (false);
	}

	cinfo.err = jpeg_std_error(&jerr);
	
	jpeg_create_decompress(&cinfo);

	if (setjmp(setjmp_buffer))
	{
		std::cerr << "Error in jpeg file" << std::endl;
		jpeg_destroy_decompress(&cinfo);
		fclose(file);
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
	fclose(file);

	delete buffer;

	return (true);
}


