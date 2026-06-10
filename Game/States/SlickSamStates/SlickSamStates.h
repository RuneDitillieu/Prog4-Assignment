#ifndef SLICKSAMSTATES_H
#define SLICKSAMSTATES_H

#include "GameObject.h"
#include "SpriteComponent.h"

namespace QBert
{
	class QBertMoveComp;
	class LevelBase;

	// base state
	class SlickSamState
	{
	public:
		SlickSamState(dae::GameObject* slickSam, dae::SpriteComp* spriteComp,
			QBertMoveComp* moveComp, LevelBase* level, QBertMoveComp* qbertMoveComp);
		virtual ~SlickSamState() = default;
		virtual void OnEnter() {};
		virtual void OnExit() {};
		virtual std::unique_ptr<SlickSamState> Update();
		virtual std::unique_ptr<SlickSamState> OnNotify(dae::Event, dae::Subject*);

	protected:
		dae::GameObject* m_slickSam;
		dae::SpriteComp* m_pConnSprite;
		QBertMoveComp* m_pMoveComp;
		LevelBase* m_pConnLevel;
		QBertMoveComp* m_pQBertMoveComp;
	};

	class DroppingSlickSamState final : public SlickSamState
	{
	public:
		DroppingSlickSamState(dae::GameObject* coily, dae::SpriteComp* spriteComp,
			QBertMoveComp* moveComp, LevelBase* level, QBertMoveComp* qbertMoveComp);

		void OnEnter() override;
		std::unique_ptr<SlickSamState> Update() override;
	};

	class IdleSlickSamState final : public SlickSamState
	{
	public:
		IdleSlickSamState(dae::GameObject* slickSam, dae::SpriteComp* spriteComp,
			QBertMoveComp* moveComp, LevelBase* level, QBertMoveComp* qbertMoveComp);

		void OnEnter() override;
		std::unique_ptr<SlickSamState> Update() override;

	private:
		float m_secPassed{ 0.f };
		const float m_idleSec{ 0.5f };
	};

	class JumpingSlickSamState final : public SlickSamState
	{
	public:
		JumpingSlickSamState(dae::GameObject* slickSam, dae::SpriteComp* spriteComp,
			QBertMoveComp* moveComp, LevelBase* level, QBertMoveComp* qbertMoveComp);

		void OnEnter() override;
		std::unique_ptr<SlickSamState> Update() override;
		std::unique_ptr<SlickSamState> OnNotify(dae::Event, dae::Subject*) override;
	};

	class FallingSlickSamState final : public SlickSamState
	{
	public:
		FallingSlickSamState(dae::GameObject* slickSam, dae::SpriteComp* spriteComp,
			QBertMoveComp* moveComp, LevelBase* level, QBertMoveComp* qbertMoveComp);

		void OnEnter() override;
		std::unique_ptr<SlickSamState> Update() override;
	};

	class StunnedSlickSamState final : public SlickSamState
	{
	public:
		StunnedSlickSamState(dae::GameObject* slickSam, dae::SpriteComp* spriteComp,
			QBertMoveComp* moveComp, LevelBase* level, QBertMoveComp* qbertMoveComp);

		void OnEnter() override;
		std::unique_ptr<SlickSamState> Update() override;

	private:
		float m_secPassed{ 0.f };
		const float m_maxSec{ 1.5f };
	};
}

#endif