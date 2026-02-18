#pragma once
#include <string>
#include <memory>
#include "Transform.h"
#include "SDL3/SDL_pixels.h"

namespace dae
{
	class Font;
	class TextComponent;
	class Texture2D;
	class Component;
	class GameObject final
	{
	public:
		virtual void Update(float deltaTime);
		virtual void Render() const;

		void SetTexture(const std::string& filename);
		void SetPosition(float x, float y);

		// Components
		template<typename T>
		void AddComponent(std::unique_ptr<T>&& component)
		{
			m_components.push_back(std::move(component));
		}
		template<typename T>
		T* GetComponent()
		{
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
			for (auto& component : m_components)
			{
				if (component->GetType() == typeid(T))
				{
					component.reset();
					return;
				}
			}
		}

		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		Transform m_transform{};
		std::shared_ptr<Texture2D> m_texture{};

		std::vector<std::unique_ptr<Component>> m_components;
	};
}
