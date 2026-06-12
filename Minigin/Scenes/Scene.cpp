#include <algorithm>
#include "Scene.h"

using namespace dae;

Scene::Scene(SceneName sceneName, std::function<void(Scene&)> loadFunc)
	: m_sceneName(sceneName), m_loadFunc(loadFunc)
{ }

void Scene::Load()
{
	m_loadFunc(*this);
	for (auto& object : m_objects)
	{
		object->Start();
	}
}

void Scene::UnLoad()
{
	m_objects.clear();
}

void Scene::Add(std::unique_ptr<GameObject> object)
{
	assert(object != nullptr && "Cannot add a null GameObject to the scene.");
	assert((std::find_if(m_objects.begin(), m_objects.end(),
		[&object] (auto& obj) { return obj.get() == object.get(); })
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
		m_invalidVec = true;
	}
}

void Scene::RemoveMarkedForRemoval()
{
	m_objects.erase(
		std::remove_if(
			m_objects.begin(), 
			m_objects.end(), 
			[](const auto& ptr) { return ptr == nullptr; }), 
		m_objects.end());

	m_objects.erase(
		std::remove_if(
			m_objects.begin(),
			m_objects.end(),
			[](const std::unique_ptr<GameObject>& ptr) { return ptr.get()->GetIsMarkedForRemoval(); }
		),
		m_objects.end()
	);
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void Scene::ReorderObjects()
{
	if (m_reorderObjects)
	{
		std::sort(m_objects.begin(), m_objects.end(),
			[](const std::unique_ptr<dae::GameObject>& obj1, const std::unique_ptr<dae::GameObject>& obj2)
			{ return obj1->GetRenderPriority() < obj2->GetRenderPriority(); });
	}
}

void Scene::Update()
{
	for(auto& object : m_objects)
	{
		if (object == nullptr || m_invalidVec)
			continue;
		object->Update();
	}

	m_invalidVec = false;

	RemoveMarkedForRemoval();
	ReorderObjects();
}

void Scene::LateUpdate()
{
	for (auto& object : m_objects)
	{
		if (object == nullptr)
			continue;
		object->LateUpdate();
	}

	RemoveMarkedForRemoval();
	ReorderObjects();
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		if (object == nullptr)
			continue;
		object->Render();
	}
}

std::vector<GameObject*> Scene::GetObjectsByTag(Tag tag) const
{
	std::vector<GameObject*> vec{};

	for (auto& object : m_objects)
	{
		if (object != nullptr && object->GetTag() == tag)
		{
			vec.emplace_back(object.get());
		}
		if (object != nullptr)
		{
			for (GameObject* child : object->GetChildrenByTag(tag))
			{
				vec.emplace_back(child);
			}
		}
	}

	return vec;
}

[[nodiscard]] std::unique_ptr<GameObject> Scene::GetGameObjectOwnership(GameObject* pObject)
{
	// get object
	auto it = std::find_if(m_objects.begin(), m_objects.end(), [pObject](auto& obj) { return obj.get() == pObject; });

	if (it != m_objects.end())
	{
		// grab ownership
		std::unique_ptr<GameObject> object = std::move(*it);
		*it = nullptr;

		// erase empty slot
		m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), *it));

		// give ownership
		return object;
	}
	else
	{
		return nullptr;
	}
}
