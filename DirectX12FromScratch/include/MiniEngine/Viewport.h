#pragma once

# include "MiniEngine/Geometry.h"
# include "MiniEngine/Camera.h"
# include "MiniEngine/CommandList.h"

namespace MiniEngine
{
    class Viewport
    {
    public:
        Viewport(Vector2f position, Vector2f size, Vector2f percent);
        ~Viewport();

        void            attachCamera(Camera *camera);

        bool            render(CommandList &commandList);

        Vector2f const  &getPosition() const;
        Vector2f const  &getSize() const;
		Vector2f const	&getPercent() const;

		void			setSize(Vector2f size);

    protected:
        Camera      *_camera;
        Vector2f    _position;
        Vector2f    _size;
		Vector2f	_percent;
    };
}