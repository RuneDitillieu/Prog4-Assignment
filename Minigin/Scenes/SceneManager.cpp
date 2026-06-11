#include "SceneManager.h"

#include "InputManager.h"
#include "Scene.h"

void dae::SceneManager::Update()
{
	m_scenes[m_activeSceneIdx]->Update();
}

void dae::SceneManager::LateUpdate()
{
	m_scenes[m_activeSceneIdx]->LateUpdate();
}

void dae::SceneManager::Render()
{
	m_scenes[m_activeSceneIdx]->Render();
}

dae::Scene& dae::SceneManager::CreateScene(SceneName sceneName, std::function<void(Scene&)> loadFunc)
{
	m_scenes.emplace_back(std::make_unique<Scene>(sceneName, loadFunc));
	return *m_scenes.back();
}

dae::Scene* dae::SceneManager::GetActiveScene() const
{
	return m_scenes[m_activeSceneIdx].get();
}

void dae::SceneManager::SetActiveScene(SceneName sceneName)
{
	size_t sceneIdx{ 0 };

	for (auto& scene : m_scenes)
	{
		if (scene->GetSceneName() == sceneName)
		{
			break;
		}
		++sceneIdx;
	}

	if (m_scenes.size() <= sceneIdx) return;

	size_t prevScene = m_activeSceneIdx;
	m_activeSceneIdx = sceneIdx;

	dae::InputManager::GetInstance().ClearCommands();
	m_scenes[m_activeSceneIdx]->Load();
	m_scenes[prevScene]->UnLoad();
}
