#ifndef COILYSTATES_H
#define COILYSTATES_H

#include "GameObject.h"
#include <typeindex>
#include "SpriteComponent.h"

namespace QBert
{
	class CoilyState
	{
	public:
		CoilyState(dae::GameObject* coily, dae::SpriteComp* spriteComp) : m_coily(coily), m_pConnSprite(spriteComp) {}
		virtual ~CoilyState() = default;
		virtual void OnEnter() { };
		virtual void OnExit() { };
		virtual std::unique_ptr<CoilyState> Update() { return nullptr; };

	protected:
		dae::GameObject* m_coily;
		dae::SpriteComp* m_pConnSprite;
	};


	class EggState : public CoilyState
	{
	public:
		EggState(dae::GameObject* coily, dae::SpriteComp* spriteComp);

		void OnEnter() override;
		std::unique_ptr<CoilyState> Update() override;

	private:
		glm::vec3 m_goalLocation{};
	};


	class SnakeState : public CoilyState
	{
	public:
		SnakeState(dae::GameObject* coily, dae::SpriteComp* spriteComp);

		void OnEnter() override;
		std::unique_ptr<CoilyState> Update() override;

	private:
	};
}

#endif // !COILYSTATES_H