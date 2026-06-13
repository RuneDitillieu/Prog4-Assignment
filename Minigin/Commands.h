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


	class MuteCommand final : public Command
	{
	public:
		MuteCommand() : Command() {}
		void Execute() override
		{
			ServiceLocator::GetSoundSystem().MuteUnmuteSound();
		}
	};

	class ConfirmCommand final : public Command
	{
	public:
		ConfirmCommand(IObserver* observer) : Command()
		{
			m_subject->AddObserver(observer);
		}

		void Execute() override
		{
			Event e{ make_sdbm_hash("OnConfirmPressed") };
			m_subject->NotifyObservers(e);
		}

	private:
		std::unique_ptr<Subject> m_subject{ std::make_unique<Subject>(nullptr) };
	};

	class NextCommand final : public Command
	{
	public:
		NextCommand(IObserver* observer) : Command()
		{
			m_subject->AddObserver(observer);
		}

		void Execute() override
		{
			Event e{ make_sdbm_hash("OnNextPressed") };
			m_subject->NotifyObservers(e);
		}

	private:
		std::unique_ptr<Subject> m_subject{ std::make_unique<Subject>(nullptr) };
	};

	class PrevCommand final : public Command
	{
	public:
		PrevCommand(IObserver* observer) : Command()
		{
			m_subject->AddObserver(observer);
		}

		void Execute() override
		{
			Event e{ make_sdbm_hash("OnPreviousPressed") };
			m_subject->NotifyObservers(e);
		}

	private:
		std::unique_ptr<Subject> m_subject{ std::make_unique<Subject>(nullptr) };
	};

	class StartCommand final : public Command
	{
	public:
		StartCommand(IObserver* observer) : Command()
		{
			m_subject->AddObserver(observer);
		}

		void Execute() override
		{
			Event e{ make_sdbm_hash("OnStartPressed") };
			m_subject->NotifyObservers(e);
		}

	private:
		std::unique_ptr<Subject> m_subject{ std::make_unique<Subject>(nullptr) };
	};
}

#endif // !COMMANDS_H