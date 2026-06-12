#include "PlayerCoilyStates.h"
#include "DeltaTime.h"

#include "QBertMoveComponent.h"
#include "LevelBase.h"

QBert::PlayerDroppingEggState::PlayerDroppingEggState(dae::GameObject* coily, dae::SpriteComp* spriteComp,
    QBertMoveComp* moveComp, LevelBase* level, QBertMoveComp* qbertMoveComp)
        : PlayerCoilyState(coily, spriteComp, moveComp, level, qbertMoveComp)
{ }

void QBert::PlayerDroppingEggState::OnEnter()
{
    int randNr = rand() % 2;
    glm::vec2 spawnTile{0, 0};
    if (randNr == 0)
    {
        spawnTile.x = 1;
    }
    else
    {
        spawnTile.y = 1;
    }

    m_pMoveComp->Reset(spawnTile, false);

    m_pConnSprite->SetCurFrame(1);
    m_coily->SetLocalPosition(m_coily->GetLocalPosition() + glm::vec3(0, -200, 0));
    m_pMoveComp->m_isEnabled = false;
}

std::unique_ptr<QBert::PlayerCoilyState> QBert::PlayerDroppingEggState::Update()
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
        return std::make_unique<PlayerIdleEggState>(m_coily, m_pConnSprite, m_pMoveComp, m_pConnLevel, m_pQBertMoveComp);
    }

    return nullptr;
}
