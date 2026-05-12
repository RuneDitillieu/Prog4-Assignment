#include "QBertStates.h"

QBert::MoveState::MoveState(dae::GameObject* qbert)
	: QBertState(qbert)
{
	m_pConnSpriteComp = qbert->GetComponent<dae::SpriteComp>();
}

void QBert::MoveState::OnEnter()
{
}

std::unique_ptr<QBert::QBertState> QBert::MoveState::Update()
{
	return nullptr;
}

std::type_index QBert::MoveState::GetType() const
{
	return typeid(MoveState);
}