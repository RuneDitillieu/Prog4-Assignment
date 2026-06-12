#include "CoilyActor.h"
#include "GameObject.h"
#include "LevelManager.h"
#include "QBertActor.h"
#include "QBertMoveComponent.h"
#include "QBertScoreComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Tags.h"

QBert::CoilyActorComp::CoilyActorComp(dae::GameObject* pOwner, LevelBase* pLevel, const std::vector<QBertMoveComp*>& qbertMoves)
	: dae::Component(pOwner)
{
	m_pState = std::make_unique<DroppingEggState>(GetOwner(), GetOwner()->GetComponent<dae::SpriteComp>(),
		GetOwner()->GetComponent<QBertMoveComp>(), pLevel, qbertMoves);
	m_pState->OnEnter();
}

QBert::CoilyActorComp::~CoilyActorComp()
{
	auto qberts = dae::SceneManager::GetInstance().GetActiveScene()->GetObjectsByTag(dae::Tag(Tag::QBert));
	for (auto qbert : qberts)
	{
		qbert->GetSubject()->RemoveObserver(this);
	}
}

void QBert::CoilyActorComp::Start()
{
	GetOwner()->GetSubject()->AddObserver(this);
	auto qberts = dae::SceneManager::GetInstance().GetActiveScene()->GetObjectsByTag(dae::Tag(Tag::QBert));
	for (auto qbert : qberts)
	{
		qbert->GetSubject()->AddObserver(this);
		GetOwner()->GetSubject()->AddObserver(qbert->GetComponent<QBertActorComp>());
	}

	auto score = dae::SceneManager::GetInstance().GetActiveScene()->GetFirstObjectByType<ScoreComp>();
	GetOwner()->GetSubject()->AddObserver(score);

	auto levelManager = dae::SceneManager::GetInstance().GetActiveScene()->GetFirstObjectByType<LevelManager>();
	levelManager->GetOwner()->GetSubject()->AddObserver(this);
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