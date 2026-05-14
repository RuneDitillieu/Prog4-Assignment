#include "QBertActor.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "SpriteComponent.h"
#include "QBertMoveComponent.h"
#include "SpriteComponent.h"

QBert::QBertActorComp::QBertActorComp(dae::GameObject* pOwner)
	: dae::Component(pOwner)
{ 
	pOwner->SetLocalPosition(100, 300);
	pOwner->SetScale(3.f);
	auto rc = pOwner->AddComponent(std::make_unique<dae::RenderComponent>(pOwner, "QBertSprites.png"));
	auto sprite = pOwner->AddComponent(std::make_unique<dae::SpriteComp>(pOwner, "QBertSprites.png", 8, 1, false));
	auto move = pOwner->AddComponent(std::make_unique<QBert::QBertMoveComp>(pOwner, 
		glm::vec3((rc->GetSize().x / 8.f) * pOwner->GetScale() / 2.f, (rc->GetSize().y * pOwner->GetScale()) / 10.f * 9.f, 0),
		glm::vec2(0, 0), true, false));
	sprite->SetCurFrame(5);

	m_pState = std::make_unique<QBert::IdleQBertState>(pOwner, sprite, move);
}

void QBert::QBertActorComp::Update()
{
	auto state = m_pState->Update();
	if (state != nullptr)
	{
		m_pState->OnExit();
		m_pState.reset(state.release());
		m_pState->OnEnter();
	}
}

void QBert::QBertActorComp::Notify(dae::Event event, dae::Subject* subject)
{
	auto state = m_pState->OnNotify(event, subject);
	if (state != nullptr)
	{
		m_pState->OnExit();
		m_pState.reset(state.release());
		m_pState->OnEnter();
	}
}

std::type_index QBert::QBertActorComp::GetType() const
{
	return typeid(QBertActorComp);
}