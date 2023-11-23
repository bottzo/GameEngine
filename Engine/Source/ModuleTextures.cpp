#include "ModuleTextures.h"
#include "DirectXTex.h"
#include "Files.h"
#include "GL/glew.h"

unsigned int ModuleTextures::LoadTexture(const char* path)
{
	char* buffer;
	size_t size = FileToBuffer(path, &buffer);
	if (size == 0)
		return 0;
	DirectX::TexMetadata meta;
	DirectX::ScratchImage sImage;
	HRESULT hr = DirectX::LoadFromDDSMemory(buffer, size, DirectX::DDS_FLAGS::DDS_FLAGS_NONE, &meta, sImage);
	if (!SUCCEEDED(hr))
	{
		hr = DirectX::LoadFromTGAMemory(buffer, size, &meta, sImage);
		if (!SUCCEEDED(hr))
		{
			hr = DirectX::LoadFromWICMemory(buffer, size, DirectX::WIC_FLAGS::WIC_FLAGS_NONE, &meta, sImage);
			if (!SUCCEEDED(hr))
			{
				LOG("Error: Could not load the texture from the file");
				return 0;
			}
		}
	}

	int internalFormat;
	int format;
	int type;

	switch (meta.format)
	{
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
		internalFormat = GL_RGBA8;
		format = GL_RGBA;
		type = GL_UNSIGNED_BYTE;
		break;
	case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
	case DXGI_FORMAT_B8G8R8A8_UNORM:
		internalFormat = GL_RGBA8;
		format = GL_BGRA;
		type = GL_UNSIGNED_BYTE;
		break;
	case DXGI_FORMAT_B5G6R5_UNORM:
		internalFormat = GL_RGB8;
		format = GL_BGR;
		type = GL_UNSIGNED_BYTE;
		break;
	default:
		assert(false && "Unsupported format");
	}

	unsigned int texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	for (size_t i = 0; i < meta.mipLevels; ++i)
	{
		const DirectX::Image* mip = sImage.GetImage(i, 0, 0);
		glTexImage2D(GL_TEXTURE_2D, i, internalFormat, meta.width, meta.height, 0, format, type, sImage.GetPixels());
	}

	//TODO: Mipmaps usually not generated when loading the texture??
	if (meta.mipLevels == 1)
		glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, meta.mipLevels - 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
	return texId;
}