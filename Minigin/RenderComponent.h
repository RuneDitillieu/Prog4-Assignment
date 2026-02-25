#pragma once
#include "Component.h"
#include <memory>
#include <string>

namespace dae
{
	class Texture2D;
	class Transform;
	class GameObject;
	class RenderComponent final : public Component
	{
	public:
		void Render(const Transform& transform) const override;
		void SetTexture(const std::string& filename);
		void SetTexture(std::shared_ptr<Texture2D>&& texture);

		std::type_index GetType() const override;

		RenderComponent(GameObject* pOwner, const std::string& filename);
		RenderComponent(GameObject* pOwner);
		~RenderComponent();
		RenderComponent(const RenderComponent& other) = delete;
		RenderComponent(RenderComponent&& other) = delete;
		RenderComponent& operator=(const RenderComponent& other) = delete;
		RenderComponent& operator=(RenderComponent&& other) = delete;

	private:
		std::shared_ptr<Texture2D> m_texture{};
	};
}