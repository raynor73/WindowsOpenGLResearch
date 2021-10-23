#pragma once

#include <optional>
#include <unordered_map>
#include <string>
#include <memory>
#include <game_engine/textures_repository.h>
#include <game_engine/without_generated_methods.h>
#include <rendering_engine/texture_info.h>
#include <game_engine/rendering_window_info_provider.h>
#include <game_engine/bitmap_loader.h>
#include <rendering_engine/opengl_error_detector.h>
#include <sstream>

namespace GameEngine
{
namespace RenderingEngine
{
class OpenGLTexturesRepository : public TexturesRepository, public WithoutGeneratedMethods
{
    std::unordered_map<std::string, TextureInfo> m_textures;

    RenderingWindowInfoProvider* m_renderingWindowInfoProvider;
    BitmapLoader* m_bitmapLoader;
    std::shared_ptr<OpenGLErrorDetector> m_openGLErrorDetector;

public:
    OpenGLTexturesRepository(
        RenderingWindowInfoProvider* renderingWindowInfoProvider,
        BitmapLoader* bitmapLoader,
        std::shared_ptr<OpenGLErrorDetector> openGLErrorDetector
    ) : m_renderingWindowInfoProvider(renderingWindowInfoProvider),
        m_bitmapLoader(bitmapLoader),
        m_openGLErrorDetector(openGLErrorDetector) 
    {}

    virtual void createTexture(const std::string& name, uint32_t width, uint32_t height, const std::vector<uint8_t>& data) override;
    virtual void createDisplayDensityFactorAwareTexture(const std::string& name, const std::string& path) override;
    virtual void createTexture(const std::string& name, const std::string& path) override;
    virtual void removeTexture(const std::string& name) override;
    virtual void removeAllTextures() override;

    std::optional<TextureInfo> findTexture(const std::string& name);

private:
    inline void throwIfTextureAlreadyExists(const std::string& name) {
        if (m_textures.count(name) > 0) {
            std::stringstream ss;
            ss << "Texture " << name << " already exists";
            throw std::domain_error(ss.str());
        }
    }

    std::string buildDensityPathSegment();

    static const std::vector<float> AVAILABLE_BITMAP_DENSITIES;
};
}
}
