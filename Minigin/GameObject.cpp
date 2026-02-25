#include "GameObject.h"
#include "Component.h"

void dae::GameObject::Update(float deltaTime)
{
	UpdateWorldPosition();
	for (auto& component : m_components)
	{
		component->Update(deltaTime);
	}
}

void dae::GameObject::Render() const
{
	for (auto& component : m_components)
	{
		component->Render(m_worldTransform);
	}
}

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
	for (GameObject* child : m_children)
	{
		child->SetPositionDirty();
	}
}

void dae::GameObject::SetParent(GameObject* newParent, bool keepWorldPosition)
{
	if (newParent != this
		&& !IsParentOf(newParent)
		&& m_parent != newParent)	
	{
		if (m_parent == nullptr)
		{
			SetLocalPosition(GetWorldPosition());
		}
		else 
		{
			if (keepWorldPosition)
			{
				SetLocalPosition(GetWorldPosition() - m_parent->GetWorldPosition());
			}
			SetPositionDirty();

			m_parent->RemoveChild(this);
		}

		m_parent = newParent;

		if(m_parent)
			m_parent->AddChild(this);
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
	m_children.push_back(newChild);
}

void dae::GameObject::RemoveChild(GameObject* childToRemove)
{
	m_children.erase(
		std::remove(m_children.begin(), m_children.end(), childToRemove)
	);
}

