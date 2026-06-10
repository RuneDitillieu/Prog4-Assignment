#include "DeltaTime.h"
#include "SceneManager.h"
#include "UggWrongwayStates.h"

QBert::FallingUggWrongwayState::FallingUggWrongwayState(dae::GameObject* uggWrongway, dae::SpriteComp* spriteComp,
    QBertMoveComp* moveComp, LevelBase* level, QBertMoveComp* qbertMoveComp, bool goesRight)
        : UggWrongwayState(uggWrongway, spriteComp, moveComp, level, qbertMoveComp, goesRight)
{ }

void QBert::FallingUggWrongwayState::OnEnter()
{
}

std::unique_ptr<QBert::UggWrongwayState> QBert::FallingUggWrongwayState::Update()
{
    glm::vec3 fallingDir{ 0, -100, 0 };
    if (m_goesRight)
    {
        fallingDir.x = 370.f;
    }
    else
    {
        fallingDir.x = -370.f;
    }

    m_uggWrongway->SetLocalPosition(m_uggWrongway->GetLocalPosition() + fallingDir *
        dae::DeltaTime::GetInstance().GetDeltaTime());

    if (m_uggWrongway->GetLocalPosition().y >= 800)
    {
        m_uggWrongway->IsEnabled(false);
        m_uggWrongway->MarkForRemoval();
    }

    return nullptr;
}
