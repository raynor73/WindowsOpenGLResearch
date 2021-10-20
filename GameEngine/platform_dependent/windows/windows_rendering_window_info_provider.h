#pragma once

#include <game_engine/rendering_window_info_provider.h>

namespace GameEngine
{
class WindowsRenderingWindowInfoProvider : public RenderingWindowInfoProvider
{
public:
    explicit WindowsRenderingWindowInfoProvider(
        float initialWidth, 
        float initialHeight, 
        float initialDensityFactor
    ) : m_width(initialWidth), 
        m_height(initialHeight), 
        m_densityFactor(initialDensityFactor)
    {}


    virtual float width() const override { return m_width; }
    virtual float height() const override { return m_height; }
    virtual float densityFactor() const override { return m_densityFactor; }

private:
    float m_width;
    float m_height;
    float m_densityFactor;
};
}
