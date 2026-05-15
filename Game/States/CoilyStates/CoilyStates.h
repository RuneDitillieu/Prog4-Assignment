#ifndef COILYSTATES_H
#define COILYSTATES_H

#include "GameObject.h"
#include "SpriteComponent.h"

namespace QBert
{
	class QBertMoveComp;
	class LevelBase;

	// base state
	class CoilyState
	{
	public:
		CoilyState(dae::GameObject* coily, dae::SpriteComp* spriteComp, QBertMoveComp* moveComp, LevelBase* level)
			: m_coily(coily), m_pConnSprite(spriteComp), m_pMoveComp(moveComp), m_pConnLevel(level) {}
		virtual ~CoilyState() = default;
		virtual void OnEnter() { };
		virtual void OnExit() { };
		virtual std::unique_ptr<CoilyState> Update() { return nullptr; };

	protected:
		dae::GameObject* m_coily;
		dae::SpriteComp* m_pConnSprite;
		QBertMoveComp* m_pMoveComp;
		LevelBase* m_pConnLevel;
	};


	// egg states

	class IdleEggState : public CoilyState
	{
	public:
		IdleEggState(dae::GameObject* coily, dae::SpriteComp* spriteComp, 
			QBertMoveComp* moveComp, LevelBase* level);

		void OnEnter() override;
		void OnExit() override;
		std::unique_ptr<CoilyState> Update() override;

	private:
		float m_secPassed{ 0.f };
		const float m_idleSec{ 0.5f };
	};

	class JumpingEggState : public CoilyState
	{
	public:
		JumpingEggState(dae::GameObject* coily, dae::SpriteComp* spriteComp, 
			QBertMoveComp* moveComp, LevelBase* level);

		void OnEnter() override;
		std::unique_ptr<CoilyState> Update() override;

	private:
	};


	// snake states

	class IdleSnakeState : public CoilyState
	{
	public:
		IdleSnakeState(dae::GameObject* coily, dae::SpriteComp* spriteComp, QBertMoveComp* moveComp, LevelBase* level, QBertMoveComp* qbertMoveComp);

		void OnEnter() override;
		std::unique_ptr<CoilyState> Update() override;

	private:
		float m_secPassed{ 0.f };
		const float m_idleSec{ 0.5f };

		QBertMoveComp* m_pQBertMoveComp;
	};

	class JumpingSnakeState : public CoilyState
	{
	public:
		JumpingSnakeState(dae::GameObject* coily, dae::SpriteComp* spriteComp, QBertMoveComp* moveComp, LevelBase* level, QBertMoveComp* qbertMoveComp);

		void OnEnter() override;
		std::unique_ptr<CoilyState> Update() override;

	private:
		QBertMoveComp* m_pQBertMoveComp;
	};
}

#endif // !COILYSTATES_H