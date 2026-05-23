#include "DiscActor.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Tags.h"

QBert::DiscActorComp::DiscActorComp(dae::GameObject* pOwner, const glm::vec2& tile)
	: dae::Component(pOwner)
	, m_tile(tile)
{ 
	m_pState = std::make_unique<QBert::IdleDiscState>(pOwner);
	m_pState->OnEnter();
}

QBert::DiscActorComp::~DiscActorComp()
{
	auto qberts = dae::SceneManager::GetInstance().GetActiveScene()->GetObjectsByTag(dae::Tag(QBert::Tag::Player));
	for (auto qbert : qberts)
	{
		qbert->GetSubject()->RemoveObserver(this);
	}
}

void QBert::DiscActorComp::Start()
{
	auto qberts = dae::SceneManager::GetInstance().GetActiveScene()->GetObjectsByTag(dae::Tag(QBert::Tag::Player));
	for (auto qbert : qberts)
	{
		qbert->GetSubject()->AddObserver(this);
	}
}

void QBert::DiscActorComp::Update()
{
	auto state = m_pState->Update();
	if (state != nullptr)
	{
		m_pState->OnExit();
		m_pState.reset(state.release());
		m_pState->OnEnter();
	}
}

void QBert::DiscActorComp::Notify(dae::Event event, dae::Subject* subject)
{
	auto state = m_pState->OnNotify(event, subject);
	if (state != nullptr)
	{
		m_pState->OnExit();
		m_pState.reset(state.release());
		m_pState->OnEnter();
	}
}

std::type_index QBert::DiscActorComp::GetType() const
{
	return typeid(DiscActorComp);
}