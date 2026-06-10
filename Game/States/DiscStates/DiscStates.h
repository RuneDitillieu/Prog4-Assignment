#ifndef DISCSTATES_H
#define DISCSTATES_H

#include "Events.h"
#include <memory>
#include "GameObject.h"

namespace QBert
{
	class Subject;
	class DiscState
	{
	public:
		DiscState(dae::GameObject* disc) : m_disc(disc) {}
		virtual ~DiscState() = default;
		virtual void OnEnter() {};
		virtual void OnExit() {};
		virtual std::unique_ptr<DiscState> Update() { return nullptr; };
		virtual std::unique_ptr<DiscState> OnNotify(dae::Event, dae::Subject*) { return nullptr; };

	protected:
		dae::GameObject* m_disc;
	};

	class IdleDiscState final : public DiscState
	{
	public:
		IdleDiscState(dae::GameObject* disc);

		std::unique_ptr<DiscState> OnNotify(dae::Event event, dae::Subject*) override;
	};

	class MovingDiscState final : public DiscState
	{
	public:
		MovingDiscState(dae::GameObject* disc);

		void OnEnter() override;
		std::unique_ptr<DiscState> Update() override;

	private:
		const float m_speed{ 120.f };
		glm::vec3 m_goalLocation{};
	};

	class DropOffDiscState final : public DiscState
	{
	public:
		DropOffDiscState(dae::GameObject* disc);

		std::unique_ptr<DiscState> Update() override;

	private:
		float m_secPassed{ 0.f };
		const float m_maxSec{ 1.f };
	};
}

#endif // !DISCSTATES_H

