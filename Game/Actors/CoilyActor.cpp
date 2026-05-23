#include "CoilyActor.h"
#include "GameObject.h"
#include "QBertMoveComponent.h"

QBert::CoilyActorComp::CoilyActorComp(dae::GameObject* pOwner, LevelBase* pLevel, QBertMoveComp* qbertMove)
	: dae::Component(pOwner)
{
	m_pState = std::make_unique<QBert::IdleEggState>(GetOwner(), GetOwner()->GetComponent<dae::SpriteComp>(), 
		GetOwner()->GetComponent<QBert::QBertMoveComp>(), pLevel, qbertMove);
	m_pState->OnEnter();
}

void QBert::CoilyActorComp::Start()
{
	GetOwner()->GetSubject()->AddObserver(this);
}

void QBert::CoilyActorComp::Update()
{
	auto state = m_pState->Update();
	if (state != nullptr)
	{
		m_pState->OnExit();
		m_pState.reset(state.release());
		m_pState->OnEnter();
	}
}

void QBert::CoilyActorComp::Notify(dae::Event event, dae::Subject* subject)
{
	auto state = m_pState->OnNotify(event, subject);
	if (state != nullptr)
	{
		m_pState->OnExit();
		m_pState.reset(state.release());
		m_pState->OnEnter();
	}
}

std::type_index QBert::CoilyActorComp::GetType() const
{
	return typeid(CoilyActorComp);
}