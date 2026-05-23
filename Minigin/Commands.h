#ifndef COMMANDS_H
#define COMMANDS_H

#include "GameObject.h"
#include "ServiceLocator.h"

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
		GameActorCommand(GameObject* actor) : Command(), m_actor(actor) {}
		virtual ~GameActorCommand() = default;
		
	protected:
		GameObject* GetGameActor() const { return m_actor; }
	private:
		GameObject* m_actor;
	};


	class MuteCommand : public Command
	{
	public:
		MuteCommand() : Command() {}
		void Execute() override
		{
			ServiceLocator::GetSoundSystem().MuteUnmuteSound();
		}
	};
}

#endif // !COMMANDS_H