#include "DeltaTime.h"
#include "UggWrongwayStates.h"

QBert::IdleUggWrongwayState::IdleUggWrongwayState(dae::GameObject* uggWrongway, dae::SpriteComp* spriteComp,
    QBertMoveComp* moveComp, LevelBase* level, QBertMoveComp* qbertMoveComp, bool goesRight)
        : UggWrongwayState(uggWrongway, spriteComp, moveComp, level, qbertMoveComp, goesRight)
{ }

void QBert::IdleUggWrongwayState::OnEnter()
{
    int curFrame{ m_pConnSprite->GetCurFrame() };
    if (curFrame != 0)
    {
        m_pConnSprite->SetCurFrame(curFrame - 2);
    }
}

std::unique_ptr<QBert::UggWrongwayState> QBert::IdleUggWrongwayState::Update()
{
    UggWrongwayState::Update();

    m_secPassed += dae::DeltaTime::GetInstance().GetDeltaTime();

    if (m_secPassed >= m_idleSec)
    {
        return std::make_unique<JumpingUggWrongwayState>(m_uggWrongway, m_pConnSprite,
            m_pMoveComp, m_pConnLevel, m_pQBertMoveComp, m_goesRight);
    }

    return nullptr;
}
