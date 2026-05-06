#pragma once
//#include "Observers.h"
#include <vector>
#include "GameObject.h"
#include "Events.h"

namespace dae
{
	class IObserver;
	class Subject
	{
	public:
		Subject() = default;

		void AddObserver(IObserver* observer);

		void RemoveObserver(IObserver* observer);

		void NotifyObservers(Event event);

	private:
		std::vector<IObserver*> m_observers;
	};
}