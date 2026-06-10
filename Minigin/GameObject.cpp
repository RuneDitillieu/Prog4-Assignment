#include "GameObject.h"
#include "Subject.h"
#include "SceneManager.h"

void dae::GameObject::Start()
{
	for (auto& component : m_components)
	{
		component->Start();
	}

	for (auto& child : m_children)
	{
		child->Start();
	}
}

void dae::GameObject::Update()
{
	if (!m_isEnabled) return;

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
	if (!m_isEnabled) return;

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
	if (!m_isEnabled) return;

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
			m_worldTransform.SetPosition(m_localTransform.GetPosition());
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
		std::unique_ptr<GameObject> thisOwnership{ nullptr };

		if (newParent == nullptr)
		{
			SetLocalPosition(GetWorldPosition());
		}
		else 
		{
			if (keepWorldPosition)
			{
				SetLocalPosition(GetWorldPosition() - newParent->GetWorldPosition());
			}
			SetPositionDirty();
		}

		if (m_parent == nullptr)
		{
			thisOwnership = SceneManager::GetInstance().GetActiveScene()->GetGameObjectOwnership(this);
		}
		else
		{
			thisOwnership = m_parent->RemoveChild(this);
		}

		m_parent = newParent;

		if (!thisOwnership)
		{
			// currently owned by main.cpp, but scene has a check so you can't still add it
			// when an object already has a parent:
			// will just release the double
			thisOwnership = std::unique_ptr<GameObject>(this);
		}

		if (m_parent)
		{
			m_parent->AddChild(std::move(thisOwnership));
		}
		else
		{
			SceneManager::GetInstance().GetActiveScene()->Add(std::move(thisOwnership));
		}
	}
}

bool dae::GameObject::IsParentOf(GameObject* possibleParent) const
{
	if (possibleParent == nullptr) return false;

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

void dae::GameObject::AddChild(std::unique_ptr<GameObject> newChild)
{
	m_children.push_back(std::move(newChild));
}

[[nodiscard]] std::unique_ptr<dae::GameObject> dae::GameObject::RemoveChild(GameObject* childToRemove)
{
	// get object
	auto it = std::find_if(m_children.begin(), m_children.end(), [childToRemove](auto& ptr) { return ptr.get() == childToRemove; });

	if (it != m_children.end())
	{
		// grab ownership
		std::unique_ptr<GameObject> object = std::move(*it);
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

dae::Subject* dae::GameObject::GetSubject() 
{ 
	return m_subject.get();
}

void dae::GameObject::IsEnabled(bool isEnabled, bool applyToComponents, bool applyToChildren)
{
	m_isEnabled = isEnabled;

	if (applyToComponents)
	{
		for (auto& comp : m_components)
		{
			comp->m_isEnabled = isEnabled;
		}
	}

	if (applyToChildren)
	{
		for (auto& child : m_children)
		{
			child->IsEnabled(isEnabled, applyToComponents, applyToChildren);
		}
	}
}

std::vector<dae::GameObject*> dae::GameObject::GetChildrenByTag(Tag tag) const
{
	std::vector<GameObject*> objects{};
	for (auto& child : m_children)
	{
		if (child->GetTag() == tag)
		{
			objects.emplace_back(child.get());
		}

		for (auto& childChild : child->GetChildrenByTag(tag))
		{
			objects.emplace_back(childChild);
		}
	}

	return objects;
}