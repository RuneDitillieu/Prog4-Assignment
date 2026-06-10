#include "UggWrongwayStates.h"

#include "QBertMoveComponent.h"

QBert::JumpingUggWrongwayState::JumpingUggWrongwayState(dae::GameObject* uggWrongway, dae::SpriteComp* spriteComp,
    QBertMoveComp* moveComp, LevelBase* level, QBertMoveComp* qbertMoveComp, bool goesRight)
        : UggWrongwayState(uggWrongway, spriteComp, moveComp, level, qbertMoveComp, goesRight)
{}

void QBert::JumpingUggWrongwayState::OnEnter()
{
    int randNr{ rand() % 2 };
    glm::vec3 moveDir{ 0, 0, 0 };

    if (m_goesRight)
    {
        if (randNr == 0)
        {
            moveDir.x = 1;
            moveDir.y = -1;
            m_pConnSprite->SetCurFrame(2);
        }
        else
        {
            moveDir.y = -1;
            m_pConnSprite->SetCurFrame(6);
        }
    }
    else
    {
        if (randNr == 0)
        {
            moveDir.x = -1;
            moveDir.y = 1;
            m_pConnSprite->SetCurFrame(6);
        }
        else
        {
            moveDir.x = -1;
            m_pConnSprite->SetCurFrame(2);
        }
    }

    m_pMoveComp->Move(moveDir);
}

std::unique_ptr<QBert::UggWrongwayState> QBert::JumpingUggWrongwayState::Update()
{
    UggWrongwayState::Update();

    if (!m_pMoveComp->IsMoving())
    {
        return std::make_unique<IdleUggWrongwayState>(m_uggWrongway, m_pConnSprite,
            m_pMoveComp, m_pConnLevel, m_pQBertMoveComp, m_goesRight);
    }

    return nullptr;
}

std::unique_ptr<QBert::UggWrongwayState> QBert::JumpingUggWrongwayState::OnNotify(dae::Event event, dae::Subject* subject)
{
    auto state = UggWrongwayState::OnNotify(event, subject);
    if (state != nullptr) return state;

    if (event.id == dae::make_sdbm_hash("ACTOR_FELL")
        && subject == m_uggWrongway->GetSubject())
    {
        m_pMoveComp->m_isEnabled = false;
        return std::make_unique<FallingUggWrongwayState>(m_uggWrongway, m_pConnSprite,
            m_pMoveComp, m_pConnLevel, m_pQBertMoveComp, m_goesRight);
    }

    return nullptr;
}
