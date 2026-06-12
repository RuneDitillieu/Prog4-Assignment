#include "DiscActor.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Tags.h"
#include "LevelBase.h"
#include "SpriteComponent.h"

QBert::DiscActorComp::DiscActorComp(dae::GameObject* pOwner, const glm::vec2& tile)
	: dae::Component(pOwner)
	, m_tile(tile)
{ 
	m_pState = std::make_unique<IdleDiscState>(pOwner);
	m_pState->OnEnter();

	m_pConnSprite = pOwner->GetComponent<dae::SpriteComp>();
}

QBert::DiscActorComp::~DiscActorComp()
{
	auto qberts = dae::SceneManager::GetInstance().GetActiveScene()->GetObjectsByTag(dae::Tag(Tag::QBert));
	for (auto qbert : qberts)
	{
		qbert->GetSubject()->RemoveObserver(this);
	}
}

void QBert::DiscActorComp::Start()
{
	auto qberts = dae::SceneManager::GetInstance().GetActiveScene()->GetObjectsByTag(dae::Tag(Tag::QBert));
	for (auto qbert : qberts)
	{
		qbert->GetSubject()->AddObserver(this);
	}
}

void QBert::DiscActorComp::Update()
{
	auto state = m_pState->Update();
	if (state != nullptr)
	{
		m_pState->OnExit();
		m_pState.reset(state.release());
		m_pState->OnEnter();
	}
}

void QBert::DiscActorComp::Notify(dae::Event event, dae::Subject* subject)
{
	auto state = m_pState->OnNotify(event, subject);
	if (state != nullptr)
	{
		m_pState->OnExit();
		m_pState.reset(state.release());
		m_pState->OnEnter();
	}
}

void QBert::DiscActorComp::ResetDisc(const glm::vec2& newTile, LevelBase* level)
{
	m_tile = newTile;

	m_pState = std::make_unique<IdleDiscState>(GetOwner());
	m_pState->OnEnter();

	GetOwner()->SetLocalPosition(level->GetMiddlePosOfTile(static_cast<int>(newTile.x), static_cast<int>(newTile.y))
			- glm::vec3((m_pConnSprite->GetSpriteSize().x * GetOwner()->GetScale() / 2.f),
			m_pConnSprite->GetSpriteSize().y * GetOwner()->GetScale() / 2.f, 0));
}

std::type_index QBert::DiscActorComp::GetType() const
{
	return typeid(DiscActorComp);
}