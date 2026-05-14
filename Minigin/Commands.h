#ifndef COMMANDS_H
#define COMMANDS_H

#include <SDL3/SDL.h>
#include "GameObject.h"
#include "MovementComponent.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include "ServiceLocator.h"

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
}

#endif // !COMMANDS_H