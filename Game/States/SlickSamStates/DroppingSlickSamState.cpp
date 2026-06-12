#include "SlickSamStates.h"

#include "DeltaTime.h"

#include "QBertMoveComponent.h"
#include "LevelBase.h"

QBert::DroppingSlickSamState::DroppingSlickSamState(dae::GameObject* coily, dae::SpriteComp* spriteComp,
    QBertMoveComp* moveComp, LevelBase* level, const std::vector<QBertMoveComp*>& qbertMoveComps)
        : SlickSamState(coily, spriteComp, moveComp, level, qbertMoveComps)
{ }

void QBert::DroppingSlickSamState::OnEnter()
{
    m_pConnSprite->SetCurFrame(2);
    m_slickSam->SetLocalPosition(m_slickSam->GetLocalPosition() + glm::vec3(0, -200, 0));
    m_pMoveComp->m_isEnabled = false;
}

std::unique_ptr<QBert::SlickSamState> QBert::DroppingSlickSamState::Update()
{
    m_slickSam->SetLocalPosition(m_slickSam->GetLocalPosition() + glm::vec3(0, 300.f, 0)
        * dae::DeltaTime::GetInstance().GetDeltaTime());

    glm::vec3 coilyPos{ m_slickSam->GetLocalPosition() + m_pMoveComp->GetFeetPos() };
    int col{ static_cast<int>(m_pMoveComp->GetCurrentTile().x) };
    int row{ static_cast<int>(m_pMoveComp->GetCurrentTile().y) };
    glm::vec3 tilePos{ m_pConnLevel->GetMiddlePosOfTile(col, row) };

    if (glm::length(coilyPos - tilePos) <= 5.f)
    {
        m_pMoveComp->m_isEnabled = true;
        m_pMoveComp->Reset(glm::vec2(col, row), true);
        return std::make_unique<IdleSlickSamState>(m_slickSam, m_pConnSprite, m_pMoveComp, m_pConnLevel, m_pQBertMoveComps);
    }

    return nullptr;
}
