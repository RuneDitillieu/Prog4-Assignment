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
		explicit SlickSamState(dae::GameObject* slickSam, dae::SpriteComp* spriteComp,
			QBertMoveComp* moveComp, LevelBase* level, const std::vector<QBertMoveComp*>& qbertMoveComps);
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
		std::vector<QBertMoveComp*> m_pQBertMoveComps;
	};

	class DroppingSlickSamState final : public SlickSamState
	{
	public:
		explicit DroppingSlickSamState(dae::GameObject* coily, dae::SpriteComp* spriteComp,
			QBertMoveComp* moveComp, LevelBase* level, const std::vector<QBertMoveComp*>& qbertMoveComps);

		void OnEnter() override;
		std::unique_ptr<SlickSamState> Update() override;
	};

	class IdleSlickSamState final : public SlickSamState
	{
	public:
		explicit IdleSlickSamState(dae::GameObject* slickSam, dae::SpriteComp* spriteComp,
			QBertMoveComp* moveComp, LevelBase* level, const std::vector<QBertMoveComp*>& qbertMoveComps);

		void OnEnter() override;
		std::unique_ptr<SlickSamState> Update() override;

	private:
		float m_secPassed{ 0.f };
		const float m_idleSec{ 0.5f };
	};

	class JumpingSlickSamState final : public SlickSamState
	{
	public:
		explicit JumpingSlickSamState(dae::GameObject* slickSam, dae::SpriteComp* spriteComp,
			QBertMoveComp* moveComp, LevelBase* level, const std::vector<QBertMoveComp*>& qbertMoveComps);

		void OnEnter() override;
		std::unique_ptr<SlickSamState> Update() override;
		std::unique_ptr<SlickSamState> OnNotify(dae::Event, dae::Subject*) override;
	};

	class FallingSlickSamState final : public SlickSamState
	{
	public:
		explicit FallingSlickSamState(dae::GameObject* slickSam, dae::SpriteComp* spriteComp,
			QBertMoveComp* moveComp, LevelBase* level, const std::vector<QBertMoveComp*>& qbertMoveComps);

		void OnEnter() override;
		std::unique_ptr<SlickSamState> Update() override;
	};

	class StunnedSlickSamState final : public SlickSamState
	{
	public:
		explicit StunnedSlickSamState(dae::GameObject* slickSam, dae::SpriteComp* spriteComp,
			QBertMoveComp* moveComp, LevelBase* level, const std::vector<QBertMoveComp*>& qbertMoveComps);

		void OnEnter() override;
		std::unique_ptr<SlickSamState> Update() override;

	private:
		float m_secPassed{ 0.f };
		const float m_maxSec{ 1.5f };
	};
}

#endif