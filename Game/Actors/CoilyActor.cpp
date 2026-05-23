#include "CoilyActor.h"
#include "GameObject.h"
#include "QBertMoveComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Tags.h"

QBert::CoilyActorComp::CoilyActorComp(dae::GameObject* pOwner, LevelBase* pLevel, QBertMoveComp* qbertMove)
	: dae::Component(pOwner)
{
	m_pState = std::make_unique<QBert::IdleEggState>(GetOwner(), GetOwner()->GetComponent<dae::SpriteComp>(), 
		GetOwner()->GetComponent<QBert::QBertMoveComp>(), pLevel, qbertMove);
	m_pState->OnEnter();
}

QBert::CoilyActorComp::~CoilyActorComp()
{
	auto qberts = dae::SceneManager::GetInstance().GetActiveScene()->GetObjectsByTag(dae::Tag(QBert::Tag::Player));
	for (auto qbert : qberts)
	{
		qbert->GetSubject()->RemoveObserver(this);
	}
}

void QBert::CoilyActorComp::Start()
{
	GetOwner()->GetSubject()->AddObserver(this);
	auto qberts = dae::SceneManager::GetInstance().GetActiveScene()->GetObjectsByTag(dae::Tag(QBert::Tag::Player));
	for (auto qbert : qberts)
	{
		qbert->GetSubject()->AddObserver(this);
	}
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