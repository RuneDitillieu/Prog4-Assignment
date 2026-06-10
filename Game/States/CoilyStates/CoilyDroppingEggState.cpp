#include "CoilyStates.h"
#include "DeltaTime.h"

#include "QBertMoveComponent.h"
#include "LevelBase.h"

QBert::DroppingEggState::DroppingEggState(dae::GameObject* coily, dae::SpriteComp* spriteComp,
    QBertMoveComp* moveComp, LevelBase* level, QBertMoveComp* qbertMoveComp)
        : CoilyState(coily, spriteComp, moveComp, level, qbertMoveComp)
{ }

void QBert::DroppingEggState::OnEnter()
{
    m_pConnSprite->SetCurFrame(1);
    m_coily->SetLocalPosition(m_coily->GetLocalPosition() + glm::vec3(0, -200, 0));
    m_pMoveComp->m_isEnabled = false;
}

std::unique_ptr<QBert::CoilyState> QBert::DroppingEggState::Update()
{
    m_coily->SetLocalPosition(m_coily->GetLocalPosition() + glm::vec3(0, 300.f, 0) * dae::DeltaTime::GetInstance().GetDeltaTime());

    glm::vec3 coilyPos{ m_coily->GetLocalPosition() + m_pMoveComp->GetFeetPos() };
    int col{ static_cast<int>(m_pMoveComp->GetCurrentTile().x) };
    int row{ static_cast<int>(m_pMoveComp->GetCurrentTile().y) };
    glm::vec3 tilePos{ m_pConnLevel->GetMiddlePosOfTile(col, row) };

    if (glm::length(coilyPos - tilePos) <= 5.f)
    {
        m_pMoveComp->m_isEnabled = true;
        m_pMoveComp->Reset(glm::vec2(col, row), false);
        return std::make_unique<IdleEggState>(m_coily, m_pConnSprite, m_pMoveComp, m_pConnLevel, m_pQBertMoveComp);
    }

    return nullptr;
}
