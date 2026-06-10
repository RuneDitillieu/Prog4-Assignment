#include "LevelBase.h"
#include "GameObject.h"
#include "TileComponent.h"
#include "RenderComponent.h"
#include "ServiceLocator.h"
#include "Sounds.h"
#include "DiscActor.h"

QBert::LevelBase::LevelBase(dae::GameObject* pOwner, int tileType, bool revertable, int start, int win, int middle)
	: Component(pOwner)
{
	const size_t levelSize{ 7 };

	m_tiles.reserve(levelSize);
	for (size_t idx{ 0 }; idx < levelSize; ++idx)
	{
		m_tiles.emplace_back(std::vector<QBert::TileComp*>{});
		m_tiles[idx].reserve(levelSize - idx);
	}

	for (size_t colIdx{ 0 }; colIdx < levelSize; ++colIdx)
	{
		for (size_t rowIdx{ 0 }; rowIdx < levelSize; ++rowIdx)
		{
			if (colIdx + rowIdx >= levelSize)
			{
				continue;
			}

			auto go = std::make_unique<dae::GameObject>();
			m_tiles[colIdx].push_back(go->AddComponent(std::make_unique<TileComp>(go.get(), tileType, revertable, start, win, middle)));
			float scale{ 3.f };
			m_tileSize = (go->GetComponent<dae::RenderComponent>()->GetSize().y / 3) * scale;
			go->SetLocalPosition(colIdx * 0.5f * m_tileSize + rowIdx * -0.5f * m_tileSize, colIdx * 0.75f * m_tileSize + rowIdx * 0.75f * m_tileSize);
			go.release()->SetParent(pOwner, false);
		}
	}

	m_middlePosOffset = glm::vec3(m_tileSize / 2.f, m_tileSize / 4.f, 0);
}

QBert::TileComp* QBert::LevelBase::GetTile(int col, int row) const
{
	if (col < 0 || row < 0) 
	{
		return nullptr;
	}
	else if (m_tiles.size() > static_cast<size_t>(col) 
		&& m_tiles[col].size() > static_cast<size_t>(row))
	{
		return m_tiles[col][row];
	}
	else
	{
		return nullptr;
	}
}

bool QBert::LevelBase::TurnTile(int col, int row) const
{
	if (!GetTile(col, row))
	{
		return false;
	}
	else if (GetTile(col, row)->Turn())
	{
		if (AreAllTilesCorrect())
		{
			dae::Event e(dae::make_sdbm_hash("LEVEL_COMPLETED"));
			GetOwner()->GetSubject()->NotifyObservers(e);
			dae::ServiceLocator::GetSoundSystem().Play(dae::SoundId(QBert::Sound::Victory), 0.5f);
		}

		return true;
	}
	else
	{
		return false;
	}
}

bool QBert::LevelBase::RevertTile(int col, int row) const
{
	if (!GetTile(col, row))
	{
		return false;
	}
	else if (GetTile(col, row)->Revert())
	{
		return true;
	}
	else
	{
		return false;
	}
}

glm::vec3 QBert::LevelBase::GetMiddlePosOfTile(int col, int row) const
{
	if (GetTile(col, row))
	{
		return GetTile(col, row)->GetMiddlePos();
	}
	else if(col == -1 || row == -1)
	{
		glm::vec2 offset{ 0.f, 0.f};
		if (col == -1 && row != -1)
		{
			offset.y = 0.5f;
		}
		if (row == -1 && col != -1)
		{
			offset.x = 0.5f;
		}

		return GetOwner()->GetWorldPosition()
			+ glm::vec3((col + offset.x)  * 0.5f * m_tileSize + (row + offset.y) * -0.5f * m_tileSize, (col + offset.x) * 0.75f * m_tileSize + (row + offset.y) * 0.75f * m_tileSize, 0)
			+ m_middlePosOffset;
	}
	else
	{
		return GetOwner()->GetWorldPosition()
			+ glm::vec3(col * 0.5f * m_tileSize + row * -0.5f * m_tileSize, col * 0.75f * m_tileSize + row * 0.75f * m_tileSize, 0)
			+ m_middlePosOffset;
	}
}

QBert::TileType QBert::LevelBase::GetTileType(int col, int row) const
{
	if (GetTile(col, row))
	{
		return TileType::Tile;
	}
	else if (GetDisc(col, row) != nullptr
		&& GetDisc(col, row)->IsEnabled())
	{
		return TileType::Disc;
	}
	else
	{
		return TileType::Void;
	}
}

dae::GameObject* QBert::LevelBase::GetDisc(int col, int row) const
{
	auto it = std::find_if(m_discs.begin(), m_discs.end(), [col, row](DiscActorComp* disc) { return disc->GetTile() == glm::vec2(col, row); });
	if (it != m_discs.end())
	{
		auto disc = *it;
		return disc->GetOwner();
	}

	else return nullptr;
}

void QBert::LevelBase::SetDiscs(std::vector<DiscActorComp*>&& discs)
{
	m_discs.clear();
	m_discs = std::move(discs);
}

int QBert::LevelBase::GetActiveDiscAmount() const
{
	int amount{};
	for (auto& disc : m_discs)
	{
		if (disc->GetOwner()->IsEnabled())
			++amount;
	}

	return amount;
}

bool QBert::LevelBase::AreAllTilesCorrect() const
{
	for (auto vec : m_tiles)
	{
		for (auto tile : vec)
		{
			if (!tile->IsWinColor())
			{
				return false;
			}
		}
	}

	return true;
}

void QBert::LevelBase::ResetBase(TileParams tileParams, const glm::vec2& tilePos1, const glm::vec2& tilePos2)
{
	for (auto col : m_tiles)
	{
		for (auto tile : col)
		{
			tile->ResetTile(tileParams);
		}
	}

	m_discs[0]->ResetDisc(tilePos1, this);
	m_discs[0]->GetOwner()->IsEnabled(true);
	m_discs[1]->ResetDisc(tilePos2, this);
	m_discs[1]->GetOwner()->IsEnabled(true);
}

std::type_index QBert::LevelBase::GetType() const
{
	return typeid(LevelBase);
}