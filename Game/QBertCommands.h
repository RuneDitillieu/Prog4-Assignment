#pragma once
#include "Commands.h"
#include "GameObject.h"
#include "QBertMoveComponent.h"

namespace QBert
{
	class QBertMoveCommand final : public dae::GameActorCommand
	{
	public:
		QBertMoveCommand(dae::GameObject* actor, glm::vec3 moveDir)
			: GameActorCommand(actor), m_moveDirection(moveDir)
		{
			m_moveComp = GetGameActor()->GetComponent<QBert::QBertMoveComp>();
		}
		~QBertMoveCommand() = default;

		void Execute() override
		{
			m_moveComp->Move(m_moveDirection);
		}

	private:
		glm::vec3 m_moveDirection;
		QBert::QBertMoveComp* m_moveComp;
	};
}