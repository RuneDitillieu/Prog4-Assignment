#ifndef PLAYERCOILYSTATES_H
#define PLAYERCOILYSTATES_H

#include "GameObject.h"
#include "SpriteComponent.h"
#include "Events.h"

namespace QBert
{
	class QBertMoveComp;
	class LevelBase;

	// base state
	class PlayerCoilyState
	{
	public:
		explicit PlayerCoilyState(dae::GameObject* coily, dae::SpriteComp* spriteComp,
			QBertMoveComp* moveComp, LevelBase* level, QBertMoveComp* qbertMoveComp);
		virtual ~PlayerCoilyState() = default;
		virtual void OnEnter() { };
		virtual void OnExit() { };
		virtual std::unique_ptr<PlayerCoilyState> Update();
		virtual std::unique_ptr<PlayerCoilyState> OnNotify(dae::Event, dae::Subject*);

	protected:
		dae::GameObject* m_coily;
		dae::SpriteComp* m_pConnSprite;
		QBertMoveComp* m_pMoveComp;
		LevelBase* m_pConnLevel;
		QBertMoveComp* m_pQBertMoveComp;
	};


	// egg states

	class PlayerDroppingEggState final : public PlayerCoilyState
	{
	public:
		explicit PlayerDroppingEggState(dae::GameObject* coily, dae::SpriteComp* spriteComp,
			QBertMoveComp* moveComp, LevelBase* level, QBertMoveComp* qbertMoveComp);

		void OnEnter() override;
		std::unique_ptr<PlayerCoilyState> Update() override;
	};

	class PlayerIdleEggState final : public PlayerCoilyState
	{
	public:
		explicit PlayerIdleEggState(dae::GameObject* coily, dae::SpriteComp* spriteComp,
			QBertMoveComp* moveComp, LevelBase* level, QBertMoveComp* qbertMoveComp);

		void OnEnter() override;
		void OnExit() override;
		std::unique_ptr<PlayerCoilyState> Update() override;

	private:
		float m_secPassed{ 0.f };
		const float m_idleSec{ 0.5f };
	};

	class PlayerJumpingEggState final : public PlayerCoilyState
	{
	public:
		explicit PlayerJumpingEggState(dae::GameObject* coily, dae::SpriteComp* spriteComp,
			QBertMoveComp* moveComp, LevelBase* level, QBertMoveComp* qbertMoveComp);

		void OnEnter() override;
		std::unique_ptr<PlayerCoilyState> Update() override;
	};


	// snake states

	class PlayerIdleSnakeState final : public PlayerCoilyState
	{
	public:
		explicit PlayerIdleSnakeState(dae::GameObject* coily, dae::SpriteComp* spriteComp,
			QBertMoveComp* moveComp, LevelBase* level, QBertMoveComp* qbertMoveComp);

		void OnEnter() override;
		std::unique_ptr<PlayerCoilyState> OnNotify(dae::Event event, dae::Subject* subject) override;
	};

	class PlayerJumpingSnakeState final : public PlayerCoilyState
	{
	public:
		explicit PlayerJumpingSnakeState(dae::GameObject* coily, dae::SpriteComp* spriteComp,
			QBertMoveComp* moveComp, LevelBase* level, QBertMoveComp* qbertMoveComp, const glm::vec3& moveDir);

		void OnEnter() override;
		std::unique_ptr<PlayerCoilyState> Update() override;
		std::unique_ptr<PlayerCoilyState> OnNotify(dae::Event event, dae::Subject* subject) override;

	private:
		glm::vec3 m_moveDir{};
	};

	class PlayerFallingSnakeState final : public PlayerCoilyState
	{
	public:
		explicit PlayerFallingSnakeState(dae::GameObject* coily, dae::SpriteComp* spriteComp,
			QBertMoveComp* moveComp, LevelBase* level, QBertMoveComp* qbertMoveComp);

		void OnEnter() override;
		void OnExit() override;
		std::unique_ptr<PlayerCoilyState> Update() override;
	};

	class PlayerStunnedCoilyState final : public PlayerCoilyState
	{
	public:
		explicit PlayerStunnedCoilyState(dae::GameObject* coily, dae::SpriteComp* spriteComp,
			QBertMoveComp* moveComp, LevelBase* level, QBertMoveComp* qbertMoveComp);

		void OnEnter() override;
		std::unique_ptr<PlayerCoilyState> Update() override;

	private:
		float m_secPassed{ 0.f };
		const float m_maxSec{ 1.5f };
	};

}

#endif // !PLAYERCOILYSTATES_H