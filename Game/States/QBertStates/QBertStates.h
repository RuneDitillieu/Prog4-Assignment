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
		QBertState(dae::GameObject* qbert, dae::SpriteComp* spriteComp, QBertMoveComp* moveComp, LevelBase* level) 
			: m_qbert(qbert), m_pConnSpriteComp(spriteComp), m_pMoveComp(moveComp), m_pConnLevel(level) {}
		virtual ~QBertState() = default;
		virtual void OnEnter() {};
		virtual void OnExit() {};
		virtual std::unique_ptr<QBertState> Update() { return nullptr; };
		virtual std::unique_ptr<QBertState> OnNotify(dae::Event, dae::Subject*) { return nullptr; };

	protected:
		dae::GameObject* m_qbert;
		dae::SpriteComp* m_pConnSpriteComp;
		QBertMoveComp* m_pMoveComp;
		LevelBase* m_pConnLevel;
	};

	class IdleQBertState : public QBertState
	{
	public:
		IdleQBertState(dae::GameObject* qbert, dae::SpriteComp* spriteComp, QBertMoveComp* moveComp, LevelBase* level);

		void OnEnter() override;
		std::unique_ptr<QBertState> Update() override;
		std::unique_ptr<QBertState> OnNotify(dae::Event event, dae::Subject*) override;
	};

	class JumpingQBertState : public QBertState
	{
	public:
		JumpingQBertState(dae::GameObject* qbert, dae::SpriteComp* spriteComp, QBertMoveComp* moveComp, LevelBase* level, const glm::vec3& moveDir);

		void OnEnter() override;
		std::unique_ptr<QBertState> Update() override;
		std::unique_ptr<QBertState> OnNotify(dae::Event event, dae::Subject* subject) override;

	private:
		glm::vec3 m_moveDir;
	};

	class TransportedQBertState : public QBertState
	{
	public:
		TransportedQBertState(dae::GameObject* qbert, dae::SpriteComp* spriteComp, QBertMoveComp* moveComp, LevelBase* level);

		void OnEnter() override;
		void OnExit() override;
		std::unique_ptr<QBertState> OnNotify(dae::Event event, dae::Subject*) override;
	};

	class DroppingQBertState : public QBertState
	{
	public:
		DroppingQBertState(dae::GameObject* qbert, dae::SpriteComp* spriteComp, QBertMoveComp* moveComp, LevelBase* level);

		std::unique_ptr<QBertState> Update() override;
	};

	class FallingQBertState : public QBertState
	{
	public:
		FallingQBertState(dae::GameObject* qbert, dae::SpriteComp* spriteComp, QBertMoveComp* moveComp, LevelBase* level);

		void OnEnter() override;
		void OnExit() override;
		std::unique_ptr<QBertState> Update() override;
	};
}

#endif // !QBERTSTATES_H

