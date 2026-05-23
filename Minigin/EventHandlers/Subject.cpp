#include "Subject.h"
#include "IObserver.h"

dae::Subject::Subject(dae::GameObject* pOwner)
	: m_pOwner(pOwner)
{ }

void dae::Subject::AddObserver(IObserver* observer)
{
	m_observers.push_back(observer);
}

void dae::Subject::RemoveObserver(IObserver* observer)
{
	m_observers.erase(std::remove_if(m_observers.begin(), m_observers.end(), [observer](auto obs) { return observer == obs; }));
}

void dae::Subject::NotifyObservers(Event event)
{
	for (auto observer : m_observers)
		observer->Notify(event, this);
}

dae::GameObject* dae::Subject::GetOwner() const
{
	return m_pOwner;
}