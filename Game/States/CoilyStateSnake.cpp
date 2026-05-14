#include "CoilyStates.h"
#include "DeltaTime.h"

QBert::SnakeState::SnakeState(dae::GameObject* coily, dae::SpriteComp* spriteComp)
	: CoilyState(coily, spriteComp)
{
}

void QBert::SnakeState::OnEnter()
{
	m_pConnSprite->SetCurFrame(2);
}

std::unique_ptr<QBert::CoilyState> QBert::SnakeState::Update()
{
	return nullptr;
}