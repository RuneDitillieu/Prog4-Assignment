#include "SpriteComponent.h"
#include "DeltaTime.h"
#include <SDL3/SDL_rect.h>
#include "RenderComponent.h"
#include "GameObject.h"

dae::SpriteComp::SpriteComp(dae::GameObject* pOwner, const std::string& fileName, int cols, int rows)
	: Component(pOwner)
	, m_fileName(fileName)
	, m_cols(cols)
	, m_rows(rows)
	, m_nrFrames(cols * rows)
{ 
	m_pConnectedRenderComponent = GetOwner()->GetComponent<dae::RenderComponent>();
	if (m_pConnectedRenderComponent == nullptr)
	{
		m_pConnectedRenderComponent = GetOwner()->AddComponent(std::make_unique<RenderComponent>(GetOwner(), m_fileName));
	}

	m_frameW = m_pConnectedRenderComponent->GetSize().x / cols;
	m_frameH = m_pConnectedRenderComponent->GetSize().y / rows;
}

dae::SpriteComp::SpriteComp(dae::GameObject* pOwner, dae::RenderComponent* pConnRenderComp, const std::string& fileName, int cols, int rows, float frameW, float frameH)
	: Component(pOwner)
	, m_pConnectedRenderComponent(pConnRenderComp)
	, m_fileName(fileName)
	, m_cols(cols)
	, m_rows(rows)
	, m_nrFrames(cols * rows)
	, m_frameW(frameW)
	, m_frameH(frameH)
{ }

void dae::SpriteComp::LateUpdate()
{
	m_accuSec += DeltaTime::GetInstance().GetDeltaTime();
	if (m_accuSec >= m_frameSec)
	{
		m_accuSec -= m_frameSec;
		++m_curFrame;
		m_curFrame %= m_nrFrames;
	}

	const int curCol{ m_curFrame % m_cols };
	const int curRow{ m_curFrame / m_cols };
	const SDL_FRect sourceRect{ (m_frameW * curCol), (m_frameH * curRow), m_frameW, m_frameH };

	m_pConnectedRenderComponent->SetSourceRect(sourceRect);
}

std::type_index dae::SpriteComp::GetType() const
{
	return typeid(SpriteComp);
}