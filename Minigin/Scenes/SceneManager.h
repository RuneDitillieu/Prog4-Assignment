#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <vector>
#include <string>
#include <memory>
#include "Scene.h"
#include "Singleton.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(std::function<void(Scene&)> loadFunc);

		Scene* GetActiveScene() const;
		void SetActiveScene(int sceneIdx);

		void Update();
		void LateUpdate();
		void Render();

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;

		int m_activeSceneIdx{ 0 };
		std::vector<std::unique_ptr<Scene>> m_scenes{};
	};
}

#endif // !SCENEMANAGER_H