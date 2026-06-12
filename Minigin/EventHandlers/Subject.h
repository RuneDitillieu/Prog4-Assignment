#ifndef SUBJECT_H
#define SUBJECT_H

#include <vector>
#include "GameObject.h"
#include "Events.h"

namespace dae
{
	class IObserver;
	class Subject
	{
	public:
		Subject(GameObject* pOwner);

		void AddObserver(IObserver* observer);
		void RemoveObserver(IObserver* observer);

		void NotifyObservers(Event event);

		GameObject* GetOwner() const;

	private:
		std::vector<IObserver*> m_observers;
		GameObject* m_pOwner;
	};
}

#endif // !SUBJECT_H