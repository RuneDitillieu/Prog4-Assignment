#include "PlayerCoilyStates.h"
#include "DeltaTime.h"
#include "QBertMoveComponent.h"

QBert::PlayerStunnedCoilyState::PlayerStunnedCoilyState(dae::GameObject* coily, dae::SpriteComp* spriteComp,
	QBertMoveComp* moveComp, LevelBase* level, QBertMoveComp* qbertMoveComp)
	: PlayerCoilyState(coily, spriteComp, moveComp, level, qbertMoveComp)
{ }

void QBert::PlayerStunnedCoilyState::OnEnter()
{
	m_pMoveComp->m_isEnabled = false;
}

std::unique_ptr<QBert::PlayerCoilyState> QBert::PlayerStunnedCoilyState::Update()
{
	m_secPassed += dae::DeltaTime::GetInstance().GetDeltaTime();
	if (m_secPassed >= m_maxSec)
	{
		return std::make_unique<PlayerDroppingEggState>(m_coily, m_pConnSprite, m_pMoveComp, m_pConnLevel, m_pQBertMoveComp);
	}

	return nullptr;
}