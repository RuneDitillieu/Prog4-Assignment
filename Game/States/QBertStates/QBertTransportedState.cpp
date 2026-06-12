#include "QBertStates.h"
#include "QBertMoveComponent.h"
#include "ServiceLocator.h"
#include "Sounds.h"

QBert::TransportedQBertState::TransportedQBertState(dae::GameObject* qbert, dae::SpriteComp* spriteComp,
	QBertMoveComp* moveComp, LevelBase* level)
	: QBertState(qbert, spriteComp, moveComp, level)
{ }

void QBert::TransportedQBertState::OnEnter()
{
	m_pMoveComp->m_isEnabled = false;
	dae::ServiceLocator::GetSoundSystem().Play(dae::SoundId(Sound::Lift), 0.5f);
}

void QBert::TransportedQBertState::OnExit()
{
	m_qbert->SetParent(nullptr, true);
}

std::unique_ptr<QBert::QBertState> QBert::TransportedQBertState::OnNotify(dae::Event event, dae::Subject*)
{
	if (event.id == dae::make_sdbm_hash("ACTOR_DROPPED"))
	{
		return std::make_unique<DroppingQBertState>(m_qbert, m_pConnSpriteComp, m_pMoveComp, m_pConnLevel);
	}

	return nullptr;
}