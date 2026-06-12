#include "UggWrongwayStates.h"

#include "QBertMoveComponent.h"
#include "Tags.h"

QBert::UggWrongwayState::UggWrongwayState(dae::GameObject* uggWrongway, dae::SpriteComp* spriteComp,
    QBertMoveComp* moveComp, LevelBase* level, const std::vector<QBertMoveComp*>& qbertMoveComps, bool goesRight)
    : m_uggWrongway(uggWrongway), m_pConnSprite(spriteComp), m_pMoveComp(moveComp), m_pConnLevel(level),
    m_pQBertMoveComps(qbertMoveComps), m_goesRight(goesRight)
{ }

std::unique_ptr<QBert::UggWrongwayState> QBert::UggWrongwayState::Update()
{
    glm::vec2 offset{0, 0};
    if (m_goesRight)
    {
        offset.y = 1;
    }
    else
    {
        offset.x = 1;
    }

    for (auto qbertMove : m_pQBertMoveComps)
    {
        if (qbertMove == nullptr || qbertMove->GetOwner()->GetTag() == dae::Tag(Tag::PlayerCoily)) continue;

        if(m_pMoveComp->GetGoalTile() + offset == qbertMove->GetCurrentTile()
                || m_pMoveComp->GetCurrentTile() + offset == qbertMove->GetGoalTile())
        {
            glm::vec3 uggWrongwayFootPos{ m_uggWrongway->GetWorldPosition() };
            glm::vec3 qbertFootPos{ qbertMove->GetOwner()->GetWorldPosition() };

            if (glm::length(uggWrongwayFootPos - qbertFootPos) <= 10.f)
            {
                dae::Event e{ dae::make_sdbm_hash("ACTOR_HIT") };
                e.args->object = qbertMove->GetOwner();
                m_uggWrongway->GetSubject()->NotifyObservers(e);
                return std::make_unique<StunnedUggWrongwayState>(m_uggWrongway, m_pConnSprite, m_pMoveComp,
                    m_pConnLevel, m_pQBertMoveComps, m_goesRight);
            }
        }
    }

    return nullptr;
}

std::unique_ptr<QBert::UggWrongwayState> QBert::UggWrongwayState::OnNotify(dae::Event event, dae::Subject*)
{
    if (event.id == dae::make_sdbm_hash("QBERT_KILLED")
        || event.id == dae::make_sdbm_hash("LEVEL_COMPLETED"))
    {
        return std::make_unique<StunnedUggWrongwayState>(m_uggWrongway, m_pConnSprite,
            m_pMoveComp, m_pConnLevel, m_pQBertMoveComps, m_goesRight);
    }

    return nullptr;
}