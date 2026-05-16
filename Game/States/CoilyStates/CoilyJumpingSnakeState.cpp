#include "CoilyStates.h"
#include "QBertMoveComponent.h"

QBert::JumpingSnakeState::JumpingSnakeState(dae::GameObject* coily, dae::SpriteComp* spriteComp, 
	QBertMoveComp* moveComp, LevelBase* level, QBertMoveComp* qbertMoveComp)
	: CoilyState(coily, spriteComp, moveComp, level)
	, m_pQBertMoveComp(qbertMoveComp)
{ }

void QBert::JumpingSnakeState::OnEnter()
{
	glm::vec2 moveDir{ m_pQBertMoveComp->GetCurrentTile() - m_pMoveComp->GetCurrentTile() };
	if (abs(moveDir.x) > abs(moveDir.y))
	{
		moveDir.y = 0;
		moveDir.x /= abs(moveDir.x);
	}
	else
	{
		moveDir.x = 0;
		moveDir.y /= abs(moveDir.y);
	}

	m_pMoveComp->Move(glm::vec3(moveDir.x, moveDir.y, 0));
	
	if (moveDir.x == 1)
	{
		m_pConnSprite->SetCurFrame(7);
	}
	else if (moveDir.x == -1)
	{
		m_pConnSprite->SetCurFrame(5);
	}
	else if (moveDir.y == 1)
	{
		m_pConnSprite->SetCurFrame(9);
	}
	else if (moveDir.y == -1)
	{
		m_pConnSprite->SetCurFrame(3);
	}
}

std::unique_ptr<QBert::CoilyState> QBert::JumpingSnakeState::Update()
{
	if (!m_pMoveComp->IsMoving())
	{
		return std::make_unique<QBert::IdleSnakeState>(m_coily, m_pConnSprite, m_pMoveComp, m_pConnLevel, m_pQBertMoveComp);
	}

	return nullptr;
}

std::unique_ptr<QBert::CoilyState> QBert::JumpingSnakeState::OnNotify(dae::Event event, dae::Subject* subject)
{
	if (event.id == dae::make_sdbm_hash("ACTOR_FELL")
		&& subject == m_coily->GetSubject())
	{
		m_pMoveComp->m_isEnabled = false;
		return std::make_unique<QBert::FallingSnakeState>(m_coily, m_pConnSprite, m_pMoveComp, m_pConnLevel);
	}

	return nullptr;
}