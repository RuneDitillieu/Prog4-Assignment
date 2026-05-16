#include "QBertActor.h"

QBert::QBertActorComp::QBertActorComp(dae::GameObject* pOwner, dae::SpriteComp* spriteComp, QBertMoveComp* moveComp, QBert::LevelBase* pLevel)
	: dae::Component(pOwner)
{ 
	m_pState = std::make_unique<QBert::IdleQBertState>(pOwner, spriteComp, moveComp, pLevel);
}

void QBert::QBertActorComp::Update()
{
	auto state = m_pState->Update();
	if (state != nullptr)
	{
		m_pState->OnExit();
		m_pState.reset(state.release());
		m_pState->OnEnter();
	}
}

void QBert::QBertActorComp::Notify(dae::Event event, dae::Subject* subject)
{
	auto state = m_pState->OnNotify(event, subject);
	if (state != nullptr)
	{
		m_pState->OnExit();
		m_pState.reset(state.release());
		m_pState->OnEnter();
	}
}

std::type_index QBert::QBertActorComp::GetType() const
{
	return typeid(QBertActorComp);
}