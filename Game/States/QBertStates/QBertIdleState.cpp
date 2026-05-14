#include "QBertStates.h"

QBert::IdleQBertState::IdleQBertState(dae::GameObject* qbert, dae::SpriteComp* spriteComp, QBert::QBertMoveComp* moveComp)
	: QBertState(qbert, spriteComp)
	, m_pMoveComp(moveComp)
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

std::unique_ptr<QBert::QBertState> QBert::IdleQBertState::OnNotify(dae::Event event, dae::Subject*)
{
	if (event.id == dae::make_sdbm_hash("OnMove"))
	{
		return std::make_unique<QBert::JumpingQBertState>(m_qbert, m_pConnSpriteComp, m_pMoveComp, event.args->dir);
	}

	return nullptr;
}