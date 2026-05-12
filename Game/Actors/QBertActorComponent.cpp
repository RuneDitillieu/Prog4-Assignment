#include "QBertActorComponent.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "SpriteComponent.h"
#include "QBertMoveComponent.h"

QBert::QBertActorComp::QBertActorComp(dae::GameObject* pOwner)
	: dae::Component(pOwner)
{ 
	pOwner->SetLocalPosition(100, 300);
	pOwner->SetScale(3.f);
	auto rc = pOwner->AddComponent(std::make_unique<dae::RenderComponent>(pOwner, "QBertSprites.png"));
	pOwner->AddComponent(std::make_unique<dae::SpriteComp>(pOwner, "QBertSprites.png", 8, 1, false));
	pOwner->AddComponent(std::make_unique<QBert::QBertMoveComp>(pOwner, 
		glm::vec3((rc->GetSize().x / 8.f) * pOwner->GetScale() / 2.f, (rc->GetSize().y * pOwner->GetScale()) / 10.f * 9.f, 0)));
}

std::type_index QBert::QBertActorComp::GetType() const
{
	return typeid(QBertActorComp);
}