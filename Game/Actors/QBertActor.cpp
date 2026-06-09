#include "QBertActor.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Tags.h"

QBert::QBertActorComp::QBertActorComp(dae::GameObject* pOwner, dae::SpriteComp* spriteComp, QBertMoveComp* moveComp, LevelBase* pLevel)
	: dae::Component(pOwner)
{ 
	m_pState = std::make_unique<IdleQBertState>(pOwner, spriteComp, moveComp, pLevel);
}

QBert::QBertActorComp::~QBertActorComp()
{
	auto discs = dae::SceneManager::GetInstance().GetActiveScene()->GetObjectsByTag(dae::Tag(Tag::Disc));
	for (auto disc : discs)
	{
		if (GetOwner()->GetParent() != disc)
		{
			disc->GetSubject()->RemoveObserver(this);
		}
	}
}

void QBert::QBertActorComp::Start()
{
	GetOwner()->GetSubject()->AddObserver(this);

	auto discs = dae::SceneManager::GetInstance().GetActiveScene()->GetObjectsByTag(dae::Tag(Tag::Disc));
	for (auto disc : discs)
	{
		disc->GetSubject()->AddObserver(this);
	}
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