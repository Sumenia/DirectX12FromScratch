#include "ImageLoader/PngImageloader.h"
#include <iostream>

PngImageLoader::PngImageLoader() : ImageLoader(EImageFormat::Type::PNG) {}

PngImageLoader::~PngImageLoader()
{
}

bool			PngImageLoader::writeToFile(const std::string &filename)
{
	FILE		*fp;
	errno_t		err;
	png_infop	png_info;
	png_structp	png_ptr;

	err = fopen_s(&fp, filename.c_str(), "wb");
	if (err)
		return (false);


	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!png_ptr)
		return (false);

	png_info = png_create_info_struct(png_ptr);
	if (!png_info)
		return (false);

	if (setjmp(png_jmpbuf(png_ptr)))
		return (false);

	png_init_io(png_ptr, fp);


	if (setjmp(png_jmpbuf(png_ptr)))
		return (false);

	png_set_IHDR(png_ptr, png_info, _width, _height,
		_bitDepth, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	png_write_info(png_ptr, png_info);


	if (setjmp(png_jmpbuf(png_ptr)))
		return (false);

	png_write_image(png_ptr, (png_bytep*)_data);


	if (setjmp(png_jmpbuf(png_ptr)))
		return (false);

	png_write_end(png_ptr, NULL);

	for (int y = 0; y < _height; y++)
		free(((png_bytep*)_data)[y]);
	free(_data);

	fclose(fp);
	return (true);
}

bool			PngImageLoader::loadFromFile(const std::string &filename)
{
	FILE		*fp;
	errno_t		err;
	png_infop	png_info;
	png_structp	png_ptr;

	
	err = fopen_s(&fp, filename.c_str(), "rb");
	if (err)
	{
		std::cerr << "Could not open " << filename << std::endl;
		return (false);
	}

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
	{
		fclose(fp);
		std::cout << "Failed to create png read struct" << std::endl;
		return (false);
	}

	png_info = png_create_info_struct(png_ptr);
	if (!png_info)
	{
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		fclose(fp);
		std::cout << "Failed to create png info struct" << std::endl;
		return (false);
	}

	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &png_info, NULL);
		fclose(fp);
		return (false);
	}

	png_init_io(png_ptr, fp);

	png_read_info(png_ptr, png_info);

	_width = png_get_image_width(png_ptr, png_info);
	_height = png_get_image_height(png_ptr, png_info);
	_bitDepth = png_get_bit_depth(png_ptr, png_info);
	setColorType(png_get_color_type(png_ptr, png_info), png_ptr);

	if (_rgbFormat == ERGBFormat::Type::GRAY ||
		_rgbFormat == ERGBFormat::Type::GRAY_ALPHA)
		png_set_gray_to_rgb(png_ptr);

	if (!removeAlphaChannel(fp, png_info, png_ptr))
		return (false);

	_data = (png_bytep*)malloc(sizeof(png_bytep) * _height);
	_length = png_get_rowbytes(png_ptr, png_info) * _height;

	for (int y = 0; y < _height; y++)
		((png_bytep*)_data)[y] = (png_byte*)malloc(png_get_rowbytes(png_ptr, png_info));

	png_read_image(png_ptr, (png_bytep*)_data);

	fclose(fp);
	return (true);
}

void			PngImageLoader::setColorType(png_byte colorType, png_structp &png_ptr)
{
	switch (colorType)
	{
	case PNG_COLOR_TYPE_GRAY:
		_rgbFormat = ERGBFormat::Type::GRAY;
		break;
	case PNG_COLOR_TYPE_GRAY_ALPHA:
		_rgbFormat = ERGBFormat::Type::GRAY_ALPHA;
		break;
	case PNG_COLOR_TYPE_PALETTE:
		png_set_palette_to_rgb(png_ptr);
		_rgbFormat = ERGBFormat::Type::RGB;
		break;
	case PNG_COLOR_TYPE_RGB:
		_rgbFormat = ERGBFormat::Type::RGB;
		break;
	case PNG_COLOR_TYPE_RGBA:
		_rgbFormat = ERGBFormat::Type::RGBA;
		break;
	default:
		break;
	}
}

bool			PngImageLoader::removeAlphaChannel(FILE *fp, png_infop &png_info, png_structp &png_ptr)
{
	png_set_strip_alpha(png_ptr);

	png_read_update_info(png_ptr, png_info);

	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &png_info, NULL);
		fclose(fp);
		return (false);
	}
	return (true);
}