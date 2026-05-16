#include "CoilyStates.h"
#include "DeltaTime.h"

QBert::FallingSnakeState::FallingSnakeState(dae::GameObject* coily, dae::SpriteComp* spriteComp,
	QBertMoveComp* moveComp, LevelBase* level)
	: CoilyState(coily, spriteComp, moveComp, level)
{ }

std::unique_ptr<QBert::CoilyState> QBert::FallingSnakeState::Update()
{
	m_coily->SetLocalPosition(m_coily->GetLocalPosition() + glm::vec3(0, 370.f, 0) * dae::DeltaTime::GetInstance().GetDeltaTime());

	m_secPassed += dae::DeltaTime::GetInstance().GetDeltaTime();
	if (m_secPassed >= m_maxSec)
		m_coily->IsEnabled(false);

	return nullptr;
}