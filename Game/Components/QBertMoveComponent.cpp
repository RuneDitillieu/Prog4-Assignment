#include "QBertMoveComponent.h"
#include "DeltaTime.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "LevelBase.h"
#include "TileComponent.h"
#include "ServiceLocator.h"
#include "Sounds.h"
#include "Events.h"

QBert::QBertMoveComp::QBertMoveComp(dae::GameObject* pOwner, const glm::vec3& feetPos, const glm::vec2& startTile,
	bool canTurn, bool canRevert, bool canStandOnDisc)
	: dae::Component(pOwner)
	, m_QBertFeetPos(feetPos)
	, m_currentTile(startTile)
	, m_goalTile(startTile)
	, m_canTurnTiles(canTurn)
	, m_canRevertTiles(canRevert)
	, m_canStandOnDisc(canStandOnDisc)
{ 
	m_pConnLevelComp = dae::SceneManager::GetInstance().GetActiveScene()->GetFirstObjectByType<QBert::LevelBase>();
	m_goalPos = glm::vec3(m_pConnLevelComp->GetTile(static_cast<int>(startTile.x), static_cast<int>(startTile.y))->GetMiddlePos());
	glm::vec3 pos{ m_goalPos.x - m_QBertFeetPos.x, m_goalPos.y - m_QBertFeetPos.y, 0 };
	GetOwner()->SetLocalPosition(pos);

	m_pConnSpriteComp = pOwner->GetComponent<dae::SpriteComp>();
}

void QBert::QBertMoveComp::Update()
{
	if (!m_isEnabled) return;

	// calc movement
	glm::vec3 dir{ glm::vec3(m_goalPos.x, m_goalPos.y, 0) 
		- (GetOwner()->GetWorldPosition() + m_QBertFeetPos) };

	float dist{ glm::length(dir) };
	if (dist >= 5.f)
	{
		// move
		dir /= dist;
		GetOwner()->SetLocalPosition(GetOwner()->GetLocalPosition() + (dir * 200.f * dae::DeltaTime::GetInstance().GetDeltaTime()));
	}
	// if goal freshly reached
	else if (m_currentTile != m_goalTile)
	{
		m_currentTile = m_goalTile;

		// change tile if possible
		if(m_pConnLevelComp->GetTileType(static_cast<int>(m_currentTile.x), static_cast<int>(m_currentTile.y))
			== TileType::Tile)
		{
			// turn
			if (m_canTurnTiles &&
				m_pConnLevelComp->TurnTile(static_cast<int>(m_currentTile.x), static_cast<int>(m_currentTile.y)))
			{
				dae::Event e(dae::make_sdbm_hash("TILE_TURNED"));
				GetOwner()->GetSubject()->NotifyObservers(e);
			}
			// revert
			else if (m_canRevertTiles)
			{
				m_pConnLevelComp->RevertTile(static_cast<int>(m_currentTile.x), static_cast<int>(m_currentTile.y));
			}
		}
		// use disc of possible
		else if (m_pConnLevelComp->GetTileType(static_cast<int>(m_currentTile.x), static_cast<int>(m_currentTile.y))
			== TileType::Disc)
		{
			// get transported
			if(m_canTurnTiles)
			{
				dae::Event e(dae::make_sdbm_hash("DISC_USED"));
				e.args->object = m_pConnLevelComp->GetDisc(static_cast<int>(m_currentTile.x), static_cast<int>(m_currentTile.y));
				GetOwner()->GetSubject()->NotifyObservers(e);
			}
			// fall into the void
			else if (!m_canStandOnDisc)
			{
				dae::Event e(dae::make_sdbm_hash("ACTOR_FELL"));
				e.args->object = m_pConnLevelComp->GetDisc(static_cast<int>(m_currentTile.x), static_cast<int>(m_currentTile.y));
				GetOwner()->GetSubject()->NotifyObservers(e);
			}
		}
		// fall into the void
		else if (m_pConnLevelComp->GetTileType(static_cast<int>(m_currentTile.x), static_cast<int>(m_currentTile.y))
			== TileType::Void)
		{
			dae::Event e(dae::make_sdbm_hash("ACTOR_FELL"));
			GetOwner()->GetSubject()->NotifyObservers(e);
		}

		PlayJumpSound();
	}
}

void QBert::QBertMoveComp::Move(const glm::vec3& direction)
{
	// check if can move
	if (!m_isEnabled) return;
	if (m_currentTile != m_goalTile) return;
	if (m_currentTile.x < -0.1f && m_currentTile.y < -0.1f) return;

	// set movement params
	glm::vec2 moveDir{ static_cast<int>(direction.x), static_cast<int>(direction.y) };

	m_goalPos = m_pConnLevelComp->GetMiddlePosOfTile(static_cast<int>(m_currentTile.x + moveDir.x), static_cast<int>(m_currentTile.y + moveDir.y));
	m_goalTile = glm::vec2(static_cast<int>(m_currentTile.x + moveDir.x), static_cast<int>(m_currentTile.y + moveDir.y));
}

void QBert::QBertMoveComp::Reset(const glm::vec2& tile, bool shouldTurn)
{
	TileComp* goalTile = m_pConnLevelComp->GetTile(static_cast<int>(tile.x), static_cast<int>(tile.y));
	if (goalTile)
	{
		// "teleport" to a certain tile
		m_goalPos = goalTile->GetMiddlePos();
		m_goalTile = tile;

		// either treat new tile as goal or current
		if (shouldTurn)
		{
			m_currentTile = glm::vec2(-1, -1);
		}
		else
		{
			m_currentTile = tile;
		}

		GetOwner()->SetLocalPosition(m_goalPos - m_QBertFeetPos);
	}
}

void QBert::QBertMoveComp::PlayJumpSound() const
{
	int soundId{ rand() % 4 };
	switch (soundId)
	{
	case 0:
		dae::ServiceLocator::GetSoundSystem().Play(dae::SoundId(Sound::Jump1), 0.5f);
		break;
	case 1:
		dae::ServiceLocator::GetSoundSystem().Play(dae::SoundId(Sound::Jump2), 0.5f);
		break;
	case 2:
		dae::ServiceLocator::GetSoundSystem().Play(dae::SoundId(Sound::Jump3), 0.5f);
		break;
	case 3:
		dae::ServiceLocator::GetSoundSystem().Play(dae::SoundId(Sound::Jump4), 0.3f);
		break;
	default:
		break;
	}
}

std::type_index QBert::QBertMoveComp::GetType() const
{
	return typeid(QBertMoveComp);
}