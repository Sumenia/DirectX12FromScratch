#pragma once

# include "MiniEngine/GraphicPipeline.h"
# include "MiniEngine/Material.h"
# include "MiniEngine/Geometry.h"

namespace MiniEngine
{
    class RenderSystem;
    class RenderTarget;
    class ConstantBuffer;

    class CommandList
    {
    public:
        CommandList(RenderSystem &system, RenderTarget *target);
        virtual ~CommandList();

        virtual bool        init() = 0;
        virtual bool        reset() = 0;

        virtual bool        begin() = 0;
        virtual bool        end() = 0;

        virtual bool        bindCameraCBV(ConstantBuffer &buffer) = 0;
        virtual bool        bindModelCBV(ConstantBuffer &buffer) = 0;
        virtual bool        bindLightsCBV(ConstantBuffer &buffer) = 0;

        virtual bool        bindMaterial(Material &material) = 0;
        virtual bool        bindMaterial(unsigned int id) = 0;
        virtual bool        setMaterialPipeline(DWORD64 type);

        virtual bool        setPipeline(GraphicPipeline &pipeline) = 0;

        RenderTarget        &getRenderTarget();
        RenderSystem        &getRenderSystem();

    protected:
        RenderSystem        &_system;
        RenderTarget        *_target;

        unsigned int        _previousMaterialId;
        DWORD64             _previousMaterialType;
    };
}