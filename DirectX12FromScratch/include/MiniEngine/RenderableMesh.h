#pragma once

# include "MiniEngine/Mesh.h"
# include "MiniEngine/RenderableObject.h"
# include "MiniEngine/MovableObject.h"

namespace MiniEngine
{
    class RenderableMesh : public Mesh, public RenderableObject, public MovableObject
    {
    public:
        RenderableMesh();
        virtual ~RenderableMesh();
    };
}