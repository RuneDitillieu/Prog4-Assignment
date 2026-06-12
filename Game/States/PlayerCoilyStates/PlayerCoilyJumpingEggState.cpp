#include "PlayerCoilyStates.h"
#include "QBertMoveComponent.h"
#include "LevelBase.h"

QBert::PlayerJumpingEggState::PlayerJumpingEggState(dae::GameObject* coily, dae::SpriteComp* spriteComp, QBertMoveComp* moveComp,
	LevelBase* level, QBertMoveComp* qbertMoveComp)
	: PlayerCoilyState(coily, spriteComp, moveComp, level, qbertMoveComp)
{ }

void QBert::PlayerJumpingEggState::OnEnter()
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

std::unique_ptr<QBert::PlayerCoilyState> QBert::PlayerJumpingEggState::Update()
{
	PlayerCoilyState::Update();

	if (!m_pMoveComp->IsMoving())
	{
		return std::make_unique<PlayerIdleEggState>(m_coily, m_pConnSprite, m_pMoveComp, m_pConnLevel, m_pQBertMoveComp);
	}

	return nullptr;
}