#include "opengl_textures_repository.h"

using namespace GameEngine;
using namespace GameEngine::RenderingEngine;
using namespace std;

const vector<float> OpenGLTexturesRepository::AVAILABLE_BITMAP_DENSITIES = {
        1, 2, 3, 4
};

void OpenGLTexturesRepository::createTexture(
    const string& name,
    uint32_t width,
    uint32_t height,
    const vector<uint8_t>& data
) {
    throwIfTextureAlreadyExists(name);

    GLuint texture;
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());

    m_textures[name] = TextureInfo{ texture, width, height };

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    m_openGLErrorDetector->checkOpenGLErrors("OpenGLTexturesRepository::createTexture from memory");
}

void OpenGLTexturesRepository::createDisplayDensityFactorAwareTexture(
    const string& name,
    const string& path
) {
    throwIfTextureAlreadyExists(name);

    stringstream ss;
    ss << buildDensityPathSegment() << path;
    auto bitmapInfo = m_bitmapLoader->loadBitmap(ss.str());

    GLuint texture;
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        bitmapInfo.width,
        bitmapInfo.height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        bitmapInfo.data.data()
    );

    m_textures[name] = TextureInfo{ texture, bitmapInfo.width, bitmapInfo.height };

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    m_openGLErrorDetector->checkOpenGLErrors("OpenGLTexturesRepository::createDisplayDensityFactorAwareTexture");
}

void OpenGLTexturesRepository::createTexture(const string& name, const string& path) {
    throwIfTextureAlreadyExists(name);

    auto bitmapInfo = m_bitmapLoader->loadBitmap(path);

    GLuint texture;
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        bitmapInfo.width,
        bitmapInfo.height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        bitmapInfo.data.data()
    );

    m_textures[name] = TextureInfo{ texture, bitmapInfo.width, bitmapInfo.height };

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    m_openGLErrorDetector->checkOpenGLErrors("OpenGLTexturesRepository::createTexture from file");
}

optional<TextureInfo> OpenGLTexturesRepository::findTexture(const string& name) {
    if (m_textures.count(name) > 0) {
        return m_textures[name];
    }
    else {
        return optional<TextureInfo>();
    }
}

void OpenGLTexturesRepository::removeTexture(const string& name) {
    if (m_textures.count(name) == 0) {
        stringstream ss;
        ss << "No texture " << name << " to remove";
        throw domain_error(ss.str());
    }

    auto textureInfo = m_textures[name];
    glDeleteTextures(1, &textureInfo.texture);
    m_textures.erase(name);

    m_openGLErrorDetector->checkOpenGLErrors("OpenGLTexturesRepository::removeTexture");
}

void OpenGLTexturesRepository::removeAllTextures() {
    vector<string> namesToDelete;

    for (auto& entry : m_textures) {
        namesToDelete.push_back(entry.first);
    }
    for (auto& name : namesToDelete) {
        removeTexture(name);
    }
}

string OpenGLTexturesRepository::buildDensityPathSegment() {
    stringstream ss;
    int roundedDensityFactor = int(ceil(m_renderingWindowInfoProvider->densityFactor()));

    for (auto availableBitmapDensity : AVAILABLE_BITMAP_DENSITIES) {
        if (availableBitmapDensity >= roundedDensityFactor) {
            ss << availableBitmapDensity << "x/";
            return ss.str();
        }
    }

    ss << AVAILABLE_BITMAP_DENSITIES.back() << "x/";
    return ss.str();
}
