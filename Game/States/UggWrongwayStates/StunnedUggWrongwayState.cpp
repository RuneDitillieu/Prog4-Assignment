#include "DeltaTime.h"
#include "UggWrongwayStates.h"

#include "QBertMoveComponent.h"

QBert::StunnedUggWrongwayState::StunnedUggWrongwayState(dae::GameObject* uggWrongway, dae::SpriteComp* spriteComp,
    QBertMoveComp* moveComp, LevelBase* level, QBertMoveComp* qbertMoveComp, bool goesRight)
        : UggWrongwayState(uggWrongway, spriteComp, moveComp, level, qbertMoveComp, goesRight)
{ }

void QBert::StunnedUggWrongwayState::OnEnter()
{
    m_pMoveComp->m_isEnabled = false;
}

std::unique_ptr<QBert::UggWrongwayState> QBert::StunnedUggWrongwayState::Update()
{
    m_secPassed += dae::DeltaTime::GetInstance().GetDeltaTime();
    if (m_secPassed >= m_maxSec)
    {
        m_uggWrongway->MarkForRemoval();
        m_uggWrongway->IsEnabled(false);
    }

    return nullptr;
}
