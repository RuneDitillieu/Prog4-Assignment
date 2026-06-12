#include "DeltaTime.h"
#include "SlickSamStates.h"

#include "QBertMoveComponent.h"

QBert::StunnedSlickSamState::StunnedSlickSamState(dae::GameObject* slickSam, dae::SpriteComp* spriteComp,
    QBertMoveComp* moveComp, LevelBase* level, const std::vector<QBertMoveComp*>& qbertMoveComps)
        : SlickSamState(slickSam, spriteComp, moveComp, level, qbertMoveComps)
{ }

void QBert::StunnedSlickSamState::OnEnter()
{
    m_pMoveComp->m_isEnabled = false;
}

std::unique_ptr<QBert::SlickSamState> QBert::StunnedSlickSamState::Update()
{
    m_secPassed += dae::DeltaTime::GetInstance().GetDeltaTime();
    if (m_secPassed >= m_maxSec)
    {
        m_slickSam->MarkForRemoval();
        m_slickSam->IsEnabled(false);
    }

    return nullptr;
}
