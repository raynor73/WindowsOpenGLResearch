#pragma once

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <sstream>
#include <game_engine/game_object_component.h>
#include <game_engine/without_generated_methods.h>

namespace GameEngine
{
class GameObject : public WithoutGeneratedMethods, public std::enable_shared_from_this<GameObject>
{
    bool m_isEnabled;
    std::string m_name;

    std::weak_ptr<GameObject> m_parent;
    std::unordered_map<std::string, std::shared_ptr<GameObject>> m_children;
    std::unordered_map<std::string, std::shared_ptr<GameObjectComponent>> m_components;

    static int s_cloneNamePostfix;

public:
    static std::shared_ptr<GameObject> create(const std::string &name) { return std::shared_ptr<GameObject>(new GameObject(name, true)); }
    static std::shared_ptr<GameObject> create(const std::string &name, bool isEnabled) { return std::shared_ptr<GameObject>(new GameObject(name, isEnabled)); }

    virtual ~GameObject();

    const std::string& name() const { return m_name; }

    bool isEnabled() const { return m_isEnabled; }
    void setEnabled(bool isEnabled) { m_isEnabled = isEnabled; }

    std::weak_ptr<GameObject> parent() { return m_parent; }
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

    std::shared_ptr<GameObject> clone();
    std::shared_ptr<GameObject> clone(const std::string& cloneName);

private:
    GameObject(std::string name, bool isEnabled) : m_name(std::move(name)), m_isEnabled(true) {}

    static int nextCloneNamePostfix();
};
}
