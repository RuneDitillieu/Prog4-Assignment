#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

#include "Component.h"

namespace dae
{
    class GameObject;
    class HealthComponent final : public Component
    {
    public:
        explicit HealthComponent(GameObject* pOwner, int maxHealth, int startLive, int maxLives);
        ~HealthComponent() = default;
        HealthComponent(const HealthComponent& other) = delete;
        HealthComponent(HealthComponent&& other) = delete;
        HealthComponent& operator=(const HealthComponent& other) = delete;
        HealthComponent& operator=(HealthComponent&& other) = delete;

        int GetMaxHealth() { return m_maxHealth; }
        int GetCurHealth() { return m_curHealth; }
        int GetCurLives() { return m_remainingLives; }
        void TakeDamage(int damage);
        void LoseLife();
        void GainLife(int amount = 1);

        std::type_index GetType() const override;

    private:
        const int m_maxHealth;
        int m_curHealth;

        const int m_maxLives;
        int m_remainingLives;
    };
}

#endif // !HEALTHCOMPONENT_H