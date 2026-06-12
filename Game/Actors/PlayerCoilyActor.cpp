#include "PlayerCoilyActor.h"

#include "LevelManager.h"
#include "QBertActor.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Tags.h"
#include "QBertMoveComponent.h"
#include "QBertScoreComponent.h"

QBert::PlayerCoilyActorComp::PlayerCoilyActorComp(dae::GameObject* pOwner, LevelBase* pLevel, QBertMoveComp* qbertMoveComp)
    : dae::Component(pOwner)
{
    m_pState = std::make_unique<PlayerDroppingEggState>(pOwner, pOwner->GetComponent<dae::SpriteComp>(),
        pOwner->GetComponent<QBertMoveComp>(), pLevel, qbertMoveComp);
    m_pState->OnEnter();
}

QBert::PlayerCoilyActorComp::~PlayerCoilyActorComp()
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

void QBert::PlayerCoilyActorComp::Start()
{
    GetOwner()->GetSubject()->AddObserver(this);

    auto qberts = dae::SceneManager::GetInstance().GetActiveScene()->GetObjectsByTag(dae::Tag(Tag::QBert));
    for (auto qbert : qberts)
    {
        qbert->GetSubject()->AddObserver(this);
        GetOwner()->GetSubject()->AddObserver(qbert->GetComponent<QBertActorComp>());
    }

    auto discs = dae::SceneManager::GetInstance().GetActiveScene()->GetObjectsByTag(dae::Tag(Tag::Disc));
    for (auto disc : discs)
    {
        disc->GetSubject()->AddObserver(this);
    }

    auto score = dae::SceneManager::GetInstance().GetActiveScene()->GetFirstObjectByType<ScoreComp>();
    GetOwner()->GetSubject()->AddObserver(score);

    auto levelManager = dae::SceneManager::GetInstance().GetActiveScene()->GetFirstObjectByType<LevelManager>();
    levelManager->GetOwner()->GetSubject()->AddObserver(this);
}

void QBert::PlayerCoilyActorComp::Update()
{
    auto state = m_pState->Update();
    if (state != nullptr)
    {
        m_pState->OnExit();
        m_pState.reset(state.release());
        m_pState->OnEnter();
    }
}

void QBert::PlayerCoilyActorComp::Notify(dae::Event event, dae::Subject* subject)
{
    auto state = m_pState->OnNotify(event, subject);
    if (state != nullptr)
    {
        m_pState->OnExit();
        m_pState.reset(state.release());
        m_pState->OnEnter();
    }
}

std::type_index QBert::PlayerCoilyActorComp::GetType() const
{
    return typeid(PlayerCoilyActorComp);
}