#include "GameObject.h"
#include "Component.h"

void dae::GameObject::Update(float deltaTime)
{
	for (auto& component : m_components)
	{
		component->Update(deltaTime);
	}
}

void dae::GameObject::Render() const
{
	for (auto& component : m_components)
	{
		component->Render(m_transform);
	}
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}

void dae::GameObject::SetParent(GameObject* newParent)
{
	if (newParent != this
		&& !IsParentOf(newParent)
		&& m_parent != newParent)	
	{
		// update transform

		if (m_parent != nullptr)
		{
			m_parent->RemoveChild(this);
		}

		m_parent = newParent;

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