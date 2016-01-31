#include "MiniEngine/RenderTarget.h"

using namespace MiniEngine;

RenderTarget::RenderTarget(RenderSystem &system) : _system(system), _clearColor{{0}}, _cameraConstantBuffer(nullptr), _modelConstantBuffer(nullptr)
{}

RenderTarget::~RenderTarget()
{
    delete _cameraConstantBuffer;
    _cameraConstantBuffer = nullptr;

    delete _modelConstantBuffer;
    _modelConstantBuffer = nullptr;

    while (_viewports.size())
    {
        delete _viewports.front();
        _viewports.pop_front();
    }
}

void RenderTarget::addViewport(Viewport *viewport)
{
    _viewports.push_back(viewport);
}

void RenderTarget::setClearColor(const float color[4])
{
    _clearColor[0] = color[0];
    _clearColor[1] = color[1];
    _clearColor[2] = color[2];
    _clearColor[3] = color[3];
}

ConstantBuffer *RenderTarget::getCameraBuffer()
{
    return (_cameraConstantBuffer);
}

ConstantBuffer *RenderTarget::getModelBuffer()
{
    return (_modelConstantBuffer);
}

unsigned int RenderTarget::getFrameIdx()
{
    return (_frameIdx);
}