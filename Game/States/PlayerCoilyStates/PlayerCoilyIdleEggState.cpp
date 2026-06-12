#include "PlayerCoilyStates.h"
#include "DeltaTime.h"
#include "QBertMoveComponent.h"
#include "LevelBase.h"
#include "SceneManager.h"

QBert::PlayerIdleEggState::PlayerIdleEggState(dae::GameObject* coily, dae::SpriteComp* spriteComp, QBertMoveComp* moveComp,
	LevelBase* level, QBertMoveComp* qbertMoveComp)
	: PlayerCoilyState(coily, spriteComp, moveComp, level, qbertMoveComp)
{}

void QBert::PlayerIdleEggState::OnEnter()
{
	m_pConnSprite->SetCurFrame(0);
}

void QBert::PlayerIdleEggState::OnExit()
{
	m_pConnSprite->SetCurFrame(3);
}

std::unique_ptr<QBert::PlayerCoilyState> QBert::PlayerIdleEggState::Update()
{
	PlayerCoilyState::Update();

	m_secPassed += dae::DeltaTime::GetInstance().GetDeltaTime();

	bool hasReachedBottom{ !m_pConnLevel->GetTile(static_cast<int>(m_pMoveComp->GetCurrentTile().x + 1),
			static_cast<int>(m_pMoveComp->GetCurrentTile().y)) };

	if (m_secPassed >= m_idleSec && !hasReachedBottom)
	{
		return std::make_unique<PlayerJumpingEggState>(m_coily, m_pConnSprite, m_pMoveComp, m_pConnLevel, m_pQBertMoveComp);
	}
	else if(m_secPassed >= m_idleSec * 3.f)
	{
		return std::make_unique<PlayerIdleSnakeState>(m_coily, m_pConnSprite, m_pMoveComp, m_pConnLevel, m_pQBertMoveComp);
	}

	return nullptr;
}