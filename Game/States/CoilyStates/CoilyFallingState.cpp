#include "CoilyStates.h"
#include "DeltaTime.h"
#include "SceneManager.h"
#include "Scene.h"

QBert::FallingSnakeState::FallingSnakeState(dae::GameObject* coily, dae::SpriteComp* spriteComp,
	QBertMoveComp* moveComp, LevelBase* level, const std::vector<QBertMoveComp*>& qbertMoveComps)
	: CoilyState(coily, spriteComp, moveComp, level, qbertMoveComps)
{ }

void QBert::FallingSnakeState::OnEnter()
{
	m_coily->SetRenderPriority(0);
	dae::SceneManager::GetInstance().GetActiveScene()->RequestReorderObjects();
}

std::unique_ptr<QBert::CoilyState> QBert::FallingSnakeState::Update()
{
	m_coily->SetLocalPosition(m_coily->GetLocalPosition() + glm::vec3(0, 370.f, 0) * dae::DeltaTime::GetInstance().GetDeltaTime());

	if (m_coily->GetLocalPosition().y >= 800)
	{
		m_coily->IsEnabled(false);
		dae::Event e{ dae::make_sdbm_hash("COILY_KILLED") };
		m_coily->GetSubject()->NotifyObservers(e);
		m_coily->MarkForRemoval();
	}

	return nullptr;
}