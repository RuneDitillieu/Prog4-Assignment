#include "QBertStates.h"
#include "DeltaTime.h"
#include "ServiceLocator.h"
#include "Sounds.h"
#include "QBertMoveComponent.h"
#include "HealthComponent.h"
#include "LevelBase.h"

QBert::FallingQBertState::FallingQBertState(dae::GameObject* qbert, dae::SpriteComp* spriteComp, QBert::QBertMoveComp* moveComp, LevelBase* level)
	: QBertState(qbert, spriteComp, moveComp, level)
{ }

void QBert::FallingQBertState::OnEnter()
{
	m_pMoveComp->m_isEnabled = false;
	dae::ServiceLocator::GetSoundSystem().Play(dae::SoundId(QBert::Sound::QBertFall));
}

void QBert::FallingQBertState::OnExit()
{
	//m_qbert->SetLocalPosition(m_pConnLevel->GetMiddlePosOfTile(0, 0) + m_pMoveComp->GetFeetPos());
	m_pMoveComp->m_isEnabled = true;
	m_pMoveComp->Reset(glm::vec2(0, 0));
	m_qbert->GetComponent<dae::HealthComponent>()->LoseLife();
}

std::unique_ptr<QBert::QBertState> QBert::FallingQBertState::Update()
{
	m_qbert->SetLocalPosition(m_qbert->GetLocalPosition() + glm::vec3(0, 370.f, 0) * dae::DeltaTime::GetInstance().GetDeltaTime());
	if (m_qbert->GetWorldPosition().y >= 800)
	{
		return std::make_unique<QBert::IdleQBertState>(m_qbert, m_pConnSpriteComp, m_pMoveComp, m_pConnLevel);
	}

	return nullptr;
}