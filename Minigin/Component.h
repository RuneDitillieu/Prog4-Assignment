#pragma once
#include <typeindex>

namespace dae
{
    class GameObject;
    class Component
    {
    public:
        explicit Component(GameObject* owner);
        virtual ~Component() = default;

        virtual void Update(float deltaTime);
        virtual void Render() const;

        virtual std::type_index GetType() const = 0;

    protected:
        GameObject* m_owner;
    };
}

