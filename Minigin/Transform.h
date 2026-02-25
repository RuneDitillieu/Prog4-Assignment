#pragma once
#include <glm/glm.hpp>

namespace dae
{
	class Transform final
	{
	public:
		const glm::vec3& GetPosition() const { return m_position; }
		void SetPosition(float x, float y, float z = 0);
		void SetPosition(const glm::vec3& position);
		float GetScale() const { return m_scale; };
		void SetScale(float scale);

	private:
		glm::vec3 m_position;
		float m_scale{ 1.f };
	};
}
