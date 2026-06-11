#include "UggWrongwayStates.h"

#include "QBertMoveComponent.h"

QBert::UggWrongwayState::UggWrongwayState(dae::GameObject* uggWrongway, dae::SpriteComp* spriteComp,
    QBertMoveComp* moveComp, LevelBase* level, QBertMoveComp* qbertMoveComp, bool goesRight)
    : m_uggWrongway(uggWrongway), m_pConnSprite(spriteComp), m_pMoveComp(moveComp), m_pConnLevel(level),
    m_pQBertMoveComp(qbertMoveComp), m_goesRight(goesRight)
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

    if(m_pMoveComp->GetGoalTile() + offset == m_pQBertMoveComp->GetCurrentTile()
            || m_pMoveComp->GetCurrentTile() + offset == m_pQBertMoveComp->GetGoalTile())
    {
        glm::vec3 uggWrongwayFootPos{ m_uggWrongway->GetWorldPosition() /*+ m_pMoveComp->GetFeetPos()*/ };
        glm::vec3 qbertFootPos{ m_pQBertMoveComp->GetOwner()->GetWorldPosition() /*+ m_pQBertMoveComp->GetFeetPos()*/ };

        if (glm::length(uggWrongwayFootPos - qbertFootPos) <= 10.f)
        {
            dae::Event e{ dae::make_sdbm_hash("ACTOR_HIT") };
            e.args->object = m_pQBertMoveComp->GetOwner();
            m_uggWrongway->GetSubject()->NotifyObservers(e);
            return std::make_unique<StunnedUggWrongwayState>(m_uggWrongway, m_pConnSprite, m_pMoveComp,
                m_pConnLevel, m_pQBertMoveComp, m_goesRight);
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
            m_pMoveComp, m_pConnLevel, m_pQBertMoveComp, m_goesRight);
    }

    return nullptr;
}