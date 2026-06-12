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
	auto qberts = dae::SceneManager::GetInstance().GetActiveScene()->GetObjectsByTag(dae::Tag(QBert::Tag::QBert));
	for (auto qbert : qberts)
	{
		qbert->GetSubject()->RemoveObserver(this);
	}
}

void QBert::LivesDisplay::Start()
{
	auto qberts = dae::SceneManager::GetInstance().GetActiveScene()->GetObjectsByTag(dae::Tag(QBert::Tag::QBert));
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
		if (subject->GetOwner() == m_player1)
		{
			int idx{ 0 };
			for (auto lives : m_pLivesP1)
			{
				++idx;

				if (idx <= event.args[0].nr)
				{
					lives->IsEnabled(true);
				}
				else
				{
					lives->IsEnabled(false);
				}
			}
		}
		else if (subject->GetOwner() == m_player2)
		{
			int idx{ 0 };
			for (auto lives : m_pLivesP2)
			{
				++idx;

				if (idx <= event.args[0].nr)
				{
					lives->IsEnabled(true);
				}
				else
				{
					lives->IsEnabled(false);
				}
			}
		}
	}
}

std::type_index QBert::LivesDisplay::GetType() const
{
	return typeid(LivesDisplay);
}