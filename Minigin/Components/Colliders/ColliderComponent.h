#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include "Component.h"
#include <glm/glm.hpp>

namespace dae
{
    class GameObject;
	class ColliderComp : public Component
	{
    public:
        ~ColliderComp() = default;
        ColliderComp(const ColliderComp& other) = delete;
        ColliderComp(ColliderComp&& other) = delete;
        ColliderComp& operator=(const ColliderComp& other) = delete;
        ColliderComp& operator=(ColliderComp&& other) = delete;

        virtual bool IsColliding(const glm::vec2& topLeft, const glm::vec2& bottomRight) const = 0;
        virtual bool IsColliding(const glm::vec2& center, float radius) const = 0;

    protected:
        explicit ColliderComp(dae::GameObject* pOwner) : Component(pOwner) {}
        bool m_isTrigger{ false };
	};
}

#endif // !COLLIDERCOMPONENT_H