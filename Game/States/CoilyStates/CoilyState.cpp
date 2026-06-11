#include "CoilyStates.h"
#include "QBertMoveComponent.h"

QBert::CoilyState::CoilyState(dae::GameObject* coily, dae::SpriteComp* spriteComp, QBertMoveComp* moveComp, LevelBase* level, QBertMoveComp* qbertMoveComp)
	: m_coily(coily), m_pConnSprite(spriteComp), m_pMoveComp(moveComp), m_pConnLevel(level), m_pQBertMoveComp(qbertMoveComp)
{ }

std::unique_ptr<QBert::CoilyState> QBert::CoilyState::Update()
{
	if(m_pMoveComp->GetGoalTile() == m_pQBertMoveComp->GetCurrentTile()
		|| m_pMoveComp->GetCurrentTile() == m_pQBertMoveComp->GetGoalTile())
	{
		glm::vec3 coilyFootPos{ m_coily->GetWorldPosition() + m_pMoveComp->GetFeetPos() };
		glm::vec3 qbertFootPos{ m_pQBertMoveComp->GetOwner()->GetWorldPosition() + m_pQBertMoveComp->GetFeetPos() };

		if (glm::length(coilyFootPos - qbertFootPos) <= 10.f)
		{
			dae::Event e{ dae::make_sdbm_hash("ACTOR_HIT") };
			e.args->object = m_pQBertMoveComp->GetOwner();
			m_coily->GetSubject()->NotifyObservers(e);

			return std::make_unique<StunnedCoilyState>(m_coily, m_pConnSprite, m_pMoveComp, m_pConnLevel, m_pQBertMoveComp);
		}
	}

	return nullptr;
}

std::unique_ptr<QBert::CoilyState> QBert::CoilyState::OnNotify(dae::Event event, dae::Subject*)
{
	if (event.id == dae::make_sdbm_hash("QBERT_KILLED")
		|| event.id == dae::make_sdbm_hash("LEVEL_COMPLETED"))
	{
		return std::make_unique<StunnedCoilyState>(m_coily, m_pConnSprite, m_pMoveComp, m_pConnLevel, m_pQBertMoveComp);
	}

	return nullptr;
}