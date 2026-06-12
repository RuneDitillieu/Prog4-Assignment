#include "PlayerCoilyStates.h"
#include "QBertMoveComponent.h"
#include "LevelBase.h"

QBert::PlayerJumpingSnakeState::PlayerJumpingSnakeState(dae::GameObject* coily, dae::SpriteComp* spriteComp,
	QBertMoveComp* moveComp, LevelBase* level, QBertMoveComp* qbertMoveComp, const glm::vec3& moveDir)
	: PlayerCoilyState(coily, spriteComp, moveComp, level, qbertMoveComp)
	, m_moveDir(moveDir)
{ }

void QBert::PlayerJumpingSnakeState::OnEnter()
{
	m_pMoveComp->Move(m_moveDir);
	
	if (m_moveDir.x == 1)
	{
		m_pConnSprite->SetCurFrame(7);
	}
	else if (m_moveDir.x == -1)
	{
		m_pConnSprite->SetCurFrame(5);
	}
	else if (m_moveDir.y == 1)
	{
		m_pConnSprite->SetCurFrame(9);
	}
	else if (m_moveDir.y == -1)
	{
		m_pConnSprite->SetCurFrame(3);
	}
}

std::unique_ptr<QBert::PlayerCoilyState> QBert::PlayerJumpingSnakeState::Update()
{
	PlayerCoilyState::Update();

	if (!m_pMoveComp->IsMoving())
	{
		return std::make_unique<PlayerIdleSnakeState>(m_coily, m_pConnSprite, m_pMoveComp, m_pConnLevel, m_pQBertMoveComp);
	}

	return nullptr;
}

std::unique_ptr<QBert::PlayerCoilyState> QBert::PlayerJumpingSnakeState::OnNotify(dae::Event event, dae::Subject* subject)
{
	auto state = PlayerCoilyState::OnNotify(event, subject);
	if (state != nullptr) return state;

	if (event.id == dae::make_sdbm_hash("ACTOR_FELL")
		&& subject == m_coily->GetSubject())
	{
		m_pMoveComp->m_isEnabled = false;
		return std::make_unique<PlayerFallingSnakeState>(m_coily, m_pConnSprite, m_pMoveComp, m_pConnLevel, m_pQBertMoveComp);
	}

	return nullptr;
}