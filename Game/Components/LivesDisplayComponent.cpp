#include "LivesDisplayComponent.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Tags.h"

QBert::LivesDisplay::LivesDisplay(dae::GameObject* pOwner, std::vector<dae::GameObject*> livesP1, std::vector<dae::GameObject*> livesP2)
	: dae::Component(pOwner)
	, m_pLivesP1(livesP1)
	, m_pLivesP2(livesP2)
{ }

QBert::LivesDisplay::~LivesDisplay()
{
	auto qberts = dae::SceneManager::GetInstance().GetActiveScene()->GetObjectsByTag(dae::Tag(Tag::QBert));
	for (auto qbert : qberts)
	{
		qbert->GetSubject()->RemoveObserver(this);
	}
}

void QBert::LivesDisplay::Start()
{
	auto qberts = dae::SceneManager::GetInstance().GetActiveScene()->GetObjectsByTag(dae::Tag(Tag::QBert));
	for (auto qbert : qberts)
	{
		qbert->GetSubject()->AddObserver(this);
	}

	m_player1 = qberts[0];
	if (qberts.size() > 1) m_player2 = qberts[1];
}

void QBert::LivesDisplay::Notify(dae::Event event, dae::Subject* subject)
{
	if (event.id == dae::make_sdbm_hash("ACTOR_LIVES_CHANGED"))
	{
		// update UI to represent player lives
		if (subject->GetOwner() == m_player1)
		{
			UpdateUI(event.args[0].nr, m_pLivesP1);
		}
		else if (subject->GetOwner() == m_player2)
		{
			UpdateUI(event.args[0].nr, m_pLivesP2);
		}
	}
}

void QBert::LivesDisplay::UpdateUI(int amLives, const std::vector<dae::GameObject*>& livesVec) const
{
	int idx{ 0 };
	for (auto lives : livesVec)
	{
		++idx;

		if (idx <= amLives)
		{
			lives->IsEnabled(true);
		}
		else
		{
			lives->IsEnabled(false);
		}
	}
}

std::type_index QBert::LivesDisplay::GetType() const
{
	return typeid(LivesDisplay);
}