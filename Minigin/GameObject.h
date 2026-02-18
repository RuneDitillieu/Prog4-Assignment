#pragma once
#include <string>
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
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		// functions
		virtual void Update(float deltaTime);
		virtual void Render() const;

		void SetPosition(float x, float y);

		// Component templates
		template<typename T>
		void AddComponent(std::unique_ptr<T>&& component)
		{
			m_components.push_back(std::move(component));
		}
		template<typename T>
		T* GetComponent()
		{
			// return first component of the same type
			for (auto& component : m_components)
			{
				if (component->GetType() == typeid(T))
					return static_cast<T*>(component.get());
			}
			return nullptr;
		}
		template<typename T>
		void RemoveComponent()
		{
			// remove and erase first component of the same type
			auto it = std::remove_if(m_components.begin(), m_components.end(),
				[](std::unique_ptr<Component>& component) { return component->GetType() == typeid(T); });
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

	private:
		Transform m_transform{};

		std::vector<std::unique_ptr<Component>> m_components;
	};
}
