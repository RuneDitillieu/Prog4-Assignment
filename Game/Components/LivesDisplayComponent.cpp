#include "LivesDisplayComponent.h"
#include "GameObject.h"

QBert::LivesDisplay::LivesDisplay(dae::GameObject* pOwner, std::vector<dae::GameObject*> lives)
	: dae::Component(pOwner)
	, m_pLives(lives)
{ }

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