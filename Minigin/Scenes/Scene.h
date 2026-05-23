#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <vector>
#include "GameObject.h"
#include "IObserver.h"
#include <functional>

namespace dae
{
	class Scene final
	{
	public:
		void Load();
		void UnLoad();

		void Add(std::unique_ptr<GameObject> object);
		void RemoveMarkedForRemoval();
		void RemoveAll();

		void Update();
		void LateUpdate();
		void Render() const;

		[[nodiscard]] GameObject* GetGameObjectOwnership(GameObject* pObject);

		Scene(std::function<void(Scene&)> loadFunc);
		~Scene() = default;
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		template<typename T>
		T* GetFirstObjectByType()
		{
			for (auto& object : m_objects)
			{
				auto comp{ object->GetComponentInChildren<T>() };
				if (comp != nullptr)
				{
					return static_cast<T*>(comp);
				}
			}
			return nullptr;
		}

		std::vector<GameObject*> GetObjectsByTag(dae::Tag tag) const;

		void RequestReorderObjects() { m_reorderObjects = true; }

	private:
		friend class SceneManager;
		explicit Scene() = default;

		std::function<void(Scene&)> m_loadFunc;
		std::vector<std::unique_ptr<GameObject>> m_objects{};

		bool m_reorderObjects{ false };
		void ReorderObjects();
	};
}

#endif // !SCENE_H