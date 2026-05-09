#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <vector>
#include "GameObject.h"
#include "Observers.h"
#include <functional>

namespace dae
{
	class Scene final
	{
	public:
		void SetLoadFunc(std::function<void()> func);
		void Load();
		void UnLoad();

		void Add(std::unique_ptr<GameObject> object);
		void Add(std::unique_ptr<IObserver> observer);
		void RemoveMarkedForRemoval();
		void RemoveAll();

		void Update();
		void LateUpdate();
		void Render() const;

		[[nodiscard]] GameObject* GetGameObjectOwnership(GameObject* pObject);

		~Scene() = default;
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private:
		friend class SceneManager;
		explicit Scene() = default;

		std::function<void()> m_loadFunc{};
		std::vector<std::unique_ptr<GameObject>> m_objects{};
		std::vector<std::unique_ptr<IObserver>> m_observers{};
	};
}

#endif // !SCENE_H