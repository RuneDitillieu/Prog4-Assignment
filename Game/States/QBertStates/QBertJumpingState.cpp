#include "QBertStates.h"
#include "QBertMoveComponent.h"

QBert::JumpingQBertState::JumpingQBertState(dae::GameObject* qbert, dae::SpriteComp* spriteComp, 
	QBert::QBertMoveComp* moveComp, LevelBase* level, const glm::vec3& moveDir)
	: QBertState(qbert, spriteComp, moveComp, level)
	, m_moveDir(moveDir)
{ }

void QBert::JumpingQBertState::OnEnter()
{
	m_pMoveComp->Move(m_moveDir);

	if (m_moveDir.x == 1)
	{
		m_pConnSpriteComp->SetCurFrame(5);
	}
	else if (m_moveDir.x == -1)
	{
		m_pConnSpriteComp->SetCurFrame(3);
	}
	else if (m_moveDir.y == 1)
	{
		m_pConnSpriteComp->SetCurFrame(7);
	}
	else if (m_moveDir.y == -1)
	{
		m_pConnSpriteComp->SetCurFrame(1);
	}
}

std::unique_ptr<QBert::QBertState> QBert::JumpingQBertState::Update()
{
	if (!m_pMoveComp->IsMoving())
	{
		return std::make_unique<QBert::IdleQBertState>(m_qbert, m_pConnSpriteComp, m_pMoveComp, m_pConnLevel);
	}

	return nullptr;
}

std::unique_ptr<QBert::QBertState> QBert::JumpingQBertState::OnNotify(dae::Event event, dae::Subject* subject)
{
	auto state = QBertState::OnNotify(event, subject);
	if (state != nullptr) return state;

	if (event.id == dae::make_sdbm_hash("DISC_USED"))
	{
		m_qbert->SetParent(event.args->object, true);
		m_pMoveComp->m_isEnabled = false;
		return std::make_unique<QBert::TransportedQBertState>(m_qbert, m_pConnSpriteComp, m_pMoveComp, m_pConnLevel);
	}
	else if (event.id == dae::make_sdbm_hash("ACTOR_FELL")
		&& subject == m_qbert->GetSubject())
	{
		return std::make_unique<QBert::FallingQBertState>(m_qbert, m_pConnSpriteComp, m_pMoveComp, m_pConnLevel);
	}

	return nullptr;
}