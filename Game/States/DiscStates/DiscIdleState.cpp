#include "DiscStates.h"

QBert::IdleDiscState::IdleDiscState(dae::GameObject* disc)
	: DiscState(disc)
{ }

std::unique_ptr<QBert::DiscState> QBert::IdleDiscState::OnNotify(dae::Event event, dae::Subject*)
{
	if (event.id == dae::make_sdbm_hash("DISC_USED")
		&& event.args->object == m_disc)
	{
		return std::make_unique<MovingDiscState>(m_disc);
	}

	return nullptr;
}