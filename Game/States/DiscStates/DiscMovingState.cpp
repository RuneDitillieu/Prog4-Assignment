#include "DiscStates.h"
#include "DeltaTime.h"
#include "SceneManager.h"
#include "LevelBase.h"
#include "SpriteComponent.h"

QBert::MovingDiscState::MovingDiscState(dae::GameObject* disc)
	: DiscState(disc)
{ }

void QBert::MovingDiscState::OnEnter()
{
	glm::vec3 levelOrigin = dae::SceneManager::GetInstance().GetActiveScene()->GetFirstObjectByType<LevelBase>()->GetMiddlePosOfTile(-1, -1);
	glm::vec2 spriteOffset = m_disc->GetComponent<dae::SpriteComp>()->GetSpriteSize();
	m_goalLocation = levelOrigin + glm::vec3(-(spriteOffset.x * m_disc->GetScale() / 2.f),
		spriteOffset.y * m_disc->GetScale() / 2.f, 0);
}

std::unique_ptr<QBert::DiscState> QBert::MovingDiscState::Update()
{
	glm::vec3 moveDir{ m_goalLocation - m_disc->GetLocalPosition() };
	moveDir = glm::normalize(moveDir);

	m_disc->SetLocalPosition(m_disc->GetLocalPosition() + (moveDir * m_speed * dae::DeltaTime::GetInstance().GetDeltaTime()));

	if (glm::length(m_goalLocation - m_disc->GetLocalPosition()) <= 5.f)
	{
		return std::make_unique<DropOffDiscState>(m_disc);
	}

	return nullptr;
}