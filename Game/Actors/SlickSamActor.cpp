#include "SlickSamActor.h"

#include "GameObject.h"
#include "LevelManager.h"
#include "QBertMoveComponent.h"
#include "QBertScoreComponent.h"
#include "SceneManager.h"
#include "Tags.h"

QBert::SlickSamActorComp::SlickSamActorComp(dae::GameObject* pOwner, LevelBase* pLevel, const std::vector<QBertMoveComp*>& qbertMoves)
	: Component(pOwner)
{
	m_pState = std::make_unique<DroppingSlickSamState>(GetOwner(), GetOwner()->GetComponent<dae::SpriteComp>(),
	GetOwner()->GetComponent<QBertMoveComp>(), pLevel, qbertMoves);
	m_pState->OnEnter();
}

QBert::SlickSamActorComp::~SlickSamActorComp()
{
	auto qberts = dae::SceneManager::GetInstance().GetActiveScene()->GetObjectsByTag(dae::Tag(Tag::QBert));
	for (auto qbert : qberts)
	{
		qbert->GetSubject()->RemoveObserver(this);
	}
}

void QBert::SlickSamActorComp::Start()
{
	GetOwner()->GetSubject()->AddObserver(this);
	auto qberts = dae::SceneManager::GetInstance().GetActiveScene()->GetObjectsByTag(dae::Tag(Tag::QBert));
	for (auto qbert : qberts)
	{
		qbert->GetSubject()->AddObserver(this);
	}

	auto scoreComp = dae::SceneManager::GetInstance().GetActiveScene()->GetFirstObjectByType<ScoreComp>();
	GetOwner()->GetSubject()->AddObserver(scoreComp);

	auto levelManager = dae::SceneManager::GetInstance().GetActiveScene()->GetFirstObjectByType<LevelManager>();
	levelManager->GetOwner()->GetSubject()->AddObserver(this);
}

void QBert::SlickSamActorComp::Update()
{
	auto state = m_pState->Update();
	if (state != nullptr)
	{
		m_pState->OnExit();
		m_pState.reset(state.release());
		m_pState->OnEnter();
	}
}

void QBert::SlickSamActorComp::Notify(dae::Event event, dae::Subject* subject)
{
	auto state = m_pState->OnNotify(event, subject);
	if (state != nullptr)
	{
		m_pState->OnExit();
		m_pState.reset(state.release());
		m_pState->OnEnter();
	}
}

std::type_index QBert::SlickSamActorComp::GetType() const
{
	return typeid(SlickSamActorComp);
}