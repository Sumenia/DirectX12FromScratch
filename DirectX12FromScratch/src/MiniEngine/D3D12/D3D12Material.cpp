#include <DirectXMath.h>
#include "MiniEngine/D3D12/D3D12Material.h"
#include "MiniEngine/D3D12/D3D12RenderSystem.h"

using namespace MiniEngine;

D3D12Material::D3D12Material(D3D12RenderSystem &system) : _system(system), _material(nullptr)
{}

D3D12Material::~D3D12Material()
{
    delete _material;
    _material = nullptr;
}

bool D3D12Material::bind(CommandList &list, unsigned int rootIdx)
{
    if (!_material)
        return (false);

    return (_material->bind(list, rootIdx));
}

bool D3D12Material::finalize()
{
    DWORD32 id = _id;
    size_t  cursor = 0;
    size_t  size = sizeof(id);
    char    *materialData = nullptr;

    // Get size of material struct
    if (_flags & Material::UNIFORM_COLOR)
        size += sizeof(DirectX::XMFLOAT3);

    // Allocate material structure
    materialData = new char[size];

    memcpy(materialData + cursor, &id, sizeof(id));
    cursor += sizeof(id);

    // Fill material structure
    if (_flags & Material::UNIFORM_COLOR)
    {
        DirectX::XMFLOAT3 color(_color.x, _color.y, _color.z);

        memcpy(materialData + cursor, &color, sizeof(color));
        cursor += sizeof(color);
    }

    // Create constant buffer
    _material = new D3D12ConstantBuffer(_system);

    if (!_material->init(size))
    {
        delete[] materialData;
        return (false);
    }

    if (!_material->update(size, materialData))
    {
        delete[] materialData;
        return (false);
    }

    delete[] materialData;
    return (true);
}