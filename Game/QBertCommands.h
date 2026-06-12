#pragma once
#include "Commands.h"
#include "GameObject.h"
#include "Subject.h"
#include "LevelManager.h"
#include "PlayerCoilyActor.h"

namespace QBert
{
	class QBertMoveCommand final : public dae::GameActorCommand
	{
	public:
		QBertMoveCommand(dae::GameObject* actor, glm::vec3 moveDir)
			: GameActorCommand(actor), m_moveDirection(moveDir)
		{
			auto qbert = actor->GetComponentInChildren<QBertActorComp>();
			if (qbert)
			{
				m_subject->AddObserver(qbert);
			}
			else
			{
				auto coily = actor->GetComponentInChildren<PlayerCoilyActorComp>();
				m_subject->AddObserver(coily);
			}
		}
		~QBertMoveCommand() = default;

		void Execute() override
		{
			dae::Event e(dae::make_sdbm_hash("OnMove"));
			e.args->dir = m_moveDirection;
			m_subject->NotifyObservers(e);
		}

	private:
		glm::vec3 m_moveDirection;
		std::unique_ptr<dae::Subject> m_subject{ std::make_unique<dae::Subject>(nullptr) };
	};

	class SkipLevelCommand final : public dae::GameActorCommand
	{
	public:
		SkipLevelCommand(dae::GameObject* actor)
			: GameActorCommand(actor)
		{
			m_levelManager = actor->GetComponent<LevelManager>();
		}

		void Execute() override
		{
			if (m_levelManager)
			{
				m_levelManager->GoToNextLevel();
			}
		}

	private:
		LevelManager* m_levelManager{ nullptr };
	};
}
