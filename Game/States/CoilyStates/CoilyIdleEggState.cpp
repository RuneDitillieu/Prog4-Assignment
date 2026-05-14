#include "CoilyStates.h"
#include "DeltaTime.h"
#include "QBertMoveComponent.h"
#include "LevelBase.h"

QBert::IdleEggState::IdleEggState(dae::GameObject* coily, dae::SpriteComp* spriteComp, QBertMoveComp* moveComp, LevelBase* level)
	: CoilyState(coily, spriteComp)
	, m_pMoveComp(moveComp)
	, m_pConnLevel(level)
{}

void QBert::IdleEggState::OnEnter()
{
	m_pConnSprite->SetCurFrame(0);
}

std::unique_ptr<QBert::CoilyState> QBert::IdleEggState::Update()
{
	m_secPassed += dae::DeltaTime::GetInstance().GetDeltaTime();

	bool hasReachedBottom{ !m_pConnLevel->GetTile(static_cast<int>(m_pMoveComp->GetCurrentTile().x + 1),
			static_cast<int>(m_pMoveComp->GetCurrentTile().y)) };

	if (m_secPassed >= m_idleSec && !hasReachedBottom)
	{
		return std::make_unique<QBert::JumpingEggState>(m_coily, m_pConnSprite, m_pMoveComp, m_pConnLevel);
	}
	else if(m_secPassed >= m_idleSec * 3.f)
	{
		return std::make_unique<QBert::SnakeState>(m_coily, m_pConnSprite);
	}

	return nullptr;
}