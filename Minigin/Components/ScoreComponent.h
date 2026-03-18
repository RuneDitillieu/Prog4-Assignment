#pragma once
#include "Component.h"

namespace dae
{
	class GameObject;
	class ScoreComponent final : public Component
	{
    public:
        ScoreComponent(GameObject* pOwner);
        ~ScoreComponent() = default;
        ScoreComponent(const ScoreComponent& other) = delete;
        ScoreComponent(ScoreComponent&& other) = delete;
        ScoreComponent& operator=(const ScoreComponent& other) = delete;
        ScoreComponent& operator=(ScoreComponent&& other) = delete;

        void AddToScore(int amount);
        int GetScore() { return m_score; }

        std::type_index GetType() const override;

    private:
        int m_score{ 0 };
	};
}