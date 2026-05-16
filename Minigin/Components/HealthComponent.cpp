#include "HealthComponent.h"
#include "GameObject.h"
#include "Subject.h"
#include "Events.h"

dae::HealthComponent::HealthComponent(GameObject* pOwner, int maxHealth, int startLives, int maxLives)
	: Component(pOwner)
	, m_maxHealth(maxHealth)
	, m_curHealth(maxHealth)
	, m_maxLives(maxLives)
	, m_remainingLives(startLives)
{ }

void dae::HealthComponent::TakeDamage(int damage)
{
	int oldHealth{ m_curHealth };

	m_curHealth -= damage;

	if (m_curHealth <= 0)
	{
		m_curHealth = 0;

		if (oldHealth > 0)
		{
			LoseLife();
		}
	}
}

void dae::HealthComponent::LoseLife()
{	
	if (m_remainingLives > 0)
	{
		--m_remainingLives;
		m_curHealth = m_maxHealth;
	}

	Event e(make_sdbm_hash("ACTOR_LIVES_CHANGED"));
	e.args[0].nr = m_remainingLives;
	GetOwner()->GetSubject()->NotifyObservers(e);
}

void dae::HealthComponent::GainLife(int amount)
{
	if (m_remainingLives >= m_maxLives) return;

	if (m_remainingLives == 0)
	{
		m_curHealth = m_maxHealth;
	}

	m_remainingLives += amount;
	if (m_remainingLives > m_maxLives)
	{
		m_remainingLives = m_maxLives;
	}

	Event e(make_sdbm_hash("ACTOR_LIVES_CHANGED"));
	e.args[0].nr = m_remainingLives;
	GetOwner()->GetSubject()->NotifyObservers(e);
}

std::type_index dae::HealthComponent::GetType() const
{
	return typeid(HealthComponent);
}