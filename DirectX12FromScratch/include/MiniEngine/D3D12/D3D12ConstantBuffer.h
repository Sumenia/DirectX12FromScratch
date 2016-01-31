#pragma once

#include <d3d12.h>
#include "MiniEngine/ConstantBuffer.h"
#include "MiniEngine/D3D12/D3D12RenderSystem.h"

namespace MiniEngine
{
	class D3D12ConstantBuffer : virtual public ConstantBuffer
	{
	public:
		D3D12ConstantBuffer(D3D12RenderSystem&);
		~D3D12ConstantBuffer();

		bool                init(unsigned int size, unsigned int nb = D3D12RenderWindow::FrameCount);
        bool                initHeap(unsigned int nb);
        bool                initRessources(unsigned int size, unsigned int nb);
        bool                initView(unsigned int size, unsigned int nb);

        void                update(CommandList &commandList, unsigned int size, void *data);

        D3D12DescriptorHeap *getHeap();

	protected:
		D3D12RenderSystem	&_system;
		ID3D12Resource		*_constantBuffer;
        ID3D12Resource		*_constantBufferUpload;
        D3D12DescriptorHeap *_heap;
	};
};