#pragma once
#include "Events.h"
#include "GameObject.h"
#include "Components/TextComponent.h"
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
		RemainingLivesDisplay(GameObject* textObject) : m_textObject(textObject) {}

		void Notify(Event event, Subject*) override
		{
			switch (event.id)
			{
			case make_sdbm_hash("PLAYER_DIED"):
				m_textObject->GetComponent<dae::TextComponent>()->SetText("# lives: ");
				break;
			}
		}

	private:
		GameObject* m_textObject;
	};

	class ScoreDisplay : public IObserver
	{
	public:
		ScoreDisplay(GameObject* textObject) : m_textObject(textObject) {}

		void Notify(Event event, Subject*) override
		{
			switch (event.id)
			{
			case make_sdbm_hash("SCORE_CHANGED"):
				m_textObject->GetComponent<dae::TextComponent>()->SetText("Score: ");
				break;
			}
		}

	private:
		GameObject* m_textObject;
	};
}