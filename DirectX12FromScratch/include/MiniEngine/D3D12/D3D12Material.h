#pragma once

#include "MiniEngine/Material.h"

namespace MiniEngine
{
    class D3D12RenderSystem;
	class D3D12ConstantBuffer;

    class D3D12Material : public Material
    {
    public:
        D3D12Material(D3D12RenderSystem &system);
        ~D3D12Material();

		bool				initCbvSrvDescriptorHeap();
        bool                bind(CommandList &list, unsigned int rootIdx);
        bool                finalize();

    protected:
        void                padSize(size_t &size, size_t sizeData);
        void                pad(size_t size, size_t &cursor, char *materialData, void *data, size_t sizeData);

        D3D12RenderSystem   &_system;
		D3D12CommandList	*_commandList;

		D3D12DescriptorHeap						*_cbvSrvDescHeap;
		D3D12ConstantBuffer						*_material;
		std::map<TextureType, ID3D12Resource*>	_texBuff;
		
    };
}