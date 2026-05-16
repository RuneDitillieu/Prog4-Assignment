#include "DiscStates.h"

QBert::IdleDiscState::IdleDiscState(dae::GameObject* disc)
	: QBert::DiscState(disc)
{ }

std::unique_ptr<QBert::DiscState> QBert::IdleDiscState::OnNotify(dae::Event event, dae::Subject*)
{
	if (event.id == dae::make_sdbm_hash("DISC_USED"))
	{
		return std::make_unique<QBert::MovingDiscState>(m_disc);
	}

	return nullptr;
}