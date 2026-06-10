#include "CoilyStates.h"
#include "DeltaTime.h"
#include "QBertMoveComponent.h"

QBert::StunnedCoilyState::StunnedCoilyState(dae::GameObject* coily, dae::SpriteComp* spriteComp,
	QBertMoveComp* moveComp, LevelBase* level, QBertMoveComp* qbertMoveComp)
	: CoilyState(coily, spriteComp, moveComp, level, qbertMoveComp)
{ }

void QBert::StunnedCoilyState::OnEnter()
{
	m_pMoveComp->m_isEnabled = false;
}

std::unique_ptr<QBert::CoilyState> QBert::StunnedCoilyState::Update()
{
	m_secPassed += dae::DeltaTime::GetInstance().GetDeltaTime();
	if (m_secPassed >= m_maxSec)
	{
		m_coily->MarkForRemoval();
		m_coily->IsEnabled(false);
	}

	return nullptr;
}