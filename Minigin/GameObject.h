#pragma once
#include <memory>
#include "Transform.h"
#include "Component.h"

namespace dae
{
	class Font;
	class TextComponent;
	class Texture2D;
	class GameObject final
	{
	public:
		GameObject() = default;
		~GameObject() = default;
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		// functions
		void Update(float deltaTime);
		void Render() const;

		void SetLocalPosition(float x, float y);

		void MarkForRemoval() { m_isMarkedForRemoval = true;  }
		bool GetIsMarkedForRemoval() { return m_isMarkedForRemoval; }

		// Component templates
		template<typename T>
		T* AddComponent(std::unique_ptr<T>&& component)
		{
			m_components.push_back(std::move(component));
			return static_cast<T*>(m_components[m_components.size() - 1].get());
		}
		template<typename T>
		T* GetComponent(size_t index = 0)
		{
			// get the index'th component of the given type, with the first one being default
			size_t nextIndex{ 0 };
			for (auto& component : m_components)
			{
				if (component->GetType() == typeid(T))
				{
					if (nextIndex == index)
						return static_cast<T*>(component.get());
					else
						++nextIndex;
				}
			}
			return nullptr;
		}
		template<typename T>
		void RemoveComponent(size_t index = 0)
		{
			// place all components of this type in the back
			auto it = std::remove_if(m_components.begin(), m_components.end(),
				[](std::unique_ptr<Component>& component) { return component->GetType() == typeid(T); });

			// if there is an index'th component then remove it, else remove the first one
			if(it + index < m_components.size())
				m_components.erase(it + index);
			else
				m_components.erase(it);
		}
		template<typename T>
		bool HasComponent()
		{
			// check if GameObject has a component of given type
			for (auto& component : m_components)
			{
				if (component->GetType() == typeid(T))
					return true;
			}
			return false;
		}

		// Parent-Child functions
		void SetParent(GameObject* newParent, bool keepWorldPosition);
		GameObject* GetParent() const { return m_parent; }
		std::vector<GameObject*>& GetChildren() { return m_children; }
		bool IsParentOf(GameObject* possibleParent) const;

	private:
		void AddChild(GameObject* newParent);
		void RemoveChild(GameObject* newParent);

		void SetLocalPosition(const glm::vec3& newPosition);
		const glm::vec3& GetWorldPosition();
		void UpdateWorldPosition();
		void SetPositionDirty();

		bool m_isMarkedForRemoval{ false };
		Transform m_localTransform{};
		Transform m_worldTransform{};
		bool m_posIsDirty{ false };

		std::vector<std::unique_ptr<Component>> m_components;
		GameObject* m_parent{ nullptr };		// non-owning
		std::vector<GameObject*> m_children;	// non-owning
	};
}
