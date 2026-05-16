#ifndef SUBJECT_H
#define SUBJECT_H

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
		Subject(dae::GameObject* pOwner);

		void AddObserver(IObserver* observer);
		void RemoveObserver(IObserver* observer);

		void NotifyObservers(Event event);

		dae::GameObject* GetOwner() const;

	private:
		std::vector<IObserver*> m_observers;
		dae::GameObject* m_pOwner;
	};
}

#endif // !SUBJECT_H