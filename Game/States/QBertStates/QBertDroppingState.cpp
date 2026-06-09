#include "QBertStates.h"
#include "DeltaTime.h"
#include "QBertMoveComponent.h"
#include "LevelBase.h"
#include "TileComponent.h"

QBert::DroppingQBertState::DroppingQBertState(dae::GameObject* qbert, dae::SpriteComp* spriteComp, QBert::QBertMoveComp* moveComp, LevelBase* level)
	: QBertState(qbert, spriteComp, moveComp, level)
{ }

std::unique_ptr<QBert::QBertState> QBert::DroppingQBertState::Update()
{
	m_qbert->SetLocalPosition(m_qbert->GetLocalPosition() + glm::vec3(0, 100.f, 0) * dae::DeltaTime::GetInstance().GetDeltaTime());

	glm::vec3 qbertPos{ m_qbert->GetLocalPosition() + m_pMoveComp->GetFeetPos() };
	glm::vec3 tilePos{ m_pConnLevel->GetTile(0, 0)->GetMiddlePos() };

	if (glm::length(qbertPos - tilePos) <= 5.f)
	{
		m_pMoveComp->m_isEnabled = true;
		m_pMoveComp->Reset(glm::vec2(0, 0), true);
		return std::make_unique<QBert::IdleQBertState>(m_qbert, m_pConnSpriteComp, m_pMoveComp, m_pConnLevel);
	}

	return nullptr;
}