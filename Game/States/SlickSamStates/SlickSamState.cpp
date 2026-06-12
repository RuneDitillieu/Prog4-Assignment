#include "SlickSamStates.h"

#include "QBertMoveComponent.h"
#include "Tags.h"

QBert::SlickSamState::SlickSamState(dae::GameObject* slickSam, dae::SpriteComp* spriteComp,
                                    QBertMoveComp* moveComp, LevelBase* level, const std::vector<QBertMoveComp*>& qbertMoveComps)
    : m_slickSam(slickSam), m_pConnSprite(spriteComp), m_pMoveComp(moveComp), m_pConnLevel(level), m_pQBertMoveComps(qbertMoveComps)
{ }

std::unique_ptr<QBert::SlickSamState> QBert::SlickSamState::Update()
{
    for (auto qbertMove : m_pQBertMoveComps)
    {
        if (qbertMove == nullptr || qbertMove->GetOwner()->GetTag() == dae::Tag(Tag::PlayerCoily)) continue;

        if(m_pMoveComp->GetGoalTile() == qbertMove->GetCurrentTile()
            || m_pMoveComp->GetCurrentTile() == qbertMove->GetGoalTile())
        {
            glm::vec3 slickSamFootPos{ m_slickSam->GetWorldPosition() + m_pMoveComp->GetFeetPos() };
            glm::vec3 qbertFootPos{ qbertMove->GetOwner()->GetWorldPosition() + qbertMove->GetFeetPos() };

            if (glm::length(slickSamFootPos - qbertFootPos) <= 10.f)
            {
                dae::Event e{ dae::make_sdbm_hash("SAM_SLICK_KILLED") };
                e.args->object = qbertMove->GetOwner();
                m_slickSam->GetSubject()->NotifyObservers(e);
                m_slickSam->IsEnabled(false);
                m_slickSam->MarkForRemoval();
            }
        }
    }
    return nullptr;
}

std::unique_ptr<QBert::SlickSamState> QBert::SlickSamState::OnNotify(dae::Event event, dae::Subject*)
{
    if (event.id == dae::make_sdbm_hash("QBERT_KILLED")
        || event.id == dae::make_sdbm_hash("LEVEL_COMPLETED"))
    {
        return std::make_unique<StunnedSlickSamState>(m_slickSam, m_pConnSprite, m_pMoveComp, m_pConnLevel, m_pQBertMoveComps);
    }

    return nullptr;
}