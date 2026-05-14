#include "QBertStates.h"
#include "QBertMoveComponent.h"

QBert::JumpingQBertState::JumpingQBertState(dae::GameObject* qbert, dae::SpriteComp* spriteComp, QBert::QBertMoveComp* moveComp, const glm::vec3& moveDir)
	: QBertState(qbert, spriteComp)
	, m_moveDir(moveDir)
	, m_pMoveComp(moveComp)
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
		return std::make_unique<QBert::IdleQBertState>(m_qbert, m_pConnSpriteComp, m_pMoveComp);
	}

	return nullptr;
}