#ifndef TILECOMPONENT_H
#define TILECOMPONENT_H

#include "Component.h"
#include "SpriteComponent.h"

namespace QBert
{
	class GameObject;
	class TileComp : public dae::Component
	{
	public:
		TileComp(dae::GameObject* pOwner, int tileType, bool revertable, int start, int win, int middle = -1);
		~TileComp() = default;
		TileComp(const TileComp& other) = delete;
		TileComp(TileComp&& other) = delete;
		TileComp& operator=(const TileComp& other) = delete;
		TileComp& operator=(TileComp&& other) = delete;

		void Turn();
		bool IsWinColor() { return m_currentTile == m_winTile; }

		std::type_index GetType() const override;

	private:
		const int m_tileType;
		const bool m_revertable;

		const int m_startTile;
		const int m_winTile;
		const int m_middleTile;

		int m_currentTile;

		dae::SpriteComp* m_pConnSprite{ nullptr };
	};
}

#endif // !TILECOMPONENT_H