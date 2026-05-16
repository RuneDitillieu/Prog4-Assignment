#include "QBertStates.h"
#include "QBertMoveComponent.h"

QBert::IdleQBertState::IdleQBertState(dae::GameObject* qbert, dae::SpriteComp* spriteComp, QBert::QBertMoveComp* moveComp, LevelBase* level)
	: QBertState(qbert, spriteComp, moveComp, level)
{ }

void QBert::IdleQBertState::OnEnter()
{
	int curFrame{ m_pConnSpriteComp->GetCurFrame() };
	m_pConnSpriteComp->SetCurFrame(curFrame - 1);
}

std::unique_ptr<QBert::QBertState> QBert::IdleQBertState::Update()
{
	return nullptr;
}

std::unique_ptr<QBert::QBertState> QBert::IdleQBertState::OnNotify(dae::Event event, dae::Subject* subject)
{
	if (event.id == dae::make_sdbm_hash("OnMove"))
	{
		return std::make_unique<QBert::JumpingQBertState>(m_qbert, m_pConnSpriteComp, m_pMoveComp, m_pConnLevel, event.args->dir);
	}
	else if (event.id == dae::make_sdbm_hash("DISC_USED"))
	{
		m_qbert->SetParent(event.args->object, true);
		m_pMoveComp->m_isEnabled = false;
		return std::make_unique<QBert::TransportedQBertState>(m_qbert, m_pConnSpriteComp, m_pMoveComp, m_pConnLevel);
	}
	// make sure to only call when qbert himself falls, not when an enemy does
	else if (event.id == dae::make_sdbm_hash("ACTOR_FELL")
		&& subject == m_qbert->GetSubject())
	{
		return std::make_unique<QBert::FallingQBertState>(m_qbert, m_pConnSpriteComp, m_pMoveComp, m_pConnLevel);
	}

	return nullptr;
}