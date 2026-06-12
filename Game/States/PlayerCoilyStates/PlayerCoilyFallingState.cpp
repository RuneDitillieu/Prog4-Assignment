#include "PlayerCoilyStates.h"
#include "DeltaTime.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ServiceLocator.h"
#include "Sounds.h"
#include "SoundSystem.h"

QBert::PlayerFallingSnakeState::PlayerFallingSnakeState(dae::GameObject* coily, dae::SpriteComp* spriteComp,
    QBertMoveComp* moveComp, LevelBase* level, QBertMoveComp* qbertMoveComp)
	: PlayerCoilyState(coily, spriteComp, moveComp, level, qbertMoveComp)
{ }

void QBert::PlayerFallingSnakeState::OnEnter()
{
	m_coily->SetRenderPriority(0);
	dae::SceneManager::GetInstance().GetActiveScene()->RequestReorderObjects();
	dae::ServiceLocator::GetSoundSystem().Play(dae::SoundId(Sound::CoilyFall));
}

void QBert::PlayerFallingSnakeState::OnExit()
{
	m_coily->SetRenderPriority(2);
}

std::unique_ptr<QBert::PlayerCoilyState> QBert::PlayerFallingSnakeState::Update()
{
	m_coily->SetLocalPosition(m_coily->GetLocalPosition() + glm::vec3(0, 370.f, 0) * dae::DeltaTime::GetInstance().GetDeltaTime());

	if (m_coily->GetLocalPosition().y >= 800)
	{
		dae::Event e{ dae::make_sdbm_hash("COILY_KILLED") };
		m_coily->GetSubject()->NotifyObservers(e);
		return std::make_unique<PlayerDroppingEggState>(m_coily, m_pConnSprite, m_pMoveComp, m_pConnLevel, m_pQBertMoveComp);
	}

	return nullptr;
}