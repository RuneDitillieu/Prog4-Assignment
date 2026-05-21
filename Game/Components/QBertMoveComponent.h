#ifndef QBERTMOVECOMPONENT_H
#define QBERTMOVECOMPONENT_H

#include "Component.h"
#include <glm/glm.hpp>
#include "SpriteComponent.h"

namespace QBert
{
	class LevelBase;
	class GameObject;
	class QBertMoveComp : public dae::Component
	{
	public:
		QBertMoveComp(dae::GameObject* pOwner, const glm::vec3& feetPos, const glm::vec2& startTile, bool canTurn, bool canRevert);
		~QBertMoveComp() = default;
		QBertMoveComp(const QBertMoveComp& other) = delete;
		QBertMoveComp(QBertMoveComp&& other) = delete;
		QBertMoveComp& operator=(const QBertMoveComp& other) = delete;
		QBertMoveComp& operator=(QBertMoveComp&& other) = delete;

		std::type_index GetType() const override;

		void Update() override;
		void Move(const glm::vec3& direction);
		glm::vec2 GetCurrentTile() const { return m_currentTile; }
		glm::vec2 GetGoalTile() const { return m_goalTile; }
		bool IsMoving() const { if (m_currentTile == m_goalTile) return false; else return true; }
		glm::vec3 GetFeetPos() { return m_QBertFeetPos; }
		void Reset(const glm::vec2& tile);

	private:
		glm::vec3 m_QBertFeetPos;
		glm::vec2 m_currentTile;
		glm::vec2 m_goalTile;
		glm::vec3 m_goalPos;

		bool m_canTurnTiles;
		bool m_canRevertTiles;

		LevelBase* m_pConnLevelComp;
		dae::SpriteComp* m_pConnSpriteComp;
	};
}

#endif // !QBERTMOVECOMPONENT_H

