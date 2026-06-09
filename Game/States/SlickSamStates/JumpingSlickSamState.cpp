#include "SlickSamStates.h"
#include "QBertMoveComponent.h"
#include "LevelBase.h"
#include "SpriteComponent.h"

QBert::JumpingSlickSamState::JumpingSlickSamState(dae::GameObject* slickSam, dae::SpriteComp* spriteComp, QBertMoveComp* moveComp,
    LevelBase* level, QBertMoveComp* qbertMoveComp)
    : SlickSamState(slickSam, spriteComp, moveComp, level, qbertMoveComp)
{ }

void QBert::JumpingSlickSamState::OnEnter()
{
    int randNr{ rand() % 2 };
    glm::vec3 moveDir{ 0, 0, 0 };

    if (randNr == 0)
    {
        moveDir.x = 1;
        m_pConnSprite->SetCurFrame(6);
    }
    else
    {
        moveDir.y = 1;
        m_pConnSprite->SetCurFrame(2);
    }

    m_pMoveComp->Move(moveDir);
}

std::unique_ptr<QBert::SlickSamState> QBert::JumpingSlickSamState::Update()
{
    SlickSamState::Update();

    if (!m_pMoveComp->IsMoving())
    {
        return std::make_unique<IdleSlickSamState>(m_slickSam, m_pConnSprite, m_pMoveComp, m_pConnLevel, m_pQBertMoveComp);
    }

    return nullptr;
}

std::unique_ptr<QBert::SlickSamState> QBert::JumpingSlickSamState::OnNotify(dae::Event event, dae::Subject* subject)
{
    auto state = SlickSamState::OnNotify(event, subject);
    if (state != nullptr) return state;

    if (event.id == dae::make_sdbm_hash("ACTOR_FELL")
        && subject == m_slickSam->GetSubject())
    {
        m_pMoveComp->m_isEnabled = false;
        return std::make_unique<FallingSlickSamState>(m_slickSam, m_pConnSprite, m_pMoveComp, m_pConnLevel, m_pQBertMoveComp);
    }

    return nullptr;
}