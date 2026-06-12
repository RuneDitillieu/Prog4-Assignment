#include "PlayerCoilyStates.h"

QBert::PlayerIdleSnakeState::PlayerIdleSnakeState(dae::GameObject* coily, dae::SpriteComp* spriteComp,
	QBertMoveComp* moveComp, LevelBase* level, QBertMoveComp* qbertMoveComp)
	: PlayerCoilyState(coily, spriteComp, moveComp, level, qbertMoveComp)
{
}

void QBert::PlayerIdleSnakeState::OnEnter()
{
	int curFrame{ m_pConnSprite->GetCurFrame() };
	m_pConnSprite->SetCurFrame(curFrame - 1);
}

std::unique_ptr<QBert::PlayerCoilyState> QBert::PlayerIdleSnakeState::OnNotify(dae::Event event, dae::Subject* subject)
{
	auto state = PlayerCoilyState::OnNotify(event, subject);
	if (state != nullptr) return state;

	if (event.id == dae::make_sdbm_hash("OnMove"))
	{
		return std::make_unique<PlayerJumpingSnakeState>(m_coily, m_pConnSprite, m_pMoveComp,
			m_pConnLevel, m_pQBertMoveComp, event.args->dir);
	}

	return nullptr;
}