#include "ArrowAnimComponent.h"
#include "DeltaTime.h"
#include "GameObject.h"

QBert::ArrowAnimComp::ArrowAnimComp(dae::GameObject* pOwner, std::vector<dae::GameObject*> arrows)
	: dae::Component(pOwner)
	, m_pArrows(arrows)
{ 
	for (auto arrow : m_pArrows)
	{
		arrow->m_isEnabled = false;
	}
}

void QBert::ArrowAnimComp::Update()
{
	m_secPassed += dae::DeltaTime::GetInstance().GetDeltaTime();
	if (m_secPassed >= m_secToWait * 3.f)
	{
		for (auto arrow : m_pArrows)
		{
			arrow->m_isEnabled = false;
		}
		m_secPassed -= m_secToWait * 3.f;
	}
	else if (m_secPassed >= m_secToWait * 2.f)
	{
		for (auto arrow : m_pArrows)
		{
			arrow->m_isEnabled = true;
		}
	}
	else if (m_secPassed >= m_secToWait)
	{
		m_pArrows[0]->m_isEnabled = true;
		m_pArrows[3]->m_isEnabled = true;
	}
}

std::type_index QBert::ArrowAnimComp::GetType() const
{
	return typeid(QBert::ArrowAnimComp);
}