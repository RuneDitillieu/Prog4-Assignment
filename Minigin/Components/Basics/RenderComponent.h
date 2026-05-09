#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H

#include "Component.h"
#include <memory>
#include <string>
#include <glm/glm.hpp>
#include <SDL3/SDL_rect.h>

namespace dae
{
	class Texture2D;
	class Transform;
	class GameObject;
	class RenderComponent final : public Component
	{
	public:
		void Render(const Transform& transform) override;
		void SetTexture(const std::string& filename);
		void SetTexture(std::shared_ptr<Texture2D>&& texture);
		void SetSourceRect(const SDL_FRect& src);
		void SetSourceRect(float x, float y, float width, float height);
		glm::vec2 GetSize() const;

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
		SDL_FRect m_sourceRect{};
		bool m_useSourceRect{ false };
	};
}

#endif // !RENDERCOMPONENT_H