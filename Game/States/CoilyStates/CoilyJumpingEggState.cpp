#include "CoilyStates.h"
#include "QBertMoveComponent.h"
#include "LevelBase.h"

QBert::JumpingEggState::JumpingEggState(dae::GameObject* coily, dae::SpriteComp* spriteComp, QBertMoveComp* moveComp, LevelBase* level)
	: CoilyState(coily, spriteComp)
	, m_pMoveComp(moveComp)
	, m_pConnLevel(level)
{ }

void QBert::JumpingEggState::OnEnter()
{
	int randNr{ rand() % 2 };
	glm::vec3 moveDir{ 0, 0, 0 };

	if (randNr == 0)
		moveDir.x = 1;
	else
		moveDir.y = 1;

	m_pMoveComp->Move(moveDir);

	m_pConnSprite->SetCurFrame(1);
}

std::unique_ptr<QBert::CoilyState> QBert::JumpingEggState::Update()
{
	if (!m_pMoveComp->IsMoving())
	{
		return std::make_unique<QBert::IdleEggState>(m_coily, m_pConnSprite, m_pMoveComp, m_pConnLevel);
	}

	return nullptr;
}