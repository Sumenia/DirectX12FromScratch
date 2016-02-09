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
	JSAMPARRAY						buffer;
	int								area;

	err = fopen_s(&file, filename.c_str(), "rb");
	if (err)
	{
		strerror_s(error, 4096, err);
		std::cerr << "Error opening file " << filename << " : " << error << std::endl;
		return (false);
	}

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, file);
	jpeg_read_header(&cinfo, TRUE);

	jpeg_start_decompress(&cinfo);

	area = cinfo.output_width * cinfo.output_components;

	buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, area, 1);


	while (cinfo.output_scanline < cinfo.output_height)
	{

		jpeg_read_scanlines(&cinfo, buffer, 1);
		std::cout << buffer << std::endl;

	}
	jpeg_finish_decompress(&cinfo);
	fclose(file);

	std::cout << "YEAHHH" << std::endl;

	return (true);
}


