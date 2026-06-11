#include "QBertStates.h"
#include "HealthComponent.h"
#include "DeltaTime.h"
#include "Events.h"
#include "QBertMoveComponent.h"
#include "ServiceLocator.h"
#include "Sounds.h"

QBert::FrozenQBertState::FrozenQBertState(dae::GameObject* qbert, dae::SpriteComp* spriteComp,
    QBert::QBertMoveComp* moveComp, LevelBase* level)
    : QBertState(qbert, spriteComp, moveComp, level)
{ }

void QBert::FrozenQBertState::OnEnter()
{
    m_pMoveComp->m_isEnabled = false;
}

void QBert::FrozenQBertState::OnExit()
{
    m_pConnSpriteComp->SetCurFrame(7);
    m_pMoveComp->m_isEnabled = true;
    m_qbert->GetChildAt(0)->IsEnabled(false);
}

std::unique_ptr<QBert::QBertState> QBert::FrozenQBertState::Update()
{
    m_secPassed += dae::DeltaTime::GetInstance().GetDeltaTime();
    if (m_secPassed >= m_maxSec)
    {
        return std::make_unique<IdleQBertState>(m_qbert, m_pConnSpriteComp, m_pMoveComp, m_pConnLevel);
    }

    return nullptr;
}