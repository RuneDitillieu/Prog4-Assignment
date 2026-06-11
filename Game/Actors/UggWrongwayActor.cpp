#include "UggWrongwayActor.h"

#include "GameObject.h"
#include "LevelManager.h"
#include "QBertActor.h"
#include "QBertMoveComponent.h"
#include "QBertScoreComponent.h"
#include "SceneManager.h"
#include "Tags.h"
#include "UggWrongwayStates.h"

QBert::UggWrongwayActor::UggWrongwayActor(dae::GameObject* pOwner, LevelBase* pLevel, QBertMoveComp* qbertMove, bool goesRight)
    : Component(pOwner)
{
    m_pState = std::make_unique<IdleUggWrongwayState>(GetOwner(), GetOwner()->GetComponent<dae::SpriteComp>(),
    GetOwner()->GetComponent<QBertMoveComp>(), pLevel, qbertMove, goesRight);
    m_pState->OnEnter();
}

QBert::UggWrongwayActor::~UggWrongwayActor()
{
    auto qberts = dae::SceneManager::GetInstance().GetActiveScene()->GetObjectsByTag(dae::Tag(Tag::Player));
    for (auto qbert : qberts)
    {
        qbert->GetSubject()->RemoveObserver(this);
    }

    if (dynamic_cast<StunnedUggWrongwayState*>(m_pState.get())
    || dynamic_cast<FallingUggWrongwayState*>(m_pState.get()))
    {
        auto levelManager = dae::SceneManager::GetInstance().GetActiveScene()->GetFirstObjectByType<LevelManager>();
        if (levelManager) levelManager->GetOwner()->GetSubject()->RemoveObserver(this);
    }
}

void QBert::UggWrongwayActor::Start()
{
    GetOwner()->GetSubject()->AddObserver(this);
    auto qberts = dae::SceneManager::GetInstance().GetActiveScene()->GetObjectsByTag(dae::Tag(Tag::Player));
    for (auto qbert : qberts)
    {
        qbert->GetSubject()->AddObserver(this);
        GetOwner()->GetSubject()->AddObserver(qbert->GetComponent<QBertActorComp>());
    }

    auto scoreComp = dae::SceneManager::GetInstance().GetActiveScene()->GetFirstObjectByType<ScoreComp>();
    GetOwner()->GetSubject()->AddObserver(scoreComp);

    auto levelManager = dae::SceneManager::GetInstance().GetActiveScene()->GetFirstObjectByType<LevelManager>();
    levelManager->GetOwner()->GetSubject()->AddObserver(this);
}

void QBert::UggWrongwayActor::Update()
{
    auto state = m_pState->Update();
    if (state != nullptr)
    {
        m_pState->OnExit();
        m_pState.reset(state.release());
        m_pState->OnEnter();
    }
}

void QBert::UggWrongwayActor::Notify(dae::Event event, dae::Subject* subject)
{
    auto state = m_pState->OnNotify(event, subject);
    if (state != nullptr)
    {
        m_pState->OnExit();
        m_pState.reset(state.release());
        m_pState->OnEnter();
    }
}

std::type_index QBert::UggWrongwayActor::GetType() const
{
    return typeid(UggWrongwayActor);
}