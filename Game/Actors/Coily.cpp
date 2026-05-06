#include "Coily.h"

QBert::Coily::Coily(dae::GameObject* pOwner)
	: dae::Component(pOwner)
{
	m_pState->OnEnter();
}

void QBert::Coily::Update()
{
	auto state = m_pState->Update();
	if (state->GetType() != m_pState->GetType()
		&& state->GetType() != CoilyState(GetOwner()).GetType())
	{
		m_pState->OnExit();
		m_pState.reset(state.release());
		m_pState->OnEnter();
	}
}

std::type_index QBert::Coily::GetType() const
{
	return typeid(Coily);
}