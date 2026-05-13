#include "TileComponent.h"
#include "GameObject.h"
#include "RenderComponent.h"

QBert::TileComp::TileComp(dae::GameObject* pOwner, int tileType, bool revertable, int start, int win, int middle)
	: dae::Component(pOwner)
	, m_tileType(tileType)
	, m_revertable(revertable)
	, m_startTile(start)
	, m_winTile(win)
	, m_middleTile(middle)
	, m_currentTile(start)
{ 
	auto rc = pOwner->AddComponent(std::make_unique<dae::RenderComponent>(pOwner, "GroundPieces.png"));
	pOwner->SetScale(3.f);
	glm::vec2 size{ rc->GetSize().x / 22.5f, rc->GetSize().y / 6 };
	m_pConnSprite = pOwner->AddComponent(std::make_unique<dae::SpriteComp>(pOwner, rc, "GroundPieces.png", 1, 3, 
		size.x, size.y, glm::vec2(m_tileType * size.x, 0), false));

	m_middlePos = glm::vec3((size.x * pOwner->GetScale()) / 2.f, (size.y * pOwner->GetScale()) / 4.f, 0);
}

bool QBert::TileComp::Turn()
{
	int curTile{ m_currentTile };

	if (m_currentTile == m_startTile)
	{
		if (m_middleTile == -1)
			m_currentTile = m_winTile;
		else
			m_currentTile = m_middleTile;
	}
	else if (m_currentTile == m_middleTile)
	{
		m_currentTile = m_winTile;
	}
	else if(m_currentTile == m_winTile
		&& m_revertable)
	{
		m_currentTile = m_startTile;
	}

	m_pConnSprite->SetCurFrame(m_currentTile);

	if (curTile == m_currentTile)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void QBert::TileComp::Revert()
{
	if (m_currentTile == m_startTile)
	{
		return;
	}
	else if (m_currentTile == m_middleTile 
		|| (m_currentTile == m_winTile && m_middleTile == -1))
	{
		m_currentTile = m_startTile;
	}
	else if (m_currentTile == m_winTile)
	{
		m_currentTile = m_middleTile;
	}

	m_pConnSprite->SetCurFrame(m_currentTile);
}

glm::vec3 QBert::TileComp::GetMiddlePos() const
{
	glm::vec3 worldPos{ GetOwner()->GetWorldPosition().x,  GetOwner()->GetWorldPosition().y, 0 };
	worldPos += m_middlePos;
	return worldPos;
}

std::type_index QBert::TileComp::GetType() const
{
	return typeid(TileComp);
}