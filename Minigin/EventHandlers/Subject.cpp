#include "Subject.h"
#include "Observers.h"

void dae::Subject::AddObserver(IObserver* observer)
{
	m_observers.push_back(observer);
}

void dae::Subject::RemoveObserver(IObserver* observer)
{
	m_observers.erase(std::remove(m_observers.begin(), m_observers.end(), observer));
}

void dae::Subject::NotifyObservers(Event event)
{
	for (auto observer : m_observers)
		observer->Notify(event, this);
}