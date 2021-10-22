#include "opengl_geometry_buffers_storage.h"
#include <sstream>
#include <exception>
#include <utility>

using namespace GameEngine::RenderingEngine;
using namespace std;

optional<GLuint> OpenGLGeometryBuffersStorage::findVbo(const string& name) {
    if (m_vbos.count(name) == 0) {
        return optional<GLuint>();
    } else {
        return m_vbos[name];
    }
}

optional<IboInfo> OpenGLGeometryBuffersStorage::findIbo(const string& name) {
    if (m_ibos.count(name) == 0) {
        return optional<IboInfo>();
    } else {
        return m_ibos[name];
    }
}

GLuint OpenGLGeometryBuffersStorage::createStaticVertexBuffer(
    const string& name,
    const std::vector<float>& vertexData
) {
    if (m_vbos.count(name) > 0) {
        stringstream ss;
        ss << "VBO " << name << " already exists";
        throw domain_error(ss.str());
    }

    GLuint buffer;
    glGenBuffers(1, &buffer);

    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    glBufferData(
        GL_ARRAY_BUFFER,
        vertexData.size() * sizeof(float),
        vertexData.data(),
        GL_STATIC_DRAW
    );

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    m_vbos[name] = buffer;

    m_openGLErrorDetector->checkOpenGLErrors("createStaticVertexBuffer");

    return buffer;
}

IboInfo OpenGLGeometryBuffersStorage::createStaticIndexBuffer(
    const string& name,
    const std::vector<uint16_t>& indices
) {
    if (m_ibos.count(name) > 0) {
        stringstream ss;
        ss << "IBO " << name << " already exists";
        throw domain_error(ss.str());
    }

    GLuint buffer;
    glGenBuffers(1, &buffer);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(uint16_t),
        indices.data(),
        GL_STATIC_DRAW
    );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    auto iboInfo = IboInfo{ buffer, indices.size() };
    m_ibos[name] = iboInfo;

    m_openGLErrorDetector->checkOpenGLErrors("createStaticIndexBuffer");

    return iboInfo;
}

void OpenGLGeometryBuffersStorage::removeStaticVertexBuffer(const string& name) {
    if (m_vbos.count(name) == 0) {
        stringstream ss;
        ss << "No vertex buffer " << name << " to remove";
        throw domain_error(ss.str());
    }

    auto buffer = m_vbos[name];
    glDeleteBuffers(1, &buffer);
    m_vbos.erase(name);

    m_openGLErrorDetector->checkOpenGLErrors("removeStaticVertexBuffer");
}

void OpenGLGeometryBuffersStorage::removeStaticIndexBuffer(const string& name) {
    if (m_ibos.count(name) == 0) {
        stringstream ss;
        ss << "No index buffer " << name << " to remove";
        throw domain_error(ss.str());
    }

    auto bufferInfo = m_ibos[name];
    glDeleteBuffers(1, &bufferInfo.ibo);
    m_ibos.erase(name);

    m_openGLErrorDetector->checkOpenGLErrors("removeStaticIndexBuffer");
}

void OpenGLGeometryBuffersStorage::removeAllBuffers() {
    vector<string> namesToDelete;

    for (auto& entry : m_vbos) {
        namesToDelete.push_back(entry.first);
    }
    for (auto& name : namesToDelete) {
        removeStaticVertexBuffer(name);
    }

    namesToDelete.clear();
    for (auto& entry : m_ibos) {
        namesToDelete.push_back(entry.first);
    }
    for (auto& name : namesToDelete) {
        removeStaticIndexBuffer(name);
    }
}

vector<string> OpenGLGeometryBuffersStorage::retrieveStoredVboNames() const {
    auto names = vector<string>(m_vbos.size());

    for (auto& entry : m_vbos) {
        names.emplace_back(entry.first);
    }

    return names;
}

vector<string> OpenGLGeometryBuffersStorage::retrieveStoredIboNames() const {
    auto names = vector<string>(m_ibos.size());

    for (auto& entry : m_ibos) {
        names.emplace_back(entry.first);
    }

    return names;
}
