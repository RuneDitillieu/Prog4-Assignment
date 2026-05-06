#include "CoilyStates.h"
#include "DeltaTime.h"
#include "RenderComponent.h"

QBert::SnakeState::SnakeState(dae::GameObject* coily)
	: CoilyState(coily)
{
}

void QBert::SnakeState::OnEnter()
{
	m_coily->GetComponent<dae::RenderComponent>()->SetTexture("CoilySnake.png");
}

std::unique_ptr<QBert::CoilyState> QBert::SnakeState::Update()
{
	return std::make_unique<CoilyState>(m_coily);
}

std::type_index QBert::SnakeState::GetType() const
{
	return typeid(SnakeState);
}