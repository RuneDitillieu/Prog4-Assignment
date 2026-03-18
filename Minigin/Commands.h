#pragma once
#include <SDL3/SDL.h>
#include "GameObject.h"
#include "Components/MovementComponent.h"
#include <glm/glm.hpp>

namespace dae
{
	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute() = 0;
	};


	class GameActorCommand : public Command
	{
	public:
		GameActorCommand(GameObject* actor) : m_actor(actor) {}
		virtual ~GameActorCommand() = default;
		
	protected:
		GameObject* GetGameActor() const { return m_actor; }
	private:
		GameObject* m_actor;
	};


	class MoveCommand final : public GameActorCommand
	{
	public:
		MoveCommand(GameObject* actor, glm::vec3 moveDir) 
			: GameActorCommand(actor), m_moveDirection(moveDir) 
		{
			m_moveComp = GetGameActor()->GetComponent<dae::MovementComponent>();
		}
		~MoveCommand() = default;

		void Execute() override
		{  
			m_moveComp->Move(m_moveDirection);
		}

	private:
		glm::vec3 m_moveDirection;
		dae::MovementComponent* m_moveComp;
	};
}