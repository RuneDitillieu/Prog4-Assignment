#pragma once
#include <SDL3/SDL.h>
#include "GameObject.h"
#include "Components/MovementComponent.h"
#include "Components/HealthComponent.h"
#include "Components/ScoreComponent.h"
#include "Sound/ServiceLocator.h"

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

	class TakeDamageCommand final : public GameActorCommand
	{
	public:
		TakeDamageCommand(GameObject* actor, int damage)
			: GameActorCommand(actor), m_damage(damage)
		{ 
			m_healthComp = GetGameActor()->GetComponent<dae::HealthComponent>();
		}
		~TakeDamageCommand() = default;

		void Execute() override
		{
			m_healthComp->TakeDamage(m_damage);
		}

	private:
		int m_damage;
		dae::HealthComponent* m_healthComp;
	};


	class TurnTileCommand final : public GameActorCommand
	{
	public:
		TurnTileCommand(GameObject* actor, int scoreIncrease)
			: GameActorCommand(actor), m_scoreIncrease(scoreIncrease)
		{
			m_scoreComp = GetGameActor()->GetComponent<dae::ScoreComponent>();
		}

		void Execute() override
		{
			m_scoreComp->AddToScore(m_scoreIncrease);
			dae::ServiceLocator::GetSoundSystem().Play(dae::SoundId(dae::SoundSystem::Sound::Jump1), 0.5f);
		}

	private:
		int m_scoreIncrease;
		dae::ScoreComponent* m_scoreComp;
	};
}