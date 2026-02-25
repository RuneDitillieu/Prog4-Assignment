#pragma once
#include "Component.h"
#include <glm/glm.hpp>

namespace dae
{
	class GameObject;
	class RotatorComponent final : public Component
	{
	public:
		RotatorComponent(GameObject* pOwner, const glm::vec3& pivot, float clockwiseDirection);
		~RotatorComponent() = default;
		RotatorComponent(const RotatorComponent& other) = delete;
		RotatorComponent(RotatorComponent&& other) = delete;
		RotatorComponent& operator=(const RotatorComponent& other) = delete;
		RotatorComponent& operator=(RotatorComponent&& other) = delete;

		void Update(float deltaTime) override;

		std::type_index GetType() const override;

	private:
		glm::vec3 m_pivot;	// local space
		float m_clockwiseDirection;
		float m_angle{ 0.f };
		float m_distance;
	};
}