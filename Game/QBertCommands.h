#pragma once
#include "Commands.h"
#include "GameObject.h"
#include "QBertMoveComponent.h"
#include "Subject.h"

namespace QBert
{
	class QBertMoveCommand final : public dae::GameActorCommand
	{
	public:
		QBertMoveCommand(dae::GameObject* actor, glm::vec3 moveDir)
			: GameActorCommand(actor), m_moveDirection(moveDir)
		{
			//m_moveComp = GetGameActor()->GetComponent<QBert::QBertMoveComp>();

			auto qbert = actor->GetComponentInChildren<QBert::QBertActorComp>();
			if (qbert)
			{
				m_subject->AddObserver(qbert);
			}
			/*else
			{
				auto coily = actor->GetComponentInChildren<QBert::Coily>();
				m_subject->AddObserver(coily);
			}*/
		}
		~QBertMoveCommand() = default;

		void Execute() override
		{
			//m_moveComp->Move(m_moveDirection);
			dae::Event e(dae::make_sdbm_hash("OnMove"));
			e.args->dir = m_moveDirection;
			m_subject->NotifyObservers(e);
		}

	private:
		glm::vec3 m_moveDirection;
		std::unique_ptr<dae::Subject> m_subject{ std::make_unique<dae::Subject>() };
		//QBert::QBertMoveComp* m_moveComp;
	};
}