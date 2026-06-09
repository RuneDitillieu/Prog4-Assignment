#include "CoilyStates.h"
#include "DeltaTime.h"

QBert::IdleSnakeState::IdleSnakeState(dae::GameObject* coily, dae::SpriteComp* spriteComp, 
	QBertMoveComp* moveComp, LevelBase* level, QBertMoveComp* qbertMoveComp)
	: CoilyState(coily, spriteComp, moveComp, level, qbertMoveComp)
{
}

void QBert::IdleSnakeState::OnEnter()
{
	int curFrame{ m_pConnSprite->GetCurFrame() };
	m_pConnSprite->SetCurFrame(curFrame - 1);
}

std::unique_ptr<QBert::CoilyState> QBert::IdleSnakeState::Update()
{
	CoilyState::Update();

	m_secPassed += dae::DeltaTime::GetInstance().GetDeltaTime();

	if (m_secPassed >= m_idleSec)
	{
		return std::make_unique<JumpingSnakeState>(m_coily, m_pConnSprite, m_pMoveComp, m_pConnLevel, m_pQBertMoveComp);
	}

	return nullptr;
}