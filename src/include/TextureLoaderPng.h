#ifndef CLASS_NCINE_TEXTURELOADERPNG
#define CLASS_NCINE_TEXTURELOADERPNG

#include <png.h>
#include "ITextureLoader.h"

namespace ncine {

/// PNG texture loader
class TextureLoaderPng : public ITextureLoader
{
  public:
	explicit TextureLoaderPng(const char *filename);
	explicit TextureLoaderPng(IFile *fileHandle);

  private:
	void init() override;
	static void readFromFileHandle(png_structp pngPtr, png_bytep outBytes, png_size_t byteCountToRead);
};

}

#endif
