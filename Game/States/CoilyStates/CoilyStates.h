#ifndef COILYSTATES_H
#define COILYSTATES_H

#include "GameObject.h"
#include "SpriteComponent.h"

namespace QBert
{
	class CoilyState
	{
	public:
		CoilyState(dae::GameObject* coily, dae::SpriteComp* spriteComp) 
			: m_coily(coily), m_pConnSprite(spriteComp) {}
		virtual ~CoilyState() = default;
		virtual void OnEnter() { };
		virtual void OnExit() { };
		virtual std::unique_ptr<CoilyState> Update() { return nullptr; };

	protected:
		dae::GameObject* m_coily;
		dae::SpriteComp* m_pConnSprite;
	};

	class QBertMoveComp;
	class LevelBase;
	class IdleEggState : public CoilyState
	{
	public:
		IdleEggState(dae::GameObject* coily, dae::SpriteComp* spriteComp, 
			QBertMoveComp* moveComp, LevelBase* level);

		void OnEnter() override;
		std::unique_ptr<CoilyState> Update() override;

	private:
		float m_secPassed{ 0.f };
		const float m_idleSec{ 0.5f };

		QBertMoveComp* m_pMoveComp;
		LevelBase* m_pConnLevel;
	};

	class JumpingEggState : public CoilyState
	{
	public:
		JumpingEggState(dae::GameObject* coily, dae::SpriteComp* spriteComp, 
			QBertMoveComp* moveComp, LevelBase* level);

		void OnEnter() override;
		std::unique_ptr<CoilyState> Update() override;

	private:
		QBertMoveComp* m_pMoveComp;
		LevelBase* m_pConnLevel;
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