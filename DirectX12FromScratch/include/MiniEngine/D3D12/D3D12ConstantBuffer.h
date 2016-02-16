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
        bool                initData(unsigned int size);

        bool                update(unsigned int size, void *data);
        bool                bind(CommandList &commandList, unsigned int rootIdx);

        bool                updateCameraMatrix(Vector3f const &position, Matrix4f const &view, Matrix4f const &projection, unsigned int nb_lights);
        bool                updateModelMatrix(Matrix4f const &model);
        bool                updateLights(std::list<std::shared_ptr<Light>> &lights);

        ID3D12Resource      *getNative();

	protected:
		D3D12RenderSystem	&_system;
		ID3D12Resource		**_constantBuffer;
        bool                _needUpdate[256];

        unsigned int        _nb;
        unsigned int        _size;

        unsigned int        _idx;
        void                *_data;
	};
};