#include "SpriteComponent.h"
#include "DeltaTime.h"
#include <SDL3/SDL_rect.h>
#include "RenderComponent.h"
#include "GameObject.h"

dae::SpriteComp::SpriteComp(dae::GameObject* pOwner, const std::string& fileName, int cols, int rows, bool autoUpdate)
	: Component(pOwner)
	, m_fileName(fileName)
	, m_cols(cols)
	, m_rows(rows)
	, m_nrFrames(cols * rows)
	, m_startPos(glm::vec2(0, 0))
	, m_doUpdate(autoUpdate)
{ 
	m_pConnectedRenderComponent = GetOwner()->GetComponent<dae::RenderComponent>();
	if (m_pConnectedRenderComponent == nullptr)
	{
		m_pConnectedRenderComponent = GetOwner()->AddComponent(std::make_unique<RenderComponent>(GetOwner(), m_fileName));
	}

	m_frameW = m_pConnectedRenderComponent->GetSize().x / m_cols;
	m_frameH = m_pConnectedRenderComponent->GetSize().y / m_rows;
}

dae::SpriteComp::SpriteComp(dae::GameObject* pOwner, dae::RenderComponent* pConnRenderComp, const std::string& fileName, 
	int cols, int rows, float frameW, float frameH, const glm::vec2& startPos, bool autoUpdate)
	: Component(pOwner)
	, m_pConnectedRenderComponent(pConnRenderComp)
	, m_fileName(fileName)
	, m_cols(cols)
	, m_rows(rows)
	, m_nrFrames(cols * rows)
	, m_frameW(frameW)
	, m_frameH(frameH)
	, m_startPos(startPos)
	, m_doUpdate(autoUpdate)
{ }

void dae::SpriteComp::LateUpdate()
{
	if (m_doUpdate)
	{
		m_accuSec += DeltaTime::GetInstance().GetDeltaTime();
		if (m_accuSec >= m_frameSec)
		{
			m_accuSec -= m_frameSec;
			++m_curFrame;
			m_curFrame %= m_nrFrames;
		}
	}

	const int curCol{ m_curFrame % m_cols };
	const int curRow{ m_curFrame / m_cols };
	const SDL_FRect sourceRect{ m_startPos.x + (m_frameW * curCol), m_startPos.y + (m_frameH * curRow), m_frameW, m_frameH };

	m_pConnectedRenderComponent->SetSourceRect(sourceRect);
}

void dae::SpriteComp::SetCurFrame(int frameIdx)
{
	m_curFrame = frameIdx;
	m_curFrame %= m_cols * m_rows;
}

void dae::SpriteComp::Set(int cols, int rows, float frameW, float frameH, const glm::vec2& startPos, bool autoUpdate)
{
	m_cols = cols;
	m_rows = rows;
	m_frameW = frameW;
	m_frameH = frameH;
	m_startPos = startPos;
	m_doUpdate = autoUpdate;

	m_curFrame = 0;
	m_accuSec = 0.f;
}

std::type_index dae::SpriteComp::GetType() const
{
	return typeid(SpriteComp);
}