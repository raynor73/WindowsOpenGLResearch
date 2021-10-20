#pragma once

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <sstream>
#include <game_engine/game_object_component.h>

namespace GameEngine
{
class GameObject
{
    bool m_isEnabled;
    std::string m_name;

    GameObject* m_parent = nullptr;
    std::unordered_map<std::string, std::shared_ptr<GameObject>> m_children;
    std::unordered_map<std::string, std::shared_ptr<GameObjectComponent>> m_components;

    static int s_cloneNamePostfix;

public:
    explicit GameObject(std::string name);
    GameObject(const GameObject&) = delete;
    GameObject(GameObject&&) = delete;
    virtual ~GameObject();

    const std::string& name() const { return m_name; }

    bool isEnabled() const { return m_isEnabled; }
    void setEnabled(bool isEnabled) { m_isEnabled = isEnabled; }

    GameObject* parent() { return m_parent; }
    const std::unordered_map<std::string, std::shared_ptr<GameObject>>& children() const { return m_children; }

    void addChild(const std::shared_ptr<GameObject>& child);
    void removeChild(const std::shared_ptr<GameObject>& child);

    void addComponent(const std::shared_ptr<GameObjectComponent>& component);
    void removeComponent(const std::shared_ptr<GameObjectComponent>& component);

    void update();
    void onAttachedToParent();
    void onDetachedFromParent();

    std::shared_ptr<GameObjectComponent> findComponent(const std::string& name);
    template<typename T>
    std::shared_ptr<T> findComponent() {
        if (m_components.count(T::TYPE_NAME) > 0) {
            return std::static_pointer_cast<T>(m_components.at(T::TYPE_NAME));
        }
        else {
            return nullptr;
        }
    }

    GameObject& operator=(const GameObject&) = delete;
    GameObject& operator=(GameObject&&) = delete;
    std::shared_ptr<GameObject> clone();
    std::shared_ptr<GameObject> clone(const std::string& cloneName);

private:
    static int nextCloneNamePostfix();
};
}
