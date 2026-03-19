#pragma once
#include "Component.h"
#include <glm/glm.hpp>

namespace dae
{
	class GameObject;
	class RotatorComponent final : public Component
	{
	public:
		RotatorComponent(GameObject* pOwner, float clockwiseSpeed);
		~RotatorComponent() = default;
		RotatorComponent(const RotatorComponent& other) = delete;
		RotatorComponent(RotatorComponent&& other) = delete;
		RotatorComponent& operator=(const RotatorComponent& other) = delete;
		RotatorComponent& operator=(RotatorComponent&& other) = delete;

		void Update() override;

		std::type_index GetType() const override;

	private:
		float m_clockwiseSpeed;
		float m_angle{ 0.f };
		float m_distance;
	};
}