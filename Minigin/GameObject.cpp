#include "GameObject.h"
#include "Component.h"
#include "Subject.h"
#include "SceneManager.h"

void dae::GameObject::Update()
{
	for (auto& component : m_components)
	{
		component->Update();
	}

	for (auto& child : m_children)
	{
		child->Update();
	}
}

void dae::GameObject::LateUpdate()
{
	for (auto& component : m_components)
	{
		component->LateUpdate();
	}

	for (auto& child : m_children)
	{
		child->LateUpdate();
	}
}

void dae::GameObject::Render()
{
	UpdateWorldPosition();
	for (auto& component : m_components)
	{
		component->Render(m_worldTransform);
	}

	for (auto& child : m_children)
	{
		child->Render();
	}
}

// position functions

void dae::GameObject::SetLocalPosition(float x, float y)
{
	m_localTransform.SetPosition(x, y, 0.0f);
	SetPositionDirty();
}

void dae::GameObject::SetLocalPosition(const glm::vec3& newPosition)
{
	m_localTransform.SetPosition(newPosition);
	SetPositionDirty();
}

const glm::vec3& dae::GameObject::GetWorldPosition()
{
	if (m_posIsDirty)
		UpdateWorldPosition();
	return m_worldTransform.GetPosition();
}

void dae::GameObject::UpdateWorldPosition()
{
	if (m_posIsDirty)
	{
		if (m_parent == nullptr)
			m_worldTransform = m_localTransform;
		else
			m_worldTransform.SetPosition(m_parent->GetWorldPosition() + m_localTransform.GetPosition());
	}
	m_posIsDirty = false;
}

void dae::GameObject::SetPositionDirty()
{
	m_posIsDirty = true;
	for (auto& child : m_children)
	{
		child->SetPositionDirty();
	}
}

// parent-child functions

void dae::GameObject::SetParent(GameObject* newParent, bool keepWorldPosition)
{
	if (newParent != this
		&& !IsParentOf(newParent)
		&& m_parent != newParent)	
	{
		GameObject* thisOwnership{ nullptr };

		if (m_parent == nullptr)
		{
			SetLocalPosition(GetWorldPosition());
			thisOwnership = SceneManager::GetInstance().GetActiveScene()->GetGameObjectOwnership(this);
		}
		else 
		{
			if (keepWorldPosition)
			{
				SetLocalPosition(GetWorldPosition() - m_parent->GetWorldPosition());
			}
			SetPositionDirty();

			thisOwnership = m_parent->RemoveChild(this);
		}

		m_parent = newParent;

		if (!thisOwnership)
		{
			// currently owner by main.cpp, but scene has a check so you can't still add it 
			// when an object already has a parent
			thisOwnership = this;
		}

		if (m_parent)
		{
			m_parent->AddChild(thisOwnership);
		}
		else
		{
			SceneManager::GetInstance().GetActiveScene()->Add(std::unique_ptr<GameObject>(thisOwnership));
		}
	}
}

bool dae::GameObject::IsParentOf(GameObject* possibleParent) const
{
	bool rootParentReached{ false };
	GameObject* parentToCheck{ possibleParent->GetParent() };

	// check if current GameObject is the parent of possibleParent or any of its parents
	while (!rootParentReached)
	{
		if (parentToCheck == nullptr)
		{
			rootParentReached = true;
		}
		else if (parentToCheck == this)
		{
			return true;
		}
		else
		{
			parentToCheck = parentToCheck->GetParent();
		}
	}

	return false;
}

void dae::GameObject::AddChild(GameObject* newChild)
{
	m_children.push_back(std::unique_ptr<GameObject>(newChild));
}

[[nodiscard]] dae::GameObject* dae::GameObject::RemoveChild(GameObject* childToRemove)
{
	// get object
	auto it = std::find_if(m_children.begin(), m_children.end(), [childToRemove](auto& ptr) { return ptr.get() == childToRemove; });

	if (it != m_children.end())
	{
		// release ownership
		GameObject* object = it->release();
		*it = nullptr;

		// erase empty slot
		m_children.erase(std::remove(m_children.begin(), m_children.end(), nullptr));

		// give ownership
		return object;
	}
	else
	{
		return nullptr;
	}
}

void dae::GameObject::InitSubject() 
{ 
	m_subject = std::make_unique<Subject>();
}

dae::Subject* dae::GameObject::GetSubject() 
{ 
	return m_subject.get();
}