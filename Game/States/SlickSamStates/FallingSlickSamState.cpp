#include "SlickSamStates.h"
#include "DeltaTime.h"
#include "SceneManager.h"
#include "Scene.h"

QBert::FallingSlickSamState::FallingSlickSamState(dae::GameObject* slickSam, dae::SpriteComp* spriteComp,
    QBertMoveComp* moveComp, LevelBase* level, const std::vector<QBertMoveComp*>& qbertMoveComps)
    : SlickSamState(slickSam, spriteComp, moveComp, level, qbertMoveComps)
{ }

void QBert::FallingSlickSamState::OnEnter()
{
    m_slickSam->SetRenderPriority(0);
    dae::SceneManager::GetInstance().GetActiveScene()->RequestReorderObjects();
}

std::unique_ptr<QBert::SlickSamState> QBert::FallingSlickSamState::Update()
{
    m_slickSam->SetLocalPosition(m_slickSam->GetLocalPosition() + glm::vec3(0, 370.f, 0) * dae::DeltaTime::GetInstance().GetDeltaTime());

    if (m_slickSam->GetLocalPosition().y >= 800)
    {
        m_slickSam->IsEnabled(false);
        m_slickSam->MarkForRemoval();
    }

    return nullptr;
}