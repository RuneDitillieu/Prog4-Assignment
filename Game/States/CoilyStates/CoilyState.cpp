#include "CoilyStates.h"
#include "QBertMoveComponent.h"

QBert::CoilyState::CoilyState(dae::GameObject* coily, dae::SpriteComp* spriteComp, QBertMoveComp* moveComp,
	LevelBase* level, const std::vector<QBertMoveComp*>& qbertMoveComps)
	: m_coily(coily), m_pConnSprite(spriteComp), m_pMoveComp(moveComp), m_pConnLevel(level), m_pQBertMoveComps(qbertMoveComps)
{ }

std::unique_ptr<QBert::CoilyState> QBert::CoilyState::Update()
{
	for (auto qbertMove : m_pQBertMoveComps)
	{
		if (qbertMove == nullptr) continue;

		if(m_pMoveComp->GetGoalTile() == qbertMove->GetCurrentTile()
			|| m_pMoveComp->GetCurrentTile() == qbertMove->GetGoalTile())
		{
			glm::vec3 coilyFootPos{ m_coily->GetWorldPosition() + m_pMoveComp->GetFeetPos() };
			glm::vec3 qbertFootPos{ qbertMove->GetOwner()->GetWorldPosition() + qbertMove->GetFeetPos() };

			if (glm::length(coilyFootPos - qbertFootPos) <= 10.f)
			{
				dae::Event e{ dae::make_sdbm_hash("ACTOR_HIT") };
				e.args->object = qbertMove->GetOwner();
				m_coily->GetSubject()->NotifyObservers(e);

				return std::make_unique<StunnedCoilyState>(m_coily, m_pConnSprite, m_pMoveComp, m_pConnLevel, m_pQBertMoveComps);
			}
		}
	}

	return nullptr;
}

std::unique_ptr<QBert::CoilyState> QBert::CoilyState::OnNotify(dae::Event event, dae::Subject*)
{
	if (event.id == dae::make_sdbm_hash("QBERT_KILLED")
		|| event.id == dae::make_sdbm_hash("LEVEL_COMPLETED"))
	{
		return std::make_unique<StunnedCoilyState>(m_coily, m_pConnSprite, m_pMoveComp, m_pConnLevel, m_pQBertMoveComps);
	}

	return nullptr;
}