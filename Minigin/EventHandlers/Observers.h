#ifndef OBSERVERS_H
#define OBSERVERS_H

#include "Events.h"
#include "GameObject.h"
#include "TextComponent.h"
//#include "Subject.h"

namespace dae
{
	class Subject;
	class IObserver
	{
	public:
		virtual ~IObserver() = default;
		virtual void Notify(Event event, Subject*) = 0;
	};


	class RemainingLivesDisplay : public IObserver
	{
	public:
		RemainingLivesDisplay(GameObject* textObject);

		void Notify(Event event, Subject*) override;

	private:
		GameObject* m_textObject{ nullptr };
	};


	class ScoreDisplay : public IObserver
	{
	public:
		ScoreDisplay(GameObject* textObject);

		void Notify(Event event, Subject*) override;

	private:
		GameObject* m_textObject;
	};
}

#endif // !OBSERVERS_H