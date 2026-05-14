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
}

void Scene::Add(std::unique_ptr<GameObject> object)
{
	assert(object != nullptr && "Cannot add a null GameObject to the scene.");
	assert((std::find_if(m_objects.begin(), m_objects.end(), [&object] (auto& obj) { return obj.get() == object.get(); }) 
		== m_objects.end()) && "Cannot add a GameObject twice.");

	if (object->GetParent())
	{
		// if an object already has a parent, then it's already owned by another object
		// so in case you then try to add it to the scene (bc main.cpp also has a unique_ptr),
		// you can safely release it
		object.release();
	}
	else
	{
		m_objects.emplace_back(std::move(object));
	}
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

[[nodiscard]] GameObject* dae::Scene::GetGameObjectOwnership(GameObject* pObject)
{
	// get object
	auto it = std::find_if(m_objects.begin(), m_objects.end(), [pObject](auto& obj) { return obj.get() == pObject; });

	if (it != m_objects.end())
	{
		// release ownership
		GameObject* object = it->release();
		*it = nullptr;

		// erase empty slot
		m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), nullptr));

		// give ownership
		return object;
	}
	else
	{
		return nullptr;
	}
}
