#include "CoilyStates.h"
#include "DeltaTime.h"

QBert::EggState::EggState(dae::GameObject* coily, dae::SpriteComp* spriteComp)
	: CoilyState(coily, spriteComp)
{}

void QBert::EggState::OnEnter()
{
	m_pConnSprite->SetCurFrame(0);
	m_goalLocation = m_coily->GetLocalPosition() + glm::vec3(0, 50.f, 0);
}

std::unique_ptr<QBert::CoilyState> QBert::EggState::Update()
{
	m_coily->SetLocalPosition(m_coily->GetLocalPosition() + glm::vec3(0, 15.f * dae::DeltaTime::GetInstance().GetDeltaTime(), 0));

	if (glm::length(m_coily->GetLocalPosition() - m_goalLocation) < 10.f)
	{
		return std::make_unique<SnakeState>(m_coily, m_pConnSprite);
	}
	else
	{
		return nullptr;
	}
}