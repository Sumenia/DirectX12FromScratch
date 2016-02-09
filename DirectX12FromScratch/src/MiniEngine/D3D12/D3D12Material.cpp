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
    size_t  size = 0;
    char    *materialData = nullptr;

    // Get size of material struct
    padSize(size, sizeof(id));

    if (!(_flags & Material::TEXTURE_AMBIENT))
        padSize(size, sizeof(DirectX::XMFLOAT3));

    if (!(_flags & Material::NORMAL_COLOR) && !(_flags & Material::TEXTURE_DIFFUSE))
        padSize(size, sizeof(DirectX::XMFLOAT3));

    if (!(_flags & Material::TEXTURE_SPECULAR))
        padSize(size, sizeof(DirectX::XMFLOAT3));

    padSize(size, sizeof(float));

    // Allocate material structure
    materialData = new char[size];

    // Fill material structure
    pad(size, cursor, materialData, &id, sizeof(id));

    if (!(_flags & Material::TEXTURE_AMBIENT))
    {
        DirectX::XMFLOAT3 color(_ka.x, _ka.y, _ka.z);
        pad(size, cursor, materialData, &color, sizeof(color));
    }

    if (!(_flags & Material::NORMAL_COLOR) && !(_flags & Material::TEXTURE_DIFFUSE))
    {
        DirectX::XMFLOAT3 color(_kd.x, _kd.y, _kd.z);
        pad(size, cursor, materialData, &color, sizeof(color));
    }

    if (!(_flags & Material::TEXTURE_SPECULAR))
    {
        DirectX::XMFLOAT3 color(_ks.x, _ks.y, _ks.z);
        pad(size, cursor, materialData, &color, sizeof(color));
    }

    pad(size, cursor, materialData, &_shininess, sizeof(float));

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

void D3D12Material::padSize(size_t &size, size_t sizeData)
{
    if ((size + sizeData) % 16 == 0 || size / 16 == (size + sizeData) / 16)
    {
        size += sizeData;
        return;
    }

    size = ((size + 16) / 16) * 16;
    size += sizeData;
}

void D3D12Material::pad(size_t size, size_t &cursor, char *materialData, void *data, size_t sizeData)
{
    if ((cursor + sizeData) % 16 != 0 && cursor / 16 != (cursor + sizeData) / 16)
        cursor = ((cursor + 16) / 16) * 16;

    memcpy(materialData + cursor, data, sizeData);
    cursor += sizeData;
}