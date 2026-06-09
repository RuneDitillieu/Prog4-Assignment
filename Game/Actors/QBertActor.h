#ifndef QBERTACTOR_H
#define QBERTACTOR_H

#include "Component.h"
#include "IObserver.h"
#include "Events.h"
#include "QBertStates.h"

namespace QBert
{
	class GameObject;
	class Subject;
	class LevelBase;
	class QBertActorComp : public dae::Component, public dae::IObserver
	{
	public:
		QBertActorComp(dae::GameObject* pOwner, dae::SpriteComp* spriteComp, QBertMoveComp* moveComp, LevelBase* pLevel);
		~QBertActorComp();
		QBertActorComp(const QBertActorComp& other) = delete;
		QBertActorComp(QBertActorComp&& other) = delete;
		QBertActorComp& operator=(const QBertActorComp& other) = delete;
		QBertActorComp& operator=(QBertActorComp&& other) = delete;

		void Start() override;
		void Update() override;
		void Notify(dae::Event event, dae::Subject* subject) override;

		std::type_index GetType() const override;

	private:
		std::unique_ptr<QBertState> m_pState;
	};
}

#endif // QBERTACTOR_H