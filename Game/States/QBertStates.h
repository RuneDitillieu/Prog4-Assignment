#ifndef QBERTSTATES_H
#define QBERTSTATES_H

#include "GameObject.h"
#include "SpriteComponent.h"

namespace QBert
{
	class QBertState
	{
	public:
		QBertState(dae::GameObject* qbert) : m_qbert(qbert) {}
		virtual ~QBertState() = default;
		virtual void OnEnter() {};
		virtual void OnExit() {};
		virtual std::unique_ptr<QBertState> Update() { return std::make_unique<QBertState>(m_qbert); };
		virtual std::type_index GetType() const { return typeid(QBertState); }

	protected:
		dae::GameObject* m_qbert;
	};

	class MoveState : public QBertState
	{
	public:
		MoveState(dae::GameObject* qbert);

		void OnEnter() override;
		std::unique_ptr<QBertState> Update() override;
		std::type_index GetType() const override;

	private:
		glm::vec3 m_goalLocation{};
		dae::SpriteComp* m_pConnSpriteComp;
	};
}

#endif // !QBERTSTATES_H

