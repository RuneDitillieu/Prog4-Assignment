#pragma once
#include <memory>
#include <vector>
#include "GameObject.h"
#include "Observers.h"

namespace dae
{
	class Scene final
	{
	public:
		void Add(std::unique_ptr<GameObject> object);
		void Add(std::unique_ptr<IObserver> observer);
		void RemoveMarkedForRemoval();
		void RemoveAll();

		void Update(float deltaTime);
		void Render() const;

		~Scene() = default;
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private:
		friend class SceneManager;
		explicit Scene() = default;

		std::vector<std::unique_ptr<GameObject>> m_objects{};
		std::vector<std::unique_ptr<IObserver>> m_observers{};
	};
}
