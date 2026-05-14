#ifndef QBERTSTATES_H
#define QBERTSTATES_H

#include "GameObject.h"
#include "SpriteComponent.h"
#include "Events.h"

namespace QBert
{
	class Subject;
	class QBertMoveComp;
	class QBertState
	{
	public:
		QBertState(dae::GameObject* qbert, dae::SpriteComp* spriteComp) : m_qbert(qbert), m_pConnSpriteComp(spriteComp) {}
		virtual ~QBertState() = default;
		virtual void OnEnter() {};
		virtual void OnExit() {};
		virtual std::unique_ptr<QBertState> Update() { return nullptr; };
		virtual std::unique_ptr<QBertState> OnNotify(dae::Event, dae::Subject*) { return nullptr; };

	protected:
		dae::GameObject* m_qbert;
		dae::SpriteComp* m_pConnSpriteComp;
	};

	class IdleQBertState : public QBertState
	{
	public:
		IdleQBertState(dae::GameObject* qbert, dae::SpriteComp* spriteComp, QBertMoveComp* moveComp);

		void OnEnter() override;
		std::unique_ptr<QBertState> Update() override;
		std::unique_ptr<QBertState> OnNotify(dae::Event event, dae::Subject*) override;

	private:
		QBertMoveComp* m_pMoveComp;
	};

	class JumpingQBertState : public QBertState
	{
	public:
		JumpingQBertState(dae::GameObject* qbert, dae::SpriteComp* spriteComp, QBertMoveComp* moveComp, const glm::vec3& moveDir);

		void OnEnter() override;
		std::unique_ptr<QBertState> Update() override;

	private:
		glm::vec3 m_moveDir;
		QBertMoveComp* m_pMoveComp;
	};
}

#endif // !QBERTSTATES_H

