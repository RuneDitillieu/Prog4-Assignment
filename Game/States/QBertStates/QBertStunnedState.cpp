#include "QBertStates.h"
#include "HealthComponent.h"
#include "DeltaTime.h"
#include "Events.h"

QBert::StunnedQBertState::StunnedQBertState(dae::GameObject* qbert, dae::SpriteComp* spriteComp,
	QBert::QBertMoveComp* moveComp, LevelBase* level)
	: QBertState(qbert, spriteComp, moveComp, level)
{ }

void QBert::StunnedQBertState::OnEnter()
{
	/*m_qbert->GetComponent<dae::HealthComponent>()->LoseLife();
	dae::Event e{ dae::make_sdbm_hash("QBERT_KILLED") };
	m_qbert->GetSubject()->NotifyObservers(e);*/
}

void QBert::StunnedQBertState::OnExit()
{
	m_pConnSpriteComp->SetCurFrame(7);
}

std::unique_ptr<QBert::QBertState> QBert::StunnedQBertState::Update()
{
	if (m_isFirstFrame)
	{
		// done like this instead of in OnEnter bc otherwise the enemy that hit us calls notify on us, 
		// and then gets called OnNotify by us in the same frame,
		// resulting in the deletion of that enemy's state before it exits its update function

		m_qbert->GetComponent<dae::HealthComponent>()->LoseLife();
		dae::Event e{ dae::make_sdbm_hash("QBERT_KILLED") };
		m_qbert->GetSubject()->NotifyObservers(e);

		m_isFirstFrame = false;
	}

	m_secPassed += dae::DeltaTime::GetInstance().GetDeltaTime();
	if (m_secPassed >= m_maxSec)
	{
		return std::make_unique<QBert::IdleQBertState>(m_qbert, m_pConnSpriteComp, m_pMoveComp, m_pConnLevel);
	}

	return nullptr;
}