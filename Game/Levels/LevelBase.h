#ifndef LEVELBASE_H
#define LEVELBASE_H

#include <vector>
#include "Component.h"
#include <glm/glm.hpp>
#include <unordered_map>

namespace QBert
{
	struct TileParams;

	enum class TileType
	{
		Tile,
		Disc,
		Void
	};

	class GameObject;
	class TileComp;
	class DiscActorComp;
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

		TileComp* GetTile(int col, int row) const;
		bool TurnTile(int col, int row) const;
		bool RevertTile(int col, int row) const;
		glm::vec3 GetMiddlePosOfTile(int col, int row) const;
		TileType GetTileType(int col, int row) const;
		dae::GameObject* GetDisc(int col, int row) const;
		void SetDiscs(std::vector<DiscActorComp*>&& discs);
		int GetActiveDiscAmount() const;

		void ResetBase(TileParams tileParams);

	private:
		bool AreAllTilesCorrect() const;
		float m_tileSize{};
		glm::vec3 m_middlePosOffset{};
		std::vector<std::vector<TileComp*>> m_tiles;
		std::vector<DiscActorComp*> m_discs{};
	};
}

#endif // !LEVELBASE_H