#pragma once
#include "Observers.h"
#include <vector>
#include "GameObject.h"

namespace dae
{
	class Subject
	{
	public:
		void AddObserver(IObserver* observer) 
		{
			m_observers.push_back(observer);
		}

		void RemoveObserver(IObserver* observer) 
		{
			m_observers.erase(std::remove(m_observers.begin(), m_observers.end(), observer));
		}

		void NotifyObservers(Event event) 
		{
			for (auto observer : m_observers)
				observer->Notify(event, this);
		}

	private:
		std::vector<IObserver*> m_observers;
	};
}