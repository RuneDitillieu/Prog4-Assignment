#include "QBertStates.h"
#include "Events.h"

QBert::QBertState::QBertState(dae::GameObject* qbert, dae::SpriteComp* spriteComp, QBertMoveComp* moveComp, LevelBase* level)
	: m_qbert(qbert), m_pConnSpriteComp(spriteComp), m_pMoveComp(moveComp), m_pConnLevel(level)
{ }

std::unique_ptr<QBert::QBertState> QBert::QBertState::OnNotify(dae::Event event, dae::Subject*)
{
	if (event.id == dae::make_sdbm_hash("ACTOR_HIT")
		&& event.args->object == m_qbert)
	{
		return std::make_unique<QBert::StunnedQBertState>(m_qbert, m_pConnSpriteComp, m_pMoveComp, m_pConnLevel);
	}

	return nullptr;
}