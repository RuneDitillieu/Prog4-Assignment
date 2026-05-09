#pragma once
#include <vector>
#include "Component.h"

namespace QBert
{
	class GameObject;
	class TileComp;
	class LevelBase : public dae::Component
	{
	public:
		LevelBase(dae::GameObject* pOwner, int tileType, bool revertable, int start, int win, int middle = -1);
		~LevelBase() = default;
		LevelBase(const LevelBase& other) = delete;
		LevelBase(LevelBase&& other) = delete;
		LevelBase& operator=(const LevelBase& other) = delete;
		LevelBase& operator=(LevelBase&& other) = delete;

		std::type_index GetType() const override;

		bool AreAllTilesCorrect() const;

	private:
		std::vector<std::vector<TileComp*>> m_tiles;
	};
}