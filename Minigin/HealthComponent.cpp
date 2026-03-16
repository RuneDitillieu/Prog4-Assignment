#include "HealthComponent.h"

dae::HealthComponent::HealthComponent(GameObject* pOwner, int maxHealth)
	: Component(pOwner)
	, m_maxHealth(maxHealth)
	, m_curHealth(maxHealth)
{ }

void dae::HealthComponent::TakeDamage(int damage)
{
	m_curHealth -= damage;
	if (m_curHealth < 0)
		m_curHealth = 0;
}

std::type_index dae::HealthComponent::GetType() const
{
	return typeid(HealthComponent);
}