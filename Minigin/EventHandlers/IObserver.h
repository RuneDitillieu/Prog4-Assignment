#ifndef IOBSERVER_H
#define IOBSERVER_H

#include "Events.h"

namespace dae
{
	class Subject;
	class IObserver
	{
	public:
		virtual ~IObserver() = default;
		virtual void Notify(Event event, Subject*) = 0;
	};
}

#endif // !IOBSERVER_H