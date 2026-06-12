#include "CoilyStates.h"
#include "QBertMoveComponent.h"
#include "LevelBase.h"

QBert::JumpingSnakeState::JumpingSnakeState(dae::GameObject* coily, dae::SpriteComp* spriteComp, 
	QBertMoveComp* moveComp, LevelBase* level, const std::vector<QBertMoveComp*>& qbertMoveComps)
	: CoilyState(coily, spriteComp, moveComp, level, qbertMoveComps)
{ }

void QBert::JumpingSnakeState::OnEnter()
{
	// default target first player
	glm::vec2 moveDir{ m_pQBertMoveComps[0]->GetCurrentTile() - m_pMoveComp->GetCurrentTile() };

	// if there is a second player and they're closer, target them instead
	if (m_pQBertMoveComps.size() > 1
		&& glm::length(m_pQBertMoveComps[1]->GetCurrentTile() - m_pMoveComp->GetCurrentTile())
		< glm::length(m_pQBertMoveComps[0]->GetCurrentTile() - m_pMoveComp->GetCurrentTile()))
	{
		moveDir = m_pQBertMoveComps[1]->GetCurrentTile() - m_pMoveComp->GetCurrentTile();
	}

	if (moveDir.x == 0 && moveDir.y == 0)
	{
		m_pConnSprite->SetCurFrame(m_pConnSprite->GetCurFrame() + 1);
		return;
	}

	bool hasReachedBottom{ !m_pConnLevel->GetTile(static_cast<int>(m_pMoveComp->GetCurrentTile().x + 1),
		static_cast<int>(m_pMoveComp->GetCurrentTile().y)) };

	if (abs(moveDir.x) > abs(moveDir.y))
	{
		if (hasReachedBottom && moveDir.x > 0)
		{
			moveDir.x = 0;
			moveDir.y = -1;
		}
		else
		{
			moveDir.y = 0;
			moveDir.x /= abs(moveDir.x);
		}
	}
	else
	{
		if (hasReachedBottom && moveDir.y > 0)
		{
			moveDir.x = -1;
			moveDir.y = 0;
		}
		else
		{
			moveDir.x = 0;
			moveDir.y /= abs(moveDir.y);
		}
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
		return std::make_unique<IdleSnakeState>(m_coily, m_pConnSprite, m_pMoveComp, m_pConnLevel, m_pQBertMoveComps);
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
		return std::make_unique<FallingSnakeState>(m_coily, m_pConnSprite, m_pMoveComp, m_pConnLevel, m_pQBertMoveComps);
	}

	return nullptr;
}