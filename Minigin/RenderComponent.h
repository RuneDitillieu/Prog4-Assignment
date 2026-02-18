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
		virtual void Render(const Transform& transform) const override;
		void SetTexture(const std::string& filename);

		virtual std::type_index GetType() const override;

		RenderComponent(GameObject* owner, const std::string& filename);
		virtual ~RenderComponent();
		RenderComponent(const RenderComponent& other) = delete;
		RenderComponent(RenderComponent&& other) = delete;
		RenderComponent& operator=(const RenderComponent& other) = delete;
		RenderComponent& operator=(RenderComponent&& other) = delete;

	private:
		std::shared_ptr<Texture2D> m_texture{};
	};
}