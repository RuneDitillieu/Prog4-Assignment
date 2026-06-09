#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update()
{
	/*for(auto& scene : m_scenes)
	{
		scene->Update();
	}*/

	m_scenes[m_activeSceneIdx]->Update();
}

void dae::SceneManager::LateUpdate()
{
	/*for (auto& scene : m_scenes)
	{
		scene->LateUpdate();
	}*/

	m_scenes[m_activeSceneIdx]->LateUpdate();
}

void dae::SceneManager::Render()
{
	/*for (const auto& scene : m_scenes)
	{
		scene->Render();
	}*/

	m_scenes[m_activeSceneIdx]->Render();
}

dae::Scene& dae::SceneManager::CreateScene(std::function<void(Scene&)> loadFunc)
{
	m_scenes.emplace_back(new Scene(loadFunc));
	return *m_scenes.back();
}

dae::Scene* dae::SceneManager::GetActiveScene() const
{
	return m_scenes[m_activeSceneIdx].get();
}

void dae::SceneManager::SetActiveScene(size_t sceneIdx)
{
	if (m_scenes.size() <= sceneIdx) return;

	m_scenes[sceneIdx]->Load();
	m_scenes[m_activeSceneIdx]->UnLoad();
	m_activeSceneIdx = sceneIdx;
}
