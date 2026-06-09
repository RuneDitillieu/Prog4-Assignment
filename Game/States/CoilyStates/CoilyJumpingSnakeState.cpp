#include "CoilyStates.h"
#include "QBertMoveComponent.h"

QBert::JumpingSnakeState::JumpingSnakeState(dae::GameObject* coily, dae::SpriteComp* spriteComp, 
	QBertMoveComp* moveComp, LevelBase* level, QBertMoveComp* qbertMoveComp)
	: CoilyState(coily, spriteComp, moveComp, level, qbertMoveComp)
{ }

void QBert::JumpingSnakeState::OnEnter()
{
	glm::vec2 moveDir{ m_pQBertMoveComp->GetCurrentTile() - m_pMoveComp->GetCurrentTile() };

	if (moveDir.x == 0 && moveDir.y == 0)
	{
		m_pConnSprite->SetCurFrame(m_pConnSprite->GetCurFrame() + 1);
		return;
	}

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
	CoilyState::Update();

	if (!m_pMoveComp->IsMoving())
	{
		return std::make_unique<IdleSnakeState>(m_coily, m_pConnSprite, m_pMoveComp, m_pConnLevel, m_pQBertMoveComp);
	}

	return nullptr;
}

std::unique_ptr<QBert::CoilyState> QBert::JumpingSnakeState::OnNotify(dae::Event event, dae::Subject* subject)
{
	auto state = CoilyState::OnNotify(event, subject);
	if (state != nullptr) return state;

	if (event.id == dae::make_sdbm_hash("ACTOR_FELL")
		&& subject == m_coily->GetSubject())
	{
		m_pMoveComp->m_isEnabled = false;
		return std::make_unique<FallingSnakeState>(m_coily, m_pConnSprite, m_pMoveComp, m_pConnLevel, m_pQBertMoveComp);
	}

	return nullptr;
}