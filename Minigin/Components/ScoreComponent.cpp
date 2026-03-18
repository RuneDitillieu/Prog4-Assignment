#include "ScoreComponent.h"
#include "GameObject.h"
#include "Subject.h"

dae::ScoreComponent::ScoreComponent(GameObject* pOwner)
	: Component(pOwner)
{ }

void dae::ScoreComponent::AddToScore(int amount)
{
	m_score += amount;
	Event e(make_sdbm_hash("SCORE_CHANGED"));
	e.args[0].nr = m_score;
	GetOwner()->GetSubject()->NotifyObservers(e);
}

std::type_index dae::ScoreComponent::GetType() const
{
	return typeid(ScoreComponent);
}