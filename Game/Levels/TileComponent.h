#ifndef TILECOMPONENT_H
#define TILECOMPONENT_H

#include "Component.h"
#include "SpriteComponent.h"
#include <glm/glm.hpp>

namespace QBert
{
	struct TileParams
	{
		int tileType;
		bool revertable;

		int startTile;
		int winTile;
		int middleTile = -1;
	};

	class GameObject;
	class TileComp final : public dae::Component
	{
	public:
		TileComp(dae::GameObject* pOwner, int tileType, bool revertable, int start, int win, int middle = -1);
		TileComp(dae::GameObject* pOwner, const TileParams& tileParams);
		~TileComp() = default;
		TileComp(const TileComp& other) = delete;
		TileComp(TileComp&& other) = delete;
		TileComp& operator=(const TileComp& other) = delete;
		TileComp& operator=(TileComp&& other) = delete;

		glm::vec3 GetMiddlePos() const;
		void FlashColors() const;

		std::type_index GetType() const override;

	private:
		void ResetTile(const TileParams& tileParams);

		bool Turn();
		bool Revert();
		bool IsWinColor() { return m_currentTile == m_winTile; }

		int m_tileType;
		bool m_revertable;

		int m_startTile;
		int m_winTile;
		int m_middleTile;

		int m_currentTile;

		glm::vec3 m_middlePos{};

		dae::SpriteComp* m_pConnSprite{ nullptr };

		friend class LevelBase;
	};
}

#endif // !TILECOMPONENT_H