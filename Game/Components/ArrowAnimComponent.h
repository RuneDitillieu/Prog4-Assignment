#ifndef ARROWANIMCOMPONENT_H
#define ARROWANIMCOMPONENT_H

#include <vector>
#include "Component.h"

namespace QBert
{
	class GameObject;
	class ArrowAnimComp final : public dae::Component
	{
	public:
		ArrowAnimComp(dae::GameObject* pOwner, std::vector<dae::GameObject*> arrows);
		~ArrowAnimComp() = default;
		ArrowAnimComp(const ArrowAnimComp& other) = delete;
		ArrowAnimComp(ArrowAnimComp&& other) = delete;
		ArrowAnimComp& operator=(const ArrowAnimComp& other) = delete;
		ArrowAnimComp& operator=(ArrowAnimComp&& other) = delete;

		void Update() override;

		std::type_index GetType() const override;

	private:
		float m_secPassed{ 0.f };
		const float m_secToWait{ 0.5f };
		std::vector<dae::GameObject*> m_pArrows;
	};
}

#endif // !ARROWANIMCOMPONENT_H
