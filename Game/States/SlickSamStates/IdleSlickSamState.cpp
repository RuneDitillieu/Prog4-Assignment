#include "DeltaTime.h"
#include "LevelBase.h"
#include "QBertMoveComponent.h"
#include "SlickSamStates.h"
#include "SpriteComponent.h"

QBert::IdleSlickSamState::IdleSlickSamState(dae::GameObject* slickSam, dae::SpriteComp* spriteComp,
    QBertMoveComp* moveComp, LevelBase* level, const std::vector<QBertMoveComp*>& qbertMoveComps)
        : SlickSamState(slickSam, spriteComp, moveComp, level, qbertMoveComps)
{}

void QBert::IdleSlickSamState::OnEnter()
{
    int curFrame{ m_pConnSprite->GetCurFrame() };
    if (curFrame != 0)
    {
        m_pConnSprite->SetCurFrame(curFrame - 2);
    }
}

std::unique_ptr<QBert::SlickSamState> QBert::IdleSlickSamState::Update()
{
    SlickSamState::Update();

    m_secPassed += dae::DeltaTime::GetInstance().GetDeltaTime();

    if (m_secPassed >= m_idleSec)
    {
        return std::make_unique<JumpingSlickSamState>(m_slickSam, m_pConnSprite, m_pMoveComp, m_pConnLevel, m_pQBertMoveComps);
    }

    return nullptr;
}