#pragma once
#include "GameObject.h"
#include <typeindex>

namespace QBert
{
	class CoilyState
	{
	public:
		CoilyState(dae::GameObject* coily) : m_coily(coily) {}
		virtual ~CoilyState() = default;
		virtual void OnEnter() { };
		virtual void OnExit() { };
		virtual std::unique_ptr<CoilyState> Update() { return std::make_unique<CoilyState>(m_coily); };
		virtual std::type_index GetType() const { return typeid(CoilyState); }

	protected:
		dae::GameObject* m_coily;
	};

	class EggState : public CoilyState
	{
	public:
		EggState(dae::GameObject* coily);

		void OnEnter() override;
		std::unique_ptr<CoilyState> Update() override;
		std::type_index GetType() const override;

	private:
		glm::vec3 m_goalLocation{};
	};

	class SnakeState : public CoilyState
	{
	public:
		SnakeState(dae::GameObject* coily);

		void OnEnter() override;
		std::unique_ptr<CoilyState> Update() override;
		std::type_index GetType() const override;

	private:
	};
}