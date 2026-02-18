#pragma once

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
    protected:
        GameObject* m_owner;
    };
}

