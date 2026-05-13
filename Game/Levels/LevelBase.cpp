#include "LevelBase.h"
#include "GameObject.h"
#include "TileComponent.h"
#include "RenderComponent.h"

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
			m_tiles[colIdx].push_back(go->AddComponent(std::make_unique<QBert::TileComp>(go.get(), tileType, revertable, start, win, middle)));
			float scale{ 3.f };
			float tileSize{ (go->GetComponent<dae::RenderComponent>()->GetSize().y / 6) * scale };
			go->SetLocalPosition(colIdx * 0.5f * tileSize + rowIdx * -0.5f * tileSize, colIdx * 0.75f * tileSize + rowIdx * 0.75f * tileSize);
			go.release()->SetParent(pOwner, false);
		}
	}
}

QBert::TileComp* QBert::LevelBase::GetTile(size_t col, size_t row) const
{
	if (m_tiles.size() > col && m_tiles[col].size() > row)
	{
		return m_tiles[col][row];
	}
	else
	{
		return nullptr;
	}
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

std::type_index QBert::LevelBase::GetType() const
{
	return typeid(LevelBase);
}