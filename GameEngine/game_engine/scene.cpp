#include <game_engine/service_locator.h>
#include "scene.h"

using namespace GameEngine;
using namespace std;

const string Scene::ROOT_GAME_OBJECT_NAME = "root";

Scene::Scene(shared_ptr<ServiceLocator> serviceLocator) :
    m_serviceLocator(std::move(serviceLocator)),
    m_rootGameObject(GameObject::create(ROOT_GAME_OBJECT_NAME))
{
    m_gameObjectsMap[m_rootGameObject->name()] = m_rootGameObject;
}

void Scene::update()
{
    m_serviceLocator->timeManager()->update();

    m_rootGameObject->update();

    /*if (m_appStateRepository->isAppInForeground()) {
        m_soundScene->setSoundListenerGain(1);
    }
    else {
        m_soundScene->setSoundListenerGain(0);
    }

    m_gesturesDispatcher->prepare();
    for (auto& event : m_touchScreen->events()) {
        m_gesturesDispatcher->onTouchEvent(event);
    }

    m_rootGameObject->update();*/

    float dt = m_serviceLocator->timeManager()->deltaTime();
    if (dt > 0) {
        m_serviceLocator->physicsEngine()->update(dt);
    }
}

void Scene::addGameObject(const std::string& parentName, const shared_ptr<GameObject>& gameObject) {
    if (m_gameObjectsMap.count(parentName) == 0) {
        std::stringstream ss;
        ss << "Parent game object " << parentName << " not found";
        throw std::domain_error(ss.str());
    }

    if (m_gameObjectsMap.count(gameObject->name()) > 0) {
        std::stringstream ss;
        ss << "Game object " << gameObject->name() << " already exists";
        throw std::domain_error(ss.str());
    }

    m_gameObjectsMap[parentName]->addChild(gameObject);
    m_gameObjectsMap[gameObject->name()] = gameObject;
}

void Scene::removeGameObject(const std::string& name) {
    if (m_gameObjectsMap.count(name) == 0) {
        std::stringstream ss;
        ss << "Game object " << name << " not found";
        throw std::domain_error(ss.str());
    }

    auto gameObject = m_gameObjectsMap[name];
    m_gameObjectsMap.erase(name);

    gameObject->parent().lock()->removeChild(gameObject);
}
