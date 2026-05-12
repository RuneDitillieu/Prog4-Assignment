#ifndef QBERTACTORCOMP_H
#define QBERTACTORCOMP_H

#include "Component.h"

namespace QBert
{
	class GameObject;
	class QBertActorComp : public dae::Component
	{
	public:
		QBertActorComp(dae::GameObject* pOwner);
		~QBertActorComp() = default;
		QBertActorComp(const QBertActorComp& other) = delete;
		QBertActorComp(QBertActorComp&& other) = delete;
		QBertActorComp& operator=(const QBertActorComp& other) = delete;
		QBertActorComp& operator=(QBertActorComp&& other) = delete;

		std::type_index GetType() const override;

	private:

	};
}

#endif // !QBERT_H