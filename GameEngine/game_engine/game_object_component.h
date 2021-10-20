#pragma once

#include <sstream>
#include <stdexcept>
#include <memory>
#include <game_engine/without_generated_methods.h>

namespace GameEngine
{
class GameObject;

class GameObjectComponent : public WithoutGeneratedMethods
{
protected:
    bool m_isEnabled;
    std::weak_ptr<GameObject> m_gameObject;

    void throwErrorIfNoGameObject() const {
        if (m_gameObject.use_count() == 0) {
            std::stringstream ss;
            ss << "No game object for component of type " << typeName();
            throw std::domain_error(ss.str());
        }
    }

public:
    GameObjectComponent() : GameObjectComponent(true) {}
    GameObjectComponent(bool isEnabled) : m_isEnabled(isEnabled) {}
    GameObjectComponent(const GameObjectComponent&) = delete;
    GameObjectComponent(GameObjectComponent&&) = delete;
    virtual ~GameObjectComponent() = default;

    bool isEnabled() const { return m_isEnabled; }
    virtual void setEnabled(bool isEnabled) { m_isEnabled = isEnabled; }

    virtual const std::string& typeName() const = 0;

    std::weak_ptr<GameObject> gameObject() { return m_gameObject; }
    void setGameObject(std::shared_ptr<GameObject> gameObject) { m_gameObject = gameObject; }
    void resetGameObject() { m_gameObject.reset(); }

    virtual void update() {};
    virtual void onAttachedToGameObject() {};
    virtual void onDetachedFromGameObject() {};
    virtual void onGameObjectAttachedToParent() {};
    virtual void onGameObjectDetachedFromParent() {};

    GameObjectComponent& operator=(const GameObjectComponent&) = delete;
    GameObjectComponent& operator=(GameObjectComponent&&) = delete;
    virtual std::shared_ptr<GameObjectComponent> clone() = 0;
};
}
