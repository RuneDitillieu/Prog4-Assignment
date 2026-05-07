#pragma once
#include "Component.h"
#include <string>

namespace dae
{
	class RenderComponent;
	class GameObject;
	class SpriteComp : public Component
	{
	public:
		SpriteComp(dae::GameObject* pOwner, const std::string& fileName, int cols, int rows);	// for evenly spaced sprites
		SpriteComp(dae::GameObject* pOwner, dae::RenderComponent* pConnRenderComp, const std::string& fileName, int cols, int rows, float frameW, float frameH);	// for uneven/partial sprites
		~SpriteComp() = default;
		SpriteComp(const SpriteComp& other) = delete;
		SpriteComp(SpriteComp&& other) = delete;
		SpriteComp& operator=(const SpriteComp& other) = delete;
		SpriteComp& operator=(SpriteComp&& other) = delete;

		void LateUpdate() override;

		std::type_index GetType() const override;

	private:
		dae::RenderComponent* m_pConnectedRenderComponent;
		std::string m_fileName;
		int m_rows;
		int m_cols;
		int m_curFrame{ 0 };
		int m_nrFrames;
		float m_frameW;
		float m_frameH;
		float m_accuSec{ 0.f };
		float m_frameSec{ 0.2f };
	};
}