#pragma once
#include <typeindex>

namespace dae
{
    class Transform;
    class GameObject;
    class Component
    {
    public:
        virtual ~Component() = default;
        Component(const Component& other) = delete;
        Component(Component&& other) = delete;
        Component& operator=(const Component& other) = delete;
        Component& operator=(Component&& other) = delete;

        virtual void Update(float) {}
        virtual void Render(const Transform&) const {}

        virtual std::type_index GetType() const = 0;

        GameObject* GetOwner() const { return m_pOwner; }

    protected:
        explicit Component(GameObject* pOwner) : m_pOwner(pOwner) {}

    private:
        GameObject* m_pOwner;
    };
}

