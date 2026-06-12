#include "ArrowAnimComponent.h"
#include "DeltaTime.h"
#include "GameObject.h"

QBert::ArrowAnimComp::ArrowAnimComp(dae::GameObject* pOwner, std::vector<dae::GameObject*> arrows)
	: dae::Component(pOwner)
	, m_pArrows(arrows)
{ 
	for (auto arrow : m_pArrows)
	{
		arrow->IsEnabled(false);
	}
}

void QBert::ArrowAnimComp::Update()
{
	m_secPassed += dae::DeltaTime::GetInstance().GetDeltaTime();

	// disable all
	if (m_secPassed >= m_secToWait * 3.f)
	{
		for (auto arrow : m_pArrows)
		{
			arrow->IsEnabled(false);
		}
		m_secPassed -= m_secToWait * 3.f;
	}
	// enable inner ones
	else if (m_secPassed >= m_secToWait * 2.f)
	{
		for (auto arrow : m_pArrows)
		{
			arrow->IsEnabled(true);
		}
	}
	// enable outer ones
	else if (m_secPassed >= m_secToWait)
	{
		m_pArrows[0]->IsEnabled(true);
		m_pArrows[3]->IsEnabled(true);
	}
}

std::type_index QBert::ArrowAnimComp::GetType() const
{
	return typeid(QBert::ArrowAnimComp);
}