#include <algorithm>
#include "Scene.h"
#include <algorithm>

using namespace dae;

void Scene::SetLoadFunc(std::function<void()> func)
{
	m_loadFunc = func;
}

void Scene::Load()
{
	m_loadFunc();
}

void Scene::UnLoad()
{
	m_objects.clear();
	m_observers.clear();
}

void Scene::Add(std::unique_ptr<GameObject> object)
{
	assert(object != nullptr && "Cannot add a null GameObject to the scene.");
	m_objects.emplace_back(std::move(object));
}

void Scene::Add(std::unique_ptr<IObserver> observer)
{
	assert(observer != nullptr && "Cannot add a null GameObject to the scene.");
	m_observers.emplace_back(std::move(observer));
}

void Scene::RemoveMarkedForRemoval()
{
	m_objects.erase(
		std::remove_if(
			m_objects.begin(),
			m_objects.end(),
			[](const auto& ptr) { return ptr.get()->GetIsMarkedForRemoval(); }
		),
		m_objects.end()
	);
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void Scene::Update()
{
	for(auto& object : m_objects)
	{
		object->Update();
	}

	RemoveMarkedForRemoval();
}

void Scene::LateUpdate()
{
	for (auto& object : m_objects)
	{
		object->LateUpdate();
	}

	RemoveMarkedForRemoval();
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

std::unique_ptr<GameObject> dae::Scene::GetGameObjectOwnership(GameObject* pObject)
{
	// get object
	auto it = std::find_if(m_objects.begin(), m_objects.end(), [pObject](auto& obj) { return obj.get() == pObject; });

	// release ownership
	std::unique_ptr<GameObject> object = std::move(*it);
	*it = nullptr;

	// erase empty slot
	m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), nullptr));

	// give ownership
	return std::move(object);
}
