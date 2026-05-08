#include "CoilyStates.h"
#include "DeltaTime.h"
#include "RenderComponent.h"
#include "SpriteComponent.h"

QBert::SnakeState::SnakeState(dae::GameObject* coily)
	: CoilyState(coily)
{
}

void QBert::SnakeState::OnEnter()
{
	auto rc = m_coily->GetComponent<dae::RenderComponent>();
	rc->SetTexture("CoilySprites.png");
	m_coily->GetComponent<dae::SpriteComp>()->Set(8, 1, rc->GetSize().x / 8, rc->GetSize().y, glm::vec2(0, 0), false);
}

std::unique_ptr<QBert::CoilyState> QBert::SnakeState::Update()
{
	return std::make_unique<CoilyState>(m_coily);
}

std::type_index QBert::SnakeState::GetType() const
{
	return typeid(SnakeState);
}