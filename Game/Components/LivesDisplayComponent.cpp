#include "LivesDisplayComponent.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Tags.h"

QBert::LivesDisplay::LivesDisplay(dae::GameObject* pOwner, std::vector<dae::GameObject*> lives)
	: dae::Component(pOwner)
	, m_pLives(lives)
{ }

QBert::LivesDisplay::~LivesDisplay()
{
	auto qberts = dae::SceneManager::GetInstance().GetActiveScene()->GetObjectsByTag(dae::Tag(QBert::Tag::Player));
	for (auto qbert : qberts)
	{
		qbert->GetSubject()->RemoveObserver(this);
	}
}

void QBert::LivesDisplay::Start()
{
	auto qberts = dae::SceneManager::GetInstance().GetActiveScene()->GetObjectsByTag(dae::Tag(QBert::Tag::Player));
	for (auto qbert : qberts)
	{
		qbert->GetSubject()->AddObserver(this);
	}
}

void QBert::LivesDisplay::Notify(dae::Event event, dae::Subject*)
{
	if (event.id == dae::make_sdbm_hash("ACTOR_LIVES_CHANGED"))
	{
		int idx{ 0 };
		for (auto lives : m_pLives)
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

std::type_index QBert::LivesDisplay::GetType() const
{
	return typeid(LivesDisplay);
}