#ifndef COILYSTATES_H
#define COILYSTATES_H

#include "GameObject.h"
#include "SpriteComponent.h"
#include "Events.h"

namespace QBert
{
	class QBertMoveComp;
	class LevelBase;

	// base state
	class CoilyState
	{
	public:
		CoilyState(dae::GameObject* coily, dae::SpriteComp* spriteComp, 
			QBertMoveComp* moveComp, LevelBase* level, const std::vector<QBertMoveComp*>& qbertMoveComps);
		virtual ~CoilyState() = default;
		virtual void OnEnter() { };
		virtual void OnExit() { };
		virtual std::unique_ptr<CoilyState> Update();
		virtual std::unique_ptr<CoilyState> OnNotify(dae::Event, dae::Subject*);

	protected:
		dae::GameObject* m_coily;
		dae::SpriteComp* m_pConnSprite;
		QBertMoveComp* m_pMoveComp;
		LevelBase* m_pConnLevel;
		std::vector<QBertMoveComp*> m_pQBertMoveComps;
	};


	// egg states

	class DroppingEggState final : public CoilyState
	{
	public:
		DroppingEggState(dae::GameObject* coily, dae::SpriteComp* spriteComp,
			QBertMoveComp* moveComp, LevelBase* level, const std::vector<QBertMoveComp*>& qbertMoveComps);

		void OnEnter() override;
		std::unique_ptr<CoilyState> Update() override;
	};

	class IdleEggState final : public CoilyState
	{
	public:
		IdleEggState(dae::GameObject* coily, dae::SpriteComp* spriteComp, 
			QBertMoveComp* moveComp, LevelBase* level, const std::vector<QBertMoveComp*>& qbertMoveComps);

		void OnEnter() override;
		void OnExit() override;
		std::unique_ptr<CoilyState> Update() override;

	private:
		float m_secPassed{ 0.f };
		const float m_idleSec{ 0.5f };
	};

	class JumpingEggState final : public CoilyState
	{
	public:
		JumpingEggState(dae::GameObject* coily, dae::SpriteComp* spriteComp, 
			QBertMoveComp* moveComp, LevelBase* level, const std::vector<QBertMoveComp*>& qbertMoveComps);

		void OnEnter() override;
		std::unique_ptr<CoilyState> Update() override;
	};


	// snake states

	class IdleSnakeState final : public CoilyState
	{
	public:
		IdleSnakeState(dae::GameObject* coily, dae::SpriteComp* spriteComp, 
			QBertMoveComp* moveComp, LevelBase* level, const std::vector<QBertMoveComp*>& qbertMoveComps);

		void OnEnter() override;
		std::unique_ptr<CoilyState> Update() override;

	private:
		float m_secPassed{ 0.f };
		const float m_idleSec{ 0.5f };
	};

	class JumpingSnakeState final : public CoilyState
	{
	public:
		JumpingSnakeState(dae::GameObject* coily, dae::SpriteComp* spriteComp,
			QBertMoveComp* moveComp, LevelBase* level, const std::vector<QBertMoveComp*>& qbertMoveComps);

		void OnEnter() override;
		std::unique_ptr<CoilyState> Update() override;
		std::unique_ptr<CoilyState> OnNotify(dae::Event event, dae::Subject* subject) override;
	};

	class FallingSnakeState final : public CoilyState
	{
	public:
		FallingSnakeState(dae::GameObject* coily, dae::SpriteComp* spriteComp, 
			QBertMoveComp* moveComp, LevelBase* level, const std::vector<QBertMoveComp*>& qbertMoveComps);

		void OnEnter() override;
		std::unique_ptr<CoilyState> Update() override;
	};

	class StunnedCoilyState final : public CoilyState
	{
	public:
		StunnedCoilyState(dae::GameObject* coily, dae::SpriteComp* spriteComp,
			QBertMoveComp* moveComp, LevelBase* level, const std::vector<QBertMoveComp*>& qbertMoveComps);

		void OnEnter() override;
		std::unique_ptr<CoilyState> Update() override;

	private:
		float m_secPassed{ 0.f };
		const float m_maxSec{ 1.5f };
	};
}

#endif // !COILYSTATES_H