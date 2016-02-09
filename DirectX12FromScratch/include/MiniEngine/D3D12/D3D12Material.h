#pragma once

#include "MiniEngine/Material.h"
#include "MiniEngine/D3D12/D3D12ConstantBuffer.h"

namespace MiniEngine
{
    class D3D12RenderSystem;

    class D3D12Material : public Material
    {
    public:
        D3D12Material(D3D12RenderSystem &system);
        ~D3D12Material();

        bool                bind(CommandList &list, unsigned int rootIdx);
        bool                finalize();

    protected:
        void                padSize(size_t &size, size_t sizeData);
        void                pad(size_t size, size_t &cursor, char *materialData, void *data, size_t sizeData);

        D3D12RenderSystem   &_system;
        D3D12ConstantBuffer *_material;
    };
}