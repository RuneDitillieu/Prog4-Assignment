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
		QBertMoveComp(dae::GameObject* pOwner, const glm::vec3& feetPos);
		~QBertMoveComp() = default;
		QBertMoveComp(const QBertMoveComp& other) = delete;
		QBertMoveComp(QBertMoveComp&& other) = delete;
		QBertMoveComp& operator=(const QBertMoveComp& other) = delete;
		QBertMoveComp& operator=(QBertMoveComp&& other) = delete;

		std::type_index GetType() const override;

		void Update() override;
		void Move(const glm::vec3& direction);

	private:
		glm::vec3 m_QBertFeetPos;
		glm::vec2 m_currentTile{ 0, 0 };
		glm::vec2 m_goalTile{ 0, 0 };
		glm::vec3 m_goalPos;
		LevelBase* m_pConnLevelComp;
		dae::SpriteComp* m_pConnSpriteComp;
	};
}

#endif // !QBERTMOVECOMPONENT_H

