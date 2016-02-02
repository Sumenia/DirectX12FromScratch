#pragma once

#include <d3d12.h>
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

        bool                update(CommandList &commandList, unsigned int rootIdx, unsigned int size, void *data);

        ID3D12Resource      *getNative(unsigned int idx);

	protected:
		D3D12RenderSystem	&_system;
		ID3D12Resource		**_constantBuffer;

        unsigned int        _nb;
        unsigned int        _size;
	};
};