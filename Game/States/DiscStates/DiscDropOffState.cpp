#include "DiscStates.h"
#include "DeltaTime.h"

QBert::DropOffDiscState::DropOffDiscState(dae::GameObject* disc)
	: DiscState(disc)
{ }

std::unique_ptr<QBert::DiscState> QBert::DropOffDiscState::Update()
{
	m_secPassed += dae::DeltaTime::GetInstance().GetDeltaTime();
	if (m_secPassed >= m_maxSec)
	{
		m_disc->IsEnabled(false, true, false);

		dae::Event e(dae::make_sdbm_hash("ACTOR_DROPPED"));
		m_disc->GetSubject()->NotifyObservers(e);
	}

	return nullptr;
}