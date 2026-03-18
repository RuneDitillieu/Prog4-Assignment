#include "Observers.h"

dae::RemainingLivesDisplay::RemainingLivesDisplay(GameObject* textObject)
	: IObserver(), m_textObject(textObject) 
{}

void dae::RemainingLivesDisplay::Notify(Event event, Subject*)
{
	switch (event.id)
	{
	case make_sdbm_hash("PLAYER_DIED"):
		TextComponent* textComp = m_textObject->GetComponent<TextComponent>();
		textComp->SetText("# lives: " + std::to_string(event.args[0].nr));
		break;
	}
}


dae::ScoreDisplay::ScoreDisplay(GameObject* textObject) : IObserver(), m_textObject(textObject) 
{}

void dae::ScoreDisplay::Notify(Event event, Subject*)
{
	switch (event.id)
	{
	case make_sdbm_hash("SCORE_CHANGED"):
		TextComponent* textComp = m_textObject->GetComponent<TextComponent>();
		textComp->SetText("Score: " + std::to_string(event.args[0].nr));
		break;
	}
}