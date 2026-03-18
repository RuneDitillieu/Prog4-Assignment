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
		RemainingLivesDisplay(GameObject* textObject);

		void Notify(Event event, Subject*) override;

	private:
		GameObject* m_textObject{ nullptr };
	};

	class ScoreDisplay : public IObserver
	{
	public:
		ScoreDisplay(GameObject* textObject) : IObserver(), m_textObject(textObject) {}

		void Notify(Event event, Subject*) override
		{
			switch (event.id)
			{
			case make_sdbm_hash("SCORE_CHANGED"):
				//TextComponent* textComp = m_textObject->GetComponent<TextComponent>();
				//textComp->SetText("Score: ");
				break;
			}
		}

	private:
		GameObject* m_textObject;
	};
}