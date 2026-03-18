#pragma once
#include "Component.h"

namespace dae
{
    class GameObject;
    class HealthComponent final : public Component
    {
    public:
        HealthComponent(GameObject* pOwner, int maxHealth);
        ~HealthComponent() = default;
        HealthComponent(const HealthComponent& other) = delete;
        HealthComponent(HealthComponent&& other) = delete;
        HealthComponent& operator=(const HealthComponent& other) = delete;
        HealthComponent& operator=(HealthComponent&& other) = delete;

        int GetMaxHealth() { return m_maxHealth; }
        int GetCurHealth() { return m_curHealth; }
        void TakeDamage(int damage);

        std::type_index GetType() const override;

    private:
        const int m_maxHealth;
        int m_curHealth;
    };
}