#pragma once

#include <glm/glm.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <string>
#include <game_engine/game_object_component.h>

namespace GameEngine
{
class TransformationComponent : public GameObjectComponent {

    bool m_isDirty = true;

    glm::vec3 m_position;
    glm::quat m_rotation;
    glm::vec3 m_scale;

    glm::vec3 m_finalPosition;
    glm::quat m_finalRotation;
    glm::vec3 m_finalScale;

    void setDirty();
    void calculateFinalTransformation();

public:
    static const std::string TYPE_NAME;

    explicit TransformationComponent(
        const glm::vec3& position,
        const glm::quat& rotation,
        const glm::vec3& scale
    ) : m_position(position), 
        m_rotation(rotation),
        m_scale(scale),
        m_finalPosition(0),
        m_finalRotation(glm::identity<glm::quat>()),
        m_finalScale(0)
    {}

    virtual const std::string& typeName() const override { return TransformationComponent::TYPE_NAME; }

    const glm::vec3& localPosition() const { return m_position; }
    const glm::quat& localRotation() const { return m_rotation; }
    const glm::vec3& localScale() const { return m_scale; }

    const glm::vec3& position();
    void setPosition(const glm::vec3& position);
    const glm::quat& rotation();
    void setRotation(const glm::quat& rotation);
    const glm::vec3& scale();
    void setScale(const glm::vec3& scale);

    virtual std::shared_ptr<GameObjectComponent> clone() override;
};
}
