#pragma once

#include <d3d12.h>
#include <DirectXMath.h>
#include "MiniEngine/ConstantBuffer.h"
#include "MiniEngine/D3D12/D3D12RenderWindow.h"

namespace MiniEngine
{
    class D3D12RenderSystem;

	class D3D12ConstantBuffer : virtual public ConstantBuffer
	{
	public:
		D3D12ConstantBuffer(D3D12RenderSystem&);
		~D3D12ConstantBuffer();

		bool                init(unsigned int size, unsigned int nb = D3D12RenderWindow::FrameCount);
        bool                initRessources(unsigned int size, unsigned int nb);

        bool                update(unsigned int size, void *data);
        bool                bind(CommandList &commandList, unsigned int rootIdx);

        bool                updateCameraMatrix(Matrix4f const &view, Matrix4f const &projection);
        bool                updateModelMatrix(Matrix4f const &model);

        ID3D12Resource      *getNative();

	protected:
		D3D12RenderSystem	&_system;
		ID3D12Resource		**_constantBuffer;
        bool                _needUpdate[256];

        unsigned int        _nb;
        unsigned int        _size;

        unsigned int        _idx;

    protected:
        struct              CameraMatrix
        {
            DirectX::XMFLOAT4X4 view;
            DirectX::XMFLOAT4X4 projection;
        }                   _camera;

        struct              ModelMatrix
        {
            DirectX::XMFLOAT4X4 model;
            DirectX::XMFLOAT3X3 modelNormal;
        }                   _model;

        enum Type
        {
            NONE,
            CAMERA,
            MODEL
        }                   _type;
	};
};