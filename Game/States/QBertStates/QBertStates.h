#ifndef QBERTSTATES_H
#define QBERTSTATES_H

#include "GameObject.h"
#include "SpriteComponent.h"
#include "Events.h"

namespace QBert
{
	class Subject;
	class QBertMoveComp;
	class LevelBase;
	class QBertState
	{
	public:
		QBertState(dae::GameObject* qbert, dae::SpriteComp* spriteComp, QBertMoveComp* moveComp, LevelBase* level);
		virtual ~QBertState() = default;
		virtual void OnEnter() {};
		virtual void OnExit() {};
		virtual std::unique_ptr<QBertState> Update() { return nullptr; };
		virtual std::unique_ptr<QBertState> OnNotify(dae::Event event, dae::Subject*);

	protected:
		dae::GameObject* m_qbert;
		dae::SpriteComp* m_pConnSpriteComp;
		QBertMoveComp* m_pMoveComp;
		LevelBase* m_pConnLevel;
	};

	class IdleQBertState final : public QBertState
	{
	public:
		IdleQBertState(dae::GameObject* qbert, dae::SpriteComp* spriteComp, QBertMoveComp* moveComp, LevelBase* level);

		void OnEnter() override;
		std::unique_ptr<QBertState> Update() override;
		std::unique_ptr<QBertState> OnNotify(dae::Event event, dae::Subject*) override;
	};

	class JumpingQBertState final : public QBertState
	{
	public:
		JumpingQBertState(dae::GameObject* qbert, dae::SpriteComp* spriteComp, QBertMoveComp* moveComp, LevelBase* level, const glm::vec3& moveDir);

		void OnEnter() override;
		std::unique_ptr<QBertState> Update() override;
		std::unique_ptr<QBertState> OnNotify(dae::Event event, dae::Subject* subject) override;

	private:
		glm::vec3 m_moveDir;
	};

	class TransportedQBertState final : public QBertState
	{
	public:
		TransportedQBertState(dae::GameObject* qbert, dae::SpriteComp* spriteComp, QBertMoveComp* moveComp, LevelBase* level);

		void OnEnter() override;
		void OnExit() override;
		std::unique_ptr<QBertState> OnNotify(dae::Event event, dae::Subject*) override;
	};

	class DroppingQBertState final : public QBertState
	{
	public:
		DroppingQBertState(dae::GameObject* qbert, dae::SpriteComp* spriteComp, QBertMoveComp* moveComp, LevelBase* level);

		std::unique_ptr<QBertState> Update() override;
	};

	class FallingQBertState final : public QBertState
	{
	public:
		FallingQBertState(dae::GameObject* qbert, dae::SpriteComp* spriteComp, QBertMoveComp* moveComp, LevelBase* level);

		void OnEnter() override;
		void OnExit() override;
		std::unique_ptr<QBertState> Update() override;
	};

	class StunnedQBertState final : public QBertState
	{
	public:
		StunnedQBertState(dae::GameObject* qbert, dae::SpriteComp* spriteComp, QBertMoveComp* moveComp, LevelBase* level);

		void OnEnter() override;
		void OnExit() override;
		std::unique_ptr<QBertState> Update() override;
		std::unique_ptr<QBertState> OnNotify(dae::Event, dae::Subject*) override { return nullptr; };

	private:
		float m_secPassed{ 0.f };
		const float m_maxSec{ 1.5f };

		bool m_isFirstFrame{ true };
	};
}

#endif // !QBERTSTATES_H

