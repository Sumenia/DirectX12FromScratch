#pragma once

#include <d3d12.h>
#include "MiniEngine/Texture.h"

namespace MiniEngine
{
	class D3D12RenderSystem;

	class D3D12Texture
	{
	public:
		D3D12Texture(D3D12RenderSystem &system);
		~D3D12Texture();

		bool				init(void *data, unsigned int width, unsigned int height);
		ID3D12Resource		*getBuffer();
		D3D12_RESOURCE_DESC &getResourceDesc();

	protected:
		D3D12RenderSystem		&_system;
		ID3D12Resource			*_buffer;

		D3D12_RESOURCE_DESC					_textureDesc;
	};
}