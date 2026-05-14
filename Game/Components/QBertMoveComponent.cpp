#include "QBertMoveComponent.h"
#include "DeltaTime.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "LevelBase.h"
#include "TileComponent.h"
#include "ServiceLocator.h"
#include "Sounds.h"
#include "Events.h"

QBert::QBertMoveComp::QBertMoveComp(dae::GameObject* pOwner, const glm::vec3& feetPos, const glm::vec2& startTile, bool canTurn, bool canRevert)
	: dae::Component(pOwner)
	, m_currentTile(startTile)
	, m_goalTile(startTile)
	, m_QBertFeetPos(feetPos)
	, m_canTurnTiles(canTurn)
	, m_canRevertTiles(canRevert)
{ 
	m_pConnLevelComp = dae::SceneManager::GetInstance().GetActiveScene()->GetFirstObjectByType<QBert::LevelBase>();
	m_goalPos = glm::vec3(m_pConnLevelComp->GetTile(static_cast<int>(startTile.x), static_cast<int>(startTile.y))->GetMiddlePos());
	glm::vec3 pos{ m_goalPos.x - m_QBertFeetPos.x, m_goalPos.y - m_QBertFeetPos.y, 0 };
	GetOwner()->SetLocalPosition(pos);

	m_pConnSpriteComp = pOwner->GetComponent<dae::SpriteComp>();
}

void QBert::QBertMoveComp::Update()
{
	glm::vec3 dir{ glm::vec3(m_goalPos.x, m_goalPos.y, 0) 
		- (GetOwner()->GetWorldPosition() + m_QBertFeetPos) };

	float dist{ glm::length(dir) };
	if (dist >= 5.f)
	{
		dir /= dist;
		GetOwner()->SetLocalPosition(GetOwner()->GetLocalPosition() + (dir * 200.f * dae::DeltaTime::GetInstance().GetDeltaTime()));
	}
	else if (m_currentTile != m_goalTile)
	{
		m_currentTile = m_goalTile;

		if (m_canTurnTiles &&
			m_pConnLevelComp->TurnTile(static_cast<int>(m_currentTile.x), static_cast<int>(m_currentTile.y)))
		{
			dae::Event e(dae::make_sdbm_hash("TILE_TURNED"));
			GetOwner()->GetSubject()->NotifyObservers(e);
		}
		else if (m_canRevertTiles)
		{
			m_pConnLevelComp->RevertTile(static_cast<int>(m_currentTile.x), static_cast<int>(m_currentTile.y));
		}

		int soundId{ rand() % 4 };
		switch (soundId)
		{
		case 0:
			dae::ServiceLocator::GetSoundSystem().Play(dae::SoundId(QBert::Sound::Jump1), 0.5f);
			break;
		case 1:
			dae::ServiceLocator::GetSoundSystem().Play(dae::SoundId(QBert::Sound::Jump2), 0.5f);
			break;
		case 2:
			dae::ServiceLocator::GetSoundSystem().Play(dae::SoundId(QBert::Sound::Jump3), 0.5f);
			break;
		case 3:
			dae::ServiceLocator::GetSoundSystem().Play(dae::SoundId(QBert::Sound::Jump4), 0.3f);
			break;
		default:
			break;
		}
	}
}

void QBert::QBertMoveComp::Move(const glm::vec3& direction)
{
	if (m_currentTile != m_goalTile) return;

	glm::vec2 moveDir{ static_cast<int>(direction.x), static_cast<int>(direction.y) };
	QBert::TileComp* goalTile = m_pConnLevelComp->GetTile(static_cast<int>(m_currentTile.x + moveDir.x), static_cast<int>(m_currentTile.y + moveDir.y));
	if (goalTile)
	{
		m_goalPos = goalTile->GetMiddlePos();
		m_goalTile = glm::vec2(static_cast<int>(m_currentTile.x + moveDir.x), static_cast<int>(m_currentTile.y + moveDir.y));
	}
}

std::type_index QBert::QBertMoveComp::GetType() const
{
	return typeid(QBertMoveComp);
}