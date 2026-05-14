#include "Coily.h"
#include "GameObject.h"
#include "QBertMoveComponent.h"

QBert::Coily::Coily(dae::GameObject* pOwner, LevelBase* pLevel)
	: dae::Component(pOwner)
{
	pLevel;
	m_pState = std::make_unique<QBert::IdleEggState>(GetOwner(), GetOwner()->GetComponent<dae::SpriteComp>(), 
		GetOwner()->GetComponent<QBert::QBertMoveComp>(), pLevel);
	m_pState->OnEnter();
}

void QBert::Coily::Update()
{
	auto state = m_pState->Update();
	if (state != nullptr)
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