#include "SlickSamStates.h"

#include "QBertMoveComponent.h"

QBert::SlickSamState::SlickSamState(dae::GameObject* slickSam, dae::SpriteComp* spriteComp, QBertMoveComp* moveComp, LevelBase* level, QBertMoveComp* qbertMoveComp)
    : m_slickSam(slickSam), m_pConnSprite(spriteComp), m_pMoveComp(moveComp), m_pConnLevel(level), m_pQBertMoveComp(qbertMoveComp)
{ }

std::unique_ptr<QBert::SlickSamState> QBert::SlickSamState::Update()
{
    if(m_pMoveComp->GetGoalTile() == m_pQBertMoveComp->GetCurrentTile()
        || m_pMoveComp->GetCurrentTile() == m_pQBertMoveComp->GetGoalTile())
    {
        glm::vec3 slickSamFootPos{ m_slickSam->GetWorldPosition() + m_pMoveComp->GetFeetPos() };
        glm::vec3 qbertFootPos{ m_pQBertMoveComp->GetOwner()->GetWorldPosition() + m_pQBertMoveComp->GetFeetPos() };

        if (glm::length(slickSamFootPos - qbertFootPos) <= 10.f)
        {
            dae::Event e{ dae::make_sdbm_hash("SAM_SLICK_KILLED") };
            m_slickSam->GetSubject()->NotifyObservers(e);
            m_slickSam->IsEnabled(false);
            m_slickSam->MarkForRemoval();
        }
    }

    return nullptr;
}

std::unique_ptr<QBert::SlickSamState> QBert::SlickSamState::OnNotify(dae::Event event, dae::Subject*)
{
    if (event.id == dae::make_sdbm_hash("QBERT_KILLED"))
    {
        //return std::make_unique<QBert::StunnedCoilyState>(m_coily, m_pConnSprite, m_pMoveComp, m_pConnLevel, m_pQBertMoveComp);
    }

    return nullptr;
}