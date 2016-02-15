#pragma once

# include "ImageLoader.h"
# include "libPNG/lpng1621/png.h"

class PngImageLoader : public ImageLoader
{
public:
	PngImageLoader();
	~PngImageLoader();
	virtual bool	loadFromFile(FILE *file);
	static bool		isPng(FILE *file);

private:
	bool			writeToFile(const std::string &filename);
	void			setColorType(png_byte colorType, png_structp &png_ptr);
	bool			removeAlphaChannel(FILE *fp, png_infop &png_info, png_structp &png_ptr);
};