#include "ModuleTextures.h"
#include "DirectXTex.h"
#include "Files.h"

void* ModuleTextures::LoadTexture(const char* path)
{
	char* buffer;
	size_t size = FileToBuffer(path, &buffer);
	if (size == 0)
		return nullptr;
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
				return nullptr;
			}
		}
	}
}