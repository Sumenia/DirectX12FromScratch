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
	JSAMPARRAY						buffer;
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
	if (setjmp(setjmp_buffer))
	{
		std::cerr << "Error in jpeg file" << std::endl;
		jpeg_destroy_decompress(&cinfo);
		fclose(file);
		return (false);
	}
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, file);
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);

	_width = cinfo.output_width;
	_height = cinfo.output_height;
	_rgbFormat = cinfo.num_components == 4 ? ERGBFormat::Type::RGBA : ERGBFormat::Type::RGB;
	_bitDepth = 8;
	_length = _width * _height * cinfo.num_components;
	_data = new unsigned char[_length];

	counter = 0;
	row_stride = _width * cinfo.num_components;
	buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);
	while (cinfo.output_scanline < cinfo.output_height)
	{
		jpeg_read_scanlines(&cinfo, buffer, 1);
		memcpy((unsigned char *)_data + counter, buffer[0], row_stride);
		counter += row_stride;
	}

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	fclose(file);

	return (true);
}


