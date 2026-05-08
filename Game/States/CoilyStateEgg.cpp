#include "CoilyStates.h"
#include "RenderComponent.h"
#include "DeltaTime.h"

QBert::EggState::EggState(dae::GameObject* coily)
	: CoilyState(coily)
{}

void QBert::EggState::OnEnter()
{
	m_coily->GetComponent<dae::RenderComponent>()->SetTexture("Eggs.png");
	m_goalLocation = m_coily->GetLocalPosition() + glm::vec3(0, 50.f, 0);
}

std::unique_ptr<QBert::CoilyState> QBert::EggState::Update()
{
	m_coily->SetLocalPosition(m_coily->GetLocalPosition() + glm::vec3(0, 15.f * dae::DeltaTime::GetInstance().GetDeltaTime(), 0));

	if (glm::length(m_coily->GetLocalPosition() - m_goalLocation) < 10.f)
	{
		return std::make_unique<SnakeState>(m_coily);
	}
	else
	{
		return std::make_unique<CoilyState>(m_coily);
	}
}

std::type_index QBert::EggState::GetType() const
{
	return typeid(EggState);
}